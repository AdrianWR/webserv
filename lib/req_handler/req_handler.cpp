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
	std::cout << "Unable to open file\n";
	return "";
	}
	// Se arquivo nao existe
	// Se autoindex == on, serve executa autoindex.php
	// Se nao ,erro
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
	error_dic[403] = "Forbidden";
	error_dic[404] = "Not Found";
	error_dic[405] = "Method Not Allowed";
	error_dic[400] = "Bad Request";
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
	(void) code;
	(void) sc;
	body = "ERROR BODY !!!!";

	return body;
}

void Error::print_error() {
	std::cout << code << " : " << msg << std::endl;
	std::cout << "body: " << body << std::endl;
}





// **********************************************************
// Req_handler class
// **********************************************************
req_handler::req_handler() {
  std::cout << "req_handler constructor" << std::endl;
}
req_handler::req_handler(Config fp) {
  std::cout << "receiving file parser as fp" << std::endl;
  std::cout << "####################################\n";
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

std::string req_handler::extract_location (ConfigBlock sc, std::string uri) {
	ConfigBlock::MapOfLocations::iterator	it;
	std::string key;
	bool match;
	size_t p;

	size_t pos1 = uri.find("/");
	std::string s = uri.substr(pos1);
	match = false;
	for (it = sc._location.begin(); it != sc._location.end(); it++) {
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

void req_handler::check_redirection(LocationBlock loc_config ) {
	if (loc_config._redirection != "") {
		std::cout << "TEM REDIRECTION !!!\n";
		std::cout << loc_config._redirection << "\n";
		std::exit(4); // Retorna um redirection
	} else {
		std::cout << "NAO TEM REDIRECTION !!!\n";
	};
}

void req_handler::check_method_GET(LocationBlock loc_config) {
	if (loc_config._allowed_methods["GET"] == 0) {
		std::cout << "Error 405 Method not Allowed\n";
		exit(5);
	};
}

std::string req_handler::what_is_asked(std::string path) {
	if (path.find(".cgi") != std::string::npos) {
		std::cout << "ask for cgi\n";
		return "cgi";
	}
	if (path.find_last_of("/") == path.size() - 1){
		std::cout << "ask for dir\n";
		return "dir";
	}
	else {
		std::cout << "ask for file\n";
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
		std::cout << "***********************************\n";
		std::cout << " HTTP RESPONSE \n";
		std::cout << "***********************************\n";
		std::cout << output << std::endl;
		std::cout << "***********************************\n";
	} else {
		std::cout << "Nao achou arquivo ! Erro 404\n";
	};
}

void req_handler::try_index_page(std::string path, LocationBlock loc_config) {
	// loop
	for (size_t i = 0; i < loc_config._index.size(); i++) {
		// monta caminho com um dos index
		std::string full_path = "." + path + loc_config._index[i];
			std::cout << "full_path: " << full_path << std::endl;
		// devolve
		std::string output = file_to_string(full_path);
		if (output.size() > 0) {
			std::cout << "|" << full_path << "|: ";
			std::cout << "|" << output << "|" << std::endl;
		break;
		};
	};
}

void req_handler::try_autoindex(LocationBlock loc_config,
								std::string host,
								std::string port) {
	// se autoindex on executa autoindex
	if (loc_config._autoindex == true) {
		std::cout << "Executando autoindex ...\n";
		AutoIndexGenerator auto_index;
		std::string ai_page = auto_index.getPage(".",host, StringToInt(port));
		std::cout << "======================\n";
		std::cout << ai_page << std::endl;
		std::cout << "======================\n";
	} else {
	// se nao devolve erro
		std::cout << "Erro 404 !\n\n";
	};
}

void req_handler::fetch_dir(std::string path,
							LocationBlock loc_config,
							std::string host,
							std::string port) {
	if (loc_config._index.size() > 0) {// Se tiver index
		std::cout << "tem index\n";
		try_index_page(path, loc_config);
	} else { // se nao houver
		try_autoindex(loc_config, host, port);
	};
}

void req_handler::handler() {

	// Recebe um http request object
	// Do request, pega:
	// 1a linha:
	// Metodo (GET)
	// url
	// 2a linha:
	// port
	// host
	// vao formar chave para config
	std::string method = "GET";
//	std::string uri = "www.site1.com/images/site1.html";
	std::string uri = "www.site1.com/images/";
	//std::string uri =		"www.site1.com/images/photo1.png";
	//std::string uri =		"www.site1.com/images/algo.cgi";
	std::string port = "8081";
	std::string host = "www.site1.com";
	int client_max_body_size = 1000;
	(void) client_max_body_size;

	// Pega configs na estrutura de configs:
	std::string conf_key = host + ":" + port;
	ConfigBlock server_config = _parsed_config_map[conf_key];
		// debug prints
		std::ofstream f("server_config.txt", std::ofstream::trunc);
		server_config.print_block_file(f);
		
	// FROM NOW ON SERVER CONFIG ON MEMORY
	
	// SO PARA POST ???
	// GERA ERRO 413 Entity to large
//	// Checa client_body_size se houver (VALIDO APENAS PARA POSTS ???)
//	if (client_max_body_size > server_config._client_max_body_size) {
//	// erro
//		LOG(ERROR) << "BODY SIZE MAIOR QUE PERMITIDO\n";
//		std::exit(3); // Retorna um erro
//	};

	// Pega location
//	std::string a1;
//	a1 = extract_location(server_config, "www.site.com/aaa/images/");
//	std::cout << "exctracted: " << a1 << "\n";
//	a1 = extract_location(server_config, "www.site.com/images/aaa/bbb/");
//	std::cout << "exctracted: " << a1 << "\n";
//	a1 = extract_location(server_config, "www.site.com/images1/aaa/bbb/");
//	std::cout << "exctracted: " << a1 << "\n";

	// Extract location
	std::string loc = extract_location(server_config, uri);
		std::cout << "location: " << loc << "\n";

	// Carrega configs da location na memoria
	LocationBlock loc_config = server_config._location[loc];
		// debug prints
		std::ofstream f2("location_config.txt", std::ofstream::trunc);
		loc_config.print_location(f2);


	// Rotina de erro
	// objeto erro
	//		input: cod do erro
	//		output: 
	//			cod
	//			msg
	//			body
	//	dic de erros
	//	funcao para gerar pag de erro (fetch)
	//
	// Se houver erro
	//	gera codigo de erro
	//	gera msg de erro
	//	gera body
	//		se houver config tenta pegar arquivo
	//			se falhar string erro padrao
	//		se nao houver config
	//			string erro padrao
	//
	// Em cada lugar que gera erro:
	// Constroi objeto erro com codigo
	//	Objeto prenche campos
	// Gera response a partir do erro
	//

	// Se tiver redirection, devolve redirection e sai.
	check_redirection(loc_config);

	// Monta caminho fisico:
	std::string path = generate_path(uri, loc, loc_config._root);
	std::cout << "path: " << path << std::endl;
	
	// Checa se GET Permitido neste location
	check_method_GET(loc_config);
	// Se get permitido:
	std::cout << "GET ALLOWED !!!!\n";
	// 0) Se for cgi
	if (what_is_asked(path) == "cgi") {
		std::cout << "CGI ...\n";
		fetch_cgi(path);
	};
	// 1) Se for arquivoi:termina sem /
	if (what_is_asked(path) == "file") {
		std::cout << "FILE ...\n";
		fetch_file(path);
	}
	// 2) Se for diretorio (termina em /)
	if (what_is_asked(path) == "dir") {
		std::cout << "DIR ...\n";
		fetch_dir(path, loc_config, host, port);
	};
	// Monta http response
	Error e1(200, server_config);
	e1.print_error();
	Error e2(4004, server_config);
	e2.print_error();

}
