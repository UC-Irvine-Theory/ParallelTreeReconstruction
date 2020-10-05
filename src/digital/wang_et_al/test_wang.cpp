#include <algorithm>
#include <cassert>
#include <optional>
#include <iostream>
#include <vector>
#include <unordered_set>

#include "debug_util.hpp"
#include "oracle.hpp"
#include "wang.hpp"

#include "test_util.hpp"

void test_sort_ancestors() {

  {
    // Test empty path
    std::vector<std::unordered_set<int>> tree = {{}};
    std::vector<int> expected, actual;
    Oracle o;
    o.Reset(tree, 0, 0);

    expected = {};
    assert(expected == sort_ancestors<std::unordered_set<int>>(o, {}));
  }

  {
    // Test single node path
    std::vector<std::unordered_set<int>> tree = {{}};
    std::vector<int> expected, actual;
    Oracle o;
    o.Reset(tree, 0, 0);

    expected = {0};
    actual = sort_ancestors<std::unordered_set<int>>(o, {0});
    assert(expected == actual);
  }

  {
    // Test single path: 0(1(2(3(4(5)))))
    std::vector<std::unordered_set<int>> tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    std::vector<int> expected, actual;
    Oracle o;
    o.Reset(tree, 0, 2);

    expected = {0,1,2,3,4,5};
    actual = sort_ancestors<std::unordered_set<int>>(o, {4,3,0,2,1,5});
    assert(expected == actual);
  }

  {
    // Test single path, with ancestors being a subpath: 0(1(2(3(4(5)))))
    std::vector<std::unordered_set<int>> tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    std::vector<int> expected, actual;
    Oracle o;
    o.Reset(tree, 0, 2);

    expected = {2,3,4};
    actual = sort_ancestors<std::unordered_set<int>>(o, {4,3,2});
    assert(expected == actual);
  }

  {
    // Test single path, with ancestors being a subset: 0(1(2(3(4(5)))))
    std::vector<std::unordered_set<int>> tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    std::vector<int> expected, actual;
    Oracle o;
    o.Reset(tree, 0, 2);

    expected = {0,2,3,4};
    actual = sort_ancestors<std::unordered_set<int>>(o, {4,3,0,2});
    assert(expected == actual);
  }

  {
    int n = 100;
    std::vector<std::unordered_set<int>> tree(n);
    std::vector<int> expected, actual;
    std::unordered_set<int> V;

    for (int i = 0; i < n; ++i) {
      if (i == 0) {
        tree[i] = {1};
      } else if (i == n - 1) {
        tree[i] = {n - 2};
      } else {
        tree[i] = {i - 1, i + 1};
      }

      // Middle subpath
      if (n / 4 <= i && i <= n * 3 / 4) {
        expected.push_back(i);
        V.insert(i);
      }
    }
    Oracle o;
    o.Reset(tree, 0, 2);

    actual = sort_ancestors(o, V);
    std::cout << "Expected: " << expected << '\n';
    std::cout << "Actual: " << actual << '\n';
    assert(expected == actual);
  }
}


void test_get_path_from_root() {
  std::vector<std::unordered_set<int>> tree;
  std::vector<int> expected;

  {
    // Test empty vertex set
    tree = {{}};
    Oracle o;
    o.Reset(tree, 0, 0);
    assert(get_path_from_root(o, {}, 0).empty());
  }

  {
    // Test singleton vertex set
    tree = {{}};
    Oracle o;
    o.Reset(tree, 0, 0);

    expected = {0};
    assert(expected == get_path_from_root(o, {0}, 0));
  }

  {
    // Test path: 0(1(2(3(4(5)))))
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    Oracle o;
    o.Reset(tree, 0, 2);

    expected = {0, 1, 2, 3, 4, 5};
    assert(expected == get_path_from_root(o, {5, 1, 3, 0, 2, 4}, 5));
  }

  {
    // Test subpath of 0(1(2(3(4(5)))))
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    Oracle o;
    o.Reset(tree, 0, 2);

    expected = {1, 3, 5};
    assert(expected == get_path_from_root(o, {5, 1, 3}, 5));
  }

  {
    // Test simple tree: 0(1 2(4 5) 3(6(7)))
    tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
    Oracle o;
    o.Reset(tree, 0, 3);

    expected = {0, 3, 6, 7};
    assert(expected == get_path_from_root(o, {7, 3, 0, 6}, 7));
  }
}


void test_get_lca() {
  std::vector<std::unordered_set<int>> tree;
  int expected, actual;

  {
    // Test lca for same vertex
    tree = {{}};
    Oracle o;
    o.Reset(tree, 0, 0);
    assert(0 == get_lca(o, {0}, 0, 0));
  }

  {
    // Test case where lca is the first endpoint vertex
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    Oracle o;
    o.Reset(tree, 0, 2);
    assert(2 == get_lca(o, {0, 1, 2, 3, 4, 5}, 2, 5));
  }

  {
    // Test case where lca is the second endpoint vertex
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    Oracle o;
    o.Reset(tree, 0, 2);
    assert(2 == get_lca(o, {0, 1, 2, 3, 4, 5}, 5, 2));
  }

  {
    // Test case simple tree: 0(1 2(4 5) 3(6(7)))
    tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
    Oracle o;
    o.Reset(tree, 0, 3);

    expected = 0;
    actual = get_lca(o, {0, 1, 2, 3, 4, 5, 6, 7}, 7, 4);
    std::cout << "Expected: " << expected << '\n';
    std::cout << "Actual: " << actual << '\n';
    assert(expected == actual);

    // Reverse order
    expected = 0;
    actual = get_lca(o, {0, 1, 2, 3, 4, 5, 6, 7}, 4, 7);
    std::cout << "Expected: " << expected << '\n';
    std::cout << "Actual: " << actual << '\n';
    assert(expected == actual);

    // Lca is not root
    expected = 2;
    actual = get_lca(o, {0, 1, 2, 3, 4, 5, 6, 7}, 4, 5);
    std::cout << "Expected: " << expected << '\n';
    std::cout << "Actual: " << actual << '\n';
    assert(expected == actual);
  }
}


void test_get_multidirectional_path() {
  std::vector<std::unordered_set<int>> tree;
  MultidirectionalPath expected, actual;

  // Test simple tree: 0(1 2(4 5) 3(6(7)))
  tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
  Oracle o;
  o.Reset(tree, 0, 3);

  {
    expected = {{0, 2, 5}, {0, 3, 6, 7}};
    actual = {get_multidirectional_path(o, {0, 1, 2, 3, 4, 5, 6, 7}, 5, 7)};
    std::cout << "Expected:\n" << expected;
    std::cout << "Actual:\n" << actual;
    assert(expected == actual);
  }

  {
    // Endpoints are not leaves
    expected = {{0, 2}, {0, 3, 6}};
    actual = {get_multidirectional_path(o, {0, 1, 2, 3, 4, 5, 6, 7}, 2, 6)};
    std::cout << "Expected:\n" << expected;
    std::cout << "Actual:\n" << actual;
    assert(expected == actual);
  }

  {
    // Path is unidirectional (left is single node)
    expected = {{0}, {0, 3, 6, 7}};
    actual = {get_multidirectional_path(o, {0, 1, 2, 3, 4, 5, 6, 7}, 0, 7)};
    std::cout << "Expected:\n" << expected;
    std::cout << "Actual:\n" << actual;
    assert(expected == actual);
  }

  {
    // Path is unidirectional (right is single node)
    expected = {{3, 6, 7}, {3}};
    actual = {get_multidirectional_path(o, {0, 1, 2, 3, 4, 5, 6, 7}, 7, 3)};
    std::cout << "Expected:\n" << expected;
    std::cout << "Actual:\n" << actual;
    assert(expected == actual);
  }

  {
    // Endpoints of the path correspond to the same vertex.
    EXPECT_EXCEPTION((get_multidirectional_path(o, {0}, 0, 0)),
                     std::invalid_argument);
  }
}


void test_find_bag() {
  std::vector<std::unordered_set<int>> tree;

  // Test simple tree: 0(1 2(4 5) 3(6(7)))
  tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
  Oracle o;
  o.Reset(tree, 0, 3);

  {
    // Answer in middle of path
    int expected = 1;
    assert(expected == find_bag(o, {0, 2, 5}, 4));
  }

  {
    // Answer at beginning of path
    int expected = 0;
    assert(expected == find_bag(o, {0, 2, 5}, 1));
  }

  {
    // Answer at end of path
    int expected = 2;
    assert(expected == find_bag(o, {0, 3, 6}, 7));
  }

  {
    // Path is a singleton
    int expected = 0;
    assert(expected == find_bag(o, {3}, 6));
  }

  {
    // Path has no ancestors of given vertex
    int expected = -1;
    assert(expected == find_bag(o, {3, 6, 7}, 1));
  }
}


void test_get_bag_sizes() {
  std::vector<std::unordered_set<int>> tree;
  std::unordered_set<int> V;

  // Test tree from Figure 3(d) of Wang et al
  // (with added root at 0, since 0-based indexed):
  // 0(9(3(2(1(6 7) 8) 4(5(11))) 10))
  tree = {{9}, {2, 6, 7}, {1, 3, 8}, {2, 4, 9}, {3, 5}, {4, 11}, {1}, {1}, {2}, {0, 3, 10}, {9}, {5}};
  Oracle o;
  o.Reset(tree, 0, 3);

  // Exclude vertex 0 from set of vertices, since it's not in Figure 3(d)
  V = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  // Get multidirectional path: 1<-2<-3->4->5
  MultidirectionalPath expected_multipath({3, 2, 1}, {3, 4, 5}),
                       actual_multipath(get_multidirectional_path(o, V, 1, 5));
  std::cout << "Expected multipath:\n" << expected_multipath;
  std::cout << "Actual multipath:\n" << actual_multipath;
  assert(expected_multipath == actual_multipath);

  // Get bag sizes
  MultidirectionalPath expected({3, 2, 3}, {3, 1, 2}),
                       actual(get_bag_sizes(o, V, actual_multipath));
  std::cout << "Expected:\n" << expected;
  std::cout << "Actual:\n" << actual;
  assert(expected == actual);
}


void test_make_undirected() {
  {
    MultidirectionalPath m({2, 1}, {2, 3, 4, 5});
    std::vector<int> expected = {1, 2, 3, 4, 5};
    std::vector<int> actual = make_undirected(m);
    std::cout << "Expected:\n" << expected << '\n';
    std::cout << "Actual:\n" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Left path is singleton
    MultidirectionalPath m({2}, {2, 3, 4, 5});
    std::vector<int> expected = {2, 3, 4, 5};
    std::vector<int> actual = make_undirected(m);
    std::cout << "Expected:\n" << expected << '\n';
    std::cout << "Actual:\n" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Right path is singleton
    MultidirectionalPath m({4, 3, 2, 1}, {4});
    std::vector<int> expected = {1, 2, 3, 4};
    std::vector<int> actual = make_undirected(m);
    std::cout << "Expected:\n" << expected << '\n';
    std::cout << "Actual:\n" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Right path is singleton
    MultidirectionalPath m({4, 3, 2, 1}, {4});
    std::vector<int> expected = {1, 2, 3, 4};
    std::vector<int> actual = make_undirected(m);
    std::cout << "Expected:\n" << expected << '\n';
    std::cout << "Actual:\n" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Left path is empty
    MultidirectionalPath m({}, {2, 3, 4, 5});
    EXPECT_EXCEPTION((make_undirected(m)), std::runtime_error);
  }

  {
    // Right path is empty
    MultidirectionalPath m({4, 3, 2, 1}, {});
    EXPECT_EXCEPTION((make_undirected(m)), std::runtime_error);
  }
}


void test_get_edge_separator() {
  std::vector<int> path {1, 2, 3, 4, 5, 6};
  std::optional<std::pair<int, int>> expected, actual;
  std::vector<int> bag_sizes;
  std::function<bool(int)> is_edge_separator;

  {
    bag_sizes = {1, 1, 1, 1, 1, 1};
    is_edge_separator = [](int size) { return size == 4; };

    expected = {4, 5};
    actual = get_edge_separator(path, bag_sizes, is_edge_separator);
    std::cout << "Expected:\n" << expected << '\n';
    std::cout << "Actual:\n" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Last vertex in the path no longer counts
    bag_sizes = {1, 1, 1, 1, 1, 1};
    is_edge_separator = [](int size) { return size == 6; };

    expected = {};
    actual = get_edge_separator(path, bag_sizes, is_edge_separator);
    std::cout << "Expected:\n" << expected << '\n';
    std::cout << "Actual:\n" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test when first vertex forms separator
    bag_sizes = {1, 1, 1, 1, 1, 1};
    is_edge_separator = [](int size) { return size == 1; };

    expected = {1, 2};
    actual = get_edge_separator(path, bag_sizes, is_edge_separator);
    std::cout << "Expected:\n" << expected << '\n';
    std::cout << "Actual:\n" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test different bag sizes
    bag_sizes = {3, 10, 4, 6, 4, 3};
    is_edge_separator = [](int size) { return 24 <= size && size <= 29; };

    expected = {5, 6};
    actual = get_edge_separator(path, bag_sizes, is_edge_separator);
    std::cout << "Expected:\n" << expected << '\n';
    std::cout << "Actual:\n" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test with input path size smaller than 2
    EXPECT_EXCEPTION((get_edge_separator({0}, {0}, [](int) { return false; })),
                     std::invalid_argument);

    // Test input vector of bag sizes smaller than 2
    EXPECT_EXCEPTION((get_edge_separator({1, 2},
                                         {0},
                                         [](int) { return false; })),
                     std::invalid_argument);

    // Test inconsistent input path sizes
    EXPECT_EXCEPTION((get_edge_separator({1, 2},
                                         {3, 4, 5},
                                         [](int) { return false; })),
                     std::invalid_argument);
  }
}


void test_split_tree() {
  std::vector<std::unordered_set<int>> tree;
  std::unordered_set<int> V;
  std::pair<std::unordered_set<int>, std::unordered_set<int>> expected, actual;

  {
    // Test path: 0(1(2(3(4(5)))))
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    V = {0, 1, 2, 3, 4, 5};
    Oracle o;
    o.Reset(tree, 0, 2);

    expected = {{0, 1, 2}, {3, 4, 5}};
    actual = split_tree(o, V, {2, 3});
    std::cout << "Expected:" << expected << '\n';
    std::cout << "Actual:" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test reverse path to trigger child/parent logic on the endpoints of the
    // edge separator (descendants now have smaller value).
    // : 5(4(3(2(1(0)))))
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    V = {0, 1, 2, 3, 4, 5};
    Oracle o;
    o.Reset(tree, 5, 2);

    expected = {{5, 4, 3}, {2, 1, 0}};
    actual = split_tree(o, V, {3, 2});
    std::cout << "Expected:" << expected << '\n';
    std::cout << "Actual:" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test splitting at beginning of path: 0(1(2(3(4(5)))))
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    V = {0, 1, 2, 3, 4, 5};
    Oracle o;
    o.Reset(tree, 0, 2);

    expected = {{0}, {1, 2, 3, 4, 5}};
    actual = split_tree(o, V, {0, 1});
    std::cout << "Expected:" << expected << '\n';
    std::cout << "Actual:" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test splitting at end of path: 0(1(2(3(4(5)))))
    tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    V = {0, 1, 2, 3, 4, 5};
    Oracle o;
    o.Reset(tree, 0, 2);

    expected = {{0, 1, 2, 3, 4}, {5}};
    actual = split_tree(o, V, {4, 5});
    std::cout << "Expected:" << expected << '\n';
    std::cout << "Actual:" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test smallest possible path: 0(1)
    tree = {{1}, {0}};
    V = {0, 1};
    Oracle o;
    o.Reset(tree, 0, 1);

    expected = {{0}, {1}};
    actual = split_tree(o, V, {0, 1});
    std::cout << "Expected:" << expected << '\n';
    std::cout << "Actual:" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test simple tree 1: 0(1 2(4 5) 3(6(7)))
    tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
    V = {0, 1, 2, 3, 4, 5, 6, 7};
    Oracle o;
    o.Reset(tree, 0, 3);

    expected = {{0, 1, 2, 3, 4, 5}, {6, 7}};
    actual = split_tree(o, V, {3, 6});
    std::cout << "Expected:" << expected << '\n';
    std::cout << "Actual:" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test simple tree 2: 0(1 2(4 5) 3(6(7)))
    tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
    V = {0, 1, 2, 3, 4, 5, 6, 7};
    Oracle o;
    o.Reset(tree, 0, 3);

    expected = {{0, 1, 2, 3, 4, 5, 6}, {7}};
    actual = split_tree(o, V, {6, 7});
    std::cout << "Expected:" << expected << '\n';
    std::cout << "Actual:" << actual << '\n';
    assert(expected == actual);
  }

  {
    // Test simple tree 3: 0(1 2(4 5) 3(6(7)))
    tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
    V = {0, 1, 2, 3, 4, 5, 6, 7};
    Oracle o;
    o.Reset(tree, 0, 3);

    expected = {{0, 1, 3, 6, 7}, {2, 4, 5}};
    actual = split_tree(o, V, {0, 2});
    std::cout << "Expected:" << expected << '\n';
    std::cout << "Actual:" << actual << '\n';
    assert(expected == actual);
  }
}


void test_make_directed() {
  // Tree is a single edge 0->1
  std::vector<std::unordered_set<int>> tree {{1}, {0}};
  std::optional<std::pair<int, int>> expected, actual;

  {
    // Root is 0
    Oracle o;
    o.Reset(tree, 0, 1);
    expected = {{0, 1}};
    actual = make_directed(o, {{0, 1}});
    std::cout << "Expected: " << expected << '\n';
    std::cout << "Actual: " << actual << '\n';
    assert(expected == actual);
  }

  {
    // Root is 1
    Oracle o;
    o.Reset(tree, 1, 1);
    expected = {{1, 0}};
    actual = make_directed(o, {{0, 1}});
    std::cout << "Expected: " << expected << '\n';
    std::cout << "Actual: " << actual << '\n';
    assert(expected == actual);
  }
}


void test_reconstruct_tree() {

  {
    // Test singleton
    std::vector<std::unordered_set<int>> tree = {{}};
    std::unordered_set<int> V = {0};
    Oracle o;
    o.Reset(tree, 0, 0);

    auto E = reconstruct_tree(o, std::move(V));
    assert(o.IsHiddenTree({E.begin(), E.end()}));
  }

  {
    // Test single edge
    std::vector<std::unordered_set<int>> tree = {{1}, {0}};
    std::unordered_set<int> V = {0, 1};
    Oracle o;
    o.Reset(tree, 0, 1);

    auto E = reconstruct_tree(o, std::move(V));
    assert(o.IsHiddenTree({E.begin(), E.end()}));
  }

  {
    // Test path size 4: 0(1(2(3)))
    std::vector<std::unordered_set<int>> tree = {{1}, {0, 2}, {1, 3}, {2}};
    std::unordered_set<int> V = {0, 1, 2, 3};
    Oracle o;
    o.Reset(tree, 0, 2);

    auto E = reconstruct_tree(o, std::move(V));
    assert(o.IsHiddenTree({E.begin(), E.end()}));
  }

  {
    // Test path size 5: 0(1(2(3(4))))
    std::vector<std::unordered_set<int>> tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3}};
    std::unordered_set<int> V = {0, 1, 2, 3, 4};
    Oracle o;
    o.Reset(tree, 0, 2);

    auto E = reconstruct_tree(o, std::move(V));
    assert(o.IsHiddenTree({E.begin(), E.end()}));
  }

  {
    // Test path size 6: 0(1(2(3(4(5)))))
    std::vector<std::unordered_set<int>> tree = {{1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4}};
    std::unordered_set<int> V = {0, 1, 2, 3, 4, 5};
    Oracle o;
    o.Reset(tree, 0, 2);

    auto E = reconstruct_tree(o, std::move(V));
    assert(o.IsHiddenTree({E.begin(), E.end()}));
  }

  {
    // Test simple tree 3: 0(1 2(4 5) 3(6(7)))
    std::vector<std::unordered_set<int>> tree = {{1, 2, 3}, {0}, {0, 4, 5}, {0, 6}, {2}, {2}, {3, 7}, {6}};
    std::unordered_set<int> V = {0, 1, 2, 3, 4, 5, 6, 7};
    Oracle o;
    o.Reset(tree, 0, 3);

    auto E = reconstruct_tree(o, std::move(V));
    assert(o.IsHiddenTree({E.begin(), E.end()}));
  }
}

void test_reconstruct_tree_path_n(int n) {
  // Test n-path: 0(1(2(3(4(5(...(n)))))))
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

  auto E = reconstruct_tree(o, std::move(V));
  assert(o.IsHiddenTree({E.begin(), E.end()}));
}

void test_reconstruct_tree_large() {
  test_reconstruct_tree_path_n(1e3);
  test_reconstruct_tree_path_n(5e3);
  test_reconstruct_tree_path_n(1e4);
}


int main() {
  test_sort_ancestors();
  test_get_path_from_root();
  test_get_lca();
  test_get_multidirectional_path();
  test_find_bag();
  test_get_bag_sizes();
  test_make_undirected();
  test_get_edge_separator();
  test_split_tree();
  test_make_directed();
  test_reconstruct_tree();
  test_reconstruct_tree_large();
}
