#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "http_request.hpp"
#include "http_response.hpp"
#include "config.hpp"

enum RequestedContentType
{
  CONTENT_CGI,
  CONTENT_DIR,
  CONTENT_FILE
};

/**
 * @brief HttpRequest
 * @details
 * This class is used to represent an HTTP request.
 */

class HttpRequestHandler
{
public:
  typedef std::map<std::string, std::string> HeaderMap;

protected:
  HttpMethod _method;
  HeaderMap _headers;
  std::string _host;
  std::string _port;
  std::string _uri;
  std::string _body;
  std::string _cgi_pass; // Post
  std::string _location;
  std::string _path;
  int _content_length;
  Config::BlockMap _config_map;
  ConfigBlock _server_config;
  LocationBlock _location_config;

  char **_env;
  char **_cmd;

  bool _forbidden_method(LocationBlock location_config);
  bool _check_redirection();
  RequestedContentType _get_request_content(std::string path);
  std::string _extract_location(std::string uri);
  std::string _generate_path(std::string uri, std::string location, std::string root);
  HttpResponse _get_script_output(std::FILE *temp_file);
  HttpResponse _fetch_cgi();
  void _load_config(Config &config);

public:
  HttpRequestHandler(){};
  HttpRequestHandler(const HttpRequest &);
  virtual ~HttpRequestHandler();

  virtual HttpResponse create_response(Config &config);
};

class GetRequestHandler : public HttpRequestHandler
{
protected:
  HttpResponse _fetch_file(std::string path);
  HttpResponse _fetch_dir(std::string path, std::string host, std::string port);
  HttpResponse _try_autoindex(std::string host, std::string port);
  HttpResponse _try_index_page(std::string path);

public:
  GetRequestHandler(){};
  GetRequestHandler(const HttpRequest &);

  HttpResponse create_response(Config &config);
};

class PostRequestHandler : public HttpRequestHandler
{
protected:
  HttpResponse _fetch_file(std::string path);

public:
  PostRequestHandler(){};
  PostRequestHandler(const HttpRequest &);

  HttpResponse create_response(Config &config);
};

class DeleteRequestHandler : public HttpRequestHandler
{
protected:
  HttpResponse _fetch_file(std::string path);

public:
  DeleteRequestHandler(){};
  DeleteRequestHandler(const HttpRequest &);

  HttpResponse create_response(Config &config);
};

class UnknownRequestHandler : public HttpRequestHandler
{
public:
  UnknownRequestHandler(){};
  UnknownRequestHandler(const HttpRequest &);

  HttpResponse create_response(Config &config);
};

class HttpRequestFactory
{
public:
  static HttpRequestHandler &makeRequest(const HttpRequest &message);
};

#endif
