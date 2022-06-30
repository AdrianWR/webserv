#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

class ServerBlock {
private:
  std::string _host;
  unsigned short _port;

public:
  ServerBlock() {}
  ServerBlock(const std::string &host, unsigned short port)
      : _host(host), _port(port) {}

  std::string getHost() const { return _host; }
  unsigned short getPort() const { return _port; }

  int initializeServerSocket();
};

class HttpServer {
private:
  std::vector<ServerBlock> _servers;

public:
  HttpServer();
  HttpServer(const HttpServer &);
  HttpServer &operator=(const HttpServer &);
  ~HttpServer();

  HttpServer(const std::string &config_file);

  void setup(const std::string &config_file);
  void addServerBlock(const ServerBlock &serverBlock);
  void run();
};

#endif