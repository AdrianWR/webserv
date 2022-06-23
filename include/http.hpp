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

class BaseHttp {
private:
  http_method_t _method;

public:
  BaseHttp();
  BaseHttp(const BaseHttp &);
  BaseHttp &operator=(const BaseHttp &);
  virtual ~BaseHttp() = 0;

  BaseHttp(const char *buffer); // Construct with header from client
  BaseHttp(const char *buffer,
           int length); // Construct with header and body from client

  void setMethod(http_method_t method);
  http_method_t getMethod();
};

class HttpResponse : public BaseHttp {
public:
  HttpResponse(){};
  ~HttpResponse(){};
  HttpResponse(const HttpResponse &);
  HttpResponse &operator=(const HttpResponse &);
  HttpResponse(char *content);
};

#endif