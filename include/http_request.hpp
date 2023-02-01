#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "utils.hpp"
#include "enum.hpp"
#include <map>
#include <string>

class HttpRequest
{

public:
  class HttpException : public std::exception
  {
  public:
    HttpException(const std::string &message) : message(message) {}
    ~HttpException() throw() {}
    virtual const char *what() const throw() { return message.c_str(); }

  private:
    std::string message;
  };

public:
  typedef std::pair<std::string, std::string> HeaderField;
  typedef std::map<std::string, std::string> HeaderMap;
  typedef std::map<std::string, HttpMethod> MethodMap;

protected:
  static const MethodMap _methodMap;
  static const std::map<HttpStatusCode, std::string> _status_codes;
  static const std::string _crlf;
  static const std::string _header_delimiter;
  static const std::string _http_version;
  static MethodMap _initializeMethodNames();

  HttpMethod _method;
  HeaderMap _headers;
  std::string _path;
  std::string _version;
  std::string _request_body; // Post

  HeaderMap _parse_status_line(const std::string &statusLine);
  HeaderField _parse_header_field(const std::string &str);
  size_t _get_chunk_size(int &fd);
  size_t _convert_chunk_size(std::string chunk_size);
  void _next_line(std::string &ss, std::string::size_type pos);

public:
  HttpRequest();
  HttpRequest(const HttpRequest &);
  HttpRequest &operator=(const HttpRequest &);
  HttpRequest(const char *buffer, int &fd); // Construct with header and body from client
  ~HttpRequest();

  HttpMethod getMethod() const;
  const HeaderMap getHeaders() const;
  const std::string getPath() const;
  void parse(const char *buffer, int &fd);
};

std::ostream &operator<<(std::ostream &os,
                         const HttpRequest::HeaderMap &header_map);

#endif
