#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <unordered_set>


using namespace std;


const int maxn=2*100*1000+5;

int C1 = 14, C2, m, k, n, d, numtest, g, reps, t_number;
int splitting_edge_round;
bool visit [maxn];
vector<unordered_set<int> > ancestors_list;

vector<int> adj[maxn];
vector<int> v;
long total_query;

vector<pair<int,int> > edges, verify_edges;




void dfs(){
    verify_edges.clear();
    vector< pair<int,int> > vec;
    vec.push_back(make_pair(-1, 0));
    while (!vec.empty()) {
        pair<int,int> tmp= vec.back();
        //cout<<verify_edges.size()<<end
        int p= tmp.first;
        int vv= tmp.second;
        //cout<<p<<" "<<vv<<endl;
        vec.pop_back();
        if (p >= 0)
            ancestors_list[vv] = ancestors_list[p];
        ancestors_list[vv].insert(vv);
        for (int i=0; i<adj[vv].size();i++) {
            int w=adj[vv][i];
            if (w != p){
                vec.push_back(make_pair(vv,w));
                verify_edges.push_back(make_pair(vv,w));
            }
        }
  }

}

bool IsAncestor(int x, int y) {
    total_query++;
    return ancestors_list[y].find(x) != ancestors_list[y].end();
}

void brute_force(int vv, int st, int fn){
    visit[vv]=true;
    while(true){
        int tmp=-1;
        for(int i=st+1;i<=fn;i++){
            if(!visit[v[i]] && ancestors_list[v[i]].find(vv)!=ancestors_list[v[i]].end()){
                if(tmp==-1)
                    tmp=v[i];
                else if(ancestors_list[tmp].find(v[i])!=ancestors_list[tmp].end())
                    tmp=v[i];
            }
        }

        if(tmp==-1)
            break;
        //cout<<tmp<<" "<<visit[tmp]<<endl;
        edges.push_back(make_pair(vv, tmp));
        brute_force(tmp, st, fn);
    }
}

vector<int> random_sample(vector<int>& s, int t, int st, int fn){
    int sz= fn-st+1;
    //cout<<sz<<endl;
    vector<int>res;
    vector<int> rev;

    if(t>=sz){
        for(int i=st;i<=fn;i++)
            res.push_back(s[i]);
        return res;
    }
    for(int i=0;i<t;i++){
        int pos=rand()%sz;
        swap(s[st+i], s[st+pos]);
        rev.push_back(pos);
    }
    //cout<<t<<endl;
    for(int i=0;i<t;i++){
        res.push_back(s[st+i]);
    }
    for(int i=t-1;i>-1;i--){
        swap(s[st+i],s[st+rev[i]]);
    }
    return res;
}



bool comparison(pair<int, int> x, pair<int,int> y){
    total_query--;
    return (!IsAncestor(x.first, y.first));

}
bool comparison2(int x, int y){
    total_query--;
    return (!IsAncestor(x,y));
}

int counter(int x, vector <int>& desc, int st, int fn){
    int count1=0;
    for(int i=st;i<=fn;i++)
        if(IsAncestor(x, desc[i]))
            count1++;
    return count1;
}

int find_parent(int child, vector<int>& y, int m_sz){
    splitting_edge_round++;
    vector<int> y2;
    for(int i=0;i<y.size();i++)
        if(y[i]!=child && IsAncestor(y[i], child))
            y2.push_back(y[i]);
    for(int i=0;i<2;i++)
        if(y2.size()> m_sz){
            splitting_edge_round+=2;
            vector <int> sample = random_sample(y2, m_sz, 0, y2.size()-1);
            total_query+=(m_sz * (m_sz-1)/2);
            sort(sample.begin(), sample.end(), comparison2);
            vector<int> y_tmp;
            y_tmp.clear();
            for(int i=0;i<y2.size();i++){
                if(IsAncestor(sample[0], y2[i]))
                        y_tmp.push_back(y2[i]);
            }
            y2=y_tmp;
        }
    if(y2.size()>m_sz)
        return -1;
    splitting_edge_round++;
    total_query+=(y.size() * (y2.size()-1)/2);
    sort(y2.begin(), y2.end(), comparison2);
    return y2[0];
}


pair<int,int> find_splitting_edge(int vv, vector<int>& y, int st, int fn){
    int sz=fn-st+1;
    //C2 = (d+2)*(d+1)*(d+2)*(d+1)*(log(2*sz)/log(sz));
    //C2=(d+2);
    //C2=1;
    m = C1 * sqrt (sz);
    k= min(int (C2 * log (sz)),sz);
    vector<int> ancestors = y;
    if(y.size()> sz / k && y.size()>m){
        vector<int> res= random_sample(y, m, 0 , y.size()-1);
        vector<pair<int,int> > tmp;
        tmp.clear();
        if(find(res.begin(),res.end(),vv)==res.end())
            res.push_back(vv);
        if(find(res.begin(),res.end(),v[st])==res.end())
            res.push_back(v[st]);
        for(int i=0;i<res.size();i++){
            vector<int> tmp2= random_sample(v, k, st, fn);
            tmp.push_back(make_pair( res[i], counter(res[i],tmp2, 0, tmp2.size()-1)));
        }
        splitting_edge_round++;
        for(int i=0;i<tmp.size();i++){
            if(tmp[i].second>= k / (d+1))
                break;
            if(i==tmp.size()-1)
                return make_pair(-1,-1);
        }
        for(int i=0;i<tmp.size();i++){
            if(tmp[i].second<= k* d / (d+1))
                break;
            if(i==tmp.size()-1)
                return make_pair(-1,-1);
        }
        for(int i=0;i<tmp.size();i++){
            if(tmp[i].second>= k/(d+1) && tmp[i].second<= k * d / (d+1)){
                splitting_edge_round++;
                int act_cnt= counter(tmp[i].first, v, st, fn);
                if(act_cnt>= sz/(d+2) && act_cnt<= sz*(d+1)/(d+2)){
                    int p= find_parent(tmp[i].first, y , m);
                    if(p==-1)
                        return make_pair(-1, -1);
                    return make_pair(p, tmp[i].first);
                }
                else
                    return make_pair(-1,-1);
            }
        }
        total_query+=(tmp.size()*(tmp.size()-1)/2);
        splitting_edge_round++;
        sort(tmp.begin(), tmp.end(), comparison);
        for(int i=0;i<tmp.size()-1;i++){
            vector<int> y2;
            y2.clear();
            if(tmp[i].second< k / (d+1) && tmp[i+1].second> k*d / (d+1)){
                //cout<<"salam"<<endl;
                splitting_edge_round++;
                for(int i=0;i<y.size();i++)
                    if(IsAncestor(y[i],tmp[i].second) && IsAncestor(tmp[i+1].second, y[i]))
                       y2.push_back(y[i]);
                y=y2;
                break;
            }
        }

    }
    if(y.size()> sz / k && y.size()>m)
        return make_pair(-1, -1);
    splitting_edge_round++;
    for(int i=0;i<y.size();i++){
        vector <int> tmp = random_sample(v, k, st, fn);
        int estimate = counter(y[i], tmp, 0, tmp.size()-1);
        if(estimate>= k/ (d+1) && estimate <= k* d / (d+1)){
                int act_cnt= counter(y[i], v, st, fn);
                if(act_cnt>= sz/(d+2) && act_cnt<= sz*(d+1)/(d+2)){
                    int p= find_parent(y[i], y, m);
                    if(p==-1)
                        return make_pair(-1, -1);
                    return make_pair(p, y[i]);
                }
                return make_pair(-1,-1);
        }
    }
    return make_pair(-1,-1);

}


int reconstruct(int st,int fn){ //root is always v[st]
    int sz=fn-st+1;
    //cout<<st<<" "<<fn<<endl;
    if(sz<=g){
        //cout<<v[st]<<" "<<st<<" "<<fn<<endl;
        brute_force(v[st], st, fn);

        total_query= total_query + (sz-1)*(sz-2);
        return 1;
    }
    splitting_edge_round=0;

    while(true){
        vector <int> y;
        int vv= v[rand() % sz + st];
        splitting_edge_round++;
        y.push_back(v[st]);
        for(int i=st+1;i<=fn;i++)
            if(IsAncestor(v[i], vv))
                y.push_back(v[i]);
        pair<int, int> edge = find_splitting_edge(vv, y, st, fn);
        if(edge != make_pair(-1,-1)){ // not NULL
            edges.push_back(edge);
            int r2=edge.second;
            for(int i=st+1;i<=fn;i++)
                if(v[i]==r2){
                    swap(v[i],v[fn]);
                    break;
                }
            int pos=st+1;
            for(int i=st+1;i<fn;i++)
                if(!IsAncestor(r2,v[i])){
                    swap(v[i],v[pos]);
                    pos++;
                }
            splitting_edge_round++;
            if(sz/(d+2)> pos-st || fn-pos+1< sz/(d+2))
                cout<<"fuck me"<<endl;
            swap(v[pos],v[fn]);
            int splitting_edge_round_copy =splitting_edge_round;
            int l1 = reconstruct(st,pos-1);
            int l2 = reconstruct(pos,fn);
            return max(l1,l2)+ splitting_edge_round_copy;
        }
    }
}


void adjust_algorithm(int rep, int c){
    v.clear();
    edges.clear();
    total_query=0;
    splitting_edge_round=0;
    C2=c;
    memset(visit, 0, sizeof visit);
    for(int l=0;l<n;l++)
        v.push_back(l);
    int my_round = reconstruct(0, n-1);
    sort(edges.begin(), edges.end());
    if(edges!=verify_edges)
        cout<<"Something is wrong!!"<<endl;
    cout<<n<<"\t"<<d<<"\t"<<numtest<<"\t"<<rep<<"\t"<<c<<"\t"<<total_query<<"\t"<<my_round<<endl;
}

int main(){

    srand(time(NULL));
    //ifstream fin("/Users/ramtinafshar/Desktop/out.txt");
    cin>>t_number;
    reps=3;
    cout << "n\td\tk\trep\tC2\tquery\tround\n";
    for(int i=0;i<t_number;i++){
        cin>>n>>d>>numtest;
        ancestors_list.clear();
        verify_edges.clear();
        g=d;
        for(int l=0;l<n;l++){
            unordered_set <int> st;
            ancestors_list.push_back(st);
            adj[l].clear();
            int sz;
            cin>>sz;
            for(int h=0;h<sz;h++){
                int x;
                cin>>x;
                adj[l].push_back(x);
            }
        }
        dfs();
        sort(verify_edges.begin(), verify_edges.end());
        for(int j=0;j<reps;j++)
            adjust_algorithm(j, 25);
    }



    return 0;
}
