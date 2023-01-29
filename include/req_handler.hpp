#ifndef REQ_HANDLER_HPP
#define REQ_HANDLER_HPP

#include "http.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include "log.hpp"
#include "error.hpp"
#include "http_response.hpp"

// *****************************************************
// Class RequestHander
// *****************************************************

class RequestHandler
{
public:
	RequestHandler();
	RequestHandler(Config fp, HttpRequest req);
	~RequestHandler();
	RequestHandler &operator=(const RequestHandler &s);

public:
	void handler();

private:
	std::string extract_extension(std::string path);
	void add_content_type(std::string path);
	std::string extract_location(std::string uri);
	std::string generate_path(std::string url, std::string location, std::string root);
	bool check_redirection();
	bool check_method_allowed(std::string m);
	std::string what_is_asked(std::string path);
	void fetch_file(std::string path);
	void fetch_cgi();
	void try_index_page(std::string path);
	void try_autoindex(std::string host, std::string port);
	void fetch_dir(std::string path, std::string host, std::string port);
	bool load_configs();
	void handle_GET();
	void handle_DELETE();
	void handle_POST();
	void _get_script_output(std::FILE *temp_file);

private:
	// Config objects
	Config::BlockMap _parsed_config_map;
	ConfigBlock server_config;
	LocationBlock loc_config;

	// Inputs from http_request
	std::string _host;
	std::string _port;
	std::string _method;
	std::string _uri;
	std::string _request_body; // Post
	int _content_length;			 // Post
	std::string _cgi_pass;		 // Post

	char **_env;
	char **_cmd;

	// Internals
	std::string _loc;
	std::string _path;

public:
	// Output
	HttpResponse _http_response;
};

#endif
