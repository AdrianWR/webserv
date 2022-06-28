#include "log.hpp"
#include "socket.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>

void sigint_handler(int sig) {
  LOG(INFO) << "Received SIGINT, shutting down...";
  LOG(DEBUG) << "Signal number: " << sig;
  exit(0);
}

int main(void) {
  struct sigaction sigIntHandler;

  FILELog::ReportingLevel() = FILELog::FromString("DEBUG");

  sigIntHandler.sa_handler = sigint_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  LOG(INFO) << "Starting new webserver on port 8080";
  try {
    TCPServerSocket s("localhost", 8080, NON_BLOCKING);
    s.server();
  } catch (std::exception &e) {
    LOG(ERROR) << e.what();
  }
  return (0);
}