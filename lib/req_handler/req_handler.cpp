#include "req_handler.hpp"
// **********************************************************
// Class Req_handler
// **********************************************************
req_handler::req_handler() {
  std::cout << "req_handler constructor" << std::endl;
}
req_handler::req_handler(Config fp) {
	LOG(INFO) << "Initializing req_handler";
  _parsed_config_map = fp.getBlockMap();
  //	print_mapc(_parsed_config_map);
}

req_handler::~req_handler() {
  std::cout << "req_handler destructor" << std::endl;
}

req_handler &req_handler::operator=(const req_handler &s) {
  if (this != &s)
    return *this;
  return *this;
}

std::string req_handler::extract_location (std::string uri) {
	ConfigBlock::MapOfLocations::iterator	it;
	std::string key;
	bool match;
	size_t p;

	size_t pos1 = uri.find("/");
	std::string s = uri.substr(pos1);
	match = false;
	for (it = server_config._location.begin(); it != server_config._location.end(); it++) {
		key = it->first;
		if (key != "/") {
			p = s.find(key + "/");
			if (p == 0) {
				match = true;
				break;
			}
		};
	}
	if (!match) {
		key = "/";
	}
	return key;
}

std::string req_handler::generate_path(std::string uri, std::string location,
                                       std::string root) {
	// pega location
	// pega root
	// troca location por root no url
	std::string str = uri.replace(uri.find(location), location.length(), root);
	return str.substr(str.find("/"));
}

bool req_handler::check_redirection() {
	if (this->loc_config._redirection != "") {
		LOG(INFO) << "There is redirection ...";
		// Gera "erro"
		Error error(301, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
			std::cout << "------\n" << "response: \n" << "------\n" << _http_response.serialize();
		return true;
	} else {
		LOG(INFO) << "No Redirection ...";
		return false;
	};
}

bool req_handler::check_method_GET() {
	if (this->loc_config._allowed_methods["GET"] == 0) {
		LOG(INFO) << "GET Not Allowed";
		Error error(405, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
			std::cout << "------\n" << "response: \n" << "------\n" << _http_response.serialize();
		//
		return false;
	};
	LOG(INFO) << "GET Allowed";
	return true;
}

std::string req_handler::what_is_asked(std::string path) {
	if (path.find(".php") != std::string::npos) {
		return "cgi";
	}
	if (path.find_last_of("/") == path.size() - 1){
		return "dir";
	}
	else {
		return "file";
	}
}

void req_handler::fetch_cgi(std::string path) {
	(void) path;
		// Monta environment
		// Monta args
		// executa (fork etc)
		// devolve output
}

void req_handler::fetch_file(std::string path) {
	// Tenta pegar arquivo.
	std::string full_path = "." + path;
	std::string output = file_to_string(full_path);
	if (output.size() > 0) {
		LOG(INFO) << "File fetched ...";
		// Generate HTTP Response
		_http_response.set(200, "OK", output);
			std::cout << "------\n" << "response: \n" << "------\n" << _http_response.serialize();
	} else {
		LOG(INFO) << "Error 404 Not Found";
		Error error(404, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
			std::cout << "------\n" << "response: \n" << "------\n" << _http_response.serialize();
	};
}

void req_handler::try_index_page(std::string path) {
	// loop
	for (size_t i = 0; i < this->loc_config._index.size(); i++) {
		// monta caminho com um dos index
		std::string full_path = "." + path + this->loc_config._index[i];
			std::cout << "full_path: " << full_path << std::endl;
		// devolve
		std::string output = file_to_string(full_path);
		if (output.size() > 0) {
			LOG(INFO) << "Autoindex:";
//			std::cout << "|" << full_path << "|: ";
//			std::cout << "|" << output << "|" << std::endl;
		// Generate HTTP Response
		_http_response.set(200, "OK" , output);
			std::cout << "------\n" << "response: \n" << "------\n" << _http_response.serialize();
		break;
		};
	};
}

void req_handler::try_autoindex(std::string host, std::string port) {
	// se autoindex on executa autoindex
	if (this->loc_config._autoindex == true) {
		LOG(INFO) << "Autoindex ON";
		AutoIndexGenerator auto_index;
		std::string ai_page = auto_index.getPage(".",host, StringToInt(port));
		std::cout << "======================\n";
		std::cout << ai_page << std::endl;
		std::cout << "======================\n";
		// Generate HTTP Response
		_http_response.set(200, "OK", ai_page);
			std::cout << "------\n" << "response: \n" << "------\n" << _http_response.serialize();
	} else {
	// se nao devolve erro
		LOG(INFO) << "404 No index";
		Error error(404, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
			std::cout << "------\n" << "response: \n" << "------\n" << _http_response.serialize();
	};
}

void req_handler::fetch_dir(std::string path, std::string host, std::string port) {
	if (this->loc_config._index.size() > 0) {// Se tiver index
		LOG(INFO) << "Tem pagina de Index";
		try_index_page(path);
	} else { // se nao houver
		try_autoindex(host, port);
	};
}

void req_handler::load_configs() {
	// Pega configs na estrutura de configs:
	std::string conf_key = this->_host + ":" + this->_port;
	server_config = _parsed_config_map[conf_key];
	LOG(INFO) << "server_config retrieved from memory ...";
		// debug prints
		std::ofstream f("server_config.txt", std::ofstream::trunc);
		server_config.print_block_file(f);
		
	// Extract location
	this->_loc = extract_location(this->_uri);
	// Carrega configs da location na memoria
	this->loc_config = server_config._location[this->_loc];
	LOG(INFO) << "loc_config retrieved from memory: " << _loc;
		// debug prints
		std::ofstream f2("location_config.txt", std::ofstream::trunc);
		loc_config.print_location(f2);
	// ================================================================
}

void req_handler::handle_GET () {
	// Se tiver redirection, devolve redirection e sai.
	if (check_redirection()) return;

	// Monta caminho fisico:
	std::string path = generate_path(this->_uri, this->_loc, this->loc_config._root);
	LOG(INFO) << "path generated ...";
	
	// Checa se GET Permitido neste location
	if (!check_method_GET()) return;

	// 0) Se for cgi
	if (what_is_asked(path) == "cgi") {
		LOG(INFO) << "CGI requested ...";
		fetch_cgi(path);
	};
	// 1) Se for arquivoi:termina sem /
	if (what_is_asked(path) == "file") {
		LOG(INFO) << "FILE requested...";
		fetch_file(path);
	}
	// 2) Se for diretorio (termina em /)
	if (what_is_asked(path) == "dir") {
		LOG(INFO) << "DIR requested...";
		fetch_dir(path, this->_host, this->_port);
	};
	// ================================================================
}

void req_handler::handler() {

	// ================================================================
	// Get Inputs
	// ================================================================
	// Recebe um http request object
	// Do request, pega:
	// 1a linha:
	// Metodo (GET)
	// url
	// 2a linha:
	// port
	// host
	// vao formar chave para config
	//
	this->_method = "GET";
//	this->_uri = "www.site1.com/images/site1.html";
//	this->_uri = "www.site1.com/images/site9.html";
	this->_uri = "www.site1.com/images/";
	//std::string uri =		"www.site1.com/images/photo1.png";
	//std::string uri =		"www.site1.com/images/algo.cgi";
	this->_port = "8081";
	this->_host = "www.site1.com";
	int client_max_body_size = 1000;
	(void) client_max_body_size;
	// QQR erro no request dispara um 400 bad request
	// ================================================================

	// ================================================================
	// Load Configs
	// ================================================================
	// Populates class atributes with inputs and config values
	load_configs(); // FROM NOW ON SERVER CONFIG ON MEMORY
	 
	// ================================================================
	// START OF PARSING (one function for each method)
	//              GET POST DELETE OTHER
	// ================================================================
	if (this->_method == "GET") {
		handle_GET();
	}
	if (this->_method == "POST") {
	// POST
	}
	if (this->_method == "DELETE") {
	// DELETE
	}
	if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE") {
	// UNKNOWN
	}
	
	// Monta http response (em cada funcao que precisa)
	// Serializa
	// Envia
	_http_response.set(200, "OK", "Body");
	_http_response._headers["a"] = "b";
	std::cout << _http_response.serialize();
}
