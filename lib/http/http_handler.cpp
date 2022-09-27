#include "http.hpp"
#include "log.hpp"
#include "utils.hpp"
#include <exception>

// HttpResponse HttpHandler::generateResponse(HttpRequest &h, Config &config) {
//   HttpResponse k;

//   BaseHttp::HeaderMap headers = h.getHeaders();
//   Config::BlockMap blocks = config.getBlockMap();

//   try {
//     ConfigBlock block = blocks.at(headers["host"]);
//   } catch (std::out_of_range const&) {
//     throw BaseHttp::HttpException("Invalid host configuration");
//     LOG(DEBUG) << "Invalid host configuration";
//   }
//   return k;
// }
