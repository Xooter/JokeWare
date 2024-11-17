#include <cstring>
#include <iostream>

#if _WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

const int PORT = 6665;
const char *ipAddress = "10.147.20.187";

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Uso: " << argv[0] << " <comando> <mensaje>" << std::endl;
    return 1;
  }

  const char *command = argv[1];
  const char *message = argv[2];

#if _WIN32
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "Error al inicializar Winsock." << std::endl;
    return 1;
  }

  SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == INVALID_SOCKET) {
    std::cerr << "Error al crear el socket." << std::endl;
    WSACleanup();
    return 1;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  inet_pton(AF_INET, ipAddress, &serverAddress.sin_addr);

  if (connect(clientSocket, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) == SOCKET_ERROR) {
    std::cerr << "Error en la conexión." << std::endl;
    closesocket(clientSocket);
    WSACleanup();
    return 1;
  }

#else
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    std::cerr << "Error al crear el socket." << std::endl;
    return 1;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ipAddress, &serverAddress.sin_addr) <= 0) {
    std::cerr << "Error en la dirección IP." << std::endl;
    close(clientSocket);
    return 1;
  }

  if (connect(clientSocket, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) < 0) {
    std::cerr << "Error en la conexión." << std::endl;
    close(clientSocket);
    return 1;
  }
#endif

  std::string fullMessage = std::string(command) + "@" + message;
  send(clientSocket, fullMessage.c_str(), fullMessage.size(), 0);

  std::cout << "Mensaje: " << fullMessage << std::endl;

  char buffer[1024] = {0};
  int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
  if (bytesReceived > 0) {
    std::cout << "Respuesta: " << std::string(buffer, bytesReceived)
              << std::endl;
  } else {
    std::cerr << "Error al recibir la respuesta del servidor." << std::endl;
  }

#if _WIN32
  closesocket(clientSocket);
  WSACleanup();
#else
  close(clientSocket);
#endif

  return 0;
}
