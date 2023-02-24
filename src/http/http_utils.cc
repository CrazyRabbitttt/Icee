#include "http/http_utils.h"

namespace Icee::Http {

/*
 * origin string => "hello:this:new:world:!" ， 通过
 * 需要被分割符号进行分割，{hello, this, new, world}
 */
auto Split(const std::string &str, const char *delim) -> std::vector<std::string> {
  std::vector<std::string> res;
  if (str.empty()) {
    return res;
  }
  int begin_index{0};
  size_t target_index;
  auto delim_len = strlen(delim);
  while ((target_index = str.find(delim, begin_index)) != std::string::npos) {  // 只要是能够进行分割出数据
    res.emplace_back(str.substr(begin_index, target_index - begin_index));
    begin_index = target_index + delim_len;
  }
  /*
   * corner case: "hello:world:haha"  =>
   *                          ^【在这里开始] 但是没有再找到 : 了，还是需要加上 haha
   * */
  if (begin_index != str.size()) {
    res.emplace_back(str.substr(begin_index, str.size() - begin_index));
  }
  return std::move(res);
}

auto Join(const std::vector<std::string> &tokens, const char *deli) noexcept -> std::string {
  assert(!tokens.empty());
  std::string res{};
  if (tokens.size() == 1) {
    return tokens[0];
  }
  auto n = tokens.size();
  for (int i = 0; i < n; i++) {
    if (i != n - 1) {
      res += ":";
    }
    res += tokens[i];
  }
  return res;
}

}  // namespace Icee::Http