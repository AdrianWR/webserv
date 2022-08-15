#include "req_handler.hpp"

// Construtor
req_handler::req_handler(){
	std::cout << "req_handler constructor" << std::endl;
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
