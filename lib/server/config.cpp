#include "server.hpp"
#include <sstream>

static ServerBlock _parseBlock(std::stringstream &ss) { ServerBlock sb; }

HttpServer::serverBlockVector const setup(const std::string &config_file) {
  serverBlockVector serverBlocks;
  std::ifstream configFile(config_file);
  if (!configFile.is_open()) {
    throw HttpServerException("Could not open config file");
  }
  std::string line;
  while (std::getline(configFile, line)) {
    std::string host;
    unsigned short port;
    std::istringstream iss(line);
    iss >> host >> port;
    serverBlocks.push_back(ServerBlock(host, port));
  }
  return serverBlocks;
}