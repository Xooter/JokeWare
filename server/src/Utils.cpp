#include <cstddef> // Asegúrate de incluir esto para std::byte
#if _WIN32
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#endif
#include <fstream>

inline size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  std::ofstream *out = reinterpret_cast<std::ofstream *>(userp);
  size_t totalSize = size * nmemb;
  out->write(reinterpret_cast<const char *>(contents), totalSize);
  return totalSize;
}
