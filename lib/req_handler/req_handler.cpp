#include "req_handler.hpp"
#include "http.hpp"
// **********************************************************
// Class Req_handler
// **********************************************************
req_handler::req_handler() {
  std::cout << "req_handler constructor" << std::endl;
}
req_handler::req_handler(Config fp, HttpRequest req) {
	LOG(INFO) << "Initializing req_handler";
  	_parsed_config_map = fp.getBlockMap();
  	//	print_mapc(_parsed_config_map);
	HttpRequest::HeaderMap heard = req.getHeaders();
	_host = heard["host"].substr(0, heard["host"].find(":"));
	_port = heard["host"].substr(heard["host"].rfind(":") + 1);
	_method = heard["method"];
	_uri = _host + heard["path"];
	LOG(INFO) << "uri: " << _uri;

	// Fazer o post com o post
	// this->_client_max_body_size = 1000;
	// this->_request_body = "corpo do arquivo.\n";


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
	return ("." + str.substr(str.find("/")));
}

bool req_handler::check_redirection() {
	if (this->loc_config._redirection != "") {
		LOG(INFO) << "There is redirection ...";
		// Gera "erro"
		Error error(301, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
		return true;
	} else {
		LOG(INFO) << "No Redirection ...";
		return false;
	};
}

bool req_handler::check_method_allowed(std::string m) {
	if (this->loc_config._allowed_methods[m] == 0) {
		LOG(INFO) << m << " Not Allowed";
		Error error(405, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
		//
		return false;
	};
	LOG(INFO) << m << " Allowed";
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
	std::string full_path = path;
	std::string output = file_to_string(full_path);
	if (output.size() > 0) {
		LOG(INFO) << "File fetched ...";
		// Generate HTTP Response
		_http_response.set(200, "OK", output);
	} else {
		LOG(INFO) << "Error 404 Not Found";
		Error error(404, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
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
			LOG(INFO) << "ai_page:\n" << ai_page;
		// Generate HTTP Response
		_http_response.set(200, "OK", ai_page);
	} else {
	// se nao devolve erro
		LOG(INFO) << "404 No index";
		Error error(404, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
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
	LOG(INFO) << "GET Method ...";
	// Se tiver redirection, devolve redirection e sai.
	if (check_redirection()) return;

	
	// Checa se GET Permitido neste location
	if (!check_method_allowed("GET")) return;

	// 0) Se for cgi
	if (what_is_asked(this->_path) == "cgi") {
		LOG(INFO) << "CGI requested ...";
		fetch_cgi(this->_path);
	};
	// 1) Se for arquivoi:termina sem /
	if (what_is_asked(this->_path) == "file") {
		LOG(INFO) << "FILE requested...";
		fetch_file(this->_path);
	}
	// 2) Se for diretorio (termina em /)
	if (what_is_asked(_path) == "dir") {
		LOG(INFO) << "DIR requested...";
		fetch_dir(this->_path, this->_host, this->_port);
	};
	// ================================================================
}

void req_handler::handle_DELETE () {

	LOG(INFO) << "DELETE Method ...";
	// Checa se DELETE Permitido neste location
	if (!check_method_allowed("DELETE")) return;
	// Se for diretorio, forbidden
	if (what_is_asked(this->_path) == "dir") {
		LOG(INFO) << "DIR requested...";
		Error error(403, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
		return;
	}
	// Se nao existir arquivoL 404
	if (what_is_asked(this->_path) == "file") {
		LOG(INFO) << "FILE requested...";
		if (!file_exist(this->_path)) {
			Error error(404, this->server_config);
			// Generate HTTP Response
			_http_response.set(error.code, error.msg, error.body);
			return;
		}
		else {
			// Se existir, deleta
			LOG(INFO) << "Deleting file: " << this->_path;
			std::remove(this->_path.c_str());
			// Generate Http Response
			_http_response.set(200, "OK", "");
		}
	}
	// ================================================================
}

void req_handler::handle_POST () {
	LOG(INFO) << "POST ...";
	// Checa se POST Permitido neste location
	if (!check_method_allowed("POST")) return;
	// Checa tamanho
	if (_client_max_body_size > server_config._client_max_body_size) {
		LOG(INFO) << "413 Entity to large !";
		Error error(413, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
		return;
	}
	if (what_is_asked(this->_path) == "file") {
		LOG(INFO) << "FILE requested...";
		// Se tem upload path altera o path para usar o do config
		if (this->loc_config._upload_path != "") {
			// monta path
			_path = generate_path(this->_uri, this->_loc, this->loc_config._upload_path);
			LOG(INFO) << "upload path updated: " << _path;
		};
		// Salva arquivo criando diretorio
		LOG(INFO) << "POST OK";
		// se for multiform part data
		bool file_created = string_to_file(_path, _request_body);
		if (file_created) {
			// Generate HTTP Response
			_http_response.set(200,"OK", "");
			return;
		}
		else {
			Error error(404, this->server_config);
			// Generate HTTP Response
			_http_response.set(error.code, error.msg, error.body);
			return;
		}
	}
	if (what_is_asked(this->_path) == "dir") {
		LOG(INFO) << "DIR requested...";
		// Generate HTTP Response
		_http_response.set(200,"OK", "");
		return;
	};
	if (what_is_asked(this->_path) == "cgi") {
		LOG(INFO) << "CGI requested...";
		// Generate HTTP Response
		//
		return;
	}
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
	//
	LOG(DEBUG) << "handler() function ini";
	
//	this->_method = "POST";
//	this->_uri = "www.site1.com/images/aa";
//	this->_client_max_body_size = 1000;
//	this->_request_body = "corpo do arquivo.\n";
////

//	this->_method = "DELETE";
//	this->_uri = "www.site1.com/images/a";
//	
//	this->_method = "GET";
//	this->_uri = "www.site1.com/images/site1.html";
//	this->_uri = "www.site1.com/images/site9.html";
//	this->_uri = "www.site1.com/images/";
	//std::string uri =		"www.site1.com/images/photo1.png";
	//std::string uri =		"www.site1.com/images/algo.cgi";
//	this->_port = "8081";
//	this->_host = "www.site1.com";
	// QQR erro no request dispara um 400 bad request
	// ================================================================

	// ================================================================
	// Load Configs
	// ================================================================
	// Populates class atributes with inputs and config values
	load_configs(); // FROM NOW ON SERVER CONFIG ON MEMORY
	//
	// Monta caminho fisico:
	_path = generate_path(this->_uri, this->_loc, this->loc_config._root);
	LOG(DEBUG) << "uri: " << _uri;
	LOG(DEBUG) << "loc: " << _loc;
	LOG(DEBUG) << "root: " << loc_config._root;

	LOG(INFO) << "path generated: " << _path;
	 
	// ================================================================
	// START OF PARSING (one function for each method)
	//              GET POST DELETE OTHER
	// ================================================================
	if (this->_method == "GET") {
		handle_GET();
		return;
	}
	if (this->_method == "POST") {
	// POST
		handle_POST();
		return;
	}
	if (this->_method == "DELETE") {
	// DELETE
		handle_DELETE();
		return;
	}
	// UNKNOWN
	Error error(405, this->server_config);
	// Generate HTTP Response
	_http_response.set(error.code, error.msg, error.body);
	
	return;
}
