#include "socket.hpp"
#include <exception>
#include <gtest/gtest.h>

TEST(TestSocket, TestSocketDefaultConstructor) {
  Socket s;

  ASSERT_TRUE(s.listener_sockfd == -1);
  ASSERT_TRUE(s.host == "");
  ASSERT_TRUE(s._port == 0);
}

TEST(TestSocket, TestSocketHostPortConstructor) {
  Socket s("localhost", 8080);

  ASSERT_TRUE(s.listener_sockfd != -1);
  ASSERT_TRUE(s.host == "localhost");
  ASSERT_TRUE(s._port == 8080);
}

TEST(TestServerSocket, TestBindAndListen) {

  int val, sock_info;
  socklen_t len = sizeof(val);

  TCPServerSocket s("localhost", 8080);
  sock_info =
      getsockopt(s.listener_sockfd, SOL_SOCKET, SO_ACCEPTCONN, &val, &len);
  ASSERT_EQ(sock_info, 0);
  ASSERT_GT(val, 0);
}

TEST(TestServerSocket, TestAccept) {}