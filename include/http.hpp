#ifndef HTTP_HPP
#define HTTP_HPP

typedef enum http_method {
  HTTP_GET,
  HTTP_POST,
  HTTP_PUT,
  HTTP_DELETE,
  HTTP_HEAD,
  HTTP_OPTIONS,
  HTTP_TRACE,
  HTTP_CONNECT,
  HTTP_PATCH,
  HTTP_UNKNOWN
} http_method_t;

class HttpHeader {
public:
  HttpHeader();
  ~HttpHeader();
  HttpHeader(const HttpHeader &);
  HttpHeader &operator=(const HttpHeader &);
  HttpHeader()
};

class HttpResponse {
public:
  HttpResponse();
  ~HttpResponse();
  HttpResponse(const HttpResponse &);
  HttpResponse &operator=(const HttpResponse &);
  HttpResponse(int code, const char *message);

  HttpHeader &header();
  int code;
  const char *message;
};

#endif