#ifndef SERVER_HPP
#define SERVER_HPP

#include "socket.hpp"
#include "utils.hpp"
#include "config.hpp"
#include <map>
#include <string>
#include <vector>

class HttpServer
{

public:
  typedef std::vector<TCPServerSocket *> SocketsVector;
  typedef std::vector<struct pollfd> pollFdVector;
  typedef pollFdVector::iterator pollFdVectorIterator;

public:
  class HttpServerException : public std::exception
  {
  private:
    std::string _message;

  public:
    HttpServerException(const std::string &message) : _message(message) {}
    ~HttpServerException() throw() {}
    virtual const char *what() const throw() { return _message.c_str(); }
  };

private:
  static const unsigned int BUFFER_SIZE = 1024;

  HttpServer::SocketsVector _sockets;

  static void _handleConnection(int &it, Config &config);
  static SocketsVector _initSockets(Config config);

public:
  HttpServer();
  HttpServer(const HttpServer &);
  HttpServer &operator=(const HttpServer &);
  ~HttpServer();

  void run(Config config);
};

#endif
