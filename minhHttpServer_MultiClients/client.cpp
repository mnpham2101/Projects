#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

// Function to send an HTTP GET request
void sendHttpGetRequest(const std::string& url) {
    std::string response;

    // Get server address information
    addrinfo hints{}, *serverInfo = nullptr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo(url.c_str(), "8081", &hints, &serverInfo);
    if (result != 0) {
        std::cerr << "Failed to get server address information: " << gai_strerror(result) << std::endl;
        return;
    }

    // Create socket
    int socketHandle = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    if (socketHandle == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        freeaddrinfo(serverInfo);
    }

    // Connect to the server
    if (connect(socketHandle, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
        std::cerr << "Failed to connect to the server." << std::endl;
        close(socketHandle);
        freeaddrinfo(serverInfo);
        return;
    }

    // Send GET request
    std::string httpRequest = "GET / HTTP/1.1\r\n";
    httpRequest += "Host: " + url + "\r\n";
    httpRequest += "Connection: close\r\n\r\n";

    if (send(socketHandle, httpRequest.c_str(), httpRequest.length(), 0) == -1) {
        std::cerr << "Failed to send GET request." << std::endl;
        close(socketHandle);
        freeaddrinfo(serverInfo);
        return;
    }

    // Receive and store the response
    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = recv(socketHandle, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        response += buffer;
    }

    // Close the socket and free address info
    close(socketHandle);
    freeaddrinfo(serverInfo);

    std::cout << "Response: " << response << std::endl;

}

int main() {
    std::string url = "127.0.0.1";
    std::thread arr[1000];
    // Send HTTP GET request for 1 client
    sendHttpGetRequest(url);

    for (int i = 0; i<1000; i++){
        arr[i] = std::thread(sendHttpGetRequest, url);
    }

    for (int i = 0; i<1000; i++){
        arr[i].join();
    }

    return 0;
}
