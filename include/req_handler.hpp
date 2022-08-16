#ifndef REQ_HANDLER_HPP
#define REQ_HANDLER_HPP

#include "utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

class req_handler {
public:
	// Constructor
	req_handler();
	req_handler(file_parser_c fp);
//	req_handler(std::string fp);
	// Destructor
	~req_handler();
	// Assignment operator
	req_handler &operator=(const req_handler &s);
};
#endif

// Do request, pega:
	// 1a linha:
		// Metodo (GET)
		// url
	// 2a linha:
		// port
		// host
			// vao formar chave para config

// Pega config ok
	// Construtor recebe um objeto file_parser_c ?

// Recebe um http request object

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


