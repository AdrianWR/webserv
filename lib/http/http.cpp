#include "http.hpp"
#include "log.hpp"
#include <algorithm>
#include <sstream>
#include <string>

BaseHttp::BaseHttp() : _method(HTTP_UNKNOWN) {}

BaseHttp::~BaseHttp() {}

// BaseHttp::BaseHttp(const char *buffer) {}

BaseHttp::HeaderField BaseHttp::_parseHeaderField(const std::string &str) {
  std::string::size_type pos = str.find(':');
  if (pos == std::string::npos)
    return HeaderField("", "");
  std::string key = str.substr(0, pos);
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  std::string value = str.substr(pos + 1);
  if (value.size() > 0 && value[0] == ' ')
    value.erase(0, 1);
  return HeaderField(key, value);
}

#include <cstring>

BaseHttp::HeaderMap BaseHttp::_parseStatusLine(const std::string &str) {
  std::stringstream ss(str);
  HeaderMap headers;

  ss >> headers["method"] >> headers["path"] >> headers["version"];
  if (ss.fail())
    throw HttpException("Invalid status line");
  if (headers["method"] == "")
    throw HttpException("Invalid status line: method is empty");
  return headers;
}

BaseHttp::HeaderMap BaseHttp::parseHeader(const char *buffer) {
  HeaderMap headers;
  std::string ss(buffer);

  // Parse first header line
  std::string::size_type pos = ss.find("\r\n");
  if (pos == std::string::npos)
    throw HttpException("Failed to parse header");
  std::string header = ss.substr(0, pos);
  headers = _parseStatusLine(header);
  ss.erase(0, pos + 2);

  // Parse remaining header lines
  while (ss.size() > 0) {
    pos = ss.find("\r\n");
    if (pos == std::string::npos)
      throw HttpException("Failed to parse header");
    std::string header = ss.substr(0, pos);
    headers.insert(_parseHeaderField(header));
    ss.erase(0, pos + 2);
  }

  return headers;
}

HttpMethod BaseHttp::getMethod() { return _method; }
