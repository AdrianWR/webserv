#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include "log.hpp"
#include "enum.hpp"

#define DOUBLE_CRLF "\r\n\r\n"

// *****************************************************
// Auxiliary Functions
// *****************************************************

void receive_line(int fd, std::string &line, std::string delimiter);

bool ends_in_two_delimiters(std::string buffer);
std::string file_to_string(std::string file_path);
bool string_to_file(std::string file_path, std::string str);

std::string IntToString(int a);
size_t StringToInt(std::string s);
bool file_exist(std::string path);
std::string path_is(std::string s);
bool end_in_slash(std::string str);

template <typename T>
void print_vector(std::vector<T> v, std::ofstream &cout)
{
  typename std::vector<T>::iterator it;

  int i = 0;

  for (it = v.begin(); it != v.end(); it++)
  {
    cout << "[" << i << "]\t|" << (*it) << "|" << std::endl;
    i++;
  }
}

template <typename T>
void print_vectorc(std::vector<T> v)
{
  std::ofstream cout("config_vector.txt", std::ofstream::trunc);

  for (unsigned int i = 0; i < v.size(); i++)
  {
    std::cout << "[" << i << "]\n";
    v[i].print_block_file(cout);
  }
}

template <typename A, typename B>
void print_map(std::map<A, B> m, std::ofstream &cout)
{
  typename std::map<A, B>::iterator it;
  for (it = m.begin(); it != m.end(); it++)
  {
    cout << "|" << it->first << " : " << it->second << "|" << std::endl;
  }
}

template <typename A, typename B>
void print_mapc(std::map<A, B> m)
{
  typename std::map<A, B>::iterator it;
  std::ofstream cout("config_map.txt", std::ofstream::trunc);

  for (it = m.begin(); it != m.end(); it++)
  {
    cout << it->first << ":\n";
    (it->second).print_block_file(cout);
  }
  cout.close();
}

#endif
