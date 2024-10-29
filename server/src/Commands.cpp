#include "Commands.hpp"
#include <cctype>
#include <cstdlib>
#include <curl/curl.h>
#include <fstream>
#include <string>

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

inline size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  std::ofstream *out = reinterpret_cast<std::ofstream *>(userp);
  size_t totalSize = size * nmemb;
  out->write(reinterpret_cast<const char *>(contents), totalSize);
  return totalSize;
}

bool Commands::downloadImage(const std::string &url,
                             const std::string &filePath) {
  CURL *curl;
  CURLcode res;
  std::ofstream outFile(filePath, std::ios::binary);

  if (!outFile.is_open()) {
    return false;
  }

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    outFile.close();

    if (res != CURLE_OK) {
      return false;
    }
  }
  return true;
}

void Commands::wallpaperCommand(const string &params) {
  const std::string filePath = "./image.jpg";
  const bool downloaded = downloadImage(params, filePath);
  if (!downloaded) {
    string response = "Error en descargar imagen";
    send(clientSocket, response.c_str(), response.length(), 0);
  }

  string response = "Comando wallpaper";
  send(clientSocket, response.c_str(), response.length(), 0);
}

void Commands::osCommand(const string &params) {
  std::system(params.c_str());

  string response = "Success";
  send(clientSocket, response.c_str(), response.length(), 0);
}

void Commands::dialogCommand(const string &params) {
  string response = "Comando dialog";
  send(this->clientSocket, response.c_str(), response.length(), 0);
}
