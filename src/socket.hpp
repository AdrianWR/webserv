#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <cerrno>
#include <cstring>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

enum BlockingMode { BLOCKING, NON_BLOCKING };

class Socket {

public:
  class SocketException : public std::exception {
  public:
    SocketException(const std::string &msg) : message(msg) {
    }
    ~SocketException() throw() {}
    const char *what() const throw() { return message.c_str(); }

  private:
    std::string message;
  };

protected:
  static const int MAX_HOSTNAME = 200;
  static const int SIN_FAMILY = AF_INET;
  static const unsigned int BUFFER_SIZE = 1024;

  int _socket(int domain, int type, int protocol);
  void _close();

public:
  int listener_sockfd;
  bool is_valid;
  unsigned short int _port;
  enum BlockingMode _mode;

public:
  Socket();
  Socket(const Socket &);
  Socket &operator=(const Socket &);
  virtual ~Socket();

  Socket(const unsigned short &port, enum BlockingMode mode = NON_BLOCKING);

  void setNonBlocking(const bool);
  bool isReady() const;
  bool isValid() const;
};

class TCPServerSocket : public Socket {
protected:
  static const int MAX_CONNECTIONS = 5;
  void _setsockopt(int sockfd);
  void _bind(int listener_sockfd, unsigned short port);
  void _listen(int listener_sockfd);
  void _blocking_server();
  void _non_blocking_server();

public:
  TCPServerSocket();
  TCPServerSocket(const TCPServerSocket &);
  TCPServerSocket &operator=(const TCPServerSocket &);
  TCPServerSocket(const unsigned short &port,
                  enum BlockingMode mode = NON_BLOCKING);
  ~TCPServerSocket();

  static int accept_connection(int listener_sockfd);
  void handleConnection(int client_sockfd);
  struct pollfd getPollfd() const;

  void server();
};

class TCPClientSocket : public Socket {
public:
  TCPClientSocket();
  TCPClientSocket(const TCPClientSocket &);
  TCPClientSocket &operator=(const TCPClientSocket &);
  ~TCPClientSocket();

private:
  void _connect();
};

#endif
