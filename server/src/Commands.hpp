#pragma once
#include <iostream>
#include <sys/socket.h>

using namespace std;

enum CommandType { MOUSE, UNKNOWN };
class Commands {
private:
  CommandType getCommandType(const string &command);
  void acceptCommand(CommandType command, const string &params);

public:
  int clientSocket;
  void processCommand(const char *command, const char *params);
};

void acceptCommand(const char *command, const char *params);
