#include "utils.hpp"

// **********************************************************
// Utility functions
// **********************************************************
std::string file_to_string(std::string file_path) {
	std::string line;
	std::stringstream buffer;
	std::ifstream myfile(file_path.c_str());

	// Se arquivo existe, serve arquivo
	if (myfile.is_open()) {
//		while (getline(myfile, line)) {
//			buffer << line;
//		};
		buffer << myfile.rdbuf();
		myfile.close();
		return buffer.str();
	} else {
	// Substituir por throe de erro 
	// Erro 500 ?
	LOG(INFO) << "Unable to open file";
	return "";
	}
}

bool string_to_file(std::string file_path, std::string str) {
	std::ofstream new_file;

	new_file.open(file_path.c_str(), std::ios::binary);
	if (!new_file.is_open()) return false;
	LOG(DEBUG) << "file opened ? " << new_file.is_open();
	new_file.write(str.c_str(), str.length());
	new_file.close();
	return true;
}

std::string IntToString(int a) {
  std::ostringstream temp;

  temp << a;
  return temp.str();
}

size_t StringToInt(std::string s) {
  std::stringstream temp;
  size_t i;

  temp << s;
  temp >> i;

  return i;
}

bool file_exist(std::string path) {
	struct stat   buffer;   
	return (stat (path.c_str(), &buffer) == 0);
}

// **********************************************************

// *****************************************************
// Class Config
// *****************************************************
Config::Config() {}

Config::~Config() {}

Config &Config::operator=(const Config &s) {
  if (this != &s)
    return *this;
  return *this;
}

void Config::printa_linha(std::fstream &fileStream) {
  std::string buffer;

  fileStream >> buffer;
  std::cout << "|" << buffer;
  fileStream >> buffer;
  std::cout << " " << buffer << "|" << std::endl;
}

LocationBlock Config::parse_location(std::fstream &fs, std::string buffer) {
  ReservedWords r = ReservedWords();
  std::string last_rword = "";
  LocationBlock loc;

  loc = LocationBlock();
  while (buffer.compare("}") != 0) {
	// Check comment
	if (buffer[0] == '#') {
		while (!r.is_reserved_word(buffer) && buffer.compare("}") != 0)  {
			  fs >> buffer;
		};
	};
    if (r.is_reserved_word(buffer)) {
      last_rword = buffer;
      fs >> buffer;
    };
    // redirection
    if (!last_rword.compare("redirection")) {
      loc._redirection = buffer;
    }
    // root
    else if (!last_rword.compare("root")) {
      loc._root = buffer;
    }
    // upload_path
    else if (!last_rword.compare("upload_path")) {
      loc._upload_path = buffer;
    }
    // cgi_pass
    else if (!last_rword.compare("cgi_pass")) {
      loc._cgi_pass = buffer;
    }
    // autoindex
    else if (!last_rword.compare("autoindex")) {
      if (!buffer.compare("true"))
        loc._autoindex = true;
      else
        loc._autoindex = false;
    }
    // index
    else if (!last_rword.compare("index")) {
      loc._index.push_back(buffer);
    }
    // alllowed_methods
    else if (!last_rword.compare("allowed_methods")) {
      std::string key = buffer;
      fs >> buffer;
      bool aux;
      if (!buffer.compare("true"))
        aux = true;
      else
        aux = false;
      loc._allowed_methods[key] = aux;
    }
    // cgi_param
    else if (!last_rword.compare("cgi_param")) {
      std::string key = buffer;
      fs >> buffer;
      loc._cgi_param[key] = buffer;
    }
    fs >> buffer;
  }
  return (loc);
}

ConfigBlock Config::parse_config_block_file(std::fstream &fileStream,
                                            std::string &buffer) {
	int temp_port;
	ConfigBlock stub;
	std::string last_rword = "";
	std::string location_key = "";
	ReservedWords r = ReservedWords();
	LocationBlock stub_loc;

	stub = ConfigBlock();
	stub._block_name = buffer;
	LOG(INFO) << "Parsing Block: " << buffer;
	while (buffer.compare("}") != 0) {
	// Check comment
	if (buffer[0] == '#') {
		while (!r.is_reserved_word(buffer) && buffer.compare("}") != 0)  {
			  fileStream >> buffer;
		};
	};
	if (buffer.compare("}") == 0) break;
	// If is reserved word then cursor is at the begining of line
	if (r.is_reserved_word(buffer)) {
		last_rword = buffer;
		fileStream >> buffer;
	};
	// client_max_body_size
	if (!last_rword.compare("client_max_body_size")) {
	std::istringstream(buffer) >> stub._client_max_body_size;
	}
	// server_name
	else if (!last_rword.compare("server_name")) {
	  stub._server_name.push_back(buffer);
	}
	// listen
	else if (!last_rword.compare("listen")) {
		std::istringstream(buffer) >> temp_port;
		stub._listen.push_back(temp_port);
		_available_ports.insert(static_cast<short>(temp_port));
	}
	// error_page
	else if (!last_rword.compare("error_page")) {
		int key;
		std::istringstream(buffer) >> key;
		fileStream >> buffer;
		stub._error_page[key] = buffer;
	}
	// location
	else if (!last_rword.compare("location")) {
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
	if (stub._location.find("/") == stub._location.end()) {
		stub.add_default_location();
		std::cout << "Nao tem location /\n";
	}
	else {
		std::cout << "Tem location / \n";
	};
	// return
	return (stub);
}

void Config::generate_config_map() {
  ConfigBlock stub;
  std::string key;

  if (_config_vector.size() == 0)
    return;
  for (size_t i = 0; i < _config_vector.size(); i++) {
    stub = _config_vector[i];
    for (size_t j = 0; j < stub._listen.size(); j++) {
      for (size_t k = 0; k < stub._server_name.size(); k++) {
        key = stub._server_name[k] + ":" + IntToString(stub._listen[j]);
        _config_map[key] = stub;
        _config_map[key]._listen.clear();
        _config_map[key]._listen.push_back(stub._listen[j]);
        _config_map[key]._server_name.clear();
        _config_map[key]._server_name.push_back(stub._server_name[k]);
      }
    }
  } // for config_vector
}

/**
 * @brief: Get the headers of the HTTP request. "method", "path" and "version"
 * are always present.
 *
 * @return: A std::map<std::string, std::string> of header fields.
 *
 * @note: The returned map is a copy of the internal map.
 */
void Config::parse_file(std::string file) {
	ConfigBlock stub;
	std::fstream fileStream;
	std::string buffer = "";

	fileStream.open(file.c_str());
	if (!fileStream.is_open()) {
		LOG(ERROR) << "Failed to open file " << file;
	return;
	}

	// file loop
	LOG(INFO) << "Parsing Config File";
	while (fileStream >> buffer) {
		stub = ConfigBlock();
		stub = parse_config_block_file(fileStream, buffer);
		// se stub nao tem listen ou server name -> erro
		if (!stub.check_listen_and_server_name()){
			LOG(ERROR) << "Missing 'listen' and 'server_name' directives !";
		};
		// se stub nano tem location -> colocar um location default
		//stub.add_default_location();
		_config_vector.push_back(stub);
	};
//	std::cout << "Vector: \n";
//	print_vectorc(_config_vector);
//	if (_config_vector.empty()){
//		LOG(ERROR) << "Empty config !";
//	}
	generate_config_map();
	// Output config map to a file config_map.txt for easy checking
	print_mapc(_config_map);
}

// *****************************************************
// Class Config
// *****************************************************
ReservedWords::ReservedWords() {
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

bool ReservedWords::is_reserved_word(std::string query_string) {
  if (list.find(query_string) == list.end())
    return false;
  return true;
}

// *****************************************************
// Class Location
// *****************************************************
LocationBlock::LocationBlock() {
  _allowed_methods["GET"] = true;
  _allowed_methods["POST"] = true;
  _allowed_methods["DELETE"] = true;
  _redirection = "";
  _root = "/www/";
  _autoindex = true;
  //	_index.push_back("none");
  //	_cgi_param["none"] = "none";
  _cgi_pass = "";
  _upload_path = "./www/upload/";
}

LocationBlock::~LocationBlock() {}

LocationBlock &LocationBlock::operator=(const LocationBlock &rhs) {
  if (this != &rhs) {
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

void LocationBlock::print_location(std::ofstream &cout) {
  cout << "get_allowed:"
       << "\t\t" << _allowed_methods["GET"] << std::endl;
  cout << "post_allowed:"
       << "\t\t" << _allowed_methods["POST"] << std::endl;
  cout << "delete_allowed:"
       << "\t\t" << _allowed_methods["DELETE"] << std::endl;
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
ConfigBlock::ConfigBlock() {
  _block_name = "empty";
  _listen.push_back(-1);
  _server_name.push_back("none");
  _error_page[404] = "./errors/404.html";
  _client_max_body_size =
      8; // max size for the client body, defaults to 8 000
  //	_location["none"] = location();
}

ConfigBlock::~ConfigBlock() {}

ConfigBlock &ConfigBlock::operator=(const ConfigBlock &rhs) {
  if (this != &rhs) {
    _block_name = rhs._block_name;
    _listen = rhs._listen;
    _server_name = rhs._server_name;
    _error_page = rhs._error_page;
    _client_max_body_size = rhs._client_max_body_size;
    _location = rhs._location;
  }
  return *this;
}

void ConfigBlock::print_block_file(std::ofstream &cout) {
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
	for (i = _location.begin(); i != _location.end(); i++) {
		cout << "------------------------------\n";
		cout << "location:"
			 << "\t\t" << i->first << std::endl;
		i->second.print_location(cout);
	};
	cout << "=======================================================\n";
}

bool ConfigBlock::check_listen_and_server_name(){
	if (_listen.front() == -1) return false;
	if (!_server_name.front().compare("none")) return false;
	return true;
}

void ConfigBlock::add_default_location() {
	LocationBlock lb;
	_location["/"] = lb;
}

// *****************************************************
// Class AutoIndexGenerator
// *****************************************************
AutoIndexGenerator::AutoIndexGenerator(void) { return; }

AutoIndexGenerator::AutoIndexGenerator(AutoIndexGenerator const &src) {
  (void)src;
  return;
}

AutoIndexGenerator::~AutoIndexGenerator(void) { return; }

AutoIndexGenerator &
AutoIndexGenerator::operator=(AutoIndexGenerator const &src) {
  (void)src;
  return *this;
}

std::string AutoIndexGenerator::getPage(const char *path,
                                        std::string const &host, int port,
										std::string loc) {
  std::string dirName(path);
  LOG(DEBUG) << "dirname: " << dirName;
  DIR *dir = opendir(path);
  std::string page = "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
            <title>" +
                     dirName + "</title>\n\
    </head>\n\
    <body>\n\
    <h1>INDEX</h1>\n\
    <p>\n";

  if (dir == NULL) {
    std::cerr << "Error: could not open [" << path << "]" << std::endl;
    return "";
  }
  if (dirName[0] != '/')
    dirName = "/" + dirName;
  for (struct dirent *dirEntry = readdir(dir); dirEntry;
       dirEntry = readdir(dir)) {
    page += AutoIndexGenerator::getLink(std::string(dirEntry->d_name), loc, host, port);
//    page += AutoIndexGenerator::getLink(std::string(dirEntry->d_name), dirName, host, port);
  }
  page += "\
    </p>\n\
    </body>\n\
    </html>\n";
  closedir(dir);
  return page;
}

std::string AutoIndexGenerator::getLink(std::string const &dirEntry,
                                        std::string const &dirName,
                                        std::string const &host, int port) {
  std::stringstream ss;
  ss << "\t\t<p><a href=\"http://" + host + ":" << port
     << dirName + "/" + dirEntry + "\">" + dirEntry + "</a></p>\n";
  return ss.str();
}

Config::PortSet Config::getAvailablePorts(void) { return _available_ports; }

Config::BlockMap Config::getBlockMap(void) { return _config_map; }
