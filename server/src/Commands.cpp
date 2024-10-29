#include "Commands.hpp"
#include <cctype>

CommandType Commands::getCommandType(const string &command) {
  if (command == "mouse")
    return MOUSE;
  return UNKNOWN;
}

void Commands::processCommand(const char *command, const char *params) {
  CommandType cmdType = this->getCommandType(command);
  this->acceptCommand(cmdType, params);
}

void Commands::acceptCommand(CommandType command, const string &params) {
  switch (command) {
  case MOUSE:
    break;
  default:
    string response = "Comando desconocido";
    send(clientSocket, response.c_str(), response.length(), 0);
    break;
  }
}
