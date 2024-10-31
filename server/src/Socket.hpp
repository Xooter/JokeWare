#pragma once
#include "Commands.hpp"
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

class Socket {
private:
  SOCKET serverSocket;
  Commands commands;

public:
  void init(const int PORT);
  void mainLoop();
};
