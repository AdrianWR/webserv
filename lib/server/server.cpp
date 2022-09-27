#include "req_handler.hpp"
#include "server.hpp"
#include "http.hpp"
#include "log.hpp"
#include "socket.hpp"
#include "utils.hpp"
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

void HttpServer::_handleConnection(int &fd, Config &config) {
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
    request.parse(buffer);
    req_handler rh(config, request);

    // (void)config;
    // HttpResponse response = HttpHandler::generateResponse(request, config);
    // (void)response;

    // headers = request.getHeaders();
	rh.handler();
//	std::string buff = rh._http_response.serialize();
    std::string buff = "HTTP/1.1 200 OK\nContent-Type: "
                       "text/plain\nContent-Length: 12\n\nHello world!i\n";
		LOG(INFO) << "buffer: " << buffer;

    if (send(fd, buff.c_str(), buff.size(), 0) < 0) {
      throw HttpServerException("Error writing to socket");
    }
  }
}

HttpServer::SocketsVector HttpServer::_initSockets(Config config) {
  Config::PortSet ports = config.getAvailablePorts();

  HttpServer::SocketsVector sockets;
  TCPServerSocket *s;

  Config::PortSet::const_iterator it;
  for (it = ports.begin(); it != ports.end(); it++) {
    s = new TCPServerSocket(*it);
    sockets.push_back(s);
  }

  return sockets;
}

void HttpServer::run(Config config) {

  HttpServer::SocketsVector sockets = _initSockets(config);
  // Config::PortSet ports = config.getAvailablePorts();
  // HttpServer::SocketsVector sockets(ports.begin(), ports.end());

  pollFdVector fds;
  SocketsVector::iterator it;
  for (it = sockets.begin(); it != sockets.end(); it++) {
    fds.push_back((*it)->getPollfd());
  }
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
        _handleConnection(fds[i].fd, config);
      } else if (fds[i].fd == -1) {
        fds.erase(fds.begin() + i);
      }
    }
  }
}
