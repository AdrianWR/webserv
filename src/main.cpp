#include "log.hpp"
#include "server.hpp"
#include "socket.hpp"
#include "utils.hpp"
#include "config.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>

#define DEFAULT_CONFIG_PATH "./www/conf/conf"

void sigint_handler(int sig)
{
  LOG(INFO) << "Received SIGINT, shutting down...";
  LOG(DEBUG) << "Signal number: " << sig;
  exit(0);
}

static bool set_config_file(int argc, char **argv, std::string &file_path)
{
  if (argc > 2)
  {
    std::cout << "Usage: ./webserv [config_file_path]";
    return (EXIT_SUCCESS);
  }
  if (argc == 1)
    file_path = DEFAULT_CONFIG_PATH;
  else if (argc == 2)
    file_path = argv[1];
  return (true);
}

int main(int argc, char **argv)
{

  std::string config_file;
  // Check CLI arguments
  if (!set_config_file(argc, argv, config_file))
    return (1);

  // Read config file
  LOG(INFO) << "config file: " << config_file;
  Config config;
  if (!config.parse_file(config_file))
  {
    LOG(INFO) << "Error reading config file. Exiting";
    exit(1);
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
  try
  {
    HttpServer s;
    s.run(config);
  }
  catch (std::exception &e)
  {
    LOG(ERROR) << e.what();
  }
  return (EXIT_SUCCESS);
}
