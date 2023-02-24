#include "http/header.h"

#include "doctest/doctest.h"
#include "http/http_utils.h"

using Icee::Http::CRLF;
using Icee::Http::Header;

TEST_CASE("[http/header]") {
  SUBCASE("constructor method for header") {
    std::string k = "http_version";
    std::string v1 = "1.1";
    std::string v2 = "1.2";
    Header h1(k, v1);

    CHECK(h1.IsValid());
    CHECK(h1.GetValue() == v1);
    CHECK(h1.GetKey() == k);

    h1.SetValue(v2);
    CHECK(h1.GetValue() == v2);
  }


}










