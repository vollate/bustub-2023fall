#include "primer/trie.h"
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
  if (root_ == nullptr) {
    return nullptr;
  }
  auto cur_root = std::const_pointer_cast<TrieNode>(root_);
  auto next_root = cur_root;
  for (const auto key_char : key) {
    if (auto val = cur_root->children_.find(key_char); val != cur_root->children_.end()) {
      next_root = std::const_pointer_cast<TrieNode>(cur_root->children_[key_char]);
      std::swap(cur_root, next_root);
    } else {
      return nullptr;
    }
  }
  if (!cur_root->is_value_node_) {
    return nullptr;
  }
  T *ret = nullptr;
  if (auto value_node = std::dynamic_pointer_cast<TrieNodeWithValue<T>>(cur_root); value_node != nullptr) {
    ret = value_node->value_.get();
  }
  return ret;
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should creatSe a new `TrieNodeWithValue`.
  Trie ret{root_};
  auto cur_root = root_;
  auto next_root = cur_root;
  for (const auto key_char : key) {
    if (auto val = cur_root->children_.find(key_char); val != cur_root->children_.end()) {
      next_root = std::const_pointer_cast<TrieNode>(cur_root->children_[key_char]);
      std::swap(cur_root, next_root);
    } else {
      auto new_root = std::make_shared<TrieNode>();
      for (const auto &kv_pair : cur_root->children_) {
        new_root->children_.insert(kv_pair);
      }
      new_root[key_char] = std::make_shared<TrieNode>();
    }
  }
  return ret;
}

auto Trie::Remove(std::string_view key) const -> Trie {
  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
  // TODO(vollate): unimplement
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
