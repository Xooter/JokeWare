#include <iostream>
#include <cstring>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 6665;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <comando> <mensaje>" << endl;
        return 1;
    }

    const char *ipAddress = "127.0.0.1";
    const char *command = argv[1];
    const char *message = argv[2];

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error al inicializar Winsock." << endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error al crear el socket." << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, ipAddress, &serverAddress.sin_addr);

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "Error en la conexión." << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    string fullMessage = string(command) + "@" + message;
    send(clientSocket, fullMessage.c_str(), fullMessage.size(), 0);

    cout << "Mensaje: " << fullMessage << endl;

    char buffer[1024] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        cout << "Respuesta: " << string(buffer, bytesReceived) << endl;
    } else {
        cerr << "Error al recibir la respuesta del servidor." << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
