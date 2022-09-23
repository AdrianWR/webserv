#include "req_handler.hpp"

// **********************************************************
// Utility functions
// **********************************************************
std::string file_to_string(std::string file_path) {
	std::string line;
	std::stringstream buffer;
	std::ifstream myfile(file_path.c_str());

	// Se arquivo existe, serve arquivo
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			buffer << line;
		};
		myfile.close();
		return buffer.str();
	} else {
	// Substituir por throe de erro 
	// Erro 500 ?
	std::cout << "Unable to open file\n";
	return "";
	}
}

// *****************************************************
// Class Error
// *****************************************************
Error::Error() {
  std::cout << "Error constructor" << std::endl;
}

Error::Error(size_t c, ConfigBlock sc) {
	// Init dic
	error_dic[200] = "OK";
	error_dic[301] = "Moved Permanently";
	error_dic[400] = "Bad Request";
	error_dic[403] = "Forbidden";
	error_dic[404] = "Not Found";
	error_dic[405] = "Method Not Allowed";
	error_dic[413] = "Entity to Large"; // so para post
	error_dic[500] = "Internal Server Error";
	
	// Init Values
	code = c;
	msg = error_dic[code];
	if (msg == "") { msg = "Undefined Error Code"; };
	body = fetch_error_page(code, sc);
}

Error::~Error() {
  std::cout << "Error destructor" << std::endl;
}

Error &Error::operator=(const Error &s) {
  if (this != &s)
    return *this;
  return *this;
}

std::string Error::fetch_error_page(size_t code, ConfigBlock sc) {
	std::string page_path;
//	(void) code;
//	(void) sc;
//	body = "ERROR BODY !!!!";
	
	// Pega path da pag de erro
	page_path = sc._error_page[code];
	if (page_path == "") {
		body = "No Error Page 1";
		return body;
	}
	// se nao achar arquivo, erro padrao
	body = file_to_string(page_path);
	if (body.size() == 0) {
		body = "No Error Page 2";
		return body;
	}

	return body;
}

void Error::print_error() {
	std::cout << "----------------------\n";
	std::cout << code << " : " << msg << std::endl;
	std::cout << "body: " << body << std::endl;
	std::cout << "----------------------\n";
}


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

bool req_handler::check_redirection(LocationBlock loc_config, ConfigBlock sc ) {
	(void) sc;
	if (loc_config._redirection != "") {
		LOG(INFO) << "There is redirection ...";
		// Gera "erro"
		Error error(301, sc);
			error.print_error();
		// Generate HTTP Response
		return true;
	} else {
		LOG(INFO) << "No Redirection ...";
		return false;
	};
}

bool req_handler::check_method_GET() {
	if (loc_config._allowed_methods["GET"] == 0) {
		LOG(INFO) << "GET Not Allowed";
		Error error(405, server_config);
			error.print_error();
		// Generate HTTP Response
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
		std::cout << "***********************************\n";
		std::cout << output << std::endl;
		std::cout << "***********************************\n";
		// Generate HTTP Response
	} else {
		LOG(INFO) << "Error 404 Not Found";
		Error error(404, server_config);
			error.print_error();
		// Generate HTTP Response
	};
}

void req_handler::try_index_page(std::string path) {
	// loop
	for (size_t i = 0; i < loc_config._index.size(); i++) {
		// monta caminho com um dos index
		std::string full_path = "." + path + loc_config._index[i];
			std::cout << "full_path: " << full_path << std::endl;
		// devolve
		std::string output = file_to_string(full_path);
		// Generate HTTP Response
		if (output.size() > 0) {
			LOG(INFO) << "Autoindex:";
			std::cout << "|" << full_path << "|: ";
			std::cout << "|" << output << "|" << std::endl;
		break;
		};
	};
}

void req_handler::try_autoindex(std::string host, std::string port) {
	// se autoindex on executa autoindex
	if (loc_config._autoindex == true) {
		LOG(INFO) << "Autoindex ON";
		AutoIndexGenerator auto_index;
		std::string ai_page = auto_index.getPage(".",host, StringToInt(port));
		std::cout << "======================\n";
		std::cout << ai_page << std::endl;
		std::cout << "======================\n";
		// Generate HTTP Response
	} else {
	// se nao devolve erro
		LOG(INFO) << "404 No index";
		Error error(404, server_config);
			error.print_error();
		// Generate HTTP Response
	};
}

void req_handler::fetch_dir(std::string path, std::string host, std::string port) {
	if (loc_config._index.size() > 0) {// Se tiver index
		LOG(INFO) << "Tem pagina de Index";
		try_index_page(path);
	} else { // se nao houver
		try_autoindex(host, port);
	};
}

void req_handler::load_configs() {
	// Pega configs na estrutura de configs:
	std::string conf_key = _host + ":" + _port;
	server_config = _parsed_config_map[conf_key];
	LOG(INFO) << "server_config retrieved from memory ...";
		// debug prints
		std::ofstream f("server_config.txt", std::ofstream::trunc);
		server_config.print_block_file(f);
		
	// Extract location
	_loc = extract_location(_uri);
	// Carrega configs da location na memoria
	loc_config = server_config._location[_loc];
	LOG(INFO) << "loc_config retrieved from memory: " << _loc;
		// debug prints
		std::ofstream f2("location_config.txt", std::ofstream::trunc);
		loc_config.print_location(f2);
	// ================================================================
}

void req_handler::handle_GET () {
	// Se tiver redirection, devolve redirection e sai.
	if (check_redirection(loc_config, server_config)) return;

	// Monta caminho fisico:
	std::string path = generate_path(_uri, _loc, loc_config._root);
	LOG(INFO) << "path generated ...";
	
	// Checa se GET Permitido neste location
	if (!check_method_GET()) return;

	// 0) Se for cgi
	if (what_is_asked(path) == "cgi") {
		LOG(INFO) << "CGI requested ...";
		fetch_cgi(_path);
	};
	// 1) Se for arquivoi:termina sem /
	if (what_is_asked(path) == "file") {
		LOG(INFO) << "FILE requested...";
		fetch_file(_path);
	}
	// 2) Se for diretorio (termina em /)
	if (what_is_asked(path) == "dir") {
		LOG(INFO) << "DIR requested...";
		fetch_dir(_path, _host, _port);
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
	_method = "GET";
	//std::string uri = "www.site1.com/images/site1.html";
	_uri = "www.site1.com/images/";
	//std::string uri =		"www.site1.com/images/photo1.png";
	//std::string uri =		"www.site1.com/images/algo.cgi";
	_port = "8081";
	_host = "www.site1.com";
	int client_max_body_size = 1000;
	(void) client_max_body_size;
	// QQR erro no request dispara um 400 bad request
	// ================================================================

	// ================================================================
	// Load Configs
	// ================================================================
	// Populates class atributes with inputs and config values
	load_configs();
	// FROM NOW ON SERVER CONFIG ON MEMORY
	 
	// ================================================================
	// START OF PARSING (one function for each method)
	//              GET POST DELETE OTHER
	// ================================================================
	handle_GET();
	// POST
	// DELETE
	//
	// Monta http response (em cada funcao que precisa)
	// Serializa
	// Envia
}
