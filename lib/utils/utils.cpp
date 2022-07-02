#include "utils.hpp"

// Construtor
file_parser_c::file_parser_c(){
	std::cout << "file parser constructor" << std::endl;
}
// Destrutor
file_parser_c::~file_parser_c() {
	std::cout << "file parser destructor" << std::endl;
}
// Asssignment
file_parser_c &file_parser_c::operator=(const file_parser_c &s) {
  if (this != &s) return *this;
  return *this;
}

// Atributos

// Metodos
void file_parser_c::teste(){
	std::cout << "teste metodo file_paser\n";
}

//TCPServerSocket::TCPServerSocket() {}
//
//TCPServerSocket::TCPServerSocket(const TCPServerSocket &s) : Socket(s) {}
//
//TCPServerSocket &TCPServerSocket::operator=(const TCPServerSocket &s) {
//  if (this != &s) {
//    listener_sockfd = s.listener_sockfd;
//    host = s.host;
//    _port = s._port;
//    _mode = s._mode;
//  }
//  return *this;
//}
//
///**
// * @brief Creates a new internet socket file descriptor and sets it to the
// * specified blocking mode. The socket created should be a server socket, ready
// * to accept incoming connections.
// *
// * @param host The hostname of the server to connect to. (e.g. "localhost")
// * @param port The port of the server to connect to. (e.g. 8080)
// * @param mode The blocking mode to set the socket to. Defaults to NON_BLOCKING.
// * @return int The file descriptor of the new socket.
// * @throws SocketException If the socket could not be created.
// **/
//TCPServerSocket::TCPServerSocket(const std::string &host,
//                                 const unsigned short &port,
//                                 enum BlockingMode mode)
//    : Socket(host, port, mode) {
//  _setsockopt(listener_sockfd);
//  _bind(listener_sockfd, port);
//  _listen(listener_sockfd);
//}
//
//TCPServerSocket::~TCPServerSocket() {}
//
//void TCPServerSocket::server() {
//  if (_mode == BLOCKING) {
//    _blocking_server();
//  }
//  if (_mode == NON_BLOCKING) {
//    _non_blocking_server();
//  }
//}
//
//void TCPServerSocket::_blocking_server() {
//
//  LOG(INFO) << "Listening for connections on " << host << ":" << _port;
//  while (true) {
//
//    int new_fd = accept(listener_sockfd, (struct sockaddr *)NULL, NULL);
//    if (new_fd < 0) {
//      throw SocketException("Could not accept client connection");
//    }
//
//    if (!fork()) {
//      close(listener_sockfd);
//      if (send(new_fd, "Hello, world!", 13, 0) < 0) {
//        throw SocketException("Could not send message");
//      }
//      close(new_fd);
//      exit(0);
//    }
//  }
//}
//
//void TCPServerSocket::_non_blocking_server() {
//  struct pollfd fds[200];
//  int nfds = 1;
//
//  LOG(INFO) << "Initializing server polling";
//  memset(fds, 0, sizeof(fds));
//  fds[0].fd = listener_sockfd;
//  fds[0].events = POLLIN;
//
//  while (true) {
//    int ret = poll(fds, nfds, -1);
//    if (ret < 0) {
//      throw SocketException("Error polling socket");
//    }
//    // LOG(DEBUG) << "Successfully polled client socket. Current events: " <<
//    // ret;
//    if (fds[0].revents & POLLIN) {
//      int new_fd = _accept(listener_sockfd);
//      if (new_fd < 0) {
//        throw SocketException("Could not accept client connection");
//      }
//      fds[nfds].fd = new_fd;
//      fds[nfds].events = POLLIN;
//      nfds++;
//    } else {
//      for (int i = 1; i < nfds; i++) {
//        if (fds[i].revents & POLLIN) {
//          char buffer[BUFFER_SIZE];
//          int bytes_read = recv(fds[i].fd, buffer, BUFFER_SIZE, 0);
//          if (bytes_read < 0) {
//            throw SocketException("Error reading from socket");
//          }
//          if (bytes_read == 0) {
//            close(fds[i].fd);
//            fds[i].fd = -1;
//            nfds--;
//          } else {
//            // LOG(DEBUG) << "Sending response to client:\n" << buffer;
//
//            HttpRequest request;
//            HttpRequest::HeaderMap headers = request.parse(buffer);
//            headers = request.getHeaders();
//
//            LOG(ERROR) << "Headers: " << headers;
//            std::string buff = "HTTP/1.1 200 OK\nContent-Type: "
//                               "text/plain\nContent-Length: 12\n\nHello world!";
//
//            if (send(fds[i].fd, buff.c_str(), bytes_read, 0) < 0) {
//              throw SocketException("Error writing to socket");
//            }
//          }
//        }
//      }
//    }
//  }
//}
//
//void TCPServerSocket::_setsockopt(int listener_sockfd) {
//  int yes = 1;
//  if (setsockopt(listener_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) <
//      0) {
//    throw SocketException("Could not set socket options");
//  }
//}
//
//void TCPServerSocket::_bind(int listener_sockfd, unsigned short port) {
//  struct sockaddr_in server_addr;
//
//  memset(&server_addr, 0, sizeof(server_addr));
//  server_addr.sin_family = AF_INET;
//  server_addr.sin_addr.s_addr = INADDR_ANY;
//  server_addr.sin_port = htons(port);
//
//  if (bind(listener_sockfd, (struct sockaddr *)&server_addr,
//           sizeof(server_addr)) < 0) {
//    throw SocketException("Could not bind socket");
//  }
//}
//
//void TCPServerSocket::_listen(int listener_sockfd) {
//  if (listen(listener_sockfd, MAX_CONNECTIONS) < 0) {
//    throw SocketException("Could not listen on socket");
//  }
//}
//
//int TCPServerSocket::_accept(int listener_sockfd) {
//  struct sockaddr_storage in_addr; // sockaddr_storage works for IPv4 and IPv6
//  socklen_t addr_length;
//
//  addr_length = sizeof(in_addr);
//  int fd = accept(listener_sockfd, (struct sockaddr *)&in_addr, &addr_length);
//  if (fd < 0) {
//    throw SocketException("Could not accept client connection");
//  }
//  return fd;
//}
//
//void TCPServerSocket::handleConnection(int client_sockfd) {
//  // char buffer[BUFFER_SIZE];
//  // int bytes_read = recv(client_sockfd, buffer, BUFFER_SIZE, 0);
//  // if (bytes_read < 0) {
//  //   throw SocketException("Could not read from socket");
//  // }
//  // LOG(DEBUG) << "Received " << bytes_read << " bytes from client";
//  // if (send(client_sockfd, buffer, bytes_read, 0) < 0) {
//  //   throw SocketException("Could not send message");
//  // }
//  LOG(DEBUG) << "Received connection from client";
//  if (send(client_sockfd, "Hello, world!", 13, 0) < 0) {
//    throw SocketException("Could not send message");
//  }
//  close(client_sockfd);
//}
