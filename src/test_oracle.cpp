#include <cassert>
#include <stdexcept>

#include "oracle.hpp"

#include "test_util.hpp"


void test_IsAncestor() {
  std::vector<std::unordered_set<int>> tree;
  int root = 0;

  {
    // Test single root tree
    tree = {{}};
    Oracle o;
    o.Reset(tree, root, 0);
    assert(o.IsAncestor(0, 0));
  }

  {
    // Test out of bounds
    tree = {{1}, {0}};
    Oracle o;
    o.Reset(tree, root, 0);
    EXPECT_EXCEPTION((o.IsAncestor(0, 2)), std::out_of_range)
    EXPECT_EXCEPTION((o.IsAncestor(3, 2)), std::out_of_range)
    EXPECT_EXCEPTION((o.IsAncestor(-1, 2)), std::out_of_range)
    EXPECT_EXCEPTION((o.IsAncestor(-1, -2)), std::out_of_range)
  }

  {
    // Test single path: 0(1(2(3(4(5)))))
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    Oracle o;
    o.Reset(tree, root, 2);

    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 6; ++j) {
        assert(((i <= j) == o.IsAncestor(i, j)));
      }
    }
  }

  {
    // Test simple tree: 0(1 2(4 5) 3(6(7)))
    tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
    Oracle o;
    o.Reset(tree, root, 3);

    bool expected[8][8] = {{1, 1, 1, 1, 1, 1, 1, 1},
                           {0, 1, 0, 0, 0, 0, 0, 0},
                           {0, 0, 1, 0, 1, 1, 0, 0},
                           {0, 0, 0, 1, 0, 0, 1, 1},
                           {0, 0, 0, 0, 1, 0, 0, 0},
                           {0, 0, 0, 0, 0, 1, 0, 0},
                           {0, 0, 0, 0, 0, 0, 1, 1},
                           {0, 0, 0, 0, 0, 0, 0, 1}};
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        assert(expected[i][j] == o.IsAncestor(i, j));
      }
    }
  }
}


void test_Initialize_large() {
  int n = 10 * 1000;
  std::vector<std::unordered_set<int>> tree(n);
  std::unordered_set<int> V;

  for (int i = 0; i < n; ++i) {
    V.insert(i);
    if (i == 0) {
      tree[i] = {1};
    } else if (i == n - 1) {
      tree[i] = {n - 2};
    } else {
      tree[i] = {i - 1, i + 1};
    }
  }

  Oracle o;
  o.Reset(tree, 0, 2);
}


void test_IsHiddenTree() {
  std::vector<std::unordered_set<int>> tree;
  std::unordered_set<std::pair<int, int>> expected_edges;
  int root = 0;

  {
    // Test single root tree
    tree = {{}};
    Oracle o;
    o.Reset(tree, root, 0);

    // Correct
    expected_edges = {};
    assert(o.IsHiddenTree(expected_edges));

    // Incorrect
    expected_edges = {{1, 2}};
    assert(!o.IsHiddenTree(expected_edges));
  }

  {
    // Test single path: 0(1(2(3(4(5)))))
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    Oracle o;
    o.Reset(tree, root, 2);

    // Correct
    expected_edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}};
    assert(o.IsHiddenTree(expected_edges));

    // Missing {4, 5}
    expected_edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    assert(!o.IsHiddenTree(expected_edges));

    // Missing all edges
    expected_edges = {};
    assert(!o.IsHiddenTree(expected_edges));

    // Extra edges
    expected_edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}};
    assert(!o.IsHiddenTree(expected_edges));
  }

  {
    // Test simple tree: 0(1 2(4 5) 3(6(7)))
    tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
    Oracle o;
    o.Reset(tree, root, 3);

    // Correct
    expected_edges = {{0, 1}, {0, 2}, {0, 3}, {2, 4}, {2, 5}, {3, 6}, {6, 7}};
    assert(o.IsHiddenTree(expected_edges));

    // Missing {3, 6}
    expected_edges = {{0, 1}, {0, 2}, {0, 3}, {2, 4}, {2, 5}, {6, 7}};
    assert(!o.IsHiddenTree(expected_edges));

    // Missing all edges
    expected_edges = {};
    assert(!o.IsHiddenTree(expected_edges));

    // Extra edges
    expected_edges = {{0, 1}, {0, 2}, {0, 3}, {2, 4}, {2, 5}, {3, 6}, {6, 7}, {1, 4}};
    assert(!o.IsHiddenTree(expected_edges));
  }
}


int main() {
  test_IsAncestor();
  // test_Initialize_large();
  // test_IsHiddenTree();
}
