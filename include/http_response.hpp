#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "utils.hpp"
#include "error.hpp"

class HttpResponse
{

public:
	HttpResponse();
	HttpResponse(size_t code, std::string reason, std::string body);
	HttpResponse(Error error);
	~HttpResponse();
	HttpResponse &operator=(const HttpResponse &s);

	// Atributes
public:
	std::string _delimiter;
	std::string _version;
	size_t _code;
	std::string _reason;
	std::string _status_line;
	std::vector<std::string> _header_key;
	std::vector<std::string> _header_val;

	std::string _body;

	// Methods
	void set(size_t code, std::string reason, std::string body);
	std::string serialize();
	void show();
	void insert_header(std::string key, std::string val);
	std::string extract_extension(std::string path);
	void add_content_type(std::string path);
};
#endif
