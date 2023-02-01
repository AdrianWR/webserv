#include "http_request_handler.hpp"
#include "error.hpp"

UnknownRequestHandler::UnknownRequestHandler(const HttpRequest &m) : HttpRequestHandler(m) {}

HttpResponse UnknownRequestHandler::create_response(Config &config)
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

  Error error(STATUS_CODE_METHOD_NOT_ALLOWED, _server_config);
  response.set(error.code, error.msg, error.body);

  return response;
}
