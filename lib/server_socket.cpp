#include "log.hpp"
#include "socket.hpp"

#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

TCPServerSocket::TCPServerSocket() {}

TCPServerSocket::TCPServerSocket(const TCPServerSocket &s) {}

TCPServerSocket &TCPServerSocket::operator=(const TCPServerSocket &s) {
  if (this != &s) {
    listener_sockfd = s.listener_sockfd;
    host = s.host;
    port = s.port;
    _mode = s._mode;
  }
  return *this;
}

/**
 * @brief Creates a new internet socket file descriptor and sets it to the
 * specified blocking mode. The socket created should be a server socket, ready
 * to accept incoming connections.
 *
 * @param host The hostname of the server to connect to. (e.g. "localhost")
 * @param port The port of the server to connect to. (e.g. 8080)
 * @param mode The blocking mode to set the socket to. Defaults to NON_BLOCKING.
 * @return int The file descriptor of the new socket.
 * @throws SocketException If the socket could not be created.
 **/
TCPServerSocket::TCPServerSocket(const std::string &host,
                                 const unsigned short &port,
                                 enum BlockingMode mode)
    : Socket(host, port, mode) {
  _setsockopt(listener_sockfd);
  _bind(listener_sockfd, port);
  _listen(listener_sockfd);
}

TCPServerSocket::~TCPServerSocket() {}

void TCPServerSocket::server() {
  if (_mode == BLOCKING) {
    _blocking_server();
  }
  if (_mode == NON_BLOCKING) {
    _non_blocking_server();
  }
}

void TCPServerSocket::_blocking_server() {

  struct sockaddr_storage their_addr;
  int addr_length = sizeof their_addr;

  LOG(INFO) << "Listening for connections on " << host << ":" << port;
  while (true) {

    int new_fd = accept(listener_sockfd, (struct sockaddr *)NULL, NULL);
    if (new_fd < 0) {
      throw SocketException("Could not accept client connection");
    }

    if (!fork()) {
      close(listener_sockfd);
      if (send(new_fd, "Hello, world!", 13, 0) < 0) {
        throw SocketException("Could not send message");
      }
      close(new_fd);
      exit(0);
    }
  }
}

void TCPServerSocket::_non_blocking_server() {
  fd_set current_socket_set;
  fd_set ready_socket_set;
  int max_fd = listener_sockfd;

  LOG(INFO) << "Initializing synchronous I/O select multiplexer";
  FD_ZERO(&current_socket_set);
  FD_SET(listener_sockfd, &current_socket_set);
  max_fd = listener_sockfd;

  for (;;) {
    LOG(INFO) << "Selecting on " << max_fd + 1 << " file descriptors";
    ready_socket_set = current_socket_set;
    if (select(max_fd, &ready_socket_set, NULL, NULL, NULL) < 0) {
      throw SocketException("Could not select");
    }
    LOG(INFO) << "Select returned";
    for (int i = 0; i < max_fd; i++) {
      if (FD_ISSET(i, &ready_socket_set)) {
        if (i == listener_sockfd) {
          int client_sockfd = _accept(listener_sockfd);
          LOG(DEBUG) << "Accepted connection from " << host << ":" << port;
          FD_SET(client_sockfd, &current_socket_set);
          if (client_sockfd > max_fd) {
            max_fd = client_sockfd;
          }
        } else {
          LOG(DEBUG) << "Handling connection from " << host << ":" << port;
          handleConnection(i);
          FD_CLR(i, &current_socket_set);
        }
      }
    }
  }
}

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

int TCPServerSocket::_accept(int listener_sockfd) {
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
  // char buffer[BUFFER_SIZE];
  // int bytes_read = recv(client_sockfd, buffer, BUFFER_SIZE, 0);
  // if (bytes_read < 0) {
  //   throw SocketException("Could not read from socket");
  // }
  // LOG(DEBUG) << "Received " << bytes_read << " bytes from client";
  // if (send(client_sockfd, buffer, bytes_read, 0) < 0) {
  //   throw SocketException("Could not send message");
  // }
  LOG(DEBUG) << "Received connection from client";
  if (send(client_sockfd, "Hello, world!", 13, 0) < 0) {
    throw SocketException("Could not send message");
  }
  close(client_sockfd);
}