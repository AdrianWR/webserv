#include "req_handler.hpp"

// Construtor
rh::rh(){
	std::cout << "req_handler constructor" << std::endl;
}
// Destrutor
rh::~rh() {
	std::cout << "req_handler destructor" << std::endl;
}
// Asssignment
rh &rh::operator=(const rh &s) {
  if (this != &s) return *this;
  return *this;
}
