#include "http.hpp"
#include <sstream>
#include <string>

BaseHttp::BaseHttp() {}

BaseHttp::~BaseHttp() {}

BaseHttp::BaseHttp(const char *buffer) {}

static void _parse_header(const char *buffer) {
  std::stringstream ss(buffer);
  std::string line;
  while (std::getline(ss, line)) {
    if (line.empty()) {
      break;
    }
  }
}

http_method_t BaseHttp::getMethod() { return HTTP_UNKNOWN; }
