#include "httpserver.h"
#include "iostream"
#include "cerrno"
#include "string"
#include <thread>
int main() {
    try{
        // todo: use httpServer as singleton and proxy
        HttpServer httpServer;                              
        httpServer.init (true);
        httpServer.startServerConnection();
        string command;
        while (std::cin >> command, command != "q") {
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        httpServer.stopClientConnection();
    }
    catch (std::exception& e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    return -1;
  }
}