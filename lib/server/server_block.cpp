#include "server.hpp"
#include "socket.hpp"

ServerBlock::ServerBlock() {}

ServerBlock::ServerBlock(const ServerBlock &serverBlock) {
  _server_names = serverBlock._server_names;
  _port = serverBlock._port;
  _socket = serverBlock._socket;
}

ServerBlock &ServerBlock::operator=(const ServerBlock &serverBlock) {
  if (this != &serverBlock) {
    _server_names = serverBlock._server_names;
    _port = serverBlock._port;
    _socket = serverBlock._socket;
  }
  return *this;
}

ServerBlock::~ServerBlock() {}