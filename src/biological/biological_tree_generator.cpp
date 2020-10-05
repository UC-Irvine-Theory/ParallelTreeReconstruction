#include <cstdlib>
#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;


const int maxn=200*1000+100;
int a[maxn],cnt [maxn];

set <int> st;
vector<int> adj[maxn];

int cnttt=0;
void binary_tree_generator(int nn){

    int n= 2 * nn - 1;
    memset(a, -1, sizeof a);
    memset(cnt, 0, sizeof cnt);
    st.clear();
    for(int i=0;i<n;i++){
        adj[i].clear();
        a[i]=i;
    }
    random_shuffle(a+1, a+n);
    cnt[0]=1;
    for(int i=1;i<nn-1;i++){
      a[nn+i-2]=a[i];
      cnt[a[i]]=2;
    }

    random_shuffle(a, a+n-2);
    for(int i=0;i<n;i++)
        if(cnt[i]==0)
            st.insert(i);
    for(int i=0;i<n-2;i++){
        int x= *st.begin();
        int y=a[i];
        st.erase(st.begin());
        cnt[y]--;
        if(cnt[y]==0)
            st.insert(y);
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    int x=*st.begin();
    st.erase(st.begin());
    int y=*st.begin();
    adj[x].push_back(y);
    adj[y].push_back(x);
    for(int i=0;i<n;i++){
        cout <<adj[i].size()<<" ";
        for(int j=0;j<adj[i].size();j++)
            cout <<adj[i][j]<<" ";
        cout <<endl;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        srand(time(NULL));
    } else {
        srand(std::atoi(argv[1]));
    }
    int k, nn_count, dd_count, x;
    std::vector<int> nn, dd;

    // Read fixed values of k.
    std::cin >> k;

    // Read range of n values.
    std::cin >> nn_count;
    for (int i = 0; i < nn_count; ++i) {
        std::cin >> x;
        nn.push_back(x);
    }


    // Output number of test cases
    std::cout << k * nn_count << '\n';

    // Fixed d, changing n
    for (int i = 0; i < nn_count; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cout << nn[i] << ' ' << j << '\n';
            binary_tree_generator(nn[i]); //full binary tree with n leaves
        }
    }


    return 0;
}
