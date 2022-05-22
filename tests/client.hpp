#include <cstring>

class TCPClient {
public:
    TCPClient(const std::string& host, const std::string& port);
    
    TCPClient();
    TCPClient(const TCPClient&) = delete;
    TCPClient& operator=(const TCPClient&) = delete;
    ~TCPClient();

    void connect();
    void disconnect();
    void send(const std::string& message);
    std::string receive();
    

protected:
    int sockfd;
    struct sockaddr_in serv_addr;
    std::string host;
    std::string port;
}