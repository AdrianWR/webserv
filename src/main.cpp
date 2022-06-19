#include "log.hpp"
#include "socket.hpp"
#include <iostream>

int main(void) {

  LOG(INFO) << "Starting new webserver on port 8080";
  TCPServerSocket s("localhost", 8080);
  // s.server();
  return (0);
}