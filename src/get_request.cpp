#include "http_request_handler.hpp"
#include "autoindex.hpp"
#include "error.hpp"

GetRequestHandler::GetRequestHandler(const HttpRequest &m) : HttpRequestHandler(m) {}

HttpResponse GetRequestHandler::_fetch_dir(std::string path, std::string host, std::string port)
{
  return (_location_config._index.empty() ? _try_autoindex(host, port) : _try_index_page(path));
}

HttpResponse GetRequestHandler::_try_index_page(std::string path)
{
  HttpResponse response;

  for (size_t i = 0; i < _location_config._index.size(); i++)
  {
    std::string full_path = path + _location_config._index[i];
    // devolve
    std::string output = file_to_string(full_path);
    if (!output.empty())
    {
      response.add_content_type(full_path);
      response.set(200, "OK", output);
      break;
    };
  };

  if (response._code != 200)
  {
    Error error(404, _server_config);
    response.set(error.code, error.msg, error.body);
  };

  return response;
}

HttpResponse GetRequestHandler::_try_autoindex(std::string host, std::string port)
{
  HttpResponse response;
  std::string _uri_path = _uri.substr(_uri.find("/"));

  // se autoindex on executa autoindex
  if (_location_config._autoindex)
  {
    AutoIndexGenerator auto_index;
    std::string ai_page = auto_index.getPage(_path.c_str(), host, StringToInt(port), _uri_path);
    response.add_content_type(".html");
    response.set(200, "OK", ai_page);
  }
  else
  {
    Error error(403, _server_config);
    response.set(error.code, error.msg, error.body);
  };

  return response;
}

HttpResponse GetRequestHandler::_fetch_file(std::string path)
{
  // Tenta pegar arquivo.
  HttpResponse response;
  std::string full_path = path;
  std::string output = file_to_string(full_path);

  if (!output.empty())
  {
    response.add_content_type(path);
    response.set(200, "OK", output);
  }
  else
  {
    Error error(404, _server_config);
    response.set(error.code, error.msg, error.body);
  };

  return response;
}

HttpResponse GetRequestHandler::create_response(Config &config)
{

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

  if (_forbidden_method(_location_config))
  {
    Error error(405, _server_config);
    response.set(error.code, error.msg, error.body);
    return response;
  }

  if (_check_redirection())
  {
    Error error(301, _server_config);
    response.set(error.code, error.msg, error.body);
    response.insert_header("Location", "http://" + this->_location_config._redirection);
    return response;
  };

  _path = _generate_path(_uri, _location, _location_config._root);

  switch (_get_request_content(_path))
  {
  case CONTENT_CGI:
    response = _fetch_cgi();
    break;
  case CONTENT_FILE:
    response = _fetch_file(_path);
    break;
  case CONTENT_DIR:
    response = _fetch_dir(_path, _host, _port);
    break;
  }

  return response;
}
