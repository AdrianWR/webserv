#include "http_response.hpp"
#include "error.hpp"

// *****************************************************
// Class HttpResponse
// *****************************************************
HttpResponse::HttpResponse()
{

	_delimiter = "\r\n";
	_version = "HTTP/1.1";
	_code = 0;
	_reason = "";
	_status_line = _version + " " + IntToString(_code) + " " + _reason;
	// _header;

	_body = "";
	// Note: _header starts empty. Need to check if is empty when serializing
}

HttpResponse::HttpResponse(size_t code, std::string reason, std::string body)
{

	_delimiter = "\r\n";
	_version = "HTTP/1.1";
	_code = code;
	_reason = reason;
	_status_line = _version + " " + IntToString(_code) + " " + _reason;
	// _header;

	_body = body;
	// Note: _header starts empty. Need to check if is empty when serializing
}

// HttpResponse::HttpResponse(Error error) // : HttpResponse(error.code, error.msg, error.body)
//{
//}

HttpResponse::HttpResponse(Error error)
{
	_delimiter = "\r\n";
	_version = "HTTP/1.1";
	_code = error.code;
	_reason = error.msg;
	_status_line = _version + " " + IntToString(_code) + " " + _reason;
	// _header;

	_body = error.body;
	// Note: _header starts empty. Need to check if is empty when serializing
}

HttpResponse::~HttpResponse() {}

HttpResponse &HttpResponse::operator=(const HttpResponse &r)
{
	if (this != &r)
	{
		_delimiter = r._delimiter;
		_version = r._version;
		_code = r._code;
		_reason = r._reason;
		_status_line = r._status_line;
		_body = r._body;
		_header_key = r._header_key;
		_header_val = r._header_val;
	}
	return *this;
}

void HttpResponse::set(size_t code, std::string reason, std::string body)
{
	_delimiter = "\r\n";
	_version = "HTTP/1.1";
	_code = code;
	_reason = reason;
	_status_line = _version + " " + IntToString(_code) + " " + _reason;
	// _header;

	_body = body;
	// Note: _header starts empty. Need to check if is empty when serializing
	insert_header("content-length", IntToString((_body.size())));
}

std::string HttpResponse::serialize()
{
	std::string out;
	size_t i;

	out = _status_line + _delimiter;
	for (i = 0; i < _header_key.size(); i++)
	{
		out = out + _header_key[i] + ":" + _header_val[i] + _delimiter;
	};
	out = out + _delimiter + _body;
	return out;
}

void HttpResponse::show()
{
	std::cout << "------\n"
						<< "response: \n"
						<< "------\n"
						<< serialize();
}

void HttpResponse::insert_header(std::string key, std::string val)
{
	_header_key.push_back(key);
	_header_val.push_back(val);
}

std::string HttpResponse::extract_extension(std::string path)
{
	if (path.find(".html") != std::string::npos)
	{
		return "html";
	}
	if (path.find(".htm") != std::string::npos)
	{
		return "htm";
	}
	if (path.find(".txt") != std::string::npos)
	{
		return "txt";
	}
	if (path.find(".jpg") != std::string::npos)
	{
		return "jpg";
	}
	if (path.find(".jpeg") != std::string::npos)
	{
		return "jpeg";
	}
	if (path.find(".png") != std::string::npos)
	{
		return "png";
	}
	if (path.find(".gif") != std::string::npos)
	{
		return "gif";
	}
	if (path.find_last_of("/") == path.size() - 1)
	{
		return "txt";
	}
	else
	{
		return "";
	}
}

void HttpResponse::add_content_type(std::string path)
{
	std::string ext = extract_extension(path);
	LOG(DEBUG) << "ct path: " << path;
	LOG(INFO) << "Adding content-type for: " << ext;

	if (ext == "html" || ext == "htm")
	{
		this->insert_header("content-type", "text/html");
	}
	if (ext == "txt" || ext == "")
	{
		this->insert_header("content-type", "text/plain");
	}
	if (ext == "jpg" || ext == "jpeg")
	{
		this->insert_header("content-type", "image/jpeg");
	}
	if (ext == "png")
	{
		this->insert_header("content-type", "image/png");
	}
	if (ext == "gif")
	{
		this->insert_header("content-type", "image/gif");
	}
}