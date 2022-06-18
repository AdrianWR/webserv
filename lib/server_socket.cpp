#include "socket.hpp"

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
                                 const unsigned short &port) {
  sockfd = Socket::_socket(SIN_FAMILY, SOCK_STREAM, 0);
  std::cout << "sockfd: " << sockfd << std::endl;
  std::cout << "host: " << host << std::endl;
  std::cout << "port: " << port << std::endl;
  _setsockopt();
  _bind();
  _listen();
}

TCPServerSocket::~TCPServerSocket() {}

void TCPServerSocket::_setsockopt(void) {
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
    throw SocketException("Could not set socket options");
  }
}

void TCPServerSocket::_bind(void) {
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
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