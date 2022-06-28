#include "http.hpp"

HttpRequest::HttpRequest() : _method(HTTP_UNKNOWN) {}

HttpRequest::~HttpRequest() {}

HttpRequest::HeaderMap HttpRequest::_parseStatusLine(const std::string &str) {
  std::stringstream ss(str);
  HeaderMap headers;

  ss >> headers["method"] >> headers["path"] >> headers["version"];
  if (ss.fail())
    throw HttpException("Invalid status line");
  if (headers["method"] == "")
    throw HttpException("Invalid status line: method is empty");
  return headers;
}