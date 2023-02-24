#include "http/request.h"

#include "http/header.h"
#include "http/http_utils.h"

#include <vector>

namespace Icee::Http {

Request::Request(Method method, Version version, std::string resource_url, const std::vector<Header> &headers) noexcept
    : method_(method), version_(version), url_(std::move(resource_url)), headers_(headers), is_valid(true) {}

Request::Request(const std::string &request_str) noexcept {}

}  // namespace Icee::Http