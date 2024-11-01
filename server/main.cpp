#include "src/Socket.hpp"

int runServer();

#if _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  runServer();
  return 0;
}
#endif

int runServer() {
  Socket socket;
  const int PORT = 6665;

  socket.init(PORT);
  socket.mainLoop();

  return 0;
}
