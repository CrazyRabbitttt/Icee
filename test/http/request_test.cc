#include "http/request.h"
#include <iostream>

#include "doctest/doctest.h"
#include "http/header.h"
#include "http/http_utils.h"

using namespace Icee::Http;

TEST_CASE("[http/request]") {
  SUBCASE("接收 string formatted request & check if valid") {
    std::string request0_str =
        "GET /hello.html HTTP/1.1\r\n"
        "User-Agent: Mozilla/4.0\r\n"
        "Host: www.tutorialspoint.com\r\n"
        "Connection: Keep-Alive\r\n";
    Request request0{request0_str};
    CHECK(!request0.IsValid());

    /* first line is not request line */
    std::string request_1_str =
        "User-Agent: Mozilla/4.0\r\n"
        "GET /hello.html HTTP/1.1\r\n"
        "Host: www.tutorialspoint.com\r\n"
        "Connection: Keep-Alive\r\n"
        "\r\n";
    Request request_1{request_1_str};
    CHECK(!request_1.IsValid());

    /* method not supported */
    std::string request_2_str =
        "PUNCH /hello.html HTTP/1.1\r\n"
        "User-Agent: Mozilla/4.0\r\n"
        "Host: www.tutorialspoint.com\r\n"
        "Connection: Keep-Alive\r\n"
        "\r\n";
    Request request_2{request_2_str};
    CHECK(!request_2.IsValid());

    /* version not supported */
    std::string request_3_str =
        "GET /hello.html HTTP/2.0\r\n"
        "User-Agent: Mozilla/4.0\r\n"
        "Host: www.tutorialspoint.com\r\n"
        "Connection: Keep-Alive\r\n"
        "\r\n";
    Request request_3{request_3_str};
    CHECK(!request_3.IsValid());

    /* the bare minimum valid request */
    std::string request_4_str =
        "GET /hello.html HTTP/1.1\r\n"
        "\r\n";
    Request request_4{request_4_str};
    CHECK(request_4.IsValid());
    CHECK(request_4.GetMethod() == Method::GET);
    std::cout << "request 4's version:" << (int)request_4.GetVersion();
    CHECK(request_4.GetVersion() == Version::HTTP_1_1);

    /* connection to close request */
    std::string request_5_str =
        "HEAD /hello.html HTTP/1.1\r\n"
        "User-Agent: Mozilla/4.0\r\n"
        "Host: www.tutorialspoint.com\r\n"
        "Connection: close\r\n"
        "\r\n";
    Request request_5{request_5_str};
    CHECK(request_5.IsValid());
    CHECK(request_5.GetMethod() == Method::HEAD);
    CHECK(request_5.ShouldClose());

    /* connection to kepp alive request */
    std::string request_6_str =
        "GET /hello.html HTTP/1.1\r\n"
        "User-Agent: Mozilla/4.0\r\n"
        "Host: www.tutorialspoint.com\r\n"
        "Connection: Keep-Alive\r\n"
        "\r\n";
    Request request_6{request_6_str};
    CHECK(!request_6.ShouldClose());
  }
}
