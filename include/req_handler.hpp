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
#include "error.hpp"
#include "http_response.hpp"

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
  std::string extract_location (std::string uri);
  std::string generate_path(std::string url, std::string location, std::string root);
  bool check_redirection(LocationBlock loc_config, ConfigBlock sc);
  bool check_method_GET();
  std::string what_is_asked(std::string path);
  void fetch_file(std::string path);
  void fetch_cgi(std::string path);
  void try_index_page(std::string path);
  void try_autoindex(std::string host, std::string port);
  void fetch_dir(std::string path, std::string host, std::string port);
  void load_configs();
  void handle_GET ();

private:
  Config::BlockMap _parsed_config_map;
  ConfigBlock		server_config;
  LocationBlock		loc_config;

  std::string	_host;		// Input
  std::string	_port;		// Input
  std::string	_method;	// Input
  std::string	_uri;		// Input
  std::string	_loc;
  std::string	_path;

  //http request
  //http response
};


#endif
