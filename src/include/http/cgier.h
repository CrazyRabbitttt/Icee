#ifndef SRC_INCLUDE_HTTP_CGIER_H_
#define SRC_INCLUDE_HTTP_CGIER_H_

#include <string>
#include <vector>


namespace Icee::Http {

/*
 * 通用网关协议
 * 创建进程去接收来自于客户端的请求，内部设定好协议 然后回送给 Server
 * 虽然说 CGI 的功能能够直接在 Web 服务器中完成，但是这样的话拓展性比较好
 *
 */


class Cgier {

  explicit Cgier(const std::string &path, const std::vector<std::string> &arguments);

 private:
  std::string cgi_program_path_;
  std::vector<std::string> cgi_arguments_;
  bool valid_{true};
};





}

#endif