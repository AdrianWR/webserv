#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>

#include "log.hpp"

// *****************************************************88
// Auxiliary Functions
// *****************************************************88

std::string IntToString(int a);

template <typename T> void print_vector(std::vector<T> v, std::ofstream &cout) {
  typename std::vector<T>::iterator it;

  int i = 0;

  for (it = v.begin(); it != v.end(); it++) {
    cout << "[" << i << "]\t|" << (*it) << "|" << std::endl;
    i++;
  }
}

template <typename T> void print_vectorc(std::vector<T> v) {
  std::ofstream cout("config_vector.txt", std::ofstream::trunc);

  for (unsigned int i = 0; i < v.size(); i++) {
    std::cout << "[" << i << "]\n";
    v[i].print_block_file(cout);
  }
}

template <typename A, typename B>
void print_map(std::map<A, B> m, std::ofstream &cout) {
  typename std::map<A, B>::iterator it;
  for (it = m.begin(); it != m.end(); it++) {
    cout << "|" << it->first << " : " << it->second << "|" << std::endl;
  }
}

template <typename A, typename B> void print_mapc(std::map<A, B> m) {
  typename std::map<A, B>::iterator it;
  std::ofstream cout("config_map.txt", std::ofstream::trunc);

  for (it = m.begin(); it != m.end(); it++) {
    cout << it->first << ":\n";
    (it->second).print_block_file(cout);
  }
  cout.close();
}

// *****************************************************88
// Class ReservedWords 
// *****************************************************88
class ReservedWords {
public:
  ReservedWords();
  std::set<std::string> list;
  bool is_reserved_word(std::string query_string);
};

// *****************************************************88
// Class LocationBlock 
// *****************************************************88
class LocationBlock {
public:
  // Constructor
  LocationBlock();
  // Destructor
  ~LocationBlock();
  // Assignment operator
  LocationBlock &operator=(const LocationBlock &);

public:
  std::map<std::string, bool> _allowed_methods;
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
class ConfigBlock {
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
  std::vector<std::string> _server_name; // "host" do http request !
  int _client_max_body_size; // max size for the client body, defaults to 8
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
class Config {
public:
  typedef std::map<std::string, ConfigBlock> BlockMap;
  typedef std::vector<ConfigBlock> vector_of_blocks;
  typedef short PortType;
  typedef std::set<PortType> PortSet;

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
  void parse_file(std::string file);
  ConfigBlock parse_config_block_file(std::fstream &fileStream,
                                      std::string &buffer);
  LocationBlock parse_location(std::fstream &fs, std::string buffer);
  void printa_linha(std::fstream &fileStream);
  void generate_config_map();

  // Getters
  PortSet getAvailablePorts(void);
  BlockMap getBlockMap(void);
};

// *****************************************************88
// Class AutoIndexGeerator 
// *****************************************************88
class AutoIndexGenerator {
    public:
        AutoIndexGenerator(void);
        AutoIndexGenerator(AutoIndexGenerator const &src);
        virtual ~AutoIndexGenerator(void);

        AutoIndexGenerator   &operator=(AutoIndexGenerator const &src);

        static std::string  getPage(const char *path, std::string const &host, int port);
    private:
        static std::string  getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host, int port);

};

#endif
