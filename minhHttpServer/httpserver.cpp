#include "httpserver.h"

#include "iostream"
#include "unistd.h"
#include "cstring"

HttpServer::HttpServer(){};

HttpServer::~HttpServer(){
    stopServerConnection();
};

void HttpServer::init(bool localTest){
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    (localTest == true) ? port = LOCAL_PORT : port = 80; 
    serverAddress.sin_port  = htons(port); // Convert  port number to network byte order
}

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
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;
    return 0;
}
    
int HttpServer::startClientConnection(){
    // Accept incoming client connections
    sockaddr_in clientAddress{};
    socklen_t clientAddressSize = sizeof(clientAddress);
    clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept client connection" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Client connected" << std::endl;
    return 0;
}    

int HttpServer::receiveRequestMessage(){
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
        return 0;
    }
}

int HttpServer::sendResponseMessage(){
    // Send a response back to the client
    std::string response = httpResponse->constructResponseString(*httpResponse, true);
    // const char* response = "Server received your message!";
    ssize_t bytesSent = send(clientSocket, response.c_str(), strlen(response.c_str()), 0);

    if (bytesSent == -1) {
        std::cerr << "Failed to send response to client" << std::endl;
        return 1;
    } else {
        std::cout << "Sent response to client" << std::endl;
        return 0;
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
