#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>

class SysException : public std::exception {
public:
  SysException(const std::string &message, int err = 0)
      : msg_(message), err(err) {}
  virtual ~HTTPException() throw(){};
  const char *what() const throw() {
    return msg_ + std::string(strerror(err));
  };

private:
  std::string msg_;
  int err;

#endif