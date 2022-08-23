#include <cstdlib>
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


std::string req_handler::extract_location_from_url (std::string url) {
	size_t pos1 = url.find("/");
	size_t pos2 = url.find_last_of("/");
	std::string	s = url.substr(pos1 + 1, pos2 - pos1 - 1);
	if (pos1 == pos2) s = "";
	std::string location = "/" + s;
//	std::string location = "/" + s.substr(0, pos2);
	// debug prints
		std::cout << "pos1: " << pos1 << " " << s << std::endl;
		std::cout << "pos2: " << pos2 << " " << location << std::endl;

	return location;
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
//	std::string url =		"www.site1.com/images/";
	std::string port =		"81";
	std::string host =		"www.site1.com";
	int client_body_buffer_size =	1000;

	// Pega configs estrutura de configs:
	std::string conf_key = host + ":" + port;

	config_block_file	server_config;
	server_config = _parsed_config_map[conf_key];
		//debug prints
		std::ofstream f("teste", std::ofstream::trunc);
		server_config.print_block_file(f);
	if (server_config._block_name.compare("empty") == 0) { // REFATORAR!
		std::cout << "EMPTY !!!!\n";
		std::exit(2); // Nao tem config, nao fazer nada
	}
	else {
		std::cout << "NOT EMPTY !!!!\n";
	};

	// FROM NOW ON SERVER CONFIG ON MEMORY
		// Checa client_body_size se houver
		if (client_body_buffer_size > server_config._client_body_buffer_size) 
		{
			// erro
			std::cout << "BODY SIZE MAIOR QUE PERMITIDO\n";
			std::exit(3); // Retorna um erro
		};

// Pega location
	// Funcao extract_location_from_url:
		// Se nao tiver / eh /
		// Se tiver /
			// pega index da 2a barra
				// se nao tiver pega index do fim
			// pega da 1a / ate index
			// se nao tiver prox barra pega ate fim
	std::string loc = extract_location_from_url(url);
	std::cout << "location: " << loc << "\n";

	// Se tiver redirection, devolve redirection e sai.
	location loc_config = server_config._location[loc];
	//debug prints
		std::ofstream f2("teste2", std::ofstream::trunc);
		loc_config.print_location(f2);
	if (loc_config._redirection.compare("/") != 0) {
		std::cout << "TEM REDIRECTION !!!\n";
		std::cout << loc_config._redirection << "\n";
		std::exit(4); // Retorna um redirection
	}
	else {
		std::cout << "NAO TEM REDIRECTION !!!\n";
	};
		

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
