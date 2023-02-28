/*
 * 用来处理 Http 请求的 Server
 */

#include "core/icee_server.h"

namespace Icee::Http {

/*
 * onHandle：当收到客户端发送的数据的时候，执行的回调函数
 * 需要从 connection 中读取出来数据，然后解析，生成 response 发送数据回去
 */

void ParseHttpRequest(const std::string &serving_directory, std::shared_ptr<Cache> &cache, Connection *client_conn) {
  printf("Will call parse code\n");
  // 通过解析 client_conn 的内容
  // ET 模式需要一次性将数据读取出来(Recv()内置了while循环读取)
  int fd = client_conn->GetFd();
  auto [size, exit] = client_conn->Recv();
  // client 关闭了连接
  if (exit) {
    client_conn->GetLooper()->DeleteConnection(fd);
    return;
  }

  bool no_more_parse{false};
  // 因为是长连接，所以不断的接收，只要 Return value 不是 nullopt
  std::optional<std::string> request_option = client_conn->FindAndPopTill("\r\n\r\n");
  while (request_option != std::nullopt) {
    Request request{request_option.value()};
    std::vector<unsigned char> response_buf;
    if (!request.IsValid()) {
      auto response = Response::Make400Response();  // the request is invalid
      response.Serialize(response_buf);
      no_more_parse = true;
    } else {
      // 获得 url & make the file path
      std::string resource_full_path = serving_directory + request.GetResourceUrl();
      if (IsCgiRequest(resource_full_path)) {
        printf("Not implement this cgi code\n");
        return;
      } else {                                          // static resource request ?
        if (!IsFileExist(resource_full_path)) {         // 文件不存在
          auto response = Response::Make404Response();  // the request is invalid
          response.Serialize(response_buf);
          no_more_parse = true;
        } else {  // 文件存在
          auto response = Response::Make200Response(request.ShouldClose(), resource_full_path);
          // 1.尝试从 cache 中获取数据 & 将 response 报文序列化到 buffer 中
          std::vector<unsigned char> cache_buf;
          auto resource_cached = cache->TryLoad(resource_full_path, cache_buf);
          response.SetShouldTransferContent(request.GetMethod() != Method::HEAD && !resource_cached);
          no_more_parse = request.ShouldClose();
          response.Serialize(response_buf);  // basic data has serialize into the buffer
          // 2.如果需要传送文件，序列化到 buffer 中
          if (resource_cached) {
            // 2.1 从缓存中取出来的数据
            response_buf.insert(response_buf.end(), cache_buf.begin(), cache_buf.end());
          } else {
            // 2.2 文件没有存在于缓存中，try to store it
            LoadFile(resource_full_path, cache_buf);
            cache->TryInsert(resource_full_path, cache_buf);  // 所以说所有的数据都通过 cache
          }
        }
      }
    }
    // send the response to the
    client_conn->WriteToWriteBuffer(std::move(response_buf));
    client_conn->Send();
    if (no_more_parse) {
      break;
    }
    // 如果还是需要继续的话那么就继续从 buffer 中找到 \r\n\r\n 为结尾的数据
    // 因为一个合法的 HTTP 报文就是以 \r\n\r\n 为结尾，进行分割，不论当前收到了什么
    // 我们只从 buffer 中按照这个边界截取报文
    request_option = client_conn->FindAndPopTill("\r\n\r\n");
  }

  if (no_more_parse) {  // 将 connection 进行关闭
    client_conn->GetLooper()->DeleteConnection(fd);
    return;
  }
}

}  // namespace Icee::Http

int main(int argc, char **argv) {
  const std::string usage =
      "Usage: \n"
      "./http_server [port:(default=12138)] [directory:(default=../http_dir/)]\n";
  if (argc > 3) {
    std::cout << "The number of argument error\n";
    std::cout << usage;
    exit(EXIT_FAILURE);
  }

  Icee::NetAddress address("127.0.0.1", 12138);
  std::string directory = "../http_dir/";
  if (argc >= 2) {
    auto port = static_cast<uint16_t>(std::strtol(argv[1], nullptr, 10));
    if (port == 0) {
      std::cout << "port error\n";
      std::cout << usage;
      exit(EXIT_FAILURE);
    }
    address = {"0.0.0.0", port};
    if (argc == 3) {
      directory = argv[2];
      if (!Icee::Http::IsDirectoryExists(directory)) {
        std::cout << "directory error\n";
        std::cout << usage;
        exit(EXIT_FAILURE);
      }
    }
  }
  Icee::IceeServer http_server(address);
  auto cache = std::make_shared<Icee::Cache>();
  http_server
      .OnHandle([&](Icee::Connection *client_conn) { Icee::Http::ParseHttpRequest(directory, cache, client_conn); })
      .Begin();

  return 0;
}