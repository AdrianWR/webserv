#include "log.hpp"
#include "req_handler.hpp"
#include "server.hpp"
#include "socket.hpp"
#include "utils.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>

void sigint_handler(int sig) {
  LOG(INFO) << "Received SIGINT, shutting down...";
  LOG(DEBUG) << "Signal number: " << sig;
  exit(0);
}

bool is_in(int i, std::set<int> s) {
  if (s.find(i) == s.end())
    return false;
  return true;
}

int main(void) {

  Config config;
  config.parse_file("./www/conf/conf4");

  // std::cout << "rh: \n";
  // req_handler rh(config);
  // rh.handler(); // function input should be a http request

  // return 0;

  struct sigaction sigIntHandler;

  FILELog::ReportingLevel() = FILELog::FromString("DEBUG");

  sigIntHandler.sa_handler = sigint_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  LOG(INFO) << "Starting new webserver on port 8080";
  try {
    HttpServer s;
    s.run(config);
  } catch (std::exception &e) {
    LOG(ERROR) << e.what();
  }
  return (0);
}
