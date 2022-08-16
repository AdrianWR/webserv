#include "req_handler.hpp"
#include "utils.hpp"

// Construtor
req_handler::req_handler(){
	std::cout << "req_handler constructor" << std::endl;
}
req_handler::req_handler(file_parser_c fp){
	std::cout << "receiving file parser as fp" << std::endl;
	std::cout << "####################################\n";
	print_mapc(fp._config_map);
//	std::cout << fp << "----\n";
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
