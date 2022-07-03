#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <fstream>

class file_parser_c {
public:
	// Construtor
	file_parser_c();
	// Destrutor
	~file_parser_c();
	// Asssignment
	file_parser_c &operator=(const file_parser_c &);

	// Atributos

	// Metodos
	void teste();

	void le_arquivo(std::string arquivo);
	void printa_linha(std::fstream &fileStream);

};


//
//class Socket {
//
//public:
//  class SocketException : public std::exception {
//  public:
//    SocketException(const std::string &msg) : message(msg) {
//      if (errno != 0) {
//        message += ": ";
//        message += strerror(errno);
//      };
//    }
//    ~SocketException() throw() {}
//    const char *what() const throw() { return message.c_str(); }
//
//  private:
//    std::string message;
//  };
//
//protected:
//  static const int MAX_HOSTNAME = 200;
//  static const int SIN_FAMILY = AF_INET;
//  static const unsigned int BUFFER_SIZE = 1024;
//
//  int _socket(int domain, int type, int protocol);
//  void _close();
//
//public:
//  int listener_sockfd;
//  bool is_valid;
//  std::string host;
//  unsigned short int _port;
//  enum BlockingMode _mode;
//
//public:
//  Socket();
//  Socket(const Socket &);
//  Socket &operator=(const Socket &);
//  virtual ~Socket();
//
//  Socket(const std::string &host, const unsigned short &port,
//         enum BlockingMode mode = NON_BLOCKING);
//
//  void setNonBlocking(const bool);
//  bool isReady() const;
//  bool isValid() const;
//};
//
//class TCPServerSocket : public Socket {
//protected:
//  static const int MAX_CONNECTIONS = 5;
//  void _setsockopt(int sockfd);
//  void _bind(int listener_sockfd, unsigned short port);
//  void _listen(int listener_sockfd);
//  int _accept(int listener_sockfd);
//  void _blocking_server();
//  void _non_blocking_server();
//
//public:
//  TCPServerSocket();
//  TCPServerSocket(const TCPServerSocket &);
//  TCPServerSocket &operator=(const TCPServerSocket &);
//  TCPServerSocket(const std::string &host, const unsigned short &port,
//                  enum BlockingMode mode = NON_BLOCKING);
//  ~TCPServerSocket();
//
//  void handleConnection(int client_sockfd);
//
//  void server();
//};
//
//class TCPClientSocket : public Socket {
//public:
//  TCPClientSocket();
//  TCPClientSocket(const TCPClientSocket &);
//  TCPClientSocket &operator=(const TCPClientSocket &);
//  ~TCPClientSocket();
//
//private:
//  void _connect();
//};
//
#endif
