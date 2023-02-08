#include "http_request_handler.hpp"
#include "http_response.hpp"
#include "error.hpp"
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

#define HOST "host"
#define DEFAULT "default"

HttpRequestHandler &HttpRequestFactory::makeRequest(const HttpRequest &message)
{
  HttpMethod method = message.getMethod();
  switch (method)
  {
  case HTTP_GET:
    return *(new GetRequestHandler(message));
  case HTTP_POST:
    return *(new PostRequestHandler(message));
  case HTTP_DELETE:
    return *(new DeleteRequestHandler(message));
  default:
    return *(new HttpRequestHandler(message));
  }
}

HttpRequestHandler::HttpRequestHandler(const HttpRequest &message)
{
  _headers = message.getHeaders();
  _host = _headers[HOST].substr(0, _headers[HOST].find(":"));
  _port = _headers[HOST].substr(_headers[HOST].rfind(":") + 1);
  _method = message.getMethod();
  _uri = _host + message.getPath();
  _body = (_headers.find("body") != _headers.end() ? _headers["body"] : "");
  _content_length = (_headers.find("content-length") != _headers.end() ? StringToInt(_headers["content-length"]) : 0);
}

HttpRequestHandler::~HttpRequestHandler()
{
}

bool HttpRequestHandler::_forbidden_method(LocationBlock location_config)
{
  return (location_config._allowed_methods[_method] == HTTP_UNKNOWN);
}

RequestedContentType HttpRequestHandler::_get_request_content(std::string path)
{
  if (_cgi_pass != "" && (path.find(this->_cgi_pass) != std::string::npos))
  {
    return CONTENT_CGI;
  }
  if (path.find_last_of("/") == path.size() - 1)
  {
    return CONTENT_DIR;
  }
  else
  {
    return CONTENT_FILE;
  }
}

bool HttpRequestHandler::_check_redirection()
{
  return (_location_config._redirection != "");
}

std::string HttpRequestHandler::_extract_location(std::string uri)
{
  // pega uri da 1a barra em diante (inclusive barrra)
  // troca location /palavra por root (./dir/)

  ConfigBlock::MapOfLocations::iterator it;
  std::string key;
  bool match;
  size_t p;

  LOG(INFO) << "uri: " << uri;
  size_t pos1 = uri.find("/");
  std::string s = uri.substr(pos1);
  match = false;
  for (it = _server_config._location.begin(); it != _server_config._location.end(); it++)
  {
    key = it->first;
    LOG(INFO) << "key: " << key;
    if (key != "/")
    {
      p = s.find(key);
      if (p == 0)
      {
        match = true;
        break;
      }
    };
  }
  LOG(INFO) << "match? : " << match;
  if (!match)
  {
    key = "/";
  }
  return key;
}

std::string HttpRequestHandler::_generate_path(std::string uri, std::string location,
                                               std::string root)
{
  if (location == "/")
    root += "/";

  std::string uri_exhost = uri.substr(uri.find("/"));
  size_t pos = uri_exhost.find(location);
  size_t loc_len = location.length();
  std::string uri_root = "." + uri_exhost.replace(pos, loc_len, root);
  std::string full_path = uri_root;

  // Se uri_root eh dir e nao termina em / -> add /
  if (path_is(uri_root) == "dir" && !end_in_slash(uri_root))
  {
    full_path = full_path + "/";
  }

  return (full_path);
}

void HttpRequestHandler::_load_config(Config &config)
{
  _config_map = config.getBlockMap();
  std::string conf_key = _host + ":" + _port;
  _server_config = _config_map[conf_key];

  if (_server_config._block_name == DEFAULT)
  {
    _server_config = config.getDefaultServerConfig(_port);
    if (_server_config._block_name == DEFAULT)
    {
      LOG(INFO) << "Invalid host:port requested";
      throw HttpRequest::HttpException("Invalid host:port requested");
    }
  }
  _location = _extract_location(_uri);
  _location_config = _server_config._location[_location];
  _cgi_pass = this->_location_config._cgi_pass;
  LOG(INFO) << "LOCATION: " << _location;
  LOG(INFO) << "URI: " << _uri;
}

HttpResponse HttpRequestHandler::_fetch_cgi()
{
  HttpResponse response;
  int pid;
  std::string executable;
  std::string file_path;
  char **cmd = new char *[3];

  // Check if script exists in path
  std::ifstream file(_path.c_str(), std::ifstream::in);
  if (!file.is_open())
  {
    Error error(404, _server_config);
    response.set(error.code, error.msg, error.body);
    return response;
  }
  file.close();

  std::FILE *temp_file = std::tmpfile();
  int temp_fd = fileno(temp_file);

  executable = "/usr/bin/python3";
  file_path = this->_path;

  memset(cmd, 0, 3 * sizeof(char *));
  cmd[0] = strdup(executable.c_str());
  cmd[1] = strdup(file_path.c_str());
  this->_cmd = cmd;
  if (this->_method == HTTP_GET)
  {
    this->_env = NULL;
  }
  else
  {
    char **env = new char *[1];
    memset(env, 0, 1 * sizeof(char *));
    std::string var_string = "QUERY_STRING=" + _body;
    env[0] = strdup((var_string.c_str()));
    this->_env = env;
  }
  pid = fork();
  if (pid == 0)
  {
    dup2(temp_fd, STDOUT_FILENO);
    execve(this->_cmd[0], this->_cmd, this->_env);
    close(temp_fd);
  }
  waitpid(pid, NULL, 0);
  response = _get_script_output(temp_file);
  fclose(temp_file);

  // Free stuff
  free(_cmd[0]);
  free(_cmd[1]);
  delete[] _cmd;
  if (_method == HTTP_POST)
  {
    free(_env[0]);
    delete[] _env;
  }

  return response;
}

static int _get_file_size(std::FILE *temp_file)
{
  int size;

  fseek(temp_file, 0, SEEK_END);
  size = ftell(temp_file);
  rewind(temp_file);
  return (size);
}

HttpResponse HttpRequestHandler::_get_script_output(std::FILE *temp_file)
{

  HttpResponse response;

  int size = _get_file_size(temp_file);
  char *buffer = new char[size + 1];

  memset(buffer, 0, size + 1);
  fread(buffer, 1, size, temp_file);
  if (size < 0)
  {
    Error error(500, _server_config);
    response.set(error.code, error.msg, error.body);
  }
  else
  {
    response.set(200, "OK", std::string(buffer));
  }
  delete[] buffer;

  return response;
}

HttpResponse HttpRequestHandler::create_response(Config &config)
{
  // Defaults to unknown request handler

  HttpResponse response;

  try
  {
    _load_config(config);
  }
  catch (HttpRequest::HttpException &e)
  {
    Error error(404, this->_server_config);
    response.add_content_type(".html");
    response.set(error.code, error.msg, "<html> Error 404 </html>");
    return response;
  }

  Error error(STATUS_CODE_METHOD_NOT_ALLOWED, _server_config);
  response.set(error.code, error.msg, error.body);

  return response;
}
