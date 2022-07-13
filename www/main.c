#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

std::string file_to_string(std::string file_path)
{
	std::string			line;
	std::stringstream	buffer;
	std::ifstream		myfile(file_path);

	// Se arquivo existe, serve arquivo
	if (myfile.is_open())
	{
		while (getline(myfile,line)) { buffer << line; };
		myfile.close();
		return buffer.str();
	}
	else
	{
		std::cout << "Unable to open file"; 
		return "";
	}
	// Se arquivo nao existe
		// Se autoindex == on, serve executa autoindex.php
		// Se nao ,erro
}

int main () {
	std::string content = file_to_string("./site1/site1.html");
	size_t content_size = content.length();
	std::cout << content << ":" << content_size << "\n";
	return 0;
}
