#include <iostream>
#include "http/response.h"

#include "doctest/doctest.h"
#include "http/header.h"
#include "http/http_utils.h"

using namespace Icee::Http;

TEST_CASE("[http/response]") {
  SUBCASE("response 能够改动 header") {
    std::string status = "200 Success";
    Response response{RESPONSE_OK, false, std::string("nonexistent-file.txt")};
    auto headers = response.GetHeaders();
    bool find = false;
    if (headers.size() == 0) {
      std::cout << "the size of headers is zero\n";
    } else {
      std::cout << "the size of headers is not zero\n";
    }
    for (auto &h : headers) {
      if (h.GetKey() == HEADER_CONTENT_LENGTH) {
        find = true;
      }
    }
    CHECK(find);
    std::string new_val = "1024";
    CHECK(response.ChangeHeader(HEADER_CONTENT_LENGTH, new_val));
    find = false;
    std::string value;
    headers = response.GetHeaders();
    for (auto &h : headers) {
      if (h.GetKey() == HEADER_CONTENT_LENGTH) {
        find = true;
        value = h.GetValue();
      }
    }
    CHECK(find);
    CHECK(value == new_val);
  }
}



