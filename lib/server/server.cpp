#include "server.hpp"
#include "log.hpp"

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

void HttpServer::run() {
  if (_servers.size() == 0) {
    LOG(WARN) << "No servers to run. Exiting...";
    return;
  }
}
