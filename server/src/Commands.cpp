#include "Commands.hpp"
#include <cctype>

CommandType Commands::getCommandType(const string &command) {
  if (command == "mouse")
    return MOUSE;
  else if (command == "wallpaper")
    return WALLPAPER;
  else if (command == "os")
    return OS;
  else if (command == "dialog")
    return DIALOG;

  return UNKNOWN;
}

void Commands::processCommand(const char *command, const char *params) {
  CommandType cmdType = this->getCommandType(command);
  this->acceptCommand(cmdType, params);
}

void Commands::acceptCommand(CommandType command, const string &params) {
  switch (command) {
  case MOUSE:
    mouseCommand(params);
    break;
  case WALLPAPER:
    wallpaperCommand(params);
    break;
  case OS:
    osCommand(params);
    break;
  case DIALOG:
    dialogCommand(params);
    break;
  default:
    string response = "Comando desconocido";
    send(clientSocket, response.c_str(), response.length(), 0);
    break;
  }
}

void Commands::mouseCommand(const string &params) {
  string response = "Comando mouse";
  send(clientSocket, response.c_str(), response.length(), 0);
}

void Commands::wallpaperCommand(const string &params) {
  string response = "Comando wallpaper";
  send(clientSocket, response.c_str(), response.length(), 0);
}

void Commands::osCommand(const string &params) {
  string response = "Comando os";
  send(clientSocket, response.c_str(), response.length(), 0);
}

void Commands::dialogCommand(const string &params) {
  string response = "Comando dialog";
  send(this->clientSocket, response.c_str(), response.length(), 0);
}
