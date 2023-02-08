#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include "log.hpp"
#include "enum.hpp"

#define DEFAULT "default"

// *****************************************************88
// Class ReservedWords
// *****************************************************88
class ReservedWords
{
public:
  ReservedWords();
  std::set<std::string> list;
  bool is_reserved_word(std::string query_string);
};

// *****************************************************88
// Class LocationBlock
// *****************************************************88
class LocationBlock
{
public:
  // Constructor
  LocationBlock();
  // Destructor
  ~LocationBlock();
  // Assignment operator
  LocationBlock &operator=(const LocationBlock &);

public:
  std::map<HttpMethod, bool> _allowed_methods;
  std::string _redirection;
  std::string _root;
  bool _autoindex;
  std::vector<std::string> _index;
  std::map<std::string, std::string> _cgi_param;
  std::string _cgi_pass;
  std::string _upload_path;

public:
  void print_location(std::ofstream &cout);
};

// *****************************************************88
// Class ConfigBlock
// *****************************************************88
class ConfigBlock
{
public:
  typedef std::map<std::string, LocationBlock> MapOfLocations;

public:
  // Constructor
  ConfigBlock();
  // Destructor
  ~ConfigBlock();
  // Assignment operator
  ConfigBlock &operator=(const ConfigBlock &);

public:
  std::string _block_name; // name of block in config file
  std::vector<int> _listen;
  std::vector<std::string> _server_name;          // "host" do http request !
  int _client_max_body_size;                      // max size for the client body, defaults to 8
                                                  // 000
  std::map<int, std::string> _error_page;         // error page redirections
  std::map<std::string, LocationBlock> _location; // map with n-location configs

public:
  void print_block_file(std::ofstream &cout);
  bool check_listen_and_server_name(void);
  void add_default_location();
};

// *****************************************************88
// Class Config
// *****************************************************88
class Config
{
public:
  typedef std::map<std::string, ConfigBlock> BlockMap;
  typedef std::vector<ConfigBlock> vector_of_blocks;
  typedef short Port;
  typedef std::set<Port> PortSet;

private:
  vector_of_blocks _config_vector;
  PortSet _available_ports;
  BlockMap _config_map;

public:
  // Construtor
  Config();
  // Destrutor
  ~Config();
  // Asssignment
  Config &operator=(const Config &);

  // Atributos

  // Methods
  bool parse_file(std::string file);
  ConfigBlock parse_config_block_file(std::fstream &fileStream,
                                      std::string &buffer);
  LocationBlock parse_location(std::fstream &fs, std::string buffer);
  void printLine(std::fstream &fileStream);
  bool generate_config_map();

  // Getters
  PortSet getAvailablePorts(void);
  BlockMap getBlockMap(void);
  ConfigBlock getDefaultServerConfig(std::string port);
};

#endif