#include <cstdlib>
#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <set>

using namespace std;


const int maxn=200*1000+100;
int a[maxn],cnt [maxn];

set <int> st;
vector<int> adj[maxn];

int cnttt=0;
void tree_generator(int nn, int dd){

    int n=nn, d=dd;
    memset(a, -1, sizeof a);
    memset(cnt, 0, sizeof cnt);
    st.clear();
    for(int i=0;i<n;i++)
        adj[i].clear();

    int i_Delta = rand()%n;
    int deg=d-1;
    while(deg>0){
        int pos= rand()%(n-2);
        if(a[pos]==-1){
            deg--;
            a[pos]=i_Delta;
        }
    }
    cnt[i_Delta]=d-1;
    for(int i=0;i<n-2;i++){
        if(a[i]==-1){
            int temp=i_Delta;
            while(cnt[temp]==d-1){
                temp= rand()%n;
            }
            a[i]=temp;
            cnt[a[i]]++;
        }
    }
    int mx=0;
    for(int i=0;i<n;i++){
        if(cnt[i]==0)
            st.insert(i);
        if(i!=i_Delta)
            mx=max(mx,cnt[i]);
    }
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
    int n, d, k, nn_count, dd_count, x;
    std::vector<int> nn, dd;

    // Read fixed values of n, d and k.
    std::cin >> n >> d >> k;

    // Read range of n values.
    std::cin >> nn_count;
    for (int i = 0; i < nn_count; ++i) {
        std::cin >> x;
        nn.push_back(x);
    }

    // Read range of d values.
    std::cin >> dd_count;
    for (int i = 0; i < dd_count; ++i) {
        std::cin >> x;
        dd.push_back(x);
    }

    // Output number of test cases
    std::cout << k * (nn_count + dd_count) << '\n';

    // Fixed d, changing n
    for (int i = 0; i < nn_count; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cout << nn[i] << ' ' << d << ' ' << j << '\n';
            tree_generator(nn[i], d);
        }
    }

    // Fixed n, changing d
    for (int i = 0; i < dd_count; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cout << n << ' ' << dd[i] << ' ' << j << '\n';
            tree_generator(n, dd[i]);
        }
    }
    return 0;
}
