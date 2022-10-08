#include "socket.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

Socket::Socket() : listener_sockfd(-1), _port(0), _mode(NON_BLOCKING) {}

Socket::Socket(const Socket &s)
    : listener_sockfd(s.listener_sockfd), _port(s._port), _mode(s._mode) {}

Socket &Socket::operator=(const Socket &s) {
  if (this != &s) {
    listener_sockfd = s.listener_sockfd;
    _port = s._port;
    _mode = s._mode;
  }
  return *this;
}

/**
 * @brief Creates a new internet socket file descriptor and sets it to the
 * specified blocking mode.
 *
 * @param port The port of the server to connect to. (e.g. 8080)
 * @param mode The blocking mode to set the socket to. Defaults to NON_BLOCKING.
 * @return int The file descriptor of the new socket.
 * @throws SocketException If the socket could not be created.
 **/
Socket::Socket(const unsigned short &port, enum BlockingMode mode)
    : _port(port), _mode(mode) {
  listener_sockfd = Socket::_socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() { _close(); }

int Socket::_socket(int domain, int type, int protocol) {
  if ((listener_sockfd = socket(domain, type, protocol)) < 0) {
    throw SocketException("Could not create socket");
  }
  if (_mode == NON_BLOCKING) {
    if (fcntl(listener_sockfd, F_SETFL, O_NONBLOCK) < 0) {
      throw SocketException("Error setting socket to non-blocking");
    }
  }
  return listener_sockfd;
}

void Socket::_close() {
  int err;
  if (listener_sockfd != -1) {
    err = close(listener_sockfd);
    if (err == -1) {
      throw SocketException("Error closing socket");
    }
    listener_sockfd = -1;
  }
}
