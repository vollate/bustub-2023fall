#include "primer/trie.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string_view>
#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.

  auto cur_root = root_;
  for (const auto key_char : key) {
    if (auto val = cur_root->children_.find(key_char); val != cur_root->children_.end()) {
      cur_root = cur_root->children_.at(key_char);
    } else {
      return nullptr;
    }
  }
  if (!cur_root->is_value_node_) {
    return nullptr;
  }
  T *ret = nullptr;
  if (auto value_node = std::dynamic_pointer_cast<const TrieNodeWithValue<T>>(cur_root); value_node != nullptr) {
    ret = value_node->value_.get();
  }
  return ret;
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should creatSe a new `TrieNodeWithValue`.
  Trie ret{};
  if (key.empty()) {
    ret.root_ = std::make_shared<TrieNode>(TrieNodeWithValue<T>(std::make_shared<T>(std::move(value))));
    return ret;
  }
  auto new_root = std::make_shared<TrieNode>();
  ret.root_ = new_root;
  auto old_root = root_;
  std::shared_ptr<TrieNode> next_root = nullptr;
  for (size_t i = 0; i < key.length() - 1; ++i) {
    char key_char = key[i];
    if (old_root != nullptr) {
      new_root->children_ = old_root->children_;
      if (old_root->children_.find(key_char) != old_root->children_.end()) {
        old_root = old_root->children_.at(key_char);
      } else {
        old_root = nullptr;
      }
    }
    next_root = std::make_shared<TrieNode>();
    new_root->children_[key_char] = next_root;
    new_root = next_root;
  }
  new_root->children_.insert(
      {key[key.length() - 1], std::make_shared<TrieNode>(TrieNodeWithValue<T>(std::make_shared<T>(std::move(value))))});
  return ret;
}

auto Trie::Remove(std::string_view key) const -> Trie {
  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
  Trie ret{};
  if (key.empty()) {
    ret.root_ = std::make_shared<TrieNode>(root_->children_);
    return ret;
  }
  auto cur_root = std::make_shared<TrieNode>();
  ret.root_ = cur_root;
  auto old_root = root_;
  for (size_t i = 0; i < key.length() - 1; ++i) {
    cur_root->children_ = old_root->children_;
    cur_root = cur_root->children_.at(key[i])->Clone();
  }
  char last_char = key[key.length() - 1];
  auto target = cur_root->children_.at(last_char);
  if (target->children_.empty()) {
    cur_root->children_.erase(last_char);
  } else {
    target = std::make_shared<TrieNode>(target->children_);
  }
  return ret;
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
