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

int main(int argc, char **argv) {

	// Check CLI arguments
	if (argc != 2) {
		std::cout << "USAGE: ./webserv [config_file]\n";
		return 1;
	}
	
	// Read config file
	std::string config_file(argv[1]);
	LOG(INFO) << "config file: " << config_file;
	Config config;
	if (!config.parse_file(config_file)) {
		LOG(INFO) << "Error reading config file. Exiting";
		exit (1);
	}
	
	// Test autoindex generation
//	std::cout << "Pagina de auto_index:\n";
//	AutoIndexGenerator auto_index;
//	std::string ai_page = auto_index.getPage(".","www.site1",8080);
//	std::cout << ai_page << std::endl;
//	std::ofstream ai("autoindex.html", std::ofstream::trunc);
//	ai << ai_page;

	// // Run req_handler
	// req_handler rh(config);
	// rh.handler(); // function input should be a http request
	// return 0;

  struct sigaction sigIntHandler;

  FILELog::ReportingLevel() = FILELog::FromString("INFO");

  sigIntHandler.sa_handler = sigint_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  LOG(INFO) << "Starting new webserver";
  try {
    HttpServer s;
    s.run(config);
  } catch (std::exception &e) {
    LOG(ERROR) << e.what();
  }
  return (0);
}
