#include "http.hpp"
#include <sstream>

HttpRequest::HttpRequest() : BaseHttp() {}

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

HttpRequest &HttpRequest::operator=(const HttpRequest &s) {
  if (this != &s){
    _method = s._method;
    _headers = s._headers;

  }
  return *this;
}