#pragma once
#include <iostream>
#include <sys/socket.h>

using namespace std;

enum CommandType { MOUSE, WALLPAPER, OS, DIALOG, VOLUME, UNKNOWN };
class Commands {
private:
  CommandType getCommandType(const string &command);
  void acceptCommand(CommandType command, const string &params);

  bool mouseCommand(const string &params);
  bool wallpaperCommand(const string &params);
  bool osCommand(const string &params);
  bool dialogCommand(const string &params);
  bool volumeCommand(const string &params);

  bool downloadImage(const std::string &url, const std::string &filePath);

public:
  int clientSocket;
  void processCommand(const char *command, const char *params);
};
