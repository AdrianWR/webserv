#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <cerrno>
#include <cstring>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class Socket {

public:
  class SocketException : public std::exception {
  public:
    SocketException(const std::string &msg) : message(msg) {
      if (errno != 0) {
        message += ": ";
        message += strerror(errno);
      };
    }
    ~SocketException() throw() {}
    const char *what() const throw() { return message.c_str(); }

  private:
    std::string message;
  };

protected:
  static const int MAX_HOSTNAME = 200;
  static const int SIN_FAMILY = AF_INET;

  int _socket(int domain, int type, int protocol);
  void _close();

public:
  int sockfd;
  int sockfd_client;
  bool is_valid;
  std::string host;
  unsigned short int port;

public:
  Socket();
  Socket(const Socket &);
  Socket &operator=(const Socket &);
  virtual ~Socket();

  Socket(const std::string &host, const unsigned short &port);

  int bind_and_listen(const int port);
  int accept_connection() const;
  bool connect(const std::string host, const int port);

  void start_server(void);

  void setNonBlocking(const bool);
  bool isReady() const;
  bool isValid() const;
};

class TCPServerSocket : public Socket {
protected:
  static const int MAX_CONNECTIONS = 5;

  void _bind();
  void _listen();
  void _setsockopt();

public:
  TCPServerSocket();
  TCPServerSocket(const TCPServerSocket &);
  TCPServerSocket &operator=(const TCPServerSocket &);
  TCPServerSocket(const std::string &host, const unsigned short &port);
  ~TCPServerSocket();

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