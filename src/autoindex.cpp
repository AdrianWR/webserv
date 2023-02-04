#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>
#include "log.hpp"
#include "autoindex.hpp"

// *****************************************************
// Class AutoIndexGenerator
// *****************************************************

AutoIndexGenerator::AutoIndexGenerator(void) { return; }

AutoIndexGenerator::AutoIndexGenerator(AutoIndexGenerator const &src)
{
  (void)src;
  return;
}

AutoIndexGenerator::~AutoIndexGenerator(void) { return; }

AutoIndexGenerator &
AutoIndexGenerator::operator=(AutoIndexGenerator const &src)
{
  (void)src;
  return *this;
}

std::string AutoIndexGenerator::getPage(const char *path,
                                        std::string const &host, int port,
                                        std::string &uri_path)
{
  std::string dirName(path);
  LOG(DEBUG) << "dirname: " << dirName;
  DIR *dir = opendir(path);
  std::string page = "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
            <title>" +
                     dirName + "</title>\n\
    </head>\n\
    <body>\n\
    <h1>INDEX</h1>\n\
    <p>\n";

  if (dir == NULL)
  {
    std::cerr << "Error: could not open [" << path << "]" << std::endl;
    return "";
  }
  if (!uri_path.empty() && uri_path[uri_path.length() - 1] == '/')
    uri_path.erase(uri_path.end() - 1);
  for (struct dirent *dirEntry = readdir(dir); dirEntry;
       dirEntry = readdir(dir))
  {
    page += AutoIndexGenerator::getLink(std::string(dirEntry->d_name), uri_path, host, port);
  }
  page += "\
    </p>\n\
    </body>\n\
    </html>\n";
  closedir(dir);
  return page;
}

std::string AutoIndexGenerator::getLink(std::string const &dirEntry,
                                        std::string const &uri_path,
                                        std::string const &host, int port)
{
  std::stringstream ss;
  ss << "\t\t<p><a href=\"http://" + host + ":" << port
     << uri_path + "/" + dirEntry + "\">" + dirEntry + "</a></p>\n";
  return ss.str();
}
