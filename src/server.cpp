#include "req_handler.hpp"
#include "server.hpp"
#include "http.hpp"
#include "log.hpp"
#include "socket.hpp"
#include "utils.hpp"
#include <cstring>
#include <poll.h>

HttpServer::HttpServer() {}

HttpServer::~HttpServer() {

	LOG(INFO) << "HttpServer Destructor ...";
	size_t i;
	for (i = 0; i < _sockets.size(); i++) {
		delete _sockets[i];
	}
//	//  TCPServerSocket *s;
//	Config::PortSet::const_iterator it;
//	for (it = _ports.begin(); it != _ports.end(); it++) {
//		//    s = new TCPServerSocket(*it);
//		delete *it;
//	}n
}

HttpServer::HttpServer(const HttpServer &httpServer)
    : _servers(httpServer._servers) {}

HttpServer &HttpServer::operator=(const HttpServer &httpServer) {
  if (this != &httpServer) {
    _servers = httpServer._servers;
  }
  return *this;
}

void HttpServer::addServerBlock(const ServerBlock &serverBlock) {
  _servers.push_back(serverBlock);
}


void HttpServer::_handleConnection(int &fd, Config &config) {
//  char buffer[BUFFER_SIZE + 1] = {0};
	std::string buffer;
	char c = {0};
	int bytes_read = 1;
	int total_bytes_read = 0;

//  int total_bytes_read = recv(fd, buffer, BUFFER_SIZE, 0);
	while (bytes_read > 0) {
		bytes_read = recv(fd, &c, 1, 0);
		buffer += c;
		total_bytes_read++;
		if (ends_in_two_delimiters(buffer)) {
			break;
		}
	}
	LOG(DEBUG) << "total_bytes_read: " << total_bytes_read;
	if (bytes_read <= 0) {
		LOG(DEBUG) << "CLOSING FD: " << fd;
		close(fd);
		fd = -1;
	} else {
		HttpRequest request;
		request.parse(buffer.c_str(), fd);

		req_handler rh(config, request);
		rh.handler();
		std::string buff = rh._http_response.serialize();
//    std::string buff = "HTTP/1.1 200 OK\nContent-Type: "
//                       "text/plain\nContent-Length: 13\n\nHello world!\n";
		LOG(INFO) << "buffer size: " << buff.size();
		LOG(INFO) << "buffer:\n|" << buff << "|";;

		if (send(fd, buff.c_str(), buff.size(), 0) < 0) {
			throw HttpServerException("Error writing to socket");
		}
	}
	close(fd);
}

HttpServer::SocketsVector HttpServer::_initSockets(Config config) {
  Config::PortSet ports = config.getAvailablePorts();

  HttpServer::SocketsVector sockets;
  TCPServerSocket *s;

  Config::PortSet::const_iterator it;
  for (it = ports.begin(); it != ports.end(); it++) {
    s = new TCPServerSocket(*it);
    sockets.push_back(s);
  }

  return sockets;
}

void HttpServer::run(Config config) {

//  HttpServer::SocketsVector sockets = _initSockets(config);
 _sockets = _initSockets(config);
  // Config::PortSet ports = config.getAvailablePorts();
  // HttpServer::SocketsVector sockets(ports.begin(), ports.end());

  pollFdVector fds;
  SocketsVector::iterator it;
  for (it = _sockets.begin(); it != _sockets.end(); it++) {
    fds.push_back((*it)->getPollfd());
  }
  std::size_t listeners_size = fds.size();

  LOG(INFO) << "Initializing server FDs polling";
  while (true) {
    int ret = poll(&*fds.begin(), fds.size(), -1);
    if (ret < 0) {
      throw HttpServerException("Error polling socket");
    }

    // Check for new connections to accept
    for (std::size_t i = 0; i < listeners_size; i++) {
      if (fds[i].revents & POLLIN) {
        int new_fd = TCPServerSocket::accept_connection(fds[i].fd);
        struct pollfd new_pollfd = {new_fd, POLLIN, 0};
        fds.push_back(new_pollfd);
		LOG(DEBUG) << "New fd accepted: " << new_fd;
      }
    }

    // Check for new clients to handle
    for (std::size_t i = listeners_size; i < fds.size(); i++) {
      if (fds[i].fd > 0 && fds[i].revents & POLLIN) {
		  LOG(DEBUG) << "WILL HANDLE FD: " << fds[i].fd;
        _handleConnection(fds[i].fd, config);
			LOG(DEBUG) << "Finished handling";
      } else if (fds[i].fd <= 0) {
        fds.erase(fds.begin() + i);
      }
    }
  }
}
