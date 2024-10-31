#pragma once
#include <iostream>

enum CommandType { MOUSE, WALLPAPER, OS, DIALOG, VOLUME, MOUSE_MOVE, UNKNOWN };
class Commands {
private:
  CommandType getCommandType(const std::string &command);
  void acceptCommand(CommandType command, const std::string &params);

  bool mouseCommand(const std::string &params);
  bool wallpaperCommand(const std::string &params);
  bool osCommand(const std::string &params);
  bool dialogCommand(const std::string &params);
  bool volumeCommand(const std::string &params);
  bool mouseMoveCommand(const std::string &params);

  bool downloadImage(const std::string &url, const std::string &filePath);

public:
  int clientSocket;
  void processCommand(const char *command, const char *params);
};
