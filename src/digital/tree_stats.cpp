#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

int depth(int parent, int u, const vector<unordered_set<int>>& tree) {
  int height = 0;
  for (auto &v: tree[u]) {
    if (v != parent) {
      height = max(height, depth(u, v, tree));
    }
  }
  return height + 1;
}

int main() {
  int n, d, k;
    vector<unordered_set<int>> tree;



  for (int p = 0; p < 20; ++p) {  
    cin >> n >> d >> k;
    for (int j = 0; j < k; ++j) {
      tree = vector<unordered_set<int>>(n);
      for (int i = 0; i < n; ++i) {
        int num_neighbors;
        cin >> num_neighbors;
        for (int j = 0; j < num_neighbors; ++j) {
          int x;
          cin >> x;
          tree[i].insert(x);
        }
      }
      cout << depth(-1, 0, tree) << '\n';
    }
  }
}
