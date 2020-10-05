#ifdef DEBUG 
#define D(x) (x)
#else 
#define D(x) do{}while(0)
#endif

#include <algorithm>
#ifdef DEBUG
#include <iostream>
#endif
#include <deque>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#ifdef DEBUG
#include "debug_util.hpp"
#endif
#include "oracle.hpp"



void Oracle::Reset(const std::vector<std::unordered_set<int>>& tree, int root, int degree) {
  D(std::cerr << "Resetting oracle...\n");

  this->tree_ = tree;
  this->root_ = root;
  this->degree_ = degree;
  this->query_count_ = 0;
  this->ancestors_.clear();
  this->ancestors_ = std::vector<std::unordered_set<int>>(tree.size());

  std::vector<std::pair<int, int>> stack = {{-1, this->root_}};
  while (!stack.empty()) {
    auto [p, v] = stack.back();  // p is the parent of v
    stack.pop_back();
    if (p >= 0) {
      this->ancestors_[v] = this->ancestors_[p];
    }
    this->ancestors_[v].insert(v);
    for (auto &w: this->tree_[v]) {
      if (w != p) {
        stack.push_back({v, w});
      }
    }
  }
  D(std::cerr << "Finished initializing oracle\n");
  // #ifdef DEBUG
  // for (int i = 0; i < this->ancestors_.size(); ++i) {
  //   std::cerr << "Ancestors of " << i << ": " <<  this->ancestors_[i] << '\n';
  // }
  // #endif
}


bool Oracle::IsAncestor(int u, int v) {
  if (0 <= u && 0 <= v && std::max(u, v) < this->ancestors_.size()) {
    this->query_count_++;
    return this->ancestors_[v].find(u) != this->ancestors_[v].end();
  }
  throw std::out_of_range("At least one of the vertices given is not in the tree!");
}


bool Oracle::IsHiddenTree(
    const std::unordered_set<std::pair<int, int>>& edges) const {
  // Avoid caching hidden edges to save space.
  std::unordered_set<std::pair<int, int>> hidden_edges;

  std::vector<std::pair<int, int>> stack{{-1, this->root_}};
  while (!stack.empty()) {
    auto [u, v] = stack.back();  // u is the parent of v
    stack.pop_back();
    for (auto &w: this->tree_[v]) {
      if (w != u) {
        hidden_edges.insert({v, w});
        stack.push_back({v, w});
      }
    }
  }
  return hidden_edges == edges;
}
