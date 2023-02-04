#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <string>

// *****************************************************88
// Class AutoIndexGeerator
// *****************************************************88

class AutoIndexGenerator
{
public:
  AutoIndexGenerator(void);
  AutoIndexGenerator(AutoIndexGenerator const &src);
  virtual ~AutoIndexGenerator(void);

  AutoIndexGenerator &operator=(AutoIndexGenerator const &src);

  static std::string getPage(const char *path, std::string const &host, int port, std::string &uri_path);

private:
  static std::string getLink(std::string const &dirEntry, std::string const &uri_path, std::string const &host, int port);
};

#endif
