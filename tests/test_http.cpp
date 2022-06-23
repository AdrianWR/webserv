#include "http.hpp"
#include <gtest/gtest.h>

TEST(TestHttp, TestHttpRequestParse) {
  HttpResponse response;
  ASSERT_TRUE(response.getMethod() == HTTP_UNKNOWN);
}