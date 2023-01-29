#ifndef ERROR_HTTP
#define ERROR_HTTP

#include "utils.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "log.hpp"
// *****************************************************
// Class Error
// *****************************************************
class Error
{
public:
	Error();
	Error(size_t c, ConfigBlock sc);
	~Error();
	Error &operator=(const Error &s);

public:
	size_t code;
	std::string msg;
	std::string body;

private:
	std::map<size_t, std::string> error_dic;

public:
	std::string fetch_error_page(size_t code, ConfigBlock sc);
};

#endif
