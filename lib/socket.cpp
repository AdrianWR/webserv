#include "socket.hpp"
#include <iostream>

Socket::Socket() : sockfd(-1), host(""), port("") {
  std::cout << "Socket::Socket()" << std::endl;
}

Socket::~Socket() { std::cout << "Socket::~Socket()" << std::endl; }