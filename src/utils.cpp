#include "utils.hpp"

// **********************************************************
// Utility functions
// **********************************************************

static bool has_delimiter(std::string line, std::string delimiter)
{
  return (line.rfind(delimiter) != std::string::npos);
}

void receive_line(int fd, std::string &line, std::string delimiter)
{
  char buffer[2] = {0};
  ssize_t num_of_bytes;
  std::string temp_line;

  while (true)
  {
    num_of_bytes = recv(fd, buffer, 1, 0);
    if (num_of_bytes == ERROR)
      throw(std::exception());
    if (num_of_bytes <= 0)
      break;
    temp_line += buffer;
    if (has_delimiter(temp_line, delimiter))
      break;
  }
  line = temp_line;
  if (has_delimiter(temp_line, delimiter))
    line.resize(line.rfind(delimiter));
}

bool ends_in_two_delimiters(std::string buffer)
{
  size_t pos = 0;

  pos = buffer.rfind(DOUBLE_CRLF);
  return (pos + 4 == buffer.size() && buffer.size() > 4);
}

std::string file_to_string(std::string file_path)
{
  std::string line;
  std::stringstream buffer;
  std::ifstream myfile(file_path.c_str(), std::ifstream::in);

  LOG(INFO) << " ======= begin file_to_string ========";
  LOG(INFO) << "file_path: " << file_path;

  // Se arquivo existe, serve arquivo
  if (myfile.is_open())
  {
    buffer << myfile.rdbuf();
    myfile.close();
    return buffer.str();
  }
  else
  {
    LOG(INFO) << "Unable to open file";
    return "";
  }
}

bool string_to_file(std::string file_path, std::string str)
{
  std::ofstream new_file;

  new_file.open(file_path.c_str(), std::ios::binary);
  if (!new_file.is_open())
    return false;
  new_file.write(str.c_str(), str.length());
  new_file.close();
  return true;
}

std::string IntToString(int a)
{
  std::ostringstream temp;

  temp << a;
  return temp.str();
}

size_t StringToInt(std::string s)
{
  std::stringstream temp;
  size_t i;

  temp << s;
  temp >> i;

  return i;
}

bool file_exist(std::string path)
{
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

std::string path_is(std::string path)
{
  struct stat s;

  if (stat(path.c_str(), &s) == 0)
  {
    if (s.st_mode & S_IFDIR)
    {
      // it's a directory
      return ("dir");
    }
    else if (s.st_mode & S_IFREG)
    {
      // it's a file
      return ("file");
    }
  }
  return ("error");
}

bool end_in_slash(std::string str)
{
  char a = str[str.length() - 1];
  LOG(DEBUG) << "a: " << a;
  if (a == '/')
    return true;
  return false;
}
