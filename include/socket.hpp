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

private:
  static const int MAX_HOSTNAME = 200;
  static const int MAX_CONNECTIONS = 5;

public:
  int sockfd;
  struct sockaddr_in serv_addr;
  bool is_valid;
  std::string host;
  std::string port;

public:
  Socket();
  Socket(const Socket &);
  Socket &operator=(const Socket &);
  ~Socket();

  Socket(const std::string &host, const std::string &port);
  int create();
  int bind_and_listen(const int port);
  void accept_connection(Socket &) const;
  bool connect(const std::string host, const int port);

  bool send(const std::string) const;
  bool send(const char *, int) const;
  bool receive(std::string &) const;
  bool receive(char *, int) const;

  void setNonBlocking(const bool);
  bool isReady() const;
  bool isValid() const;

  void _close();
};

#endif