#include "http_request_handler.hpp"
#include "error.hpp"

PostRequestHandler::PostRequestHandler(const HttpRequest &m) : HttpRequestHandler(m) {}

HttpResponse PostRequestHandler::_fetch_file(std::string path)
{
  HttpResponse response;

  if (_location_config._upload_path != "")
  {
    path = _generate_path(this->_uri, this->_location, this->_location_config._upload_path);
  };

  // Save file creating directory
  bool file_created = string_to_file(path, _body);

  if (file_created)
  {
    response.set(200, "OK", "");
  }
  else
  {
    Error error(404, _server_config);
    response.set(error.code, error.msg, error.body);
  }
  return response;
}

HttpResponse PostRequestHandler::create_response(Config &config)
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
  // Checa tamanho
  if (_content_length > _server_config._client_max_body_size)
  {
    Error error(413, _server_config);
    response.set(error.code, error.msg, error.body);
    return response;
  }

  _path = _generate_path(_uri, _location, _location_config._root);

  switch (_get_request_content(_path))
  {
  case CONTENT_FILE:
    response = _fetch_file(_path);
    break;
  case CONTENT_CGI:
    response = _fetch_cgi();
    break;
  case CONTENT_DIR:
    response.set(200, "OK", "");
    break;
  }

  return response;
}
