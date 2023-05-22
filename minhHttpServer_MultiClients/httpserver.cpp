#include "httpserver.h"

#include "iostream"
#include "unistd.h"
#include "cstring"
#include <chrono>
using namespace std;

int HttpServer::mCurrentConnection = 0;

HttpServer::HttpServer(){}

HttpServer::~HttpServer(){
    stopServerConnection();
}

void HttpServer::init(bool localTest){
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    (localTest == true) ? port = LOCAL_PORT : port = 80; 
    serverAddress.sin_port  = htons(port); // Convert  port number to network byte order
}

/* start server connection by openning server socket
 * call socket and bind function to open socket
 * call listen to await for client connection
 * call setupClientResource to setup EPOLL.
 */
int  HttpServer::startServerConnection(){
    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Bind the socket to a specific IP address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    serverAddress.sin_port = htons(8081); // Convert port number to network byte order

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind socket to address" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Start listening for client connections
    if (listen(serverSocket, kBacklogSize) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    setupClientResource();

    return 0;
}

/* Configure client resource.
 * create 5 epoll instances  to manage traffic on the socket
 * create a single child thread to await traffic from clients
 * create 5 threads to handle traffic from clients.
 */
void HttpServer::setupClientResource(){
    // set up epol:
    for (int i = 0; i < kThreadPoolSize; i++) {
        if ((worker_epoll_fd_[i] = epoll_create1(0)) < 0) {
            throw runtime_error(
            "Failed to create epoll file descriptor for worker");
        }
    }

    // make a thread to listen for connection request
    listener_thread_ = thread(&HttpServer::ListenForConnection, this);

    // start 5 threads to process all clien connections
    for (int i = 0; i < kThreadPoolSize; i++) {
        worker_threads_[i] = thread(&HttpServer::ProcessConnection, this, i);
    }
}

void HttpServer::ListenForConnection(){
    while (true){
        // Accept incoming client connections
        sockaddr_in clientAddress{};
        socklen_t clientAddressSize = sizeof(clientAddress);
        clientSocket = accept4(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize,SOCK_NONBLOCK);
        if (clientSocket == -1) {
            cerr << "Failed to accept client connection" << endl;
            close(serverSocket);
            break;
        }
        // count current number of connections
        mCurrentConnection++;
        cout << "Client connected - Total client:" << mCurrentConnection << endl;
        // distribute work to worker threads by assigning a epoll instance to element of worker_epoll_fd array
        epoll_event * receiveMessageEvent = new epoll_event;
        receiveMessageEvent->events = EPOLLIN;
        receiveMessageEvent->data.fd= clientSocket;
        EpollControl(worker_epoll_fd_[mCurrentConnection],             
                        EPOLL_CTL_ADD,
                        receiveMessageEvent
                        );

        if (mCurrentConnection == HttpServer::kThreadPoolSize) mCurrentConnection = 0;
    }   
}    

/* create EPOL instance by calling epll_ctrl and pass in required parameters
* The function is required to run when: 
 * 1) ListenForConnection => create new epol
 * 2) ProcessConnection: retrieve epoll_event from array worker_events_, and run epoll_ctrl 
 * 3) ReceiveRequestMessage => modify epol event to read
 * 4) HandleEpollEvent: modify epoll instance for read/write
 * epoll_fd: epoll file descriptor
 * op: the requested operations, add, modify or delete
 * fd: the clientSocket file descriptor
 * data: the client data. Here we defined Event_Data struct to whole information that is passed between sockets.
 */
void HttpServer::EpollControl(int epoll_fd, int op, epoll_event *event) {
  if (op == EPOLL_CTL_DEL) {
    if (epoll_ctl(epoll_fd, op, event->data.fd, event) < 0) {
      throw std::runtime_error("Failed to remove file descriptor");
    }
  } else {
    if (epoll_ctl(epoll_fd, op, event->data.fd, event) < 0) {
      throw std::runtime_error("Failed to add file descriptor to ready list");
    }
  }
}

/* Process connection
 * The function performs on 1 of 5 worker thread to process client request message
 * 
 */
void HttpServer::ProcessConnection(int worker_id) {
    while (true){
        int epoll_fd = worker_epoll_fd_[worker_id];
        cout<<"Process connection from client" <<worker_id<<" epoll fd= "<<epoll_fd<<endl;
        // wait for epoll_event, referec by epoll file descriptor worker_epoll_fd_[worker_id]
        // return number of file descriptors of epoll instances
        int nfds = epoll_wait(worker_epoll_fd_[worker_id],
                            worker_events_[worker_id], HttpServer::kMaxEvents, 0);
        if (nfds <=0){
            std::this_thread::sleep_for(
                std::chrono::microseconds(10000));
            continue;
        }

        for (int i = 0; i < nfds; i++) {
            epoll_event current_event = worker_events_[worker_id][i];
            
            // if the epoll even is error or socket connection is closed
            if ((current_event.events & EPOLLHUP) ||
                (current_event.events & EPOLLERR)) {
                EpollControl(epoll_fd, EPOLL_CTL_DEL, &current_event);
            }
            // epoll event is to read/write data to socket 
            else if ((current_event.events == EPOLLIN) ||
                        (current_event.events == EPOLLOUT)) {
                HandleEpollEvent(epoll_fd, &current_event);
            } else {  // something unexpected
                EpollControl(epoll_fd, EPOLL_CTL_DEL, &current_event);
            }
        }
    }
}

void HttpServer::HandleEpollEvent(int epoll_fd, epoll_event *event) {
    
    int clientSocketFd = event->data.fd;

    ssize_t bytesReceived, bytesSent;

    if (event->events == EPOLLIN) {
        cout<<"HandleEpollEven EPOLLIN"<< event->events <<endl;
        bytesReceived = receiveRequestMessage(epoll_fd, event);
        if (bytesReceived>=0){
            // assume we received complete message and revert to wait for the next msg from client
            // complete message is assumed when bytesReceived>=0. TODO: need to improve logic 
            // change the epoll event to write and add the epoll to ready list.
            event->events = EPOLLOUT;
            EpollControl(epoll_fd, EPOLL_CTL_MOD, event);
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) {  // retry
            EpollControl(epoll_fd, EPOLL_CTL_MOD, event);
        } else {  // other error
            EpollControl(epoll_fd, EPOLL_CTL_DEL, event);
        }     
    }

    if (event->events == EPOLLOUT) {
        cout<<"HandleEpollEven EPOLLOUT"<< event->events <<endl;
        bytesSent = sendResponseMessage(epoll_fd, event);    
        if (bytesSent >= 0) {   
            // assume we sent complete message and revert to wait for the next msg from client
            // change the epoll event to read and add the epoll to ready list.
            event->events = EPOLLIN;
            EpollControl(epoll_fd, EPOLL_CTL_ADD, event);
        }
        else if (errno == EAGAIN || errno == EWOULDBLOCK) {  // retry
            EpollControl(epoll_fd, EPOLL_CTL_ADD, event);
        } else {  // other error
            EpollControl(epoll_fd, EPOLL_CTL_DEL, event);
        }
    }
}

/* receive message from client.
 * call recv to read message from clientSocket. The msg is copied to buffer.
 * rec returns bytesRead which is the number of bytes received from socket
 */
ssize_t HttpServer::receiveRequestMessage(int epoll_fd, epoll_event *event){
    // Receive data from the client
    char buffer[30000];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == -1) {
        std::cerr << "Failed to receive data from client" << std::endl;
        return 1;
    } else {
        std::cout << "Received data from client: " << std::endl;
        httpRequest = new HttpRequest();
        httpRequest->parseRequest(std::string(buffer));
        handleHttpRequest(*httpRequest);
    }
    return bytesRead;
}

/* send message from client.
 * construct a response message to sent to client.
 * call send to send message to clientSocket. The msg is copied to buffer.
 * send returns bytesRead which is the number of bytes received from socket
 */
ssize_t HttpServer::sendResponseMessage(int epoll_fd, epoll_event *event){
    // Send a response back to the client
    std::string response = httpResponse->constructResponseString(*httpResponse, true);
    // const char* response = "Server received your message!";
    ssize_t bytesSent = send(clientSocket, response.c_str(), strlen(response.c_str()), 0);

    if (bytesSent == -1) {
        std::cerr << "Failed to send response to client" << std::endl;
        return 1;
    } else {
        std::cout << "Sent response to client" << std::endl;
        EpollControl(epoll_fd, EPOLL_CTL_MOD, event);
        return bytesSent;
    }
}

int HttpServer::stopClientConnection(){
    close(clientSocket);
    return 0;
}

void HttpServer::stopServerConnection(){
    std::cout<< "server stops, close connection"<<std::endl;
    close(serverSocket);
    close(clientSocket);
}

void HttpServer::handleHttpRequest(const HttpRequest &request){ 
    httpResponse = new HttpResponse(HttpStatusCode::Ok);
    std::string content;
    content += "<!doctype html>\n";
    content += "<html>\n<body>\n\n";
    content += "<h1>Hello, world in an Html page</h1>\n";
    content += "<p>A Paragraph</p>\n\n";
    content += "</body>\n</html>\n";

    httpResponse->setHeader("Content-Type", "text/html");
    httpResponse->setContent(content);
};
