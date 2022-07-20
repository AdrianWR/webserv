#include "http.hpp"
#include "log.hpp"
#include <algorithm>
#include <cstring>
#include <sstream>
#include <string>

// Static members

const std::string BaseHttp::_delimiter = "\r\n";

const BaseHttp::MethodMap BaseHttp::_methodName =
    BaseHttp::_initializeMethodNames();

std::map<HttpMethod, std::string> BaseHttp::_initializeMethodNames() {
  std::map<HttpMethod, std::string> method_names;
  method_names.insert(std::make_pair(HTTP_GET, "GET"));
  method_names.insert(std::make_pair(HTTP_POST, "POST"));
  method_names.insert(std::make_pair(HTTP_PUT, "PUT"));
  method_names.insert(std::make_pair(HTTP_DELETE, "DELETE"));
  method_names.insert(std::make_pair(HTTP_HEAD, "HEAD"));
  method_names.insert(std::make_pair(HTTP_OPTIONS, "OPTIONS"));
  method_names.insert(std::make_pair(HTTP_CONNECT, "CONNECT"));
  method_names.insert(std::make_pair(HTTP_TRACE, "TRACE"));
  method_names.insert(std::make_pair(HTTP_PATCH, "PATCH"));
  return method_names;
}

// Constructors

BaseHttp::BaseHttp() {}

BaseHttp::BaseHttp(const BaseHttp &src)
    : _headers(src._headers), _body(src._body) {}

BaseHttp &BaseHttp::operator=(const BaseHttp &src) {
  if (this != &src) {
    _headers = src._headers;
    _body = src._body;
  }
  return *this;
}

BaseHttp::~BaseHttp() {}

// Getters

/**
 * @brief Get the headers of the HTTP request or response.
 * @return A std::map<std::string, std::string> of header fields.
 */
const BaseHttp::HeaderMap &BaseHttp::getHeaders() const { return _headers; }

/**
 * @brief Get the body of the HTTP request or response.
 * @return A std::string of the body.
 */
const std::string &BaseHttp::getBody() const { return _body; }

// Setters

/*
 * @brief Set the body of the HTTP request or response.
 * @param body The body (content) of the HTTP request or response.
 */
void BaseHttp::setBody(const std::string &body) { _body = body; }

/*
 * @brief Set a header field of the HTTP request or response.
 * @param name The name of the header field.
 * @param value The value of the header field.
 */
void BaseHttp::setHeader(const std::string &key, const std::string &value) {
  _headers[key] = value;
}

/*
 * @brief Set a header field of the HTTP request or response.
 * @param headerField The header field to set.
 */
void BaseHttp::setHeader(const HeaderField &headerField) {
  _headers[headerField.first] = headerField.second;
}

// Non-Member Functions

std::ostream &operator<<(std::ostream &os,
                         const BaseHttp::HeaderMap &header_map) {
  BaseHttp::HeaderMap::const_iterator it;

  for (it = header_map.begin(); it != header_map.end(); it++) {
    os << it->first << ": " << it->second << std::endl;
  }
  return os;
}