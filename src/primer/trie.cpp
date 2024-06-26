#include "primer/trie.h"
#include <cstddef>
#include <cstdint>
#include <list>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>
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
  auto cur_root = root_;
  for (const auto key_char : key) {
    if (auto val = cur_root->children_.find(key_char); val != cur_root->children_.end()) {
      cur_root = val->second;
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
    ret.root_ = std::make_shared<TrieNodeWithValue<T>>(root_->children_, std::make_shared<T>(std::move(value)));
    return ret;
  }
  std::shared_ptr<TrieNode> cur_new_root =
      (root_ == nullptr) ? std::make_shared<TrieNode>() : std::shared_ptr<TrieNode>(root_->Clone());
  auto last_new_root = cur_new_root;
  ret.root_ = cur_new_root;
  auto old_root = root_;
  for (char step_key : key) {
    if (old_root != nullptr) {
      cur_new_root->children_ = old_root->children_;
      if (auto next_old_root = old_root->children_.find(step_key); next_old_root != old_root->children_.end()) {
        cur_new_root->children_[step_key] = next_old_root->second->Clone();
        old_root = next_old_root->second;
      } else {
        cur_new_root->children_[step_key] = std::make_shared<TrieNode>();
        old_root = nullptr;
      }
    } else {
      cur_new_root->children_[step_key] = std::make_shared<TrieNode>();
    }
    last_new_root = cur_new_root;
    cur_new_root = std::const_pointer_cast<TrieNode>(cur_new_root->children_[step_key]);
  }
  last_new_root->children_[key[key.length() - 1]] =
      std::make_shared<TrieNodeWithValue<T>>(cur_new_root->children_, std::make_shared<T>(std::move(value)));
  return ret;
}

auto Trie::Remove(std::string_view key) const -> Trie {
  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
  // FIXME: what happened if the key is not in the trie?

  Trie ret{};
  if (key.empty()) {
    if (root_ != nullptr && !root_->children_.empty()) {
      ret.root_ = std::make_shared<TrieNode>(root_->children_);
    }
    return ret;
  }
  auto cur_new_root = std::make_shared<TrieNode>();
  std::list<std::shared_ptr<TrieNode>> history;
  ret.root_ = cur_new_root;
  auto old_root = root_;
  for (char step_key : key) {
    if (old_root != nullptr) {
      cur_new_root->children_ = old_root->children_;
      if (auto next_old_root = old_root->children_.find(step_key); next_old_root != old_root->children_.end()) {
        cur_new_root->children_[step_key] = next_old_root->second->Clone();
        old_root = next_old_root->second;
      } else {
        cur_new_root->children_[step_key] = std::make_shared<TrieNode>();
        old_root = nullptr;
      }
    } else {
      cur_new_root->children_[step_key] = std::make_shared<TrieNode>();
    }
    history.push_back(cur_new_root);
    cur_new_root = std::const_pointer_cast<TrieNode>(cur_new_root->children_[step_key]);
  }
  auto last_new_root = history.rbegin();
  if (old_root != nullptr && old_root->children_.find(key[key.length() - 1]) != old_root->children_.end()) {
    old_root = old_root->children_.find(key[key.length() - 1])->second;
  }
  if (old_root != nullptr && !old_root->children_.empty()) {
    last_new_root->get()->children_[key[key.length() - 1]] = std::make_shared<TrieNode>(old_root->children_);
  } else {
    last_new_root->get()->children_.erase(key[key.length() - 1]);
  }
  for (size_t i = 2; last_new_root != history.rend() && i <= key.length(); ++i) {
    if (!last_new_root->get()->is_value_node_ && last_new_root->get()->children_.empty()) {
      (++last_new_root)->get()->children_.erase(key[key.length() - i]);
    } else {
      break;
    }
  }
  if (ret.root_->children_.empty()) {
    ret.root_ = nullptr;
  }
  return ret;
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked
// up by the linker.

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
