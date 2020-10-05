#ifndef ORACLE_HPP_
#define ORACLE_HPP_

#include <vector>
#include <unordered_set>


// Define hash function for pairs
namespace std {
  template<class T> struct hash<std::pair<T, T>> {
    std::size_t operator()(std::pair<T, T> const& p) const noexcept {
      std::hash<T> hasher;
      std::size_t h = hasher(p.first) + 0x9e3779b9;
      h ^= hasher(p.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
      return h;
    }
  };
}


class Oracle {
public:
  Oracle() {}

  void Reset(const std::vector<std::unordered_set<int>>& tree, int root, int degree);

  // Whether u is an ancestor of v
  bool IsAncestor(int u, int v);

  // Whether given set of edges corresponds to the hidden tree.
  bool IsHiddenTree(const std::unordered_set<std::pair<int, int>>& edges) const;

  // Getters and setters
  int degree() const {
    return degree_;
  }

  int query_count() const {
    return query_count_;
  }

  void set_query_count(int val) {
    this->query_count_ = val;
  }

private:
  std::vector<std::unordered_set<int>> tree_;
  std::vector<std::unordered_set<int>> ancestors_;
  int root_;
  int degree_;
  int query_count_;
};

#endif
