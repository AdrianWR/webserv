#include "socket.hpp"
#include <stdlib.h>
#include <unistd.h>

TCPServerSocket::TCPServerSocket() {}

TCPServerSocket::TCPServerSocket(const TCPServerSocket &s) {}

TCPServerSocket &TCPServerSocket::operator=(const TCPServerSocket &s) {
  if (this != &s) {
    sockfd = s.sockfd;
    host = s.host;
    port = s.port;
  }
  return *this;
}

TCPServerSocket::TCPServerSocket(const std::string &host,
                                 const unsigned short &port)
    : Socket(host, port) {
  sockfd = Socket::_socket(SIN_FAMILY, SOCK_STREAM, 0);
  _setsockopt();
  _bind();
  _listen();
}

TCPServerSocket::~TCPServerSocket() {}

void TCPServerSocket::server() {
  while (true) {
    struct sockaddr_storage their_addr;
    int addr_length = sizeof their_addr;

    int new_fd = accept(sockfd, (struct sockaddr *)NULL, NULL);
    if (new_fd < 0) {
      throw SocketException("Could not accept client connection");
    }

    if (!fork()) {
      close(sockfd);
      if (send(new_fd, "Hello, world!", 13, 0) < 0) {
        throw SocketException("Could not send message");
      }
      close(new_fd);
      exit(0);
    }
  }
}

void TCPServerSocket::_setsockopt(void) {
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
    throw SocketException("Could not set socket options");
  }
}

void TCPServerSocket::_bind(void) {
  struct sockaddr_in serv_addr;

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    throw SocketException("Could not bind socket");
  }
}

void TCPServerSocket::_listen(void) {
  if (listen(sockfd, MAX_CONNECTIONS) < 0) {
    throw SocketException("Could not listen on socket");
  }
}