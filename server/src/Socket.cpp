#include "Socket.hpp"

void Socket::init(const int PORT) {
  this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  int _ = bind(serverSocket, (struct sockaddr *)&serverAddress,
               sizeof(serverAddress));
  listen(serverSocket, 2);
}

void Socket::mainLoop() {
  while (1) {
    int clientSocket = accept(serverSocket, NULL, NULL);
    char buffer[1024] = {0};

    if (clientSocket < 0)
      continue;

    commands.clientSocket = clientSocket;

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
      buffer[bytesReceived] = '\0'; // null

      char *command = strtok(buffer, "|");
      char *params = strtok(nullptr, "");

      commands.processCommand(command, params);
    } else if (bytesReceived == 0) {
      cout << "El cliente ha cerrado la conexión" << endl;
    } else {
      cout << "Error en recv" << endl;
    }

    close(clientSocket);
  }

  close(serverSocket);
}
