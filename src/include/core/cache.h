#ifndef SRC_INCLUDE_CORE_CACHE_H
#define SRC_INCLUDE_CORE_CACHE_H

#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "core/utils.h"

namespace Icee {

static const int INIT_CACHE_SIZE = 10 * 1024 * 1024;

auto GetCurrentTime() -> uint64_t;

class Cache {
 public:
  /*
   * Internal class: cache node. It contains the last access timestamp & acts as the node of
   * double linked list. It also contains a data_ to store the binary data
   */

  class CacheNode {
    friend class Cache;

   public:
    CacheNode() noexcept;

    CacheNode(std::string identifier, const std::vector<unsigned char> &data);

    void SetData(std::vector<unsigned char> &data);

    auto Size() const -> size_t;

    void SetIdentifier(std::string &identifier);

    auto GetIdentifier() -> std::string;

    void UpdateTimeStamp();

    auto GetTimeStamp() -> uint64_t;

    void Serialize(std::vector<unsigned char> &destination);

   private:
    /** 资源的标识符号，用于在 unordered_map 中快速的定位数据*/
    std::string identifier_;
    /** 上一次获取的时间戳，用于LRU替换*/
    int last_access_time_{0};
    /** 可能会存放 binary 数据*/
    std::vector<unsigned char> data_;

    CacheNode *prev_{nullptr};
    CacheNode *next_{nullptr};
  };

  explicit Cache(int capacity = INIT_CACHE_SIZE) noexcept;

  NON_COPYABLE_AND_MOVEABLE(Cache);

  auto GetCapacity() const noexcept -> int;

  auto GetCurSize() const noexcept -> int;

  /** 清除cache & lru*/
  void Clear();

  void RemoveFromList(const std::shared_ptr<CacheNode> &node);

  void AppendToEndOfList(const std::shared_ptr<CacheNode> &node);

  /** 如果请求的资源是存在于本 cache 中的那么存于 dest 中 & Return true */
  auto TryLoad(const std::string &identifier, std::vector<unsigned char> &destination) -> bool;

  auto TryInsert(const std::string &resource_identifier, const std::vector<unsigned char> &source) -> bool;

  void EvitOne() noexcept;

 private:
  /** support concurrency*/
  std::shared_mutex latch_;  // cause there are r-w methods, use shared mutex rather than mutex
  /** map the resource with the cache_node(many user will hold, use shared_ptr)*/
  std::unordered_map<std::string, std::shared_ptr<CacheNode>> mapping_;

  const int capacity_{INIT_CACHE_SIZE};
  int current_size_{0};

  /** dummy node of the double-linked list */
  std::shared_ptr<CacheNode> dummy_header_;
  std::shared_ptr<CacheNode> dummy_tailer_; 
};

}  // namespace Icee

#endif
