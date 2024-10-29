#include "src/Socket.hpp"

int main() {
  Socket socket;
  const int PORT = 6665;

  socket.init(PORT);
  socket.mainLoop();

  return 0;
}
