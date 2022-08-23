#ifndef REQ_HANDLER_HPP
#define REQ_HANDLER_HPP

#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

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

public:
	void handler();

private:
	std::string extract_location_from_url (std::string url);
	std::string	generate_path(std::string url, std::string location, std::string root);

public:
	file_parser_c::map_of_blocks		_parsed_config_map;
};
#endif


