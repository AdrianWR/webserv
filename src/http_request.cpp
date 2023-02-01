#include "http_request.hpp"
#include "log.hpp"
#include "utils.hpp"
#include <algorithm>
#include <sstream>
#include <string>
#include <cstring>

#define SP " "
#define CRLF "\r\n"
#define HOST "host"
#define METHOD "method"
#define TRANSFER_ENCODING "transfer-encoding"
#define CHUNKED "chunked"
#define BODY "body"

const std::string HttpRequest::_crlf = "\r\n";

const HttpRequest::MethodMap HttpRequest::_methodMap = HttpRequest::_initializeMethodNames();

HttpRequest::HttpRequest() : _method(HTTP_GET)
{
}

HttpRequest::HttpRequest(const HttpRequest &m) : _method(m._method), _path(m._path)
{
}

HttpRequest &HttpRequest::operator=(const HttpRequest &m)
{
	if (this != &m)
	{
		_method = m._method;
		_path = m._path;
	}
	return *this;
}

HttpRequest::HttpRequest(const char *buffer, int &fd)
{
	this->parse(buffer, fd);
}

HttpRequest::~HttpRequest() {}

HttpRequest::MethodMap HttpRequest::_initializeMethodNames()
{
	HttpRequest::MethodMap method_names;

	method_names["GET"] = HTTP_GET;
	method_names["POST"] = HTTP_POST;
	method_names["PUT"] = HTTP_PUT;
	method_names["DELETE"] = HTTP_DELETE;
	method_names["HEAD"] = HTTP_HEAD;
	method_names["OPTIONS"] = HTTP_OPTIONS;

	return method_names;
}

HttpRequest::HeaderField HttpRequest::_parse_header_field(const std::string &str)
{
	std::string::size_type pos = str.find(':');
	if (pos == std::string::npos)
		return HeaderField("", "");
	std::string key = str.substr(0, pos);
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	std::string value = str.substr(pos + 1);
	if (value.size() > 0 && value[0] == ' ')
		value.erase(0, 1);
	return HeaderField(key, value);
}

HttpRequest::HeaderMap HttpRequest::_parse_status_line(const std::string &str)
{
	std::stringstream ss(str);
	HeaderMap headers;
	std::string method;

	ss >> method >> _path >> _version;
	if (ss.fail())
		throw HttpException("Invalid status line !");
	if (method.size() == 0)
		throw HttpException("Invalid status line: method is empty");
	_method = _methodMap.at(method);
	return headers;
}

size_t HttpRequest::_get_chunk_size(int &fd)
{
	std::string line;
	std::size_t pos;

	receive_line(fd, line, CRLF);
	if (line == "")
		return (0);
	pos = line.find(SP);
	return (_convert_chunk_size(line.substr(0, pos)));
}

size_t HttpRequest::_convert_chunk_size(std::string chunk_size)
{
	std::size_t size;
	std::stringstream s_stream(chunk_size);

	s_stream >> std::hex >> size;
	return (size);
}

void HttpRequest::_next_line(std::string &ss, std::string::size_type pos)
{
	ss.erase(0, pos + _crlf.size());
}

/**
 * @brief Parse the header from the client. The header parsed is stored in the
 * Http object to be used in other methods.
 * @param buffer The bytes buffer containing the request header.
 * @return The header map. The keys are the header names. The values are the
 * values, as defined in RFC 2616.
 */
void HttpRequest::parse(const char *buffer, int &fd)
{
	HeaderMap headers;
	std::string ss(buffer);
	std::string body;
	size_t delimiter_size = _crlf.size();

	// Parse first header line
	std::string::size_type pos = ss.find(_crlf);
	if (pos == std::string::npos)
		throw HttpException("Failed to parse header");
	std::string header = ss.substr(0, pos);
	headers = _parse_status_line(header);
	ss.erase(0, pos + delimiter_size);

	// Parse remaining header lines
	std::string header_line;
	while (ss.size() > 0)
	{
		pos = ss.find(_crlf);
		header_line = ss.substr(0, pos);
		if (header_line.size() == 0)
			break;
		headers.insert(_parse_header_field(header_line));
		ss.erase(0, pos + delimiter_size);
	}

	// Checar se o key existe
	if (headers.find(TRANSFER_ENCODING) != headers.end())
	{
		if (headers[TRANSFER_ENCODING] == CHUNKED)
		{
			int length = 0;
			std::string temp_line;
			std::size_t chunk_size;
			chunk_size = this->_get_chunk_size(fd);
			while (chunk_size > 0)
			{
				receive_line(fd, temp_line, CRLF);
				body += temp_line;
				length += chunk_size;
				receive_line(fd, temp_line, CRLF);
				chunk_size = this->_get_chunk_size(fd);
			}
			headers.insert(HeaderField(BODY, body));
			headers.insert(HeaderField("content-length", IntToString(length)));
		}
	}
	if (_method == HTTP_POST)
	{
		body = ss.substr(2);
		headers.insert(HeaderField(BODY, body));
	}

	_headers = headers;
}

HttpMethod HttpRequest::getMethod() const { return _method; }

const std::string HttpRequest::getPath() const { return _path; }

const HttpRequest::HeaderMap HttpRequest::getHeaders() const { return _headers; }

std::ostream &operator<<(std::ostream &os,
												 const HttpRequest::HeaderMap &header_map)
{
	HttpRequest::HeaderMap::const_iterator it;

	for (it = header_map.begin(); it != header_map.end(); it++)
	{
		os << it->first << ": " << it->second << std::endl;
	}
	return os;
}
