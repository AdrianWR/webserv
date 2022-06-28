#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main () {
	std::string line;
	std::string content;
	std::stringstream buffer;
	size_t content_size;
	std::ifstream myfile ("./site1/site1.html");
	if (myfile.is_open())
	{
		while (getline(myfile,line)) { buffer << line; };
		myfile.close();
		content = buffer.str();
		content_size = content.length();
			std::cout << content << ":" << content_size << "\n";
	}
	else
		std::cout << "Unable to open file"; 
	return 0;
}
