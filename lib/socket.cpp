#include "socket.hpp"
#include <cerrno>
#include <iostream>
#include <unistd.h>

Socket::Socket() : sockfd(-1), host(""), port("") {}

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
Socket::Socket(const std::string &host, const std::string &port)
    : sockfd(-1), host(host), port(port) {}

Socket::~Socket() { _close(); }

int Socket::create() {
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    throw SocketException("Could not create socket");
  }
  return sockfd;
}

int Socket::bind_and_listen(const int port) {

  int err;

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  if ((bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
    std::cout << errno << std::endl;
    throw SocketException("Could not bind socket");
  }

  if (listen(sockfd, Socket::MAX_CONNECTIONS) < 0) {
    throw SocketException("Could not listen on socket");
  }

  return sockfd;
}

void Socket::accept_connection(Socket &s) const {
  int addr_length = sizeof(s.serv_addr);
  s.sockfd = accept(sockfd, (struct sockaddr *)&s.serv_addr,
                    (socklen_t *)&addr_length);
  if (s.sockfd < 0) {
    throw SocketException("Could not accept connection");
  }
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