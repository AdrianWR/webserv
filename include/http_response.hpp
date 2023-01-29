#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "utils.hpp"

class HttpResponse {


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
	std::vector<std::string>	_header_key;
	std::vector<std::string>	_header_val;

	std::string		_body;

// Methods
	void set(size_t code, std::string reason, std::string body);
	std::string serialize();
	void show();
	void insert_header(std::string key, std::string val);
};
#endif
