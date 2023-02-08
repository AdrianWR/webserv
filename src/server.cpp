#include "server.hpp"
#include "http_request_handler.hpp"
#include "log.hpp"
#include "socket.hpp"
#include "utils.hpp"
#include <cstring>
#include <poll.h>

HttpServer::HttpServer() {}

HttpServer::~HttpServer()
{
	size_t i;
	for (i = 0; i < _sockets.size(); i++)
	{
		delete _sockets[i];
	}
}

HttpServer::HttpServer(const HttpServer &httpServer)
		: _sockets(httpServer._sockets) {}

HttpServer &HttpServer::operator=(const HttpServer &httpServer)
{
	if (this != &httpServer)
	{
		_sockets = httpServer._sockets;
	}
	return *this;
}

void HttpServer::_handleConnection(int &fd, Config &config)
{
	std::string buffer;
	std::string temp;
	char c = {0};
	int bytes_read = 1;

	while (bytes_read > 0)
	{
		// Read statusline and headers
		bytes_read = recv(fd, &c, 1, 0);
		if (bytes_read <= 0)
			break;
		buffer += c;
		if (ends_in_two_delimiters(buffer))
		{
			// Read body
			size_t found = buffer.find("POST");
			size_t found2 = buffer.find("chunked");
			if (found != std::string::npos && found2 == std::string::npos)
			{
				found = buffer.find("Content-Length: ");
				temp = buffer.substr(found);
				size_t pos = temp.rfind("\r\n");
				temp = temp.substr(0, pos);
				found = temp.find(" ");
				temp = temp.substr(found + 1);
				int v = atoi(temp.c_str());
				LOG(DEBUG) << "v: " << v;
				for (int i = 0; i < v; i++)
				{
					bytes_read = recv(fd, &c, 1, 0);
					if (bytes_read <= 0)
						break;
					buffer += c;
				}
			}
			break;
		}
	}
	if (bytes_read < 0)
	{
		close(fd);
		fd = -1;
	}
	else
	{
		HttpRequest message(buffer.c_str(), fd);
		HttpRequestHandler &requestHandler = HttpRequestFactory::makeRequest(message);
		HttpResponse response = requestHandler.create_response(config);

		std::string buff = response.serialize();
		if (send(fd, buff.c_str(), buff.size(), 0) <= 0)
		{
			close(fd);
			fd = -1;
			throw HttpServerException("Error writing to socket");
		}
		delete &requestHandler;
	}
	close(fd);
}

HttpServer::SocketsVector HttpServer::_initSockets(Config config)
{
	Config::PortSet ports = config.getAvailablePorts();

	HttpServer::SocketsVector sockets;
	TCPServerSocket *s;

	Config::PortSet::const_iterator it;
	for (it = ports.begin(); it != ports.end(); it++)
	{
		s = new TCPServerSocket(*it);
		sockets.push_back(s);
	}

	return sockets;
}

void HttpServer::run(Config config)
{
	pollFdVector fds;

	_sockets = _initSockets(config);
	for (SocketsVector::iterator it = _sockets.begin(); it != _sockets.end(); it++)
	{
		fds.push_back((*it)->getPollfd());
	}
	std::size_t listeners_size = fds.size();

	LOG(INFO) << "Initializing server FDs polling";
	while (true)
	{
		int ret = poll(&*fds.begin(), fds.size(), -1);

		if (ret < 0)
		{
			throw HttpServerException("Error polling socket");
		}

		// Check for new connections to accept
		for (std::size_t i = 0; i < listeners_size; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				int new_fd = TCPServerSocket::accept_connection(fds[i].fd);
				struct pollfd new_pollfd = {new_fd, POLLIN | POLLOUT, 0};
				fds.push_back(new_pollfd);
				LOG(DEBUG) << "New fd accepted: " << new_fd;
			}
		}

		// Check for new clients to handle
		for (std::size_t i = listeners_size; i < fds.size(); i++)
		{
			if (fds[i].fd > 0 && (fds[i].revents & POLLIN || fds[i].revents & POLLOUT))
			{
				try
				{
					_handleConnection(fds[i].fd, config);
				}
				catch (std::exception &e)
				{
					LOG(ERROR) << e.what();
				}
			}
			else if (fds[i].fd <= 0)
			{
				fds.erase(fds.begin() + i);
			}
		}
	}
}
