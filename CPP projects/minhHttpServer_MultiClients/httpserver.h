#include "httpmessage.h"

#include <sys/socket.h>
#include "sys/epoll.h"
#include <netinet/in.h>
#include <thread>

#define LOCAL_PORT 8081

// Maximum size of an HTTP message is limited by how much bytes
// we can read or send via socket each time
constexpr size_t kMaxBufferSize = 4096;

/* EvenData includes
 * fd: file descriptor of epol instance that controls the socket
 * length
 * cursor
 * buffer: data to be transmitted in the socket.
 */ 

// struct EventData {
//   EventData() : fd(0), length(0), cursor(0), buffer() {}
//   int fd;
//   size_t length;
//   size_t cursor;
//   char buffer[kMaxBufferSize];
// };

class HttpServer {
    public:
        HttpServer();
        ~HttpServer();
        void init (bool);
        int startServerConnection();
        void stopServerConnection();
        int stopClientConnection();
    private: 
        void setupClientResource();
        void ListenForConnection();
        ssize_t receiveRequestMessage(int epoll_fd, epoll_event *event); 
        ssize_t sendResponseMessage(int epoll_fd, epoll_event *event);     
        void EpollControl(int epoll_fd, int op, epoll_event *event); 
        void ProcessConnection(int epoll_fd); 
        void HandleEpollEvent(int epoll_fd, epoll_event *event);
        void handleHttpRequest(const HttpRequest &request);
        HttpRequest * httpRequest = nullptr;
        HttpResponse * httpResponse = nullptr; 
        // in a real server, the serverAddress is init with an IP address of the interface that the traffic uses.
        // client is routed to the server Address
        sockaddr_in serverAddress{};
        int port; 
        // server socket fd and client socket fd
        int serverSocket, clientSocket; 
        static constexpr int kBacklogSize = 1000;
        static constexpr int kMaxConnections = 10000;
        static constexpr int kMaxEvents = 10000;
        static constexpr int kThreadPoolSize = 5;
        static int mCurrentConnection; 
        // epol instance array
        int worker_epoll_fd_[kMaxConnections];
        // epol even array
        epoll_event worker_events_[kThreadPoolSize][kMaxEvents];
        // listening thread
        thread listener_thread_;
        // worker thread
        thread worker_threads_[kThreadPoolSize];
};