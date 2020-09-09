#ifndef WANG_HPP_
#define WANG_HPP_


#include <unordered_set>
#include <vector>

#include "oracle.hpp"


// A multidirectional path is the path between 2 vertices i,j of the tree.
// We break the path into:
//  (1) a left subpath, from LCA(i,j) to i
//  (2) a right subpath, from LCA(i,j) to j
//  Both paths always include the lca, so none of them is ever empty.
struct MultidirectionalPath {
  std::vector<int> left;
  std::vector<int> right;

  MultidirectionalPath() {}
  MultidirectionalPath(std::vector<int> &&left_path, std::vector<int> &&right_path) : 
    left(left_path), right(right_path) {}
};


// Sort according to partial order given by IsAncestor relation.
// a < b iff IsAncestor(a, b) == true && a != b.
template <typename Iterator>
std::vector<int> sort_ancestors(Oracle& o, const Iterator& ancestors);


// Algorithm 4 of Wang et al.
// Returns all ancestors of vertex u in the given vertex set.
std::vector<int> get_path_from_root(
    Oracle& o,
    const std::unordered_set<int>& V,
    int u);


// Returns the lowest common ancestor between given vertices.
int get_lca(Oracle& o, const std::unordered_set<int>& V, int u, int v);


// Returns the Multidirectional path between given vertices.
MultidirectionalPath get_multidirectional_path(
    Oracle& o,
    const std::unordered_set<int>& V,
    int u,
    int v);


// Returns highest position in the sorted path whose element is an ancestor of
// the given vertex, or -1 if no element is an ancestor.
int find_bag(Oracle& o, const std::vector<int>& path, int u);


// Converts given MuiltidirectionalPath into a MuiltidirectionalPath with
// corresponding bag sizes.
// Returns a MultidirectionalPath where each element is the size of the bag
// of the corresponding vertex in given MultidirectionalPath. Bags include the
// vertices in the MultidirectionalPath given.
MultidirectionalPath get_bag_sizes(Oracle& o,
                                   const std::unordered_set<int>& V,
                                   const MultidirectionalPath& multipath);


// Converts the given MultidirectionalPath between two vertices into a single
// undirected path.
// Concatenates the right path (lca to right endpoint) to the reverse of the
// left path (left endpoint to lca) and removes the second occurrence of lca.
std::vector<int> make_undirected(const MultidirectionalPath& multipath);


// Returns the even edge separator if it exists.
// It receives an undirected path between two endpoints and a vector of bag
// sizes for each matching vertex in the path.
// Returns the edge (path[i - 1], path[i]) iff the bag size for path[i - 1] is
// between n/d and n*(d-1)/d.
// Throws a runtime_error if the path sizes are too small or inconsistent with
// each other.
std::optional<std::pair<int, int>> get_edge_separator(
    const std::vector<int>& path,
    const std::vector<int>& bag_sizes,
    std::function<bool(int)> is_edge_separator);


// Split tree into two subtrees, according to the given edge separator.
// The edge given must be directed (i.e. first position corresponds to the
// parent vertex and second position to the child). If not, behavior is
// undefined.
// Returns the two corresponding vertex sets.
std::pair<std::unordered_set<int>, std::unordered_set<int>> split_tree(
    Oracle& o,
    const std::unordered_set<int>& V,
    const std::pair<int, int>& edge_separator);


// Returns a pair of vertices chosen uniformly at random from V
// (without replacement).
std::pair<int, int> sample(const std::unordered_set<int>& V);


// Make edge directed (from ancestor to descendant).
std::optional<std::pair<int, int>> make_directed(
    Oracle& o,
    const std::optional<std::pair<int, int>>& edge);


// Returns a vector of all directed edges (from parent to child).
std::vector<std::pair<int, int>> reconstruct_tree(
    Oracle& o,
    std::unordered_set<int>&& V_ref);

#endif
