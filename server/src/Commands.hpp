#pragma once
#include <iostream>
#include <sys/socket.h>

using namespace std;

enum CommandType { MOUSE, WALLPAPER, OS, DIALOG, UNKNOWN };
class Commands {
private:
  CommandType getCommandType(const string &command);
  void acceptCommand(CommandType command, const string &params);

  void mouseCommand(const string &params);
  void wallpaperCommand(const string &params);
  void osCommand(const string &params);
  void dialogCommand(const string &params);

public:
  int clientSocket;
  void processCommand(const char *command, const char *params);
};
