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

std::string	req_handler::generate_path(std::string url, std::string location, std::string root) {

	std::string str = url.replace(url.find(location),location.length(),root);
	return str.substr(str.find("/"));

}

std::string file_to_string(std::string file_path)
{
	std::string			line;
	std::stringstream	buffer;
	std::ifstream		myfile(file_path.c_str());

	// Se arquivo existe, serve arquivo
	if (myfile.is_open())
	{
		while (getline(myfile,line)) { buffer << line; };
		myfile.close();
		return buffer.str();
	}
	else
	{
		std::cout << "Unable to open file"; 
		return "";
	}
	// Se arquivo nao existe
		// Se autoindex == on, serve executa autoindex.php
		// Se nao ,erro
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
	std::string url =		"www.site1.com/images/index.html";
//	std::string url =		"www.site1.com/images/photo1.png";
//	std::string url =		"www.site1.com/images/";
	std::string port =		"81";
	std::string host =		"www.site1.com";
	int client_body_buffer_size =	1000;

	// Pega configs na estrutura de configs:
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
	std::string loc = extract_location_from_url(url);
	std::cout << "location: " << loc << "\n";

	// Carrega configs da location an memoria
	location loc_config = server_config._location[loc];
	//debug prints
		std::ofstream f2("teste2", std::ofstream::trunc);
		loc_config.print_location(f2);
	// Se tiver redirection, devolve redirection e sai.
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

	std::string path = generate_path(url, loc, loc_config._root);
	std::cout << "path: " << path << std::endl;

	// Se get permitido:
	if (loc_config._allowed_methods["GET"] == 1)
	{
		std::cout << "GET ALLOWED !!!!\n";

		// 1) Se tiver extensao
			// Tenta pegar arquivo.
				// Se existir, devolve
				// se nao existir, erro
		if (path.find(".") != std::string::npos)
		{
			std::string full_path = "." + path;
			std::string output = file_to_string("./www/site1/siter1.html");
			std::cout << "***********************************\n";
			std::cout << " HTTP RESPONSE \n";
			std::cout << "***********************************\n";
			std::cout << output << std::endl;
			std::cout << "***********************************\n";
		};

	// 2) Se nao tiver extensao
		// Se tiver index
			// loop
				// monta caminho com um dos index
				// devolve
			// se nao houver
				// se autoindex on executa autoindex
				// se nao devolve erro

	// 3) Se for cgi
		// Monta environment
		// Monta args
		// executa (fork etc)
		// devolve output
	}
	else {
		std::cout << "GET NOT ALLOWED !!!";
		std::exit(5);
	};

	// Monta http response
}
