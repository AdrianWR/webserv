#include "http.hpp"
#include <gtest/gtest.h>

class HttpTest : public ::testing::Test {
protected:
  HttpTest() {}
  virtual ~HttpTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

  const char *buffer_mock = "GET /simple_get HTTP/1.1\r\n"
                            "Host: 127.0.0.1:5000\r\n"
                            "User-Agent: Mozilla/5.0\r\n"
                            "Accept: text/html\r\n"
                            "Accept-Language: en-GB, en;q=0.8\r\n"
                            "Accept-Encoding: gzip, deflate\r\n"
                            "Connection: keep-alive\r\n\r\n ";
};

TEST_F(HttpTest, TestHttpRequestParse) {
  HttpResponse response;
  ASSERT_TRUE(response.getMethod() == HTTP_GET);
}

TEST_F(HttpTest, TestParseHeader) {
  HttpResponse response;
  HttpResponse::HeaderMap headers;
  // headers =
  //       response.parse("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n");
  //   ASSERT_EQ(headers["method"], "GET");
  //   ASSERT_EQ(headers["path"], "/");
  //   ASSERT_EQ(headers["version"], "HTTP/1.1");
  //   ASSERT_EQ(headers["host"], "localhost");

  //   headers = response.parse("GET /
  //   HTTP/1.1\r\nHost:localhost\r\n\r\n"); ASSERT_EQ(headers["method"],
  //   "GET"); ASSERT_EQ(headers["path"], "/"); ASSERT_EQ(headers["version"],
  //   "HTTP/1.1"); ASSERT_EQ(headers["host"], "localhost");

  //   headers = response.parse("GET / HTTP/1.1\r\nHost:
  //   localhost\r\n\r\n"); ASSERT_EQ(headers["method"], "GET");
  //   ASSERT_EQ(headers["path"], "/");
  //   ASSERT_EQ(headers["version"], "HTTP/1.1");
  //   ASSERT_EQ(headers["host"], " localhost");

  headers = response.parse(buffer_mock);
  ASSERT_EQ(headers["method"], "GET");
  ASSERT_EQ(headers["path"], "/simple_get");
  ASSERT_EQ(headers["version"], "HTTP/1.1");
  ASSERT_EQ(headers["host"], "127.0.0.1:5000");
  ASSERT_EQ(headers["user-agent"], "Mozilla/5.0");
  ASSERT_EQ(headers["accept"], "text/html");
  ASSERT_EQ(headers["accept-language"], "en-GB, en;q=0.8");
  ASSERT_EQ(headers["accept-encoding"], "gzip, deflate");
  ASSERT_EQ(headers["connection"], "keep-alive");
}