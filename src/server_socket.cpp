#include "http_request.hpp"
#include "log.hpp"
#include "socket.hpp"

#include <poll.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

TCPServerSocket::TCPServerSocket() {}

TCPServerSocket::TCPServerSocket(const TCPServerSocket &s) : Socket(s) {}

TCPServerSocket &TCPServerSocket::operator=(const TCPServerSocket &s) {
  if (this != &s) {
    listener_sockfd = s.listener_sockfd;
    _port = s._port;
    _mode = s._mode;
  }
  return *this;
}

/**
 * @brief Creates a new internet socket file descriptor and sets it to the
 * specified blocking mode. The socket created should be a server socket, ready
 * to accept incoming connections.
 *
 * @param port The port of the server to connect to. (e.g. 8080)
 * @param mode The blocking mode to set the socket to. Defaults to NON_BLOCKING.
 * @return int The file descriptor of the new socket.
 * @throws SocketException If the socket could not be created.
 **/
TCPServerSocket::TCPServerSocket(const unsigned short &port,
                                 enum BlockingMode mode)
    : Socket(port, mode) {
  _setsockopt(listener_sockfd);
  _bind(listener_sockfd, port);
  _listen(listener_sockfd);
}

TCPServerSocket::~TCPServerSocket() {}

void TCPServerSocket::_setsockopt(int listener_sockfd) {
  int yes = 1;
  if (setsockopt(listener_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) <
      0) {
    throw SocketException("Could not set socket options");
  }
}

void TCPServerSocket::_bind(int listener_sockfd, unsigned short port) {
  struct sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(listener_sockfd, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    throw SocketException("Could not bind socket");
  }
}

void TCPServerSocket::_listen(int listener_sockfd) {
  if (listen(listener_sockfd, MAX_CONNECTIONS) < 0) {
    throw SocketException("Could not listen on socket");
  }
}

int TCPServerSocket::accept_connection(int listener_sockfd) {
  struct sockaddr_storage in_addr; // sockaddr_storage works for IPv4 and IPv6
  socklen_t addr_length;

  addr_length = sizeof(in_addr);
  int fd = accept(listener_sockfd, (struct sockaddr *)&in_addr, &addr_length);
  if (fd < 0) {
    throw SocketException("Could not accept client connection");
  }
  return fd;
}

void TCPServerSocket::handleConnection(int client_sockfd) {
  LOG(DEBUG) << "Received connection from client";
  if (send(client_sockfd, "Hello, world!", 13, 0) < 0) {
    throw SocketException("Could not send message");
  }
  close(client_sockfd);
}

struct pollfd TCPServerSocket::getPollfd() const {
  struct pollfd fd;

  fd.fd = listener_sockfd;
  fd.events = POLLIN;
  return fd;
}
