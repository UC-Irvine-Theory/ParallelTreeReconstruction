#ifndef DEBUG_UTIL_HPP_
#define DEBUG_UTIL_HPP_

#include <ostream>
#include <optional>
#include <vector>
#include <utility>

#include "wang.hpp"



// Debug unordered_sets
template <typename T>
std::ostream& operator<<(std::ostream &os, const std::unordered_set<T>& set) {
  for(auto &e: set) {
    os << e << ' ';
  }
  return os;
}


// Debug vectors
template <typename T>
std::ostream& operator<<(std::ostream &os, const std::vector<T>& vec) {
  for(auto &e: vec) {
    os << e << ' ';
  }
  return os;
}


// Debug pairs
template <typename T1, typename T2>
std::ostream& operator<<(std::ostream &os, const std::pair<T1, T2>& p) {
  os << '(' << p.first << ", " << p.second << ')';
  return os;
}


// Debug optionals
template <typename T>
std::ostream& operator<<(std::ostream &os, const std::optional<T>& p) {
  if (p) {
    os << *p;
  } else {
    os << "{}";
  }
  return os;
}


// Debug multidirectional path
inline std::ostream& operator<<(std::ostream &os, const MultidirectionalPath& path) {
  os << "left=[" << path.left << ']';
  os << " right=[" << path.right << "]\n";
  return os;
}

#endif
