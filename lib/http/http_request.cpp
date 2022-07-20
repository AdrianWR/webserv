#include "http.hpp"
#include "log.hpp"
#include <algorithm>
#include <sstream>

HttpRequest::HttpRequest() : BaseHttp() {}

HttpRequest::HttpRequest(const HttpRequest &httpRequest)
    : BaseHttp(httpRequest) {}

HttpRequest &HttpRequest::operator=(const HttpRequest &httpRequest) {
  if (this != &httpRequest) {
    BaseHttp::operator=(httpRequest);
  }
  return *this;
}

HttpRequest::~HttpRequest() {}

namespace {

HttpRequest::HeaderField _parseHeaderField(const std::string &str) {
  std::string::size_type pos = str.find(':');
  if (pos == std::string::npos)
    return HttpRequest::HeaderField("", "");
  std::string key = str.substr(0, pos);
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  std::string value = str.substr(pos + 1);
  if (value.size() > 0 && value[0] == ' ')
    value.erase(0, 1);
  return HttpRequest::HeaderField(key, value);
}

HttpRequest::HeaderMap _parseStatusLine(const std::string &str) {
  std::stringstream ss(str);
  HttpRequest::HeaderMap headers;

  ss >> headers["method"] >> headers["path"] >> headers["version"];
  if (ss.fail())
    throw HttpRequest::HttpException("Invalid status line");
  if (headers["method"] == "")
    throw HttpRequest::HttpException("Invalid status line: method is empty");
  return headers;
}

} // namespace

/**
 * @brief Parse the header from the client. The header parsed is stored in the
 * Http object to be used in other methods.
 * @param buffer The bytes buffer containing the request header.
 * @return The header map. The keys are the header names. The values are the
 * values, as defined in RFC 2616.
 */
HttpRequest::HeaderMap HttpRequest::parse(const char *buffer) {
  HeaderMap headers;
  std::string ss(buffer);
  size_t delimiter_size = _delimiter.size();

  // Parse first header line
  std::string::size_type pos = ss.find(_delimiter);
  if (pos == std::string::npos)
    throw HttpException("Failed to parse header");
  std::string header = ss.substr(0, pos);
  headers = _parseStatusLine(header); // Might be overridden by subclass
  ss.erase(0, pos + delimiter_size);

  // Parse remaining header lines
  std::string header_line;
  while (ss.size() > 0) {
    pos = ss.find(_delimiter);
    if (pos == std::string::npos)
      throw HttpException("Failed to parse header: no delimiter found.");
    header_line = ss.substr(0, pos);
    if (header_line.size() == 0)
      break;
    header_line = ss.substr(0, pos);
    headers.insert(_parseHeaderField(header_line));
    ss.erase(0, pos + delimiter_size);
  }

  _headers = headers;
  LOG(DEBUG) << "Parsed headers: " << _headers;
  return headers;
}