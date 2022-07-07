#include "log.hpp"
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

bool is_in(int i, std::set <int> s) {
		if (s.find(i) == s.end())
			return false;
		return true;
	}

int main(void) {

	file_parser_c	file_parser;
	std::cout << "Rodei\n";
	file_parser.teste();
	file_parser.le_arquivo("./www/conf/conf1");

	config_block	block;
	block = config_block();
	block.print();

	reserved_words_c rw;
	std::cout << "server_name: " << rw.is_reserved_word("server_name") << std::endl;
	std::cout << "paulo: " << rw.is_reserved_word("paulo") << std::endl;

	return 0;

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
