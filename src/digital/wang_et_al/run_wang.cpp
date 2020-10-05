#include <iostream>

#include "wang.hpp"

int main() {
  int T, reps = 3;
  std::vector<std::unordered_set<int>> tree;
  std::unordered_set<int> V;
  Oracle o;

  std::cout << "n\td\tk\trep\tquery\tround\n";
  std::cin >> T;
  for (int t = 0; t < T; ++t) {
    tree.clear();
    V.clear();
    int n, d, k, x, neighbor_count;

    // Parse tree.
    std::cin >> n >> d >> k;
    tree = std::vector<std::unordered_set<int>>(n);
    for (int i = 0; i < n; ++i) {
      V.insert(i);
      std::cin >> neighbor_count;
      for (int j = 0; j < neighbor_count; ++j) {
        std::cin >> x;
        tree[i].insert(x);
      }
    }

    // Create oracle and initialize it.
    o.Reset(tree, 0, d);

    // Reconstruct tree for a number of reps.
    for (int r = 0; r < reps; ++r) {
      o.set_query_count(0);
      auto E = reconstruct_tree(o, {V.begin(), V.end()});
      assert(o.IsHiddenTree({E.begin(), E.end()}));
      std::cout << n << '\t' << d << '\t' << k << '\t'
                << r << '\t' << o.query_count() << '\t' << 1 << '\n';
    }
  }
}
