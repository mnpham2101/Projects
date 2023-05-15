#include "httpmessage.h"

#include <sys/socket.h>
#include <netinet/in.h>

#define LOCAL_PORT 8081

class HttpServer {
    public:
        HttpServer();
        ~HttpServer();
        void init (bool);
        int startServerConnection();
        void stopServerConnection();
        int startClientConnection();
        int receiveRequestMessage();
        int sendResponseMessage();
        int stopClientConnection();
    private: 
        void handleHttpRequest(const HttpRequest &request);
        HttpRequest * httpRequest = nullptr;
        HttpResponse * httpResponse = nullptr; 
        sockaddr_in serverAddress{};
        int port; 
        int serverSocket, clientSocket; 
};