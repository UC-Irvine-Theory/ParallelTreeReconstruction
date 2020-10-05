#ifdef DEBUG 
#define D(x) (x)
#else 
#define D(x) do{}while(0)
#endif

#include <algorithm>
#ifdef DEBUG
#include <iostream>
#endif
#include <optional>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>

#ifdef DEBUG
#include "debug_util.hpp"
#endif
#include "oracle.hpp"
#include "wang.hpp"


template <typename Iterator>
std::vector<int> sort_ancestors(Oracle& o, const Iterator& ancestors) {
  std::vector<int> sorted_ancestors = {ancestors.begin(), ancestors.end()};
  std::sort(sorted_ancestors.begin(), sorted_ancestors.end(),
            [&o](int a, int b) {
              // a must be strictly "less than" b
              return o.IsAncestor(a, b) && a != b;
            });
  return sorted_ancestors;
}


std::vector<int> get_path_from_root(
    Oracle& o,
    const std::unordered_set<int>& V,
    int u) {
  std::unordered_set<int> ancestors;
  for (auto &v: V) {
    if (o.IsAncestor(v, u)) {
      ancestors.insert(v);
    }
  }
  return sort_ancestors(o, ancestors);
}


int get_lca(Oracle& o, const std::unordered_set<int>& V, int u, int v) {
  auto path = get_path_from_root(o, V, u);
  D(std::cerr << "path from root=" << path << '\n');
  if (path.empty()) {
    throw std::runtime_error("Got empty set of ancestors during call to lca()!");
  }
  for (auto w = path.rbegin(); w != path.rend(); ++w) {
    if (o.IsAncestor(*w, v)) {
      return *w;
    }
  }
  throw std::runtime_error("Vertices given to lca() do not have a common ancestor!");
}


MultidirectionalPath get_multidirectional_path(Oracle& o,
                                               const std::unordered_set<int>& V,
                                               int u,
                                               int v) {
  if (u == v) {
    throw std::invalid_argument("The given endpoints correspond to the same vertex");
  }
  auto lca = get_lca(o, V, u, v);
  D(std::cerr << "lca=" << lca << '\n');
  std::unordered_set<int> left_ancestors, right_ancestors;

  for (auto &w: V) {
    if (o.IsAncestor(w, u) && o.IsAncestor(lca, w)) {
      left_ancestors.insert(w);
    }
    if (o.IsAncestor(w, v) && o.IsAncestor(lca, w)) {
      right_ancestors.insert(w);
    }
  }
  return MultidirectionalPath(sort_ancestors(o, left_ancestors),
                              sort_ancestors(o, right_ancestors));
}


int find_bag(Oracle& o, const std::vector<int>& path, int u) {
  int m, lo = -1, hi = path.size() - 1;
  while (lo < hi) {
    m = (lo + hi + 1) >> 1;
    if (o.IsAncestor(path[m], u)) {
      lo = m;
    } else {
      hi = m - 1;
    }
  }
  return lo;
}


// void reconstruct_tree(const Oracle& o, const std::unordered_set<int>& V) {

// }

MultidirectionalPath get_bag_sizes(Oracle& o,
                                   const std::unordered_set<int>& V,
                                   const MultidirectionalPath& multipath) {
  // Initialize bag sizes to have 0 elements.
  MultidirectionalPath result(std::vector<int>(multipath.left.size()),
                              std::vector<int>(multipath.right.size()));

  for (auto &w: V) {
    int position_left = find_bag(o, multipath.left, w);
    int position_right = find_bag(o, multipath.right, w);

    // Correct position can only be positive in one of the paths.
    // If not positive in either path, then it can only be 0 on both paths,
    // or -1 on both paths.
    if ((position_left == -1 && position_right == -1) ||
        (position_left == 0 && position_right == 0)) {
      result.left[0] += 1;
      result.right[0] += 1;
    } else if (position_left > position_right) {
        result.left[position_left] += 1;
    } else if (position_left < position_right) {
      result.right[position_right] += 1;
    } else {
      throw std::runtime_error("This cannot happen!");
    }
  }
  return result;
}


std::vector<int> make_undirected(const MultidirectionalPath& multipath) {
  if (multipath.left.empty() || multipath.right.empty()) {
    throw std::runtime_error("The multidirectional path given contains an empty path!");
  }

  std::vector<int> result(multipath.left.size());

  // Reverse left path, so we get an undirected path between endpoints and
  // across the lowest common ancestor of the endpoints.
  std::reverse_copy(multipath.left.begin(),
                    multipath.left.end(),
                    result.begin());

  // Drop lowest common ancestor to avoid repeating it.
  result.pop_back();

  // Concatenate the right path, which should be in its correct ordering.
  result.insert(result.end(), multipath.right.begin(), multipath.right.end());
  return result;
}


std::optional<std::pair<int, int>> get_edge_separator(
    const std::vector<int>& path,
    const std::vector<int>& bag_sizes,
    std::function<bool(int)> is_edge_separator) {
  if (path.size() < 2 || path.size() != bag_sizes.size()) {
    throw std::invalid_argument("Path sizes are too small or not consistent!");
  }

  int i = 0, acum = 0;
  do {
    acum += bag_sizes[i++];
  } while (!is_edge_separator(acum) && i < path.size());
  if (i < path.size()) {
    return {{path[i - 1], path[i]}};
  }
  return {};
}


std::optional<std::pair<int, int>> make_directed(
    Oracle& o,
    const std::optional<std::pair<int, int>>& edge) {
  if (edge) {
    if (!o.IsAncestor(edge->first, edge->second)) {
      return {{edge->second, edge->first}};
    }
    return edge;
  }
  return {};
}


std::pair<std::unordered_set<int>, std::unordered_set<int>> split_tree(
    Oracle& o,
    const std::unordered_set<int>& V,
    const std::pair<int, int>& edge_separator) {
  std::unordered_set<int> V1, V2;

  // Assuming given separator is directed.
  int v = edge_separator.second;
  for (auto &w: V) {
    o.IsAncestor(v, w) ? V2.insert(w) : V1.insert(w);
  }
  return {V1, V2};
}


std::pair<int, int> sample(const std::unordered_set<int>& V) {
  std::vector<int> result(2);
  std::sample(V.begin(), V.end(), result.begin(), 2,
              std::mt19937{std::random_device{}()});
  return {result[0], result[1]};
}


std::vector<std::pair<int, int>> reconstruct_tree(
    Oracle& o,
    std::unordered_set<int>&& V_ref) {
  auto V = std::move(V_ref);
  int n = V.size();
  int d = o.degree();
  #ifdef DEBUG
  std::cerr << "==================================\n";
  std::cerr << "n=" << n << ' ' << "d=" << d << '\n';
  std::cerr << V << '\n';
  #endif

  // Base case
  if (n <= 1) {
    return {};
  }

  // Inductive step
  auto is_edge_separator = [n, d](int size) {
    // If the maximum degree is 1, the tree must be a single edge.
    if (d == 1) {
      return true;
    }
    // Check if floor(n/d) <= size <= ceil(n(d-1)/d).
    // Avoids problems where n/d == n(d-1)/d and n/d is not integer.
    return (n / d <= size && size <= (n * (d - 1) + d - 1) / d);
  };
  
  std::optional<std::pair<int, int>> edge_separator;
  for (int i = 0; !edge_separator; ++i) {
    auto [u, v] = sample(V);
    D(std::cerr << "Nodes sampled: " << u << ' ' << v << '\n');

    auto multipath = get_multidirectional_path(o, V, u, v);
    D(std::cerr << "Multipath: " << multipath);

    auto bag_sizes = get_bag_sizes(o, V, multipath);
    D(std::cerr << "Bag sizes: " << bag_sizes);

    edge_separator = get_edge_separator(make_undirected(multipath),
                                        make_undirected(bag_sizes),
                                        is_edge_separator);
  }
  if (!edge_separator) {
    throw std::runtime_error("Could not find an even edge separator after d^2 iterations!");
  }

  // Make first endpoint the parent and the second endpoint the child.
  edge_separator = make_directed(o, edge_separator);
  D(std::cerr << "edge_separator=" << edge_separator << '\n');

  auto [V1, V2] = split_tree(o, std::move(V), *edge_separator);
  auto E1 = reconstruct_tree(o, std::move(V1));
  auto E2 = reconstruct_tree(o, std::move(V2));

  // Combine edge sets
  auto& E = E1;
  E.insert(E.end(), std::make_move_iterator(E2.begin()),
                    std::make_move_iterator(E2.end()));
  E.push_back(*edge_separator);
  return std::move(E);  // Avoid copy constructor from lvalue reference
}
