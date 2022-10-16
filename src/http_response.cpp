#include "http_response.hpp"

// *****************************************************
// Class HttpResponse
// *****************************************************
HttpResponse::HttpResponse () {

	_delimiter = "\r\n";
	_version = "HTTP/1.1";
	_code = 0;
	_reason = "";
	_status_line = _version + " " + IntToString(_code) + " " + _reason;
	// _header;

	_body = "";
	// Note: _header starts empty. Need to check if is empty when serializing
}

HttpResponse::HttpResponse (size_t code, std::string reason, std::string body) {

	_delimiter = "\r\n";
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
	_delimiter = "\r\n";
	_version = "HTTP/1.1";
	_code = code;
	_reason = reason;
	_status_line = _version + " " + IntToString(_code) + " " + _reason;
	// _header;

	_body = body;
	// Note: _header starts empty. Need to check if is empty when serializing
	insert_header("content-length",IntToString((_body.size())));

}

std::string HttpResponse::serialize () {
	std::string out;
	size_t i;

	out = _status_line + _delimiter;
	for (i = 0; i < _header_key.size(); i++) {
		out = out + _header_key[i] + ":" + _header_val[i] + _delimiter;
	};
	out = out + _delimiter + _body;
	return out;
}

void HttpResponse::show() {
	std::cout << "------\n" << "response: \n" << "------\n" << serialize();
}

void HttpResponse::insert_header(std::string key, std::string val) {
	_header_key.push_back(key);
	_header_val.push_back(val);
}

//	out = out + _delimiter; // ?
