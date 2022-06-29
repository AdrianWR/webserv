#ifndef HTTP_HPP
#define HTTP_HPP

#include <map>
#include <string>

enum HttpMethod {
  HTTP_GET,
  HTTP_POST,
  HTTP_PUT,
  HTTP_DELETE,
  HTTP_HEAD,
  HTTP_OPTIONS,
  HTTP_CONNECT,
  HTTP_TRACE,
  HTTP_PATCH
};

enum HttpStatusCode {
  STATUS_CODE_CONTINUE = 100,
  STATUS_CODE_SWITCHING_PROTOCOLS = 101,
  STATUS_CODE_EARLY_HINTS = 103,
  STATUS_CODE_OK = 200,
  STATUS_CODE_CREATED = 201,
  STATUS_CODE_ACCEPTED = 202,
  STATUS_CODE_NON_AUTHORITATIVE_INFORMATION = 203,
  STATUS_CODE_NO_CONTENT = 204,
  STATUS_CODE_RESET_CONTENT = 205,
  STATUS_CODE_PARTIAL_CONTENT = 206,
  STATUS_CODE_MULTIPLE_CHOICES = 300,
  STATUS_CODE_MOVED_PERMANENTLY = 301,
  STATUS_CODE_FOUND = 302,
  STATUS_CODE_SEE_OTHER = 303,
  STATUS_CODE_NOT_MODIFIED = 304,
  STATUS_CODE_USE_PROXY = 305,
  STATUS_CODE_TEMPORARY_REDIRECT = 307,
  STATUS_CODE_PERMANENT_REDIRECT = 308,
  STATUS_CODE_BAD_REQUEST = 400,
  STATUS_CODE_UNAUTHORIZED = 401,
  STATUS_CODE_PAYMENT_REQUIRED = 402,
  STATUS_CODE_FORBIDDEN = 403,
  STATUS_CODE_NOT_FOUND = 404,
  STATUS_CODE_METHOD_NOT_ALLOWED = 405,
  STATUS_CODE_NOT_ACCEPTABLE = 406,
  STATUS_CODE_PROXY_AUTHENTICATION_REQUIRED = 407,
  STATUS_CODE_REQUEST_TIMEOUT = 408,
  STATUS_CODE_CONFLICT = 409,
  STATUS_CODE_GONE = 410,
  STATUS_CODE_LENGTH_REQUIRED = 411,
  STATUS_CODE_PRECONDITION_FAILED = 412,
  STATUS_CODE_PAYLOAD_TOO_LARGE = 413,
  STATUS_CODE_URI_TOO_LONG = 414,
  STATUS_CODE_UNSUPPORTED_MEDIA_TYPE = 415,
  STATUS_CODE_RANGE_NOT_SATISFIABLE = 416,
  STATUS_CODE_EXPECTATION_FAILED = 417,
  STATUS_CODE_IM_A_TEAPOT = 418,
  STATUS_CODE_MISDIRECTED_REQUEST = 421,
  STATUS_CODE_UNPROCESSABLE_ENTITY = 422,
  STATUS_CODE_LOCKED = 423,
  STATUS_CODE_FAILED_DEPENDENCY = 424,
  STATUS_CODE_UPGRADE_REQUIRED = 426,
  STATUS_CODE_PRECONDITION_REQUIRED = 428,
  STATUS_CODE_TOO_MANY_REQUESTS = 429,
  STATUS_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
  STATUS_CODE_UNAVAILABLE_FOR_LEGAL_REASONS = 451,
  STATUS_CODE_INTERNAL_SERVER_ERROR = 500,
  STATUS_CODE_NOT_IMPLEMENTED = 501,
  STATUS_CODE_BAD_GATEWAY = 502,
  STATUS_CODE_SERVICE_UNAVAILABLE = 503,
  STATUS_CODE_GATEWAY_TIMEOUT = 504,
  STATUS_CODE_HTTP_VERSION_NOT_SUPPORTED = 505,
  STATUS_CODE_VARIANT_ALSO_NEGOTIATES = 506,
  STATUS_CODE_INSUFFICIENT_STORAGE = 507,
  STATUS_CODE_LOOP_DETECTED = 508,
  STATUS_CODE_NOT_EXTENDED = 510,
  STATUS_CODE_NETWORK_AUTHENTICATION_REQUIRED = 511
};

class BaseHttp {

public:
  class HttpException : public std::exception {
  public:
    HttpException(const std::string &message) : message(message) {}
    ~HttpException() throw() {}
    virtual const char *what() const throw() { return message.c_str(); }

  private:
    std::string message;
  };

public:
  // typedef enum HttpMethod HttpMethod;
  typedef std::pair<std::string, std::string> HeaderField;
  typedef std::map<std::string, std::string> HeaderMap;
  typedef std::map<HttpMethod, std::string> MethodMap;

protected:
  HttpMethod _method;
  HeaderMap _headers;
  static const MethodMap _methodName;
  static const std::map<HttpStatusCode, std::string> _status_codes;
  static const std::string _delimiter;
  static const std::string _header_delimiter;
  static const std::string _http_version;

  virtual HeaderMap _parseStatusLine(const std::string &statusLine);
  HeaderField _parseHeaderField(const std::string &str);

  static std::map<HttpMethod, std::string> _initializeMethodNames();

public:
  BaseHttp();
  BaseHttp(const BaseHttp &);
  BaseHttp &operator=(const BaseHttp &);
  virtual ~BaseHttp() = 0;

  BaseHttp(const char *buffer); // Construct with header from client
  BaseHttp(const char *buffer,
           int length); // Construct with header and body from client

  HttpMethod getMethod();
  HeaderMap getHeaders();
  HeaderMap parse(const char *buffer);
};

std::ostream &operator<<(std::ostream &os,
                         const BaseHttp::HeaderMap &header_map);

class HttpRequest : public BaseHttp {

protected:
  HeaderMap _parseStatusLine(const std::string &statusLine);

public:
  HttpRequest();
  HttpRequest(const HttpRequest &);
  HttpRequest &operator=(const HttpRequest &);
  ~HttpRequest();
};

class HttpResponse : public BaseHttp {
public:
  HttpResponse(){};
  ~HttpResponse(){};
  HttpResponse(const HttpResponse &);
  HttpResponse &operator=(const HttpResponse &);
  HttpResponse(const char *content);
};

#endif