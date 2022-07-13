#ifndef SERVER_HPP
#define SERVER_HPP

#include "socket.hpp"
#include <string>
#include <vector>

class ServerBlock {
private:
  std::string _host;
  unsigned short _port;
  TCPServerSocket _socket;

public:
  ServerBlock();
  ServerBlock(const ServerBlock &serverBlock);
  ~ServerBlock();
  ServerBlock(const std::string &host, unsigned short port);

  std::string getHost() const { return _host; }
  unsigned short getPort() const { return _port; }
  TCPServerSocket getSocket() const { return _socket; }

  int initializeServerSocket();
};

class HttpServer {

public:
  typedef std::vector<struct pollfd> pollFdVector;
  typedef pollFdVector::iterator pollFdVectorIterator;

public:
  class HttpServerException : public std::exception {
  private:
    std::string _message;

  public:
    HttpServerException(const std::string &message) : _message(message) {}
    ~HttpServerException() throw() {}
    virtual const char *what() const throw() { return _message.c_str(); }
  };

private:
  static const unsigned int BUFFER_SIZE = 1024;

  std::vector<ServerBlock> _servers;

  void _handleConnection(int &it);

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