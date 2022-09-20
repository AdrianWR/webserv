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
  std::string what_is_asked(std::string path);

public:
  Config::BlockMap _parsed_config_map;
};
#endif
