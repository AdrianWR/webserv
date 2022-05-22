// Constructors

TCPClient::TCPClient() : sockfd(-1), host(""), port("") {}

TCPClient::TCPClient(const std::string& host, const std::string& port)
{
    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        throw std::runtime_error("ERROR opening socket");
    }

    // Set the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(std::stoi(port));
    in_addr_t ip = inet_addr(host.c_str());
    serv_addr.sin_addr.s_addr = ip;
}

TCPClient::~TCPClient()
{
    disconnect();
    close(sockfd);
}

TCPClient::TCPClient(const TCPClient& c): sockfd(c.sockfd), host(c.host), port(c.port) {}

TCPClient& TCPClient::operator=(const TCPClient& c)
{
    if (this != &c)
    {
        sockfd = c.sockfd;
        host = c.host;
        port = c.port;
    }
    return *this;
}

TCPClient::conect()
{
    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        throw std::runtime_error("ERROR connecting");
    }
}