#include "core/cache.h"

#include <vector>

#include "doctest/doctest.h"

using Icee::Cache;


TEST_CASE("[core/cache]") {
  SUBCASE("simple test") {
    CHECK(28 == 3 * 8 + 4);
  }

}
