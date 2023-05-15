#include "httpserver.h"
#include "iostream"
#include "cerrno"

int main() {
    try{
        // todo: use httpServer as singleton and proxy
        HttpServer httpServer;                              
        httpServer.init (true);
        httpServer.startServerConnection();
        httpServer.startClientConnection();
        httpServer.receiveRequestMessage();
        httpServer.sendResponseMessage();
        httpServer.stopClientConnection();
    }
    catch (std::exception& e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    return -1;
  }
}