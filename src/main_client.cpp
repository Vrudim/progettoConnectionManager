#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

int main() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Errore inizializzazione Winsock\n";
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Errore creazione socket\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1",
                  &serverAddress.sin_addr) != 1) {
        std::cerr << "Indirizzo IP non valido\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    if (connect(clientSocket,
                reinterpret_cast<sockaddr*>(&serverAddress),
                sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Connessione fallita: "
                  << WSAGetLastError() << '\n';
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::string message = "Hello Server!";

    int bytesSent = send(
        clientSocket,
        message.c_str(),
        static_cast<int>(message.size()),
        0
    );

    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Errore durante send()\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char buffer[1024]{};

    int bytesReceived = recv(
        clientSocket,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Risposta del server: " << buffer << '\n';
    } else {
        std::cerr << "Errore o connessione chiusa durante recv()\n";
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}