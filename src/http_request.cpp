#include "http.hpp"
#include <sstream>

HttpRequest::HttpRequest() : BaseHttp() {}

HttpRequest::~HttpRequest() {}

HttpRequest::HeaderMap HttpRequest::_parseStatusLine(const std::string &str)
{
  LOG(DEBUG) << "===== _parseStatusLine =====";
  LOG(DEBUG) << "Status Line: " << str;
  std::stringstream ss(str);
  HeaderMap headers;

  ss >> headers["method"] >> headers["path"] >> headers["version"];
  if (ss.fail())
    throw HttpException("Invalid status lineeeee");
  if (headers["method"] == "")
    throw HttpException("Invalid status line: method is empty");
  return headers;
}
