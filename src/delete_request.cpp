#include "http_request_handler.hpp"
#include "error.hpp"

DeleteRequestHandler::DeleteRequestHandler(const HttpRequest &m) : HttpRequestHandler(m) {}

HttpResponse DeleteRequestHandler::_fetch_file(std::string path)
{
  HttpResponse response;

  if (!file_exist(path))
  {
    Error error(404, _server_config);
    response.set(error.code, error.msg, error.body);
    return response;
  }

  if (std::remove(this->_path.c_str()) == 0)
  {
    response.set(200, "OK", "");
  }
  else
  {
    Error error(403, _server_config);
    response.set(error.code, error.msg, error.body);
  }

  return response;
}

HttpResponse DeleteRequestHandler::create_response(Config &config)
{

  HttpResponse response;
  Error error;

  try
  {
    _load_config(config);
  }
  catch (HttpRequest::HttpException &e)
  {
    error.set(404, this->_server_config);
    response.add_content_type(".html");
    response.set(error.code, error.msg, "<html> Error 404 </html>");
    return response;
  }

  if (_forbidden_method(_location_config))
  {
    error.set(405, _server_config);
    response.set(error.code, error.msg, error.body);
    return response;
  }

  _path = _generate_path(_uri, _location, _location_config._root);

  switch (_get_request_content(_path))
  {
  case CONTENT_CGI:
    error.set(403, _server_config);
    response.set(error.code, error.msg, error.body);
    break;
  case CONTENT_FILE:
    response = _fetch_file(_path);
    break;
  case CONTENT_DIR:
    error.set(403, _server_config);
    response.set(error.code, error.msg, error.body);
    break;
  }

  return response;
}
