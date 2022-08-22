#include "req_handler.hpp"

// Construtor
req_handler::req_handler(){
	std::cout << "req_handler constructor" << std::endl;
}
req_handler::req_handler(file_parser_c fp){
	std::cout << "receiving file parser as fp" << std::endl;
	std::cout << "####################################\n";
	_parsed_config_map = fp._config_map;
//	print_mapc(_parsed_config_map);
}
// Destrutor
req_handler::~req_handler() {
	std::cout << "req_handler destructor" << std::endl;
}
// Asssignment
req_handler &req_handler::operator=(const req_handler &s) {
  if (this != &s) return *this;
  return *this;
}

void req_handler::handler() {

	// Pega config ok
		// Construtor recebe um objeto file_parser_c

	// Recebe um http request object
	// Do request, pega:
		// 1a linha:
			// Metodo (GET)
			// url
		// 2a linha:
			// port
			// host
				// vao formar chave para config
	std::string method =	"GET";
	std::string url =		"www.site1.com/images/photo1.png";
	std::string port =		"81";
	std::string host =		"www.site1.com";
//	int client_body_size =	1000;

	// Pega configs estrutura de configs:
	std::string conf_key = host + ":" + port;

	config_block_file	server_config;
	server_config = _parsed_config_map[conf_key];
		//debug prints
		//std::ofstream f("teste", std::ofstream::trunc);
		//server_config.print_block_file(f);
	if (server_config._block_name.compare("empty") == 0) {
		std::cout << "EMPTY !!!!\n";
	}
	else {
		std::cout << "NOT EMPTY !!!!\n";
	};



	

	// Checa client_body_size se houver
	
		// ok ou
		// erro

// Pega location
	// Se tiver redirection, devolve redirection e sai.

// Monta caminho fisico:
	// pega location
	// pega root
	// troca location por root no url

// Se get permitido:
// 1) Se nao tiver extensao
	// Se tiver index
		// loop
			// monta caminho com um dos index
			// devolve
		// se nao houver
			// se autoindex on executa autoindex
			// se nao devolve erro

// 2) Se tiver extensao
// Tenta pegar arquivo.
	// Se existir, devolve
	// se nao existir, erro

// 3) Se for cgi
	// Monta environment
	// Monta args
	// executa (fork etc)
	// devolve output

// Monta http response
}
