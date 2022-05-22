#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class Socket {
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

  bool create();
  bool bind(const int port);
  bool listen() const;
  bool accept(Socket &) const;
  bool connect(const std::string host, const int port);

  bool send(const std::string) const;
  bool send(const char *, int) const;
  bool receive(std::string &) const;
  bool receive(char *, int) const;

  void setNonBlocking(const bool);
  bool isReady() const;
  bool isValid() const;

  void close();
};

#endif