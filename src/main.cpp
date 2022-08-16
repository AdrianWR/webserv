#include "log.hpp"
#include "server.hpp"
#include "socket.hpp"
#include "utils.hpp"
#include "req_handler.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>

void sigint_handler(int sig) {
  LOG(INFO) << "Received SIGINT, shutting down...";
  LOG(DEBUG) << "Signal number: " << sig;
  exit(0);
}

bool is_in(int i, std::set <int> s) {
		if (s.find(i) == s.end())
			return false;
		return true;
	}

int main(void) {

	file_parser_c	file_parser;
	file_parser.le_arquivo("./www/conf/conf4");

	std::cout << "rh: \n";
	req_handler	rh(file_parser);
	rh.handler();	// function input should be a http request

	return 0;

  struct sigaction sigIntHandler;

  FILELog::ReportingLevel() = FILELog::FromString("DEBUG");

  sigIntHandler.sa_handler = sigint_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  LOG(INFO) << "Starting new webserver on port 8080";
  try {
    // s.server();
    HttpServer s;
    s.run();
  } catch (std::exception &e) {
    LOG(ERROR) << e.what();
  }
  return (0);
}
