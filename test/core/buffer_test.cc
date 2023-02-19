#include "core/buffer.h"

#include <cstring>

#include "doctest/doctest.h"

using Icee::Buffer;
using Icee::INITIAL_BUFFER_SIZE;  // 1024

TEST_CASE("[core/buffer]") {
  Buffer buf(INITIAL_BUFFER_SIZE);
  REQUIRE(buf.Size() == 0);
  REQUIRE(buf.Capacity() == INITIAL_BUFFER_SIZE);

  MESSAGE("appending the c-str into the buffer[both side, head & tail]");

  {
    const char msg1[1024] = "pending the content form the beginning!";
    const char msg2[1024] = "filling the content to the tail of buffer!";
    buf.AppendHead(msg1);
    buf.Append(msg2);

    snprintf((char *)msg1 + strlen(msg1), strlen(msg2) + 1, "%s", msg2);
    CHECK(std::strncmp((char *)buf.Data(), msg1, std::strlen(msg1)) == 0);
    buf.Clear();
    CHECK(buf.Size() == 0);
  }

  MESSAGE("appending std::string into buffer from both side");
  {
    const std::string msg1 = "Greeting from beginning!";
    const std::string msg2 = "Farewell from end~";
    buf.AppendHead(msg1);
    buf.Append(msg2);
    const std::string together = msg1 + msg2;
    CHECK(buf.ToStringView() == together);
    buf.Clear();
    CHECK(buf.Size() == 0);
  }

  MESSAGE("find and pop based on the target first found");
  {
    const std::string msg =
        "GET / HTTP/1.1\r\n"
        "Connection: Keep-Alive\r\n"
        "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
        "Accept-Language: en-us\r\n"
        "\r\n";
    const std::string next_msg = "Something belongs to next message";
    buf.Append(msg);
    buf.Append(next_msg);
    auto op_str = buf.FindAndPopTill("\r\n\r\n");
    CHECK((op_str.has_value() && op_str.value() == msg));
    CHECK(buf.ToStringView() ==
          next_msg);  // 因为检查到了 \r\n\r\n, 原数据中的 \r\n\r\n及之前的数据全部清除掉了，所以说只剩下了 next_message
  }
}
