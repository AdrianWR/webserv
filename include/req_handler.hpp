#ifndef REQ_HANDLER_HPP
#define REQ_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "utils.hpp"

class req_handler {
public:
	// Constructor
	req_handler();
	req_handler(file_parser_c fp);
//	req_handler(std::string fp);
	// Destructor
	~req_handler();
	// Assignment operator
	req_handler &operator=(const req_handler &s);
};
#endif
