#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Errore inizializzazione Winsock\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Errore creazione socket\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress),
             sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Errore bind: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Errore listen\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server in ascolto sulla porta 8080...\n";

    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Errore accept\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    char buffer[1024]{};

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Messaggio ricevuto: " << buffer << '\n';

        std::string response = "Hello Client!";
        send(clientSocket, response.c_str(),
             static_cast<int>(response.size()), 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}