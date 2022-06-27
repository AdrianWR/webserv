#include "http.hpp"
#include <gtest/gtest.h>

class HttpTest : public ::testing::Test {
protected:
  HttpTest() {}
  virtual ~HttpTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(HttpTest, TestHttpRequestParse) {
  HttpResponse response;
  ASSERT_TRUE(response.getMethod() == HTTP_UNKNOWN);
}

TEST_F(HttpTest, TestParseHeader) {
  HttpResponse response;

  HttpResponse::HeaderMap headers =
      response.parseHeader("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n");
  ASSERT_EQ(headers["method"], "GET");
  ASSERT_EQ(headers["path"], "/");
  ASSERT_EQ(headers["version"], "HTTP/1.1");
  ASSERT_EQ(headers["host"], "localhost");

  headers = response.parseHeader("GET / HTTP/1.1\r\nHost:localhost\r\n\r\n");
  ASSERT_EQ(headers["method"], "GET");
  ASSERT_EQ(headers["path"], "/");
  ASSERT_EQ(headers["version"], "HTTP/1.1");
  ASSERT_EQ(headers["host"], "localhost");

  headers = response.parseHeader("GET / HTTP/1.1\r\nHost:  localhost\r\n\r\n");
  ASSERT_EQ(headers["method"], "GET");
  ASSERT_EQ(headers["path"], "/");
  ASSERT_EQ(headers["version"], "HTTP/1.1");
  ASSERT_EQ(headers["host"], " localhost");
}