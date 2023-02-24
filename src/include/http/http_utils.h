#ifndef SRC_INCLUDE_HTTP_HTTP_UTILS_H
#define SRC_INCLUDE_HTTP_HTTP_UTILS_H

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

namespace Icee::Http {

/*
 * static 全局的变量，只能是引入了这个文件才能够进行访问
 */

static constexpr char SPACE[] = {" "};

// 这里供给所有的 Http 模块使用，能够直接使用 static const variable
static constexpr char READ_WRITE_PERMISSION = 0600;
static constexpr char CRLF[] = {"\r\n"};
static constexpr char DEFAULT_ROUTE[] = {"index.html"};
static constexpr char COLON[] = {":"};
static constexpr char Dot[] = {"."};

/* 常见的 header & value*/

/** Response status*/
static constexpr char RESPONSE_OK[] = {"200 OK"};
static constexpr char RESPONSE_BAD_REQUEST[] = {"400 Bad Request"};
static constexpr char RESPONSE_NOT_FOUND[] = {"404 Not Found"};
static constexpr char RESPONSE_SERVICE_UNAVAILABLE[] = {"503 Service Unavailable"};


static constexpr char MIME_HTML[] = {"text/html"};
static constexpr char MIME_CSS[] = {"text/css"};
static constexpr char MIME_PNG[] = {"image/png"};
static constexpr char MIME_JPG[] = {"image/jpg"};
static constexpr char MIME_JPEG[] = {"image/jpeg"};
static constexpr char MIME_GIF[] = {"image/gif"};
static constexpr char MIME_OCTET[] = {"application/octet-stream"};

/** 常见的 Header & Value */
static constexpr char HEADER_SERVER[] = {"Server"};
static constexpr char SERVER_ICEE[] = {"Icee/1.0"};
static constexpr char HEADER_CONNECTION[] = {"Connection"};
static constexpr char CONNECTION_CLOSE[] = {"Close"};
static constexpr char CONNECTION_KEEP_ALIVE[] = {"Keep-Alive"};
static constexpr char HEADER_CONTENT_LENGTH[] = {"Content-Length"};
static constexpr char HEADER_CONTENT_TYPE[] = {"Content-Type"};
static constexpr char HTTP_VERSION[] = {"Http/1.1"};


/** HTTP Method enum class, do not support GET/HEAD method */
enum class Method { GET, HEAD, UNSUPPORTED };
/** HTTP Version, support HTTP/1.1*/
enum class Version { HTTP_1_1, UNSUPPORTED };
/** File type: the extension */
enum class Extension { HTML, CSS, PNG, JPG, JPEG, GIF, OCTET };

/*** Method map, method => string */
static const std::unordered_map<Method, std::string> Method_String_Map {
    {Method::GET, "GET"},
    {Method::HEAD, "HEAD"},
    {Method::UNSUPPORTED, "UNSUPPORTED"}
};

/** Version map, version => string */
static const std::unordered_map<Version, std::string> Version_String_Map {
    {Version::HTTP_1_1, "Http/1.1"},
    {Version::UNSUPPORTED, "UNSUPPORTED"}
};

static const std::unordered_map<Extension, std::string> Extension_String_Map {
    {Extension::HTML, "HTML"},  {Extension::CSS, "CSS"},
    {Extension::PNG, "PNG"},    {Extension::JPG, "JPG"},
    {Extension::JPEG, "JPEG"},  {Extension::GIF, "GIF"},
    {Extension::OCTET, "OCTET"}};

/*
 * Split s string into many sub string, 通过传入的指定的 分隔符 进行数据的分割
 */
auto Split(const std::string &str, const char *delimer = SPACE) -> std::vector<std::string>;

auto Trim(const std::string &str, const char* delim = SPACE) -> std::string;

auto ToUpper(std::string str) -> std::string;

auto IsFileExist(const std::string& file_path) -> bool;

auto FileSize(const std::string &file_path) -> size_t;

auto Format(const std::string &str) noexcept -> std::string;

/** 传进来一个 string(可能是任意类型的不标准的、带有前后空白的方法) 获得 Method 类型的返回值*/
auto ToMethod(const std::string &method_str) noexcept -> Method;

auto ToVersion(const std::string &version_str) noexcept -> Version;

auto ToExtension(const std::string &raw_str) noexcept -> Extension;

auto ExtensionToMime(const Extension &extension) noexcept -> std::string;

/*
 * Join, join the vector of values by the separator ":"
 */

auto Join(const std::vector<std::string> &tokens, const char *deli = SPACE) noexcept -> std::string;


















}  // namespace Icee::Http

#endif