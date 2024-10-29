#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;
const int PORT = 6665;

int main() {
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  int _ = connect(clientSocket, (struct sockaddr *)&serverAddress,
                  sizeof(serverAddress));

  const char *message = "mouse|2";
  send(clientSocket, message, strlen(message), 0);

  cout << "Mensaje enviado al servidor: " << message << endl;

  // recv
  char buffer[1024] = {0};
  int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
  if (bytesReceived > 0) {
    cout << "Respuesta del servidor: " << string(buffer, bytesReceived) << endl;
  } else {
    cerr << "Error al recibir la respuesta del servidor." << endl;
  }

  close(clientSocket);

  return 0;
}
