#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <map>


class reserved_words_c {
public:
	reserved_words_c();
	std::set <std::string>	list;
	bool	is_reserved_word(std::string query_string);
};


class location {
public:
	// Constructor
	location();
	// Destructor
	~location();
	// Assignment operator
	location &operator=(const location &);

public:
	std::map<std::string, bool>			_allowed_methods;
	std::string							_redirection;
	std::string							_root;
	bool								_autoindex;
	std::vector<std::string>			_index;
	std::map<std::string, std::string>	_cgi_param;
	std::string							_cgi_pass;
	std::string							_upload_path;

public:
	void print_location();
};

class config_block_file {
public:
	// Constructor
	config_block_file();
	// Destructor
	~config_block_file();
	// Assignment operator
	config_block_file &operator=(const config_block_file &);
public:
	std::vector<int>					_listen;
	std::vector<std::string>			_server_name; // "host" do http request !
	int									_client_body_buffer_size; // max size for the client body, defaults to 8 000
	std::map<int, std::string>			_error_page; // error page redirections
	std::map<std::string, location>		_location; // map with n-location configs

public:
	void print_block_file();

};

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
	void parse_location(std::fstream &fs, std::string location_key, config_block_file  &cb);
	void printa_linha(std::fstream &fileStream);

};

// Rotina para ler arquivo e colocar em estrutura temporaria

// Rotina para traduzir da estrutura temporaria para a estrutura final



// Ler arquivo por blocks config_block_files
// No final de cada block, explodir para um map com chave server_name;listen
	// Varrer vector de config_block_file e para cada block fazer produto cartesiano
	// de server_name e listen
		// gerar chave
		// copiar bloco para map
		// acertar listen e server name para ter um so no bloco do map

	










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
///public:
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
