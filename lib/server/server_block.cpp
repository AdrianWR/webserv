#include "server.hpp"
#include "socket.hpp"

ServerBlock::ServerBlock() {}

ServerBlock::ServerBlock(const ServerBlock &serverBlock)
    : _host(serverBlock._host), _port(serverBlock._port) {}

ServerBlock::~ServerBlock() {}

ServerBlock::ServerBlock(const std::string &host, unsigned short port)
    : _host(host), _port(port) {
  _socket = TCPServerSocket(_host, _port, NON_BLOCKING);
}