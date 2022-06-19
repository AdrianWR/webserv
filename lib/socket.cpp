#include "socket.hpp"
// #include <cerrno>
// #include <iostream>
#include <unistd.h>

Socket::Socket() : sockfd(-1), host(""), port(0) {}

Socket::Socket(const Socket &s)
    : sockfd(s.sockfd), host(s.host), port(s.port) {}

Socket &Socket::operator=(const Socket &s) {
  if (this != &s) {
    sockfd = s.sockfd;
    host = s.host;
    port = s.port;
  }
  return *this;
}

// Host and Port Constructor
Socket::Socket(const std::string &host, const unsigned short &port)
    : host(host), port(port) {
  sockfd = Socket::_socket(SIN_FAMILY, SOCK_STREAM, 0);
}

Socket::~Socket() { _close(); }

int Socket::_socket(int domain, int type, int protocol) {
  if ((sockfd = socket(domain, type, protocol)) < 0) {
    throw SocketException("Could not create socket");
  }
  return sockfd;
}

void Socket::_close() {
  int err;
  if (sockfd != -1) {
    err = close(sockfd);
    if (err == -1) {
      throw SocketException("Error closing socket");
    }
    sockfd = -1;
  }
}