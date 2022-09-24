#include "http_response.hpp"

// *****************************************************
// Class HttpResponse
// *****************************************************
HttpResponse::HttpResponse () {

	_delimiter = "\r\f";
	_version = "HTTP/1.1";
	_code = 0;
	_reason = "";
	_status_line = _version + " " + IntToString(_code) + " " + _reason;
	// _header;

	_body = "";
	// Note: _header starts empty. Need to check if is empty when serializing
}

HttpResponse::HttpResponse (size_t code, std::string reason, std::string body) {

	_delimiter = "\r\f";
	_version = "HTTP/1.1";
	_code = code;
	_reason = reason;
	_status_line = _version + " " + IntToString(_code) + " " + _reason;
	// _header;

	_body = body;
	// Note: _header starts empty. Need to check if is empty when serializing
}


HttpResponse::~HttpResponse () {
	std::cout << "HttpResponse destructor\n";
}

HttpResponse &HttpResponse::operator=(const HttpResponse &s) {
  if (this != &s)
    return *this;
  return *this;
}

void HttpResponse::set(size_t code, std::string reason, std::string body) {
	_delimiter = "\r\f";
	_version = "HTTP/1.1";
	_code = code;
	_reason = reason;
	_status_line = _version + " " + IntToString(_code) + " " + _reason;
	// _header;

	_body = body;
	// Note: _header starts empty. Need to check if is empty when serializing
}

std::string HttpResponse::serialize () {
	std::string out;
	HeaderMap::iterator it;

	out = _status_line + _delimiter;
	for (it = _headers.begin(); it != _headers.end(); it++) {
		out = out + (it->first) + ":" + (it->second) + _delimiter;
	};
	out = out + _body + _delimiter;
	out = out + _delimiter; // ?
	return out;
}

void HttpResponse::show() {
	std::cout << "------\n" << "response: \n" << "------\n" << serialize();
}
