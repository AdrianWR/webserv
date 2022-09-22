#ifndef REQ_HANDLER_HPP
#define REQ_HANDLER_HPP

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
class Error {
public:
	Error();
	Error(size_t c, ConfigBlock sc);
	~Error();
	Error &operator=(const Error &s);

public:
	size_t			code;
	std::string		msg;
	std::string		body;

private:
	std::map<size_t, std::string>	error_dic;

public:
	std::string fetch_error_page(size_t code, ConfigBlock sc);
	void print_error();
};


// *****************************************************
// Class ReqHander
// *****************************************************
class req_handler {
public:
  // Constructor
  req_handler();
  req_handler(Config fp);
  //	req_handler(std::string fp);
  // Destructor
  ~req_handler();
  // Assignment operator
  req_handler &operator=(const req_handler &s);

public:
  void handler();

private:
  std::string extract_location (ConfigBlock sc, std::string uri);
  std::string generate_path(std::string url, std::string location,
                            std::string root);
  bool check_redirection(LocationBlock loc_config, ConfigBlock sc);
  void check_method_GET(LocationBlock loc_config);
  std::string what_is_asked(std::string path);
  void fetch_file(std::string path);
  void fetch_cgi(std::string path);
  void try_index_page(std::string path, LocationBlock loc_config);
  void try_autoindex(LocationBlock loc_config, std::string host, std::string port);
  void fetch_dir(std::string path, LocationBlock loc_config, std::string host, std::string port);

public:
  Config::BlockMap _parsed_config_map;
};


#endif
