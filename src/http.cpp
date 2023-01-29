#include "http.hpp"
#include "log.hpp"
#include "utils.hpp"
#include <algorithm>
#include <sstream>
#include <string>
#include <cstring>

#define SP " "
#define CRLF "\r\n"

const std::string BaseHttp::_delimiter = "\r\n";
const BaseHttp::MethodMap BaseHttp::_methodName =
		BaseHttp::_initializeMethodNames();

std::map<HttpMethodEnum, std::string> BaseHttp::_initializeMethodNames()
{
	std::map<HttpMethodEnum, std::string> method_names;
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

BaseHttp::BaseHttp() : _methodEnum(HTTP_GET) {}

BaseHttp::~BaseHttp() {}

// BaseHttp::BaseHttp(const char *buffer) {}

BaseHttp::HeaderField BaseHttp::_parseHeaderField(const std::string &str)
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

BaseHttp::HeaderMap BaseHttp::_parseStatusLine(const std::string &str)
{
	std::stringstream ss(str);
	HeaderMap headers;

	LOG(DEBUG) << "ss: |" << ss << "|";
	ss >> headers["method"] >> headers["path"] >> headers["version"];
	if (ss.fail())
		throw HttpException("Invalid status line !");
	if (headers["method"] == "")
		throw HttpException("Invalid status line: method is empty");
	return headers;
}

size_t BaseHttp::_get_chunk_size(int &fd)
{
	std::string line;
	std::size_t pos;

	receive_line(fd, line, CRLF);
	if (line == "")
		return (0);
	pos = line.find(SP);
	return (_convert_chunk_size(line.substr(0, pos)));
}

size_t BaseHttp::_convert_chunk_size(std::string chunk_size)
{
	std::size_t size;
	std::stringstream s_stream(chunk_size);

	s_stream >> std::hex >> size;
	return (size);
}

/**
 * @brief Parse the header from the client. The header parsed is stored in the
 * Http object to be used in other methods.
 * @param buffer The bytes buffer containing the request header.
 * @return The header map. The keys are the header names. The values are the
 * values, as defined in RFC 2616.
 */
BaseHttp::HeaderMap BaseHttp::parse(const char *buffer, int &fd)
{
	HeaderMap headers;
	std::string ss(buffer);
	std::string body;
	size_t delimiter_size = _delimiter.size();

	// Parse first header line
	std::string::size_type pos = ss.find(_delimiter);
	if (pos == std::string::npos)
		throw HttpException("Failed to parse header");
	std::string header = ss.substr(0, pos);
	headers = _parseStatusLine(header); // Might be overridden by subclass
	ss.erase(0, pos + delimiter_size);
	LOG(DEBUG) << "header: |" << header << "|";

	// Parse remaining header lines
	std::string header_line;
	while (ss.size() > 0)
	{
		pos = ss.find(_delimiter);
		header_line = ss.substr(0, pos);
		if (header_line.size() == 0)
			break;
		LOG(DEBUG) << "header_line: |" << header_line << "|";
		headers.insert(_parseHeaderField(header_line));
		ss.erase(0, pos + delimiter_size);
	}

	// Checar se o key existe
	if (headers.find("transfer-encoding") != headers.end())
	{
		if (headers["transfer-encoding"] == "chunked")
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
			headers.insert(HeaderField("body", body));
			headers.insert(HeaderField("content-length", IntToString(length)));
		}
	}
	if (headers["method"] == "POST")
	{
		body = ss.substr(2);
		headers.insert(HeaderField("body", body));
	}

	_headers = headers;
	LOG(INFO) << "Parsed headers:\n"
						<< _headers;
	return headers;
}

HttpMethodEnum BaseHttp::getMethod() { return _methodEnum; }

/**
 * @brief: Get the headers of the HTTP request. "method", "path" and "version"
 * are always present.
 *
 * @return: A std::map<std::string, std::string> of header fields.
 *
 * @note: The returned map is a copy of the internal map.
 */
BaseHttp::HeaderMap BaseHttp::getHeaders() { return _headers; }

std::ostream &operator<<(std::ostream &os,
												 const BaseHttp::HeaderMap &header_map)
{
	BaseHttp::HeaderMap::const_iterator it;

	for (it = header_map.begin(); it != header_map.end(); it++)
	{
		os << it->first << ": " << it->second << std::endl;
	}
	return os;
}
