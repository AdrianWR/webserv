#include "config.hpp"
#include "utils.hpp"
#include <set>
#include <utility>
#include <exception>

// **********************************************************

// *****************************************************
// Class Config
// *****************************************************
Config::Config() {}

Config::~Config()
{
}

Config &Config::operator=(const Config &s)
{
  if (this != &s)
    return *this;
  return *this;
}

void Config::printLine(std::fstream &fileStream)
{
  std::string buffer;

  fileStream >> buffer;
  std::cout << "|" << buffer;
  fileStream >> buffer;
  std::cout << " " << buffer << "|" << std::endl;
}

LocationBlock Config::parse_location(std::fstream &fs, std::string buffer)
{
  ReservedWords r = ReservedWords();
  std::string last_rword = "";
  LocationBlock loc;

  loc = LocationBlock();
  while (buffer.compare("}") != 0)
  {
    // Check comment
    if (buffer[0] == '#')
    {
      while (!r.is_reserved_word(buffer) && buffer.compare("}") != 0)
      {
        fs >> buffer;
      };
    };
    if (r.is_reserved_word(buffer))
    {
      last_rword = buffer;
      fs >> buffer;
    };
    // redirection
    if (!last_rword.compare("redirection"))
    {
      loc._redirection = buffer;
    }
    // root
    else if (!last_rword.compare("root"))
    {
      loc._root = buffer;
    }
    // upload_path
    else if (!last_rword.compare("upload_path"))
    {
      loc._upload_path = buffer;
    }
    // cgi_pass
    else if (!last_rword.compare("cgi_pass"))
    {
      loc._cgi_pass = buffer;
    }
    // autoindex
    else if (!last_rword.compare("autoindex"))
    {
      if (!buffer.compare("true"))
        loc._autoindex = true;
      else
        loc._autoindex = false;
    }
    // index
    else if (!last_rword.compare("index"))
    {
      loc._index.push_back(buffer);
    }
    // alllowed_methods
    else if (!last_rword.compare("allowed_methods"))
    {
      HttpMethod method_key;
      std::string key = buffer;

      fs >> buffer;
      bool aux;
      if (!buffer.compare("true"))
        aux = true;
      else
        aux = false;
      if (key == "GET")
      {
        method_key = HTTP_GET;
      }
      else if (key == "POST")
      {
        method_key = HTTP_POST;
      }
      else if (key == "DELETE")
      {
        method_key = HTTP_DELETE;
      }
      else
      {
        method_key = HTTP_UNKNOWN;
      }
      loc._allowed_methods[method_key] = aux;
    }
    // cgi_param
    else if (!last_rword.compare("cgi_param"))
    {
      std::string key = buffer;
      fs >> buffer;
      loc._cgi_param[key] = buffer;
    }
    fs >> buffer;
  }
  return (loc);
}

ConfigBlock Config::parse_config_block_file(std::fstream &fileStream,
                                            std::string &buffer)
{
  int temp_port;
  ConfigBlock stub;
  std::string last_rword = "";
  std::string location_key = "";
  ReservedWords r = ReservedWords();
  LocationBlock stub_loc;

  stub = ConfigBlock();
  stub._block_name = buffer;
  LOG(INFO) << "Parsing Block: " << buffer;
  while (buffer.compare("}") != 0)
  {
    // Check comment
    if (buffer[0] == '#')
    {
      while (!r.is_reserved_word(buffer) && buffer.compare("}") != 0)
      {
        fileStream >> buffer;
      };
    };
    if (buffer.compare("}") == 0)
      break;
    // If is reserved word then cursor is at the begining of line
    if (r.is_reserved_word(buffer))
    {
      last_rword = buffer;
      fileStream >> buffer;
    };
    // client_max_body_size
    if (!last_rword.compare("client_max_body_size"))
    {
      std::istringstream(buffer) >> stub._client_max_body_size;
    }
    // server_name
    else if (!last_rword.compare("server_name"))
    {
      stub._server_name.push_back(buffer);
    }
    // listen
    else if (!last_rword.compare("listen"))
    {
      std::istringstream(buffer) >> temp_port;
      stub._listen.push_back(temp_port);
      _available_ports.insert(static_cast<short>(temp_port));
    }
    // error_page
    else if (!last_rword.compare("error_page"))
    {
      int key;
      std::istringstream(buffer) >> key;
      fileStream >> buffer;
      stub._error_page[key] = buffer;
    }
    // location
    else if (!last_rword.compare("location"))
    {
      std::string location_key = buffer;
      stub_loc = parse_location(fileStream, buffer);
      stub._location[location_key] = stub_loc;
    }
    fileStream >> buffer;
  } // end while

  // Remove defaults from containers
  if (stub._listen.front() == -1)
    stub._listen.erase(stub._listen.begin());
  if (!stub._server_name.front().compare("none"))
    stub._server_name.erase(stub._server_name.begin());
  // Se location vazio ou se nao tem location /, adiciona location default
  if (stub._location.find("/") == stub._location.end())
  {
    stub.add_default_location();
  };
  return (stub);
}

bool Config::generate_config_map()
{
  ConfigBlock stub;
  std::string key;

  if (_config_vector.size() == 0)
    return true;
  for (size_t i = 0; i < _config_vector.size(); i++)
  {
    stub = _config_vector[i];
    std::set<int> ports;
    for (size_t j = 0; j < stub._listen.size(); j++)
    {
      std::pair<std::set<int>::iterator, bool> result = ports.insert(stub._listen[j]);
      if (!result.second)
      {
        return false;
      }
      for (size_t k = 0; k < stub._server_name.size(); k++)
      {
        key = stub._server_name[k] + ":" + IntToString(stub._listen[j]);
        _config_map[key] = stub;
        _config_map[key]._listen.clear();
        _config_map[key]._listen.push_back(stub._listen[j]);
        _config_map[key]._server_name.clear();
        _config_map[key]._server_name.push_back(stub._server_name[k]);
      }
    }
  }
  return true;
}

/**
 * @brief: Get the headers of the HTTP request. "method", "path" and "version"
 * are always present.
 *
 * @return: A std::map<std::string, std::string> of header fields.
 *
 * @note: The returned map is a copy of the internal map.
 */
bool Config::parse_file(std::string file)
{
  ConfigBlock stub;
  std::fstream fileStream;
  std::string buffer = "";

  fileStream.open(file.c_str());
  if (!fileStream.is_open())
  {
    LOG(ERROR) << "Failed to open file " << file;
    return false;
  }
  // file loop
  LOG(INFO) << "Parsing Config File";
  while (fileStream >> buffer)
  {
    stub = ConfigBlock();
    stub = parse_config_block_file(fileStream, buffer);
    // se stub nao tem listen ou server name -> erro
    if (!stub.check_listen_and_server_name())
    {
      LOG(ERROR) << "Missing 'listen' and 'server_name' directives !";
    };
    // se stub nano tem location -> colocar um location default
    _config_vector.push_back(stub);
  };
  return generate_config_map();
}

ConfigBlock Config::getDefaultServerConfig(std::string port)
{
  ConfigBlock c;

  vector_of_blocks::const_iterator it;
  for (it = _config_vector.begin(); it != _config_vector.end(); it++)
  {
    std::vector<int>::const_iterator pit;
    for (pit = it->_listen.begin(); pit != it->_listen.end(); pit++)
    {
      if (*pit == (int)StringToInt(port))
      {
        std::string key = it->_server_name[0] + ":" + port;
        return _config_map[key];
      }
    }
  }
  return ConfigBlock();
}

// *****************************************************
// Class ReservedWords
// *****************************************************
ReservedWords::ReservedWords()
{
  list.insert("listen");
  list.insert("root");
  list.insert("server_name");
  list.insert("error_page");
  list.insert("client_max_body_size");
  list.insert("location");
  list.insert("cgi_pass");
  list.insert("cgi_param");
  list.insert("get_allowed");
  list.insert("post_allowed");
  list.insert("delete_allowed");
  list.insert("autoindex");
  list.insert("index");
  list.insert("redirection");
  list.insert("upload_path");
  list.insert("allowed_methods");
}

bool ReservedWords::is_reserved_word(std::string query_string)
{
  if (list.find(query_string) == list.end())
    return false;
  return true;
}

// *****************************************************
// Class LocationBlock
// *****************************************************
LocationBlock::LocationBlock()
{
  _allowed_methods[HTTP_GET] = true;
  _allowed_methods[HTTP_POST] = true;
  _allowed_methods[HTTP_DELETE] = true;
  _redirection = "";
  _root = "/www/";
  _autoindex = true;
  _cgi_pass = "";
  _upload_path = "/www/uploads";
}

LocationBlock::~LocationBlock()
{
}

LocationBlock &LocationBlock::operator=(const LocationBlock &rhs)
{
  if (this != &rhs)
  {
    _allowed_methods = rhs._allowed_methods;
    _redirection = rhs._redirection;
    _root = rhs._root;
    _autoindex = rhs._autoindex;
    _index = rhs._index;
    _cgi_param = rhs._cgi_param;
    _cgi_pass = rhs._cgi_pass;
    _upload_path = rhs._upload_path;
  }
  return *this;
}

void LocationBlock::print_location(std::ofstream &cout)
{
  cout << "get_allowed:"
       << "\t\t" << _allowed_methods[HTTP_GET] << std::endl;
  cout << "post_allowed:"
       << "\t\t" << _allowed_methods[HTTP_POST] << std::endl;
  cout << "delete_allowed:"
       << "\t\t" << _allowed_methods[HTTP_DELETE] << std::endl;
  cout << "redirection:"
       << "\t\t" << _redirection << std::endl;
  cout << "root:"
       << "\t\t\t" << _root << std::endl;
  cout << "autoindex:"
       << "\t\t" << _autoindex << std::endl;
  cout << "index: \n";
  print_vector(_index, cout);
  cout << "cgi pass:"
       << "\t\t" << _cgi_pass << std::endl;
  cout << "cgi_param:\n";
  print_map(_cgi_param, cout);
  cout << "upload_path::"
       << "\t\t" << _upload_path << std::endl;
}

// *****************************************************
// Class ConfigBlock
// *****************************************************
ConfigBlock::ConfigBlock()
{
  _block_name = DEFAULT;
  _listen.push_back(-1);
  _server_name.push_back("none");
  _error_page[404] = "./errors/404.html";
  _client_max_body_size =
      8; // max size for the client body, defaults to 8 000
}

ConfigBlock::~ConfigBlock()
{
}

ConfigBlock &ConfigBlock::operator=(const ConfigBlock &rhs)
{
  if (this != &rhs)
  {
    _block_name = rhs._block_name;
    _listen = rhs._listen;
    _server_name = rhs._server_name;
    _error_page = rhs._error_page;
    _client_max_body_size = rhs._client_max_body_size;
    _location = rhs._location;
  }
  return *this;
}

void ConfigBlock::print_block_file(std::ofstream &cout)
{
  cout << "=======================================================\n";
  cout << "block_name: " << _block_name << std::endl;
  cout << "listen: \n";
  print_vector(_listen, cout);
  cout << "server_name: \n";
  print_vector(_server_name, cout);
  cout << "client_max_body_size:"
       << "\t\t" << _client_max_body_size << std::endl;
  cout << "error_page:\n";
  print_map(_error_page, cout);
  std::map<std::string, LocationBlock>::iterator i;
  for (i = _location.begin(); i != _location.end(); i++)
  {
    cout << "------------------------------\n";
    cout << "location:"
         << "\t\t" << i->first << std::endl;
    i->second.print_location(cout);
  };
  cout << "=======================================================\n";
}

bool ConfigBlock::check_listen_and_server_name()
{
  if (_listen.front() == -1)
    return false;
  if (!_server_name.front().compare("none"))
    return false;
  return true;
}

void ConfigBlock::add_default_location()
{
  LocationBlock lb;
  _location["/"] = lb;
}

Config::PortSet Config::getAvailablePorts(void) { return _available_ports; }

Config::BlockMap Config::getBlockMap(void) { return _config_map; }
