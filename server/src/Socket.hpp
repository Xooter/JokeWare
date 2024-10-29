#pragma once
#include "Commands.hpp"
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

class Socket {
private:
  int serverSocket;
  Commands commands;

public:
  void init(const int PORT);
  void mainLoop();
};
