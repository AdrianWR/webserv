#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include "utils.hpp"

class HttpResponse {

	typedef std::map<std::string, std::string>	HeaderMap;

public:
	HttpResponse ();
	HttpResponse (size_t code, std::string reason, std::string body);
	~HttpResponse ();
	HttpResponse &operator=(const HttpResponse &s);

// Atributes
public:
	std::string		_delimiter;
	std::string		_version;
	size_t			_code;
	std::string		_reason;
	std::string		_status_line;
	HeaderMap		_headers;
	std::string		_body;

// Methods
	void set(size_t code, std::string reason, std::string body);
	std::string serialize();
	void show();
};
#endif
