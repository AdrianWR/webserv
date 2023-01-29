#include "req_handler.hpp"
#include "http.hpp"
// **********************************************************
// Class Req_handler
// **********************************************************
RequestHandler::RequestHandler() {}

RequestHandler::RequestHandler(Config fp, HttpRequest req)
{
	LOG(INFO) << "Initializing req_handler";
	_parsed_config_map = fp.getBlockMap();
	HttpRequest::HeaderMap headers = req.getHeaders();

	_host = headers["host"].substr(0, headers["host"].find(":"));
	_port = headers["host"].substr(headers["host"].rfind(":") + 1);
	_method = headers["method"];
	_uri = _host + headers["path"];
	_request_body = (headers.find("body") != headers.end() ? headers["body"] : "");
	_content_length = (headers.find("content-length") != headers.end() ? StringToInt(headers["content-length"]) : 0);
	LOG(DEBUG) << "URI: " << _uri;
}

RequestHandler::~RequestHandler()
{
	if (what_is_asked(this->_path) == "cgi")
	{
		free(this->_cmd[0]);
		free(this->_cmd[1]);
		delete[] this->_cmd;
		if (this->_method == "POST")
		{
			free(this->_env[0]);
			delete[] this->_env;
		}
	}
	LOG(DEBUG) << "req_handler destructor";
}

RequestHandler &RequestHandler::operator=(const RequestHandler &s)
{
	if (this != &s)
		return *this;
	return *this;
}

std::string RequestHandler::extract_extension(std::string path)
{
	if (path.find(".html") != std::string::npos)
	{
		return "html";
	}
	if (path.find(".htm") != std::string::npos)
	{
		return "htm";
	}
	if (path.find(".txt") != std::string::npos)
	{
		return "txt";
	}
	if (path.find(".jpg") != std::string::npos)
	{
		return "jpg";
	}
	if (path.find(".jpeg") != std::string::npos)
	{
		return "jpeg";
	}
	if (path.find(".png") != std::string::npos)
	{
		return "png";
	}
	if (path.find(".gif") != std::string::npos)
	{
		return "gif";
	}
	if (path.find_last_of("/") == path.size() - 1)
	{
		return "txt";
	}
	else
	{
		return "";
	}
}

void RequestHandler::add_content_type(std::string path)
{
	std::string ext = extract_extension(path);
	LOG(DEBUG) << "ct path: " << path;
	LOG(INFO) << "Adding content-type for: " << ext;

	if (ext == "html" || ext == "htm")
	{
		_http_response.insert_header("content-type", "text/html");
	}
	if (ext == "txt" || ext == "")
	{
		_http_response.insert_header("content-type", "text/plain");
	}
	if (ext == "jpg" || ext == "jpeg")
	{
		_http_response.insert_header("content-type", "image/jpeg");
	}
	if (ext == "png")
	{
		_http_response.insert_header("content-type", "image/png");
	}
	if (ext == "gif")
	{
		_http_response.insert_header("content-type", "image/gif");
	}
}

std::string RequestHandler::extract_location(std::string uri)
{
	// pega uri da 1a barra em diante (inclusive barrra)
	// troca location /palavra por root (./dir/)

	ConfigBlock::MapOfLocations::iterator it;
	std::string key;
	bool match;
	size_t p;

	LOG(INFO) << "uri: " << uri;
	size_t pos1 = uri.find("/");
	std::string s = uri.substr(pos1);
	match = false;
	for (it = server_config._location.begin(); it != server_config._location.end(); it++)
	{
		key = it->first;
		LOG(DEBUG) << "key: " << key;
		if (key != "/")
		{
			p = s.find(key);
			if (p == 0)
			{
				match = true;
				break;
			}
		};
	}
	LOG(DEBUG) << "match? : " << match;
	if (!match)
	{
		key = "/";
	}
	return key;
}

std::string RequestHandler::generate_path(std::string uri, std::string location,
																					std::string root)
{
	// pega location
	// pega root
	// troca location por root no url
	// se uri termina em location -> acrescenta / no final do path
	LOG(INFO) << "====== begin generate_path =========";
	LOG(INFO) << "uri: " << uri;
	LOG(INFO) << "location: " << location;
	LOG(INFO) << "root: " << root;
	LOG(INFO) << "loc: " << _loc;

	if (_loc == "/")
		root += "/";
	std::string uri_exhost = uri.substr(uri.find("/"));
	LOG(DEBUG) << "uri_exhost: " << uri_exhost;
	size_t pos = uri_exhost.find(location);
	size_t loc_len = location.length();
	std::string uri_root = "." + uri_exhost.replace(pos, loc_len, root);
	LOG(DEBUG) << "pos: " << pos << " loc_len: " << loc_len;
	LOG(DEBUG) << "uri_root: " << uri_root;
	std::string full_path = uri_root;

	// Se uri_root eh dir e nao termina em / -> add /
	LOG(INFO) << "path_is: " << path_is(uri_root);
	if (path_is(uri_root) == "dir" && !end_in_slash(uri_root))
	{
		full_path = full_path + "/";
	}
	LOG(INFO) << "full_path: " << full_path;
	LOG(INFO) << "====== end generate_path =========";

	return (full_path);
}

bool RequestHandler::check_redirection()
{
	if (this->loc_config._redirection != "")
	{
		LOG(INFO) << "There is redirection ...";
		LOG(DEBUG) << "redir: " << loc_config._redirection;
		// Gera "erro"
		Error error(301, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
		_http_response.insert_header("Location", "http://" + this->loc_config._redirection);
		return true;
	}
	else
	{
		LOG(INFO) << "No Redirection ...";
		return false;
	};
}

bool RequestHandler::check_method_allowed(std::string m)
{
	if (this->loc_config._allowed_methods[m] == 0)
	{
		LOG(INFO) << m << " Not Allowed";
		Error error(405, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
		return false;
	};
	LOG(INFO) << m << " Allowed";
	return true;
}

std::string RequestHandler::what_is_asked(std::string path)
{
	if (_cgi_pass != "" && (path.find(this->_cgi_pass) != std::string::npos))
	{
		return "cgi";
	}
	if (path.find_last_of("/") == path.size() - 1)
	{
		return "dir";
	}
	else
	{
		return "file";
	}
}

int _get_file_size(std::FILE *temp_file)
{
	int size;

	fseek(temp_file, 0, SEEK_END);
	size = ftell(temp_file);
	rewind(temp_file);
	return (size);
}

void RequestHandler::_get_script_output(std::FILE *temp_file)
{
	int size = _get_file_size(temp_file);
	LOG(DEBUG) << "size: " << size;
	char *buffer = new char[size + 1];

	memset(buffer, 0, size + 1);
	fread(buffer, 1, size, temp_file);
	LOG(DEBUG) << "Opening file from cgi";
	LOG(DEBUG) << "size: " << size;
	if (size < 0)
	{
		LOG(INFO) << "Error 500 Internal Server Error";
		Error error(500, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
	}
	else
	{
		LOG(DEBUG) << "buffer: " << buffer;
		_http_response.set(200, "OK", std::string(buffer));
	}
	delete[] buffer;
}

void RequestHandler::fetch_cgi()
{
	int pid;
	std::string executable;
	std::string file_path;
	char **cmd = new char *[3];

	std::FILE *temp_file = std::tmpfile();
	int temp_fd = fileno(temp_file);

	executable = "/usr/bin/python3";
	file_path = this->_path;

	memset(cmd, 0, 3 * sizeof(char *));
	cmd[0] = strdup(executable.c_str());
	cmd[1] = strdup(file_path.c_str());
	this->_cmd = cmd;
	if (this->_method == "GET")
	{
		this->_env = NULL;
	}
	else
	{
		char **env = new char *[1];
		memset(env, 0, 1 * sizeof(char *));
		std::string var_string = "QUERY_STRING=" + this->_request_body;
		env[0] = strdup((var_string.c_str()));
		this->_env = env;
	}
	pid = fork();
	if (pid == 0)
	{
		dup2(temp_fd, STDOUT_FILENO);
		execve(this->_cmd[0], this->_cmd, this->_env);
		close(temp_fd);
	}
	waitpid(pid, NULL, 0);
	_get_script_output(temp_file);
	fclose(temp_file);
}

void RequestHandler::fetch_file(std::string path)
{
	// Tenta pegar arquivo.
	std::string full_path = path;
	std::string output = file_to_string(full_path);
	LOG(DEBUG) << "output size read from file: " << output.size();
	if (output.size() > 0)
	{
		LOG(INFO) << "File fetched ...";
		// Generate HTTP Response
		add_content_type(path);
		_http_response.set(200, "OK", output);
	}
	else
	{
		LOG(INFO) << "Error 404 Not Found";
		Error error(404, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
	};
}

void RequestHandler::try_index_page(std::string path)
{
	// loop
	for (size_t i = 0; i < this->loc_config._index.size(); i++)
	{
		// monta caminho com um dos index
		std::string full_path = path + this->loc_config._index[i];
		LOG(INFO) << "full path: " << full_path;
		// devolve
		std::string output = file_to_string(full_path);
		if (output.size() > 0)
		{
			LOG(INFO) << "Index Page:";
			// Generate HTTP Response
			_http_response.set(200, "OK", output);
			break;
		};
	};
	if (_http_response._code != 200)
	{
		LOG(INFO) << "Error 404 Not Found";
		Error error(404, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
	};
}

void RequestHandler::try_autoindex(std::string host, std::string port)
{
	// se autoindex on executa autoindex
	if (this->loc_config._autoindex == true)
	{
		LOG(INFO) << "Autoindex ON";
		AutoIndexGenerator auto_index;
		std::string ai_page = auto_index.getPage(_path.c_str(), host, StringToInt(port), _loc);
		// Generate HTTP Response
		add_content_type(".html");
		_http_response.set(200, "OK", ai_page);
	}
	else
	{
		// se nao devolve erro
		LOG(INFO) << "404 No index";
		Error error(404, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
	};
}

void RequestHandler::fetch_dir(std::string path, std::string host, std::string port)
{
	if (this->loc_config._index.size() > 0)
	{ // Se tiver index
		LOG(INFO) << "Tem pagina de Index";
		try_index_page(path);
	}
	else
	{ // se nao houver
		try_autoindex(host, port);
	};
}

bool RequestHandler::load_configs()
{
	// Pega configs na estrutura de configs:
	std::string conf_key = this->_host + ":" + this->_port;
	server_config = _parsed_config_map[conf_key];
	// Se config invalida retorna falso
	if (server_config._block_name == "empty")
	{
		LOG(INFO) << "Invalid host:port requested";
		Error error(404, this->server_config);
		// Generate HTTP Response
		add_content_type(".html");
		_http_response.set(error.code, error.msg, "<html> Error 404 </html>");
		return false;
	}
	LOG(DEBUG) << "server_config retrieved from memory ...";
	// debug prints
	std::ofstream f("server_config.txt", std::ofstream::trunc);
	server_config.print_block_file(f);

	// Extract location
	this->_loc = extract_location(this->_uri);
	// Carrega configs da location na memoria
	this->loc_config = server_config._location[this->_loc];
	LOG(INFO) << "loc_config retrieved from memory: " << _loc;
	// debug prints
	std::ofstream f2("location_config.txt", std::ofstream::trunc);
	loc_config.print_location(f2);
	_cgi_pass = this->loc_config._cgi_pass;
	// ================================================================
	return true;
}

void RequestHandler::handle_GET()
{
	LOG(INFO) << "GET Method ...";
	// Se tiver redirection, devolve redirection e sai.
	if (check_redirection())
		return;
	// Checa se GET Permitido neste location
	if (!check_method_allowed("GET"))
		return;
	// 0) Se for cgi
	if (what_is_asked(this->_path) == "cgi")
	{
		LOG(INFO) << "CGI requested ...";
		fetch_cgi();
	};
	// 1) Se for arquivoi:termina sem /
	if (what_is_asked(this->_path) == "file")
	{
		LOG(INFO) << "FILE requested...";
		fetch_file(this->_path);
	}
	// 2) Se for diretorio (termina em /)
	if (what_is_asked(_path) == "dir")
	{
		LOG(INFO) << "DIR requested...";
		fetch_dir(this->_path, this->_host, this->_port);
	};
	// ================================================================
}

void RequestHandler::handle_DELETE()
{

	LOG(INFO) << "DELETE Method ...";
	// Checa se DELETE Permitido neste location
	if (!check_method_allowed("DELETE"))
		return;
	// Se for diretorio, forbidden
	if (what_is_asked(this->_path) == "dir")
	{
		LOG(INFO) << "DIR requested...";
		Error error(403, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
		return;
	}
	// Se nao existir arquivoL 404
	if (what_is_asked(this->_path) == "file")
	{
		LOG(INFO) << "FILE requested...";
		if (!file_exist(this->_path))
		{
			Error error(404, this->server_config);
			// Generate HTTP Response
			_http_response.set(error.code, error.msg, error.body);
			return;
		}
		else
		{
			// Se existir, deleta
			LOG(INFO) << "Deleting file: " << this->_path;
			if (std::remove(this->_path.c_str()) == 0)
			{
				// Generate Http Response
				_http_response.set(200, "OK", "");
				return;
			}
			else
			{
				Error error(403, this->server_config);
				// Generate HTTP Response
				_http_response.set(error.code, error.msg, error.body);
				return;
			}
		}
	}
	// ================================================================
}

void RequestHandler::handle_POST()
{
	LOG(INFO) << "POST ...";
	// Checa se POST Permitido neste location
	if (!check_method_allowed("POST"))
		return;
	// Checa tamanho
	if (_content_length > server_config._client_max_body_size)
	{
		LOG(INFO) << "413 Entity to large !";
		Error error(413, this->server_config);
		// Generate HTTP Response
		_http_response.set(error.code, error.msg, error.body);
		return;
	}
	if (what_is_asked(this->_path) == "file")
	{
		LOG(INFO) << "FILE requested...";
		LOG(INFO) << "upload path: " << loc_config._upload_path;
		// Se tem upload path altera o path para usar o do config
		if (this->loc_config._upload_path != "")
		{
			// monta path
			_path = generate_path(this->_uri, this->_loc, this->loc_config._upload_path);
			LOG(INFO) << "upload path updated: " << _path;
		};
		// Salva arquivo criando diretorio
		LOG(INFO) << "POST OK";
		LOG(INFO) << "PATH!!!!: " << _path;
		bool file_created = string_to_file(_path, _request_body);
		if (file_created)
		{
			// Generate HTTP Response
			_http_response.set(200, "OK", "");
			return;
		}
		else
		{
			Error error(404, this->server_config);
			// Generate HTTP Response
			_http_response.set(error.code, error.msg, error.body);
			return;
		}
	}
	if (what_is_asked(this->_path) == "dir")
	{
		LOG(INFO) << "DIR requested...";
		// Generate HTTP Response
		_http_response.set(200, "OK", "");
		return;
	};
	if (what_is_asked(this->_path) == "cgi")
	{
		LOG(INFO) << "CGI requested ...";
		fetch_cgi();
		return;
	}
	// ================================================================
}

void RequestHandler::handler()
{

	// ================================================================
	// Get Inputs
	// ================================================================
	LOG(INFO) << "====== handler() function ini =====";
	LOG(INFO) << "host: " << _host;
	LOG(INFO) << "port: " << _port;
	LOG(INFO) << "method: " << _method;
	LOG(INFO) << "uri: " << _uri;
	LOG(INFO) << "content_length: " << _content_length;

	// ================================================================
	// Load Configs
	// ================================================================
	// Populates class atributes with inputs and config values
	if (!load_configs())
		return;

	// FROM NOW ON SERVER CONFIG ON MEMORY
	// Monta caminho fisico:
	_path = generate_path(this->_uri, this->_loc, this->loc_config._root);
	LOG(INFO) << "path generated: " << _path;

	LOG(DEBUG) << "loc: " << _loc;
	LOG(INFO) << "======================";

	// ================================================================
	// START OF PARSING (one function for each method)
	//              GET POST DELETE OTHER
	// ================================================================
	if (this->_method == "GET")
	{
		handle_GET();
		return;
	}
	if (this->_method == "POST")
	{
		handle_POST();
		return;
	}
	if (this->_method == "DELETE")
	{
		handle_DELETE();
		return;
	}
	// UNKNOWN Method
	Error error(STATUS_CODE_METHOD_NOT_ALLOWED, this->server_config);
	// Generate HTTP Response
	_http_response.set(error.code, error.msg, error.body);

	return;
}
