#include "core/cache.h"

#include <chrono>
#include <utility>

namespace Icee {

auto GetCurrentTime() -> uint64_t {
  auto million =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
          .count();
  return million;
}

Cache::CacheNode::CacheNode() noexcept { UpdateTimeStamp(); }

Cache::CacheNode::CacheNode(std::string identifier, const std::vector<unsigned char> &data)
    : identifier_(std::move(identifier)), data_(data) {
  UpdateTimeStamp();
}

void Cache::CacheNode::SetIdentifier(std::string &identifier) { identifier_ = identifier; }

auto Cache::CacheNode::GetIdentifier() -> std::string { return identifier_; }

auto Cache::CacheNode::Size() const -> size_t { return data_.size(); }

// should not be move
void Cache::CacheNode::SetData(std::vector<unsigned char> &data) { data_ = data; }

void Cache::CacheNode::UpdateTimeStamp() { last_access_time_ = GetCurrentTime(); }

auto Cache::CacheNode::GetTimeStamp() -> uint64_t { return last_access_time_; }

void Cache::CacheNode::Serialize(std::vector<unsigned char> &destination) {
  destination.reserve(data_.size() + destination.size());
  destination.insert(destination.end(), data_.begin(), data_.end());
}

auto Cache::GetCapacity() const noexcept -> int { return capacity_; }

auto Cache::GetCurSize() const noexcept -> int { return current_size_; }

auto Cache::TryLoad(const std::string &identifier, std::vector<unsigned char> &destination) -> bool {
  auto index = mapping_.find(identifier);
  if (index != mapping_.end()) {  // 找到了数据
    index->second->Serialize(destination);
    // 将 cache node 的位置移动到末尾 作为 most recently accessed
    RemoveFromList(index->second);
    AppendToEndOfList(index->second);

    index->second->UpdateTimeStamp();
    return true;
  }
  return false;
}

void Cache::EvitOne() noexcept {
  assert(dummy_header_->next_ != dummy_tailer_.get());
  // move the first cache node out
  auto *first_node = dummy_header_->next_;
  auto resource_size = first_node->Size();
  auto iter = mapping_.find(first_node->GetIdentifier());
  assert(iter != mapping_.end());
  RemoveFromList(iter->second);
  mapping_.erase(iter);
  current_size_ -= resource_size;
}

auto Cache::TryInsert(const std::string &resource_identifier, const std::vector<unsigned char> &source) -> bool {
  std::unique_lock<std::shared_mutex> lock(latch_);
  // 如果说本身就是存在于 Cache 中
  if (mapping_.find(resource_identifier) != mapping_.end()) {
    return false;
  }
  int needed_size = source.size();
  if (needed_size > capacity_) {
    return false;
  }
  // 容量不够了那么就 EvictOne
  while ((capacity_ - current_size_) < needed_size && !mapping_.empty()) {
    EvitOne();
  }
  // 然后创建一个新的 cache node
  auto new_node = std::make_shared<CacheNode>(resource_identifier, source);
  AppendToEndOfList(new_node);
  current_size_ += needed_size;
  mapping_[resource_identifier] = new_node;
  return true;
}

void Cache::Clear() {
  dummy_tailer_->prev_ = dummy_header_.get();
  dummy_header_->next_ = dummy_tailer_.get();

  mapping_.clear();
  current_size_ = 0;
}

Cache::Cache(int capacity) noexcept
    : capacity_(capacity), dummy_header_(std::make_shared<CacheNode>()), dummy_tailer_(std::make_shared<CacheNode>()) {
  dummy_tailer_->prev_ = dummy_header_.get();
  dummy_header_->next_ = dummy_tailer_.get();
}

// 直接获得前后的指针 然后互相指就好了（因为有哑节点的存在，不需要关心边界）
void Cache::RemoveFromList(const std::shared_ptr<CacheNode> &node) {
  auto *node_ptr = node.get();
  auto *pre_ptr = node_ptr->prev_;
  auto *next_ptr = node_ptr->next_;

  pre_ptr->next_ = next_ptr;
  next_ptr->prev_ = pre_ptr;
}

void Cache::AppendToEndOfList(const std::shared_ptr<CacheNode> &node) {
  auto *node_ptr = node.get();
  auto *prev_ptr = dummy_tailer_->prev_;

  node_ptr->prev_ = prev_ptr;
  node_ptr->next_ = dummy_tailer_.get();

  prev_ptr->next_ = node_ptr;
  dummy_tailer_->prev_ = node_ptr;
}

}  // namespace Icee
