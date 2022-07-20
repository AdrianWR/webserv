#ifndef HTTP_HANDLER_HPP

#define HTTP_HANDLER_HPP

#include "http.hpp"

class HttpHandler {
public:
  static HttpResponse &handle(HttpRequest &request);
};

#endif