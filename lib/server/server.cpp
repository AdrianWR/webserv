#include "server.hpp"
#include "http.hpp"
#include "log.hpp"
#include "socket.hpp"
#include <cstring>
#include <poll.h>

HttpServer::HttpServer() {}

HttpServer::~HttpServer() {}

HttpServer::HttpServer(const HttpServer &httpServer)
    : _servers(httpServer._servers) {}

HttpServer &HttpServer::operator=(const HttpServer &httpServer) {
  if (this != &httpServer) {
    _servers = httpServer._servers;
  }
  return *this;
}

void HttpServer::addServerBlock(const ServerBlock &serverBlock) {
  _servers.push_back(serverBlock);
}

void HttpServer::_handleConnection(int fd, int &nfds) {
  LOG(INFO) << "Handling connection on fd " << fd;
  char buffer[BUFFER_SIZE];
  int bytes_read = recv(fd, buffer, BUFFER_SIZE, 0);
  if (bytes_read < 0) {
    throw HttpServerException("Error reading from socket");
  }
  if (bytes_read == 0) {
    close(fd);
    fd = -1;
    nfds--;
  } else {
    // LOG(DEBUG) << "Sending response to client:\n" << buffer;

    HttpRequest request;
    HttpRequest::HeaderMap headers = request.parse(buffer);

    headers = request.getHeaders();

    LOG(ERROR) << "Headers: " << headers;
    std::string buff = "HTTP/1.1 200 OK\nContent-Type: "
                       "text/plain\nContent-Length: 12\n\nHello world!";

    if (send(fd, buff.c_str(), bytes_read, 0) < 0) {
      throw HttpServerException("Error writing to socket");
    }
  }
}

void HttpServer::run() {
  // if (_servers.size() == 0) {
  //   LOG(WARN) << "No servers to run. Exiting...";
  //   return;
  // }
  TCPServerSocket tcp("localhost", 8080, NON_BLOCKING);
  TCPServerSocket tcp2("localhost", 8081, NON_BLOCKING);

  struct pollfd fds[200];
  int nfds = 2;

  LOG(INFO) << "Initializing server polling";
  memset(fds, 0, sizeof(fds));
  fds[0].fd = tcp.listener_sockfd;
  fds[0].events = POLLIN;
  fds[1].fd = tcp2.listener_sockfd;
  fds[1].events = POLLIN;

  while (true) {
    int ret = poll(fds, nfds, -1);
    if (ret < 0) {
      throw HttpServerException("Error polling socket");
    }
    if (fds[0].revents & POLLIN) {
      int new_fd = tcp._accept(tcp.listener_sockfd);
      if (new_fd < 0) {
        throw HttpServerException("Could not accept client connection");
      }
      fds[nfds].fd = new_fd;
      fds[nfds].events = POLLIN;
      nfds++;
    } else if (fds[1].revents & POLLIN) {
      int new_fd = tcp._accept(tcp2.listener_sockfd);
      if (new_fd < 0) {
        throw HttpServerException("Could not accept client connection");
      }
      fds[nfds].fd = new_fd;
      fds[nfds].events = POLLIN;
      nfds++;
    } else {
      for (int i = 1; i < nfds; i++) {
        if (fds[i].revents & POLLIN) {
          _handleConnection(fds[i].fd, nfds);
        }
      }
    }
  }
}