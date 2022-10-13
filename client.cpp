// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

void	send(int port, std::string msg) {
	int	sock;
	struct sockaddr_in serv_addr;
	char buffer[4096] = {0};
	std::string content(msg);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << std::endl << RED << "< Socket creation error >" << RESET << std::endl << std::endl;
	return ;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);


	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		std::cout << std::endl << RED << "< Connection failed >" << RESET << std::endl << std::endl;
		return ;
	}

	content += "\r\n";

	std::cout << std::endl << "Sending :" << std::endl;
	std::cout << "[" << RED << content << RESET << "]" << std::endl << std::endl;

	send(sock, content.c_str(), content.size(), 0);
	read(sock, buffer, 4095);

	std::cout << std::endl << "Response :" << std::endl;
	std::cout << "[" << GREEN << std::string(buffer) << RESET << "]" << std::endl << std::endl;

	close(sock);

	return;
}

int main(void) {
	int port = 8081;
	std::string msg;
	std::string del = "\r\n";

	msg = "HTTP/1.1 GET www.site1.com:8081/getok/" + del;

	send(port, msg);

	return (0);
}
