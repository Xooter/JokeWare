#include <cstddef>
#include <fstream>
#ifdef _WIN32
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <windows.h>
#endif

inline size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  std::ofstream *out = reinterpret_cast<std::ofstream *>(userp);
  size_t totalSize = size * nmemb;
  out->write(reinterpret_cast<const char *>(contents), totalSize);
  return totalSize;
}

#ifdef _WIN32
inline void setWallpaper(const std::string &filePath) {
  SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)filePath.c_str(),
                       SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}
#endif
