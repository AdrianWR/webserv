#include "error.hpp"
#include "utils.hpp"

// *****************************************************
// Class Error
// *****************************************************
Error::Error()
{
	std::cout << "Error constructor" << std::endl;
}

const Error &Error::set(size_t c, ConfigBlock sc)
{
	// Init dic
	error_dic[200] = "OK";
	error_dic[301] = "Moved Permanently";
	error_dic[400] = "Bad Request";
	error_dic[403] = "Forbidden";
	error_dic[404] = "Not Found";
	error_dic[405] = "Method Not Allowed";
	error_dic[413] = "Entity Too Large"; // so para post
	error_dic[500] = "Internal Server Error";

	// Init Values
	code = c;
	msg = error_dic[code];
	if (msg == "")
	{
		msg = "Undefined Error Code";
	};
	body = fetch_error_page(code, sc);

	return *this;
}

Error::Error(size_t c, ConfigBlock sc)
{
	set(c, sc);
}

Error::~Error()
{
}

Error &Error::operator=(const Error &s)
{
	if (this != &s)
		return *this;
	return *this;
}

std::string Error::fetch_error_page(size_t code, ConfigBlock sc)
{
	std::string page_path;

	// Pega path da pag de erro
	page_path = sc._error_page[code];
	if (page_path == "")
	{
		body = "HTTP Error " + IntToString(code) + "\n";
		return body;
	}
	// se nao achar arquivo, erro padrao
	body = file_to_string(page_path);
	if (body.empty())
	{
		body = "HTTP Error " + IntToString(code) + "\n";
		return body;
	}

	return body;
}