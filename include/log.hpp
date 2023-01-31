/*
 * This is a simple log lib with levels and timing features. I made some
 * modifications to the original log lib to make it more suitable for this
 * project. Original source code:
 * https://bitbucket.org/volkanozyilmaz/logcpp/src/master/log.h
 */

#ifndef LOG_HPP
#define LOG_HPP

#include <cstdio>
#include <ctime>
#include <sstream>
#include <string>
#include <unistd.h>

inline std::string NowTime();

enum TLogLevel { ERROR, WARN, INFO, DEBUG };

template <typename T> class Log {
public:
  Log();
  virtual ~Log();
  std::ostringstream &Get(TLogLevel level = INFO);

public:
  static TLogLevel &ReportingLevel();
  static std::string ToString(TLogLevel level);
  static TLogLevel FromString(const std::string &level);

protected:
  std::ostringstream os;

private:
  Log(const Log &);
  Log &operator=(const Log &);
};

template <typename T> Log<T>::Log() {}

template <typename T> std::ostringstream &Log<T>::Get(TLogLevel level) {
  os << NowTime() << " | ";
  os << getpid() << " | ";
  os << "[" << ToString(level) << "] \t| ";
  return os;
}

template <typename T> Log<T>::~Log() {
  os << std::endl;
  T::Output(os.str());
}

template <typename T> TLogLevel &Log<T>::ReportingLevel() {
  static TLogLevel reportingLevel = INFO;
  return reportingLevel;
}

template <typename T> std::string Log<T>::ToString(TLogLevel level) {
  static const char *const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
  return buffer[level];
}

template <typename T> TLogLevel Log<T>::FromString(const std::string &level) {
  if (level == "DEBUG")
    return DEBUG;
  if (level == "INFO")
    return INFO;
  if (level == "WARNING")
    return WARN;
  if (level == "ERROR")
    return ERROR;
  Log<T>().Get(WARN) << "Unknown logging level '" << level
                     << "'. Using INFO level as default.";
  return INFO;
}

class Output2FILE {
public:
  static FILE *&Stream();
  static void Output(const std::string &msg);
};

inline FILE *&Output2FILE::Stream() {
  static FILE *pStream = stderr;
  return pStream;
}

inline void Output2FILE::Output(const std::string &msg) {
  FILE *pStream = Stream();
  if (!pStream)
    return;
  fprintf(pStream, "%s", msg.c_str());
  fflush(pStream);
}

#define FILELOG_DECLSPEC

class FILELOG_DECLSPEC FILELog : public Log<Output2FILE> {};

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL DEBUG
#endif

#define FILE_LOG(level)                                                        \
  if (level > FILELOG_MAX_LEVEL)                                               \
    ;                                                                          \
  else if (level > FILELog::ReportingLevel() || !Output2FILE::Stream())        \
    ;                                                                          \
  else                                                                         \
    FILELog().Get(level)

#define LOG(level)                                                             \
  if (level > FILELOG_MAX_LEVEL)                                               \
    ;                                                                          \
  else if (level > FILELog::ReportingLevel() || !Output2FILE::Stream())        \
    ;                                                                          \
  else                                                                         \
    FILELog().Get(level)

inline std::string NowTime() {
  time_t now;
  char buf[32];

  time(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S %Z", localtime(&now));
  return buf;
}

inline void initLogger(const char *file, TLogLevel level) {
  FILELog::ReportingLevel() = level;
  FILE *log_fd = fopen(file, "w");
  Output2FILE::Stream() = log_fd;
}

inline void endLogger() {
  if (Output2FILE::Stream())
    fclose(Output2FILE::Stream());
}

#endif //__LOG_H__
