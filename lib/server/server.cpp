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

void HttpServer::_handleConnection(int &fd) {
  char buffer[BUFFER_SIZE];

  int bytes_read = recv(fd, buffer, BUFFER_SIZE, 0);
  if (bytes_read < 0) {
    throw HttpServerException("Error reading from socket");
  }
  if (bytes_read == 0) {
    close(fd);
    fd = -1;
  } else {
    HttpRequest request;
    HttpRequest::HeaderMap headers = request.parse(buffer);

    headers = request.getHeaders();
    std::string buff = "HTTP/1.1 200 OK\nContent-Type: "
                       "text/plain\nContent-Length: 12\n\nHello world!";

    if (send(fd, buff.c_str(), bytes_read, 0) < 0) {
      throw HttpServerException("Error writing to socket");
    }
  }
}

void HttpServer::run() {

  TCPServerSocket s1("localhost", 8080);
  TCPServerSocket s2("localhost", 8081);

  pollFdVector fds;

  fds.push_back(s1.getPollfd());
  fds.push_back(s2.getPollfd());

  std::size_t listeners_size = fds.size();

  LOG(INFO) << "Initializing server FDs polling";
  while (true) {
    int ret = poll(&*fds.begin(), fds.size(), -1);
    if (ret < 0) {
      throw HttpServerException("Error polling socket");
    }
    // Check for new connections to accept
    for (std::size_t i = 0; i < listeners_size; i++) {
      if (fds[i].revents & POLLIN) {
        int new_fd = TCPServerSocket::accept_connection(fds[i].fd);
        struct pollfd new_pollfd = {new_fd, POLLIN, 0};
        fds.push_back(new_pollfd);
      }
    }
    // Check for new clients to handle
    for (std::size_t i = listeners_size; i < fds.size(); i++) {
      if (fds[i].fd > 0 && fds[i].revents & POLLIN) {
        _handleConnection(fds[i].fd);
      } else if (fds[i].fd == -1) {
        fds.erase(fds.begin() + i);
      }
    }
  }
}
