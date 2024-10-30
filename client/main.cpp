#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 6665

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cerr << "Uso: " << argv[0] << " <comando> <mensaje>" << endl;
    return 1;
  }

  const char *ipAddress = "127.0.0.1";
  const char *command = argv[1];
  const char *message = argv[2];

  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  if (inet_pton(AF_INET, ipAddress, &serverAddress.sin_addr) <= 0) {
    cerr << "Dirección IP no válida: " << ipAddress << endl;
    return 1;
  }

  int _ = connect(clientSocket, (struct sockaddr *)&serverAddress,
                  sizeof(serverAddress));

  string fullMessage = string(command) + "@" + message;

  send(clientSocket, fullMessage.c_str(), strlen(fullMessage.c_str()), 0);

  cout << "Mensaje: " << fullMessage << endl;

  // recv
  char buffer[1024] = {0};
  int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
  if (bytesReceived > 0) {
    cout << "Respuesta: " << string(buffer, bytesReceived) << endl;
  } else {
    cerr << "Error al recibir la respuesta del servidor." << endl;
  }

  close(clientSocket);

  return 0;
}
