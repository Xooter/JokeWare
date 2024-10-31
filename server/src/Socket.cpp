#include "Socket.hpp"

void Socket::init(const int PORT) {
#if _WIN32
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "Fallo al inicializar Winsock. Código de error: "
              << WSAGetLastError() << std::endl;
    return;
  }

  this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->serverSocket == INVALID_SOCKET) {
    std::cerr << "Fallo al crear el socket. Código de error: "
              << WSAGetLastError() << std::endl;
    WSACleanup();
    return;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  int bindResult = bind(this->serverSocket, (struct sockaddr *)&serverAddress,
                        sizeof(serverAddress));
  if (bindResult == SOCKET_ERROR) {
    std::cerr << "Fallo al enlazar el socket. Código de error: "
              << WSAGetLastError() << std::endl;
    closesocket(this->serverSocket);
    WSACleanup();
    return;
  }

  listen(this->serverSocket, 2);
#endif
}

void Socket::mainLoop() {
#if _WIN32
  while (true) {
    SOCKET clientSocket = accept(this->serverSocket, NULL, NULL);
    char buffer[1024] = {0};

    if (clientSocket == INVALID_SOCKET) {
      std::cerr << "Fallo al aceptar la conexión. Código de error: "
                << WSAGetLastError() << std::endl;
      continue;
    }

    commands.clientSocket = clientSocket;

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
      buffer[bytesReceived] = '\0'; // null terminator

      char *command = strtok(buffer, "@");
      char *params = strtok(nullptr, "");

      if (params == nullptr) {
        std::string response = "No se han proporcionado parámetros\n";
        send(clientSocket, response.c_str(), response.length(), 0);
      } else {
        commands.processCommand(command, params);
      }
    } else if (bytesReceived == 0) {
      std::cout << "El cliente ha cerrado la conexión" << std::endl;
    } else {
      std::cout << "Error en recv. Código de error: " << WSAGetLastError()
                << std::endl;
    }

    closesocket(clientSocket);
  }

  closesocket(this->serverSocket);
  WSACleanup();
#endif
}
