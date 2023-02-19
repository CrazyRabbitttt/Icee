#include "core/cache.h"

#include <vector>

#include "doctest/doctest.h"

using Icee::Cache;


TEST_CASE("[core/cache]") {

  void CacheFunc() {
    const int capacity = 20;
    Cache cache(capacity);
    std::vector<unsigned char> data = {'h', 'e', 'l', 'l', 'o', '!'};
    const auto data_size = data.size();

    REQUIRE(cache.GetCurSize() == 0);
    REQUIRE(cache.GetCapacity() == capacity);

    MESSAGE("cache should be able to cache data up to capacity, and start evict");
    {
      for (int i = 1; i <= capacity / data_size; i++) {
        std::string url = "url" + std::to_string(i);
        bool cache_success = cache.TryInsert(url, data);
        CHECK(cache_success);
        CHECK(cache.GetCurSize() == i * data_size);
      }

      std::vector<unsigned char> read_buf;
      // all url1, url2, url3 should be available
      for (int i = 1; i <= capacity / data_size; i++) {
        auto load_success = cache.TryLoad("url" + std::to_string(i), read_buf);
        CHECK(load_success);
      }
      CHECK(read_buf.size() == ((capacity / data_size) * data_size));

      // now is 3 * 6 = 18 bytes, next insert should evict the first guy
      bool cache_success = cache.TryInsert("url4", data);
      CHECK(cache_success);

      // url1 should be evicted
      bool load_success = cache.TryLoad("url1", read_buf);
      CHECK(!load_success);
    }
  }

}
