#include "socket.hpp"
#include <gtest/gtest.h>

TEST(HelloTest, World1) { ASSERT_TRUE(1 == 1); }

TEST(HelloTest, World2) { ASSERT_TRUE(1 == 1); }

TEST(CoolTest, World) { ASSERT_TRUE(2 == 1); }

TEST(TestSocket, TestSocketInitialization) {
  Socket s;

  sleep(1);
  ASSERT_TRUE(s.sockfd == -1);
}