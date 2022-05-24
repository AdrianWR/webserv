#include "socket.hpp"
#include <exception>
#include <gtest/gtest.h>

TEST(TestSocket, TestSocketDefaultConstructor) {
  Socket s;

  ASSERT_TRUE(s.sockfd == -1);
  ASSERT_TRUE(s.host == "");
  ASSERT_TRUE(s.port == "");
}

TEST(TestSocket, TestSocketHostPortConstructor) {
  Socket s("localhost", "8080");

  ASSERT_TRUE(s.sockfd == -1);
  ASSERT_TRUE(s.host == "localhost");
  ASSERT_TRUE(s.port == "8080");
}

TEST(TestSocket, TestSocketCreation) {
  Socket s;

  ASSERT_TRUE(s.sockfd == -1);
  s.create();
  ASSERT_TRUE(s.sockfd != -1);
}

TEST(TestSocket, TestBindAndListen) {
  Socket s;

  ASSERT_TRUE(s.sockfd == -1);
  s.create();
  s.bind_and_listen(8080);
  ASSERT_TRUE(s.sockfd != -1);

  Socket s1;
  s1.create();
  try {
    s1.bind_and_listen(8080);
  } catch (std::exception &e) {
    EXPECT_STREQ(e.what(), "Could not bind socket: Address already in use");
  }
}