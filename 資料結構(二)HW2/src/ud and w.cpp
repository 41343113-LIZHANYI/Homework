#include <iostream> //這份是無向有權圖
#include <vector> //包含BFS、DFS、Kruskal、Prim、Dijkstra、Bellman-Ford、Floyd
#include <algorithm>
#include <queue> 
#include <stack>
#include <utility>
using namespace std;

const int INF=1e9; 

struct Edge{
    int u,v,w; 
};

class Graph{
private:
    int n;
    int e;
    vector<pair<int,int>>* adj; 
    vector<vector<int>> matrix; 
public:
    Graph(int nodes):n(nodes),e(0){
        adj=new vector<pair<int,int>>[n];
        matrix.resize(n,vector<int>(n,INF)); 
        for(int i=0;i<n;++i)
            matrix[i][i]=0; 
    }
    virtual ~Graph(){
        delete[] adj; 
    }
    bool IsEmpty() const{return n==0;}
    int NumberOfVertices() const{return n;}
    int NumberOfEdges() const{return e;}
    virtual int Degree(int u) const{ 
        if(u<0||u>=n)return 0;
        int adj_list_degree=adj[u].size(); 
        int matrix_degree=0;
        for(int i=0;i<n;++i){
            if(matrix[u][i]!=INF&&i!=u) 
                ++matrix_degree;  
        }
        if(adj_list_degree!=matrix_degree)throw "操作錯誤:度數不相等";
        return adj_list_degree;
    }
    virtual bool ExistsEdge(int u,int v) const{
        if(u<0||u>=n||v<0||v>=n)return 0;
        bool adj_list_exist=0; 
        for(auto& nei:adj[u]){ 
            if(nei.first==v){
                adj_list_exist=1;
                break;
            }
        }
        bool matrix_exist=(matrix[u][v]!=INF); 
        if(adj_list_exist!=matrix_exist)throw "操作錯誤:存在狀態不一";
        return adj_list_exist;
    }
    virtual void InsertVertex(int v){ 
        if(v>=n){ 
            int new_n=v+1;
            vector<pair<int,int>>* new_adj=new vector<pair<int,int>>[new_n]; 
            for(int i=0;i<n;++i)new_adj[i]=adj[i];
            delete[] adj;
            adj=new_adj;
            matrix.resize(new_n); 
            for(int i=0;i<new_n;++i){
                matrix[i].resize(new_n,INF);
                matrix[i][i]=0;
            }
            n=new_n;
        }
    }
    virtual void InsertEdge(int u,int v,int w){ 
        if(u<0||u>=n||v<0||v>=n)return;
        if(!ExistsEdge(u,v)){ 
            adj[u].push_back({v,w}); 
            adj[v].push_back({u,w});
            matrix[u][v]=w; 
            matrix[v][u]=w;
            ++e;
        }
    }
    virtual void DeleteVertex(int v){
        if(v<0||v>=n)return;
        for(auto& nei:adj[v]){ 
            int u=nei.first;
            for(auto it=adj[u].begin();it!=adj[u].end();++it){ 
                if(it->first==v){
                    adj[u].erase(it); 
                    break; 
                }
            }
            --e;
        }
        for(int i=v;i<n-1;++i)adj[i]=adj[i+1];
        for(int i=0;i<n-1;++i){ 
            for(auto& target:adj[i]){
                if(target.first>v)--target.first;
            }
        }
        matrix.erase(matrix.begin()+v);
        for(int i=0;i<matrix.size();++i)matrix[i].erase(matrix[i].begin()+v);
        --n;
    }
    virtual void DeleteEdge(int u,int v){
        if(u<0||u>=n||v<0||v>=n)return;
        bool edgeFound=0;
        for(auto it=adj[u].begin();it!=adj[u].end();++it){
            if(it->first==v){
                adj[u].erase(it);
                edgeFound=1;
                break;
            }
        }
        for(auto it=adj[v].begin();it!=adj[v].end();++it){
            if(it->first==u){
                adj[v].erase(it);
                edgeFound=1;
                break;
            }
        }
        if(edgeFound){ 
            matrix[u][v]=INF;
            matrix[v][u]=INF;
            --e;
        }
    }
    void showMartix(){
        for(int k=0;k<n;++k)cout<<k<<" ";
        cout<<'\n';
        for(int i=0;i<n;++i){
            for(int j=0;j<n;++j){
                if(matrix[i][j]==INF)cout<<"X "; 
                else cout<<matrix[i][j]<<" ";
            }
            cout<<'\n';
        }
    }
    void showArraylist(){
        for(int i=0;i<n;++i){ 
            cout<<i<<": ";
            for(auto& nei:adj[i])cout<<"("<<nei.first<<",w:"<<nei.second<<") ";
            cout<<'\n';
        }
    }
    // 1. BFS
    void BFS_List(int start){
        cout<<"[Adj List] BFS: ";
        if(start<0||start>=n){cout<<'\n';return;}
        vector<bool> visited(n,0);
        queue<int> q;
        visited[start]=1;
        q.push(start);
        while(!q.empty()){
            int u=q.front(); 
            q.pop();
            cout<<u<<" ";
            for(auto& nei:adj[u]){ 
                int v=nei.first;
                if(!visited[v]){
                    visited[v]=1;
                    q.push(v);
                }
            }
        }
        cout<<'\n';
    }
    void BFS_Matrix(int start){
        cout<<"[Matrix] BFS: ";
        if(start<0||start>=n){cout<<'\n';return;}
        vector<bool> visited(n,0);
        queue<int> q;
        visited[start]=1;
        q.push(start);
        while(!q.empty()){
            int u=q.front();
            q.pop();
            cout<<u<<" ";
            for(int v=0;v<n;++v){ 
                if(matrix[u][v]!=INF&&matrix[u][v]!=0&&!visited[v]){ 
                    visited[v]=1;
                    q.push(v);
                }
            }
        }
        cout<<'\n';
    }
    // 2.DFS
    void DFSUtil_List(int u,vector<bool>& visited){
        visited[u]=1;
        cout<<u<<" ";    
        for(auto& nei:adj[u]){ 
            int v=nei.first;
            if(!visited[v])DFSUtil_List(v,visited); 
        }
    }
    void DFS_List(int start){
        cout<<"[Adj List] DFS: ";
        if(start<0||start>=n){
            cout<<'\n';
            return;
        }
        vector<bool> visited(n,0);
        DFSUtil_List(start,visited);
        cout<<'\n';
    }
    void DFSUtil_Matrix(int u,vector<bool>& visited){
        visited[u]=1;
        cout<<u<<" ";    
        for(int v=0;v<n;++v){
            if(matrix[u][v]!=INF&&matrix[u][v]!=0&&!visited[v])
                DFSUtil_Matrix(v,visited);
        }
    }
    void DFS_Matrix(int start){
        cout<<"[Matrix] DFS: ";
        if(start<0||start>=n){
            cout<<'\n';
            return;
        }
        vector<bool> visited(n,0);
        DFSUtil_Matrix(start,visited);
        cout<<'\n';
    }
    // 3. Kruskal
    int findParent(int i,vector<int>& parent){
        if(parent[i]==i)return i;
        return parent[i]=findParent(parent[i],parent);
    }
    void Kruskal_List(){
        cout<<"[Adj List] Kruskal 最小生成樹:\n";
        vector<Edge> edges;
        for(int i=0;i<n;++i){
            for(auto& nei:adj[i]){
                if(i<nei.first)edges.push_back({i,nei.first,nei.second});
            }
        }
        sort(edges.begin(),edges.end(),[](const Edge& a,const Edge& b){return a.w<b.w;});
        vector<int> parent(n);
        for(int i=0;i<n;++i)parent[i]=i;
        int totalCost=0;
        int edgeCount=0;
        for(auto& e:edges){
            int rootU=findParent(e.u,parent);
            int rootV=findParent(e.v,parent);
            if(rootU!=rootV){ 
                parent[rootU]=rootV;
                cout<<"("<<e.u<<","<<e.v<<")\n";
                totalCost+=e.w;
                ++edgeCount;
                if(edgeCount==n-1)break;
            }
        }
        cout<<"總權重: "<<totalCost<<'\n';
    }
    void Kruskal_Matrix(){
        cout<<"[Matrix] Kruskal 最小生成樹:\n";
        vector<Edge> edges;
        for(int i=0;i<n;++i){
            for(int j=i+1;j<n;++j){
                if(matrix[i][j]!=INF)edges.push_back({i,j,matrix[i][j]});
            }
        }
        sort(edges.begin(),edges.end(),[](const Edge& a,const Edge& b){return a.w<b.w;});
        vector<int> parent(n);
        for(int i=0;i<n;++i)parent[i]=i;
        int totalCost=0;
        int edgeCount=0;
        for(auto& e:edges){
            int rootU=findParent(e.u,parent);
            int rootV=findParent(e.v,parent);
            if(rootU!=rootV){
                parent[rootU]=rootV;
                cout<<"("<<e.u<<","<<e.v<<")\n";
                totalCost+=e.w;
                ++edgeCount;
                if(edgeCount==n-1)break;
            }
        }
        cout<<"總權重: "<<totalCost<<'\n';
    }
    // 4.Prim
    void Prim_List(int start){
        cout<<"[Adj List] Prim 最小生成樹 (起點 "<<start<<"):\n";
        if(start<0||start>=n)return;
        vector<int> key(n,INF);
        vector<int> parent(n,-1);
        vector<bool> inMST(n,0);
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
        key[start]=0;
        pq.push({0,start});
        int totalCost=0;
        while(!pq.empty()){
            int u=pq.top().second;
            pq.pop();
            if(inMST[u])continue;
            inMST[u]=1;
            if(parent[u]!=-1){
                cout<<"("<<parent[u]<<","<<u<<")\n";
                totalCost+=key[u];
            }
            for(auto& nei:adj[u]){
                int v=nei.first;
                int w=nei.second;
                if(!inMST[v]&&w<key[v]){
                    key[v]=w;
                    parent[v]=u;
                    pq.push({w,v});
                }
            }
        }
        cout<<"總權重: "<<totalCost<<'\n';
    }
    void Prim_Matrix(int start){
        cout<<"[Matrix] Prim 最小生成樹 (起點 "<<start<<"):\n";
        if(start<0||start>=n)
            return;
        vector<int> key(n,INF);
        vector<int> parent(n,-1);
        vector<bool> inMST(n,0);
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
        key[start]=0;
        pq.push({0,start});
        int totalCost=0;
        while(!pq.empty()){
            int u=pq.top().second;
            pq.pop();
            if(inMST[u])continue;
            inMST[u]=1;
            if(parent[u]!=-1){
                cout<<"("<<parent[u]<<","<<u<<")\n";
                totalCost+=key[u];
            }
            for(int v=0;v<n;++v){
                if(matrix[u][v]!=INF&&matrix[u][v]!=0&&!inMST[v]&&matrix[u][v]<key[v]){
                    key[v]=matrix[u][v];
                    parent[v]=u;
                    pq.push({matrix[u][v],v});
                }
            }
        }
        cout<<"總權重: "<<totalCost<<'\n';
    }
    // 5.Dijkstra
    void Dijkstra_List(int start){
        cout<<"[Adj List] Dijkstra 最短路徑 (起點 "<<start<<"):\n";
        if(start<0||start>=n)
            return;
        vector<int> dist(n,INF);
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
        dist[start]=0;
        pq.push({0,start});
        while(!pq.empty()){
            int d=pq.top().first;
            int u=pq.top().second;
            pq.pop();
            if(d>dist[u])continue;
            for(auto& nei:adj[u]){
                int v=nei.first;
                int w=nei.second;
                if(dist[u]+w<dist[v]){
                    dist[v]=dist[u]+w;
                    pq.push({dist[v],v});
                }
            }
        }
        for(int i=0;i<n;++i){
            if(dist[i]==INF)cout<<"到 "<<i<<": INF\n";
            else cout<<"到 "<<i<<": "<<dist[i]<<'\n';
        }
    }
    void Dijkstra_Matrix(int start){
        cout<<"[Matrix] Dijkstra 最短路徑 (起點 "<<start<<"):\n";
        if(start<0||start>=n)return;
        vector<int> dist(n,INF);
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
        dist[start]=0;
        pq.push({0,start});
        while(!pq.empty()){
            int d=pq.top().first;
            int u=pq.top().second;
            pq.pop();
            if(d>dist[u])continue;
            for(int v=0;v<n;++v){
                if(matrix[u][v]!=INF&&matrix[u][v]!=0&&dist[u]+matrix[u][v]<dist[v]){
                    dist[v]=dist[u]+matrix[u][v];
                    pq.push({dist[v],v});
                }
            }
        }
        for(int i=0;i<n;++i){
            if(dist[i]==INF)cout<<"到 "<<i<<": INF\n";
            else cout<<"到 "<<i<<": "<<dist[i]<<'\n';
        }
    }
    // 6.Bellman-Ford
    void BellmanFord_List(int start){
        cout<<"[Adj List] Bellman-Ford 最短路徑 (起點 "<<start<<"):\n";
        if(start<0||start>=n)return;
        vector<int> dist(n,INF);
        dist[start]=0;
        vector<Edge> edges;
        for(int u=0;u<n;++u){
            for(auto& nei:adj[u]){
                edges.push_back({u,nei.first,nei.second}); 
            }
        }
        for(int i=0;i<n-1;++i){
            for(auto& e:edges){
                if(dist[e.u]!=INF&&dist[e.u]+e.w<dist[e.v]){
                    dist[e.v]=dist[e.u]+e.w;
                }
            }
        }
        for(auto& e:edges){
            if(dist[e.u]!=INF&&dist[e.u]+e.w<dist[e.v]){
                cout<<"圖中存在負迴圈\n";
                return;
            }
        }
        for(int i=0;i<n;++i){
            if(dist[i]==INF)cout<<"到 "<<i<<": INF\n";
            else cout<<"到 "<<i<<": "<<dist[i]<<'\n';
        }
    }
    void BellmanFord_Matrix(int start){
        cout<<"[Matrix] Bellman-Ford 最短路徑 (起點 "<<start<<"):\n";
        if(start<0||start>=n)return;
        vector<int> dist(n,INF);
        dist[start]=0;
        vector<Edge> edges;
        for(int i=0;i<n;++i){
            for(int j=0;j<n;++j){
                if(matrix[i][j]!=INF&&matrix[i][j]!=0){
                   edges.push_back({i,j,matrix[i][j]}); 
                }
            }
        }
        for(int i=0;i<n-1;++i){ 
            for(auto& e:edges){
                if(dist[e.u]!=INF&&dist[e.u]+e.w<dist[e.v]){
                    dist[e.v]=dist[e.u]+e.w;
                }
            }
        }
        for(auto& e:edges){
            if(dist[e.u]!=INF&&dist[e.u]+e.w<dist[e.v]){
                cout<<"圖中存在負迴圈\n";
                return;
            }
        }
        for(int i=0;i<n;++i){
            if(dist[i]==INF)cout<<"到 "<<i<<": INF\n";
            else cout<<"到 "<<i<<": "<<dist[i]<<'\n';
        }
    }
    // 7.Floyd-Warshall
    void FloydWarshall_List(){
        cout<<"[Adj List] Floyd-Warshall 任兩點最短路徑矩陣:\n";
        vector<vector<int>> dist(n,vector<int>(n,INF));
        for(int i=0;i<n;++i){
            dist[i][i]=0;
            for(auto& nei:adj[i]){
                dist[i][nei.first]=nei.second;
            }
        }
        for(int k=0;k<n;++k){ 
            for(int i=0;i<n;++i){
                for(int j=0;j<n;++j){
                    if(dist[i][k]!=INF&&dist[k][j]!=INF&&dist[i][k]+dist[k][j]<dist[i][j]){
                        dist[i][j]=dist[i][k]+dist[k][j];
                    }
                }
            }
        }
        for(int i=0;i<n;++i){
            if(dist[i][i]<0){
                cout<<"圖中存在負迴圈\n";
                return;
            }
        }
        for(int i=0;i<n;++i){
            for(int j=0;j<n;++j){
                if(dist[i][j]==INF)cout<<"X ";
                else cout<<dist[i][j]<<" ";
            }
            cout<<'\n';
        }
    }
    void FloydWarshall_Matrix(){
        cout<<"[Matrix] Floyd-Warshall 任兩點最短路徑矩陣:\n";
        vector<vector<int>> dist=matrix;
        for(int k=0;k<n;++k){ 
            for(int i=0;i<n;++i){
                for(int j=0;j<n;++j){
                    if(dist[i][k]!=INF&&dist[k][j]!=INF&&dist[i][k]+dist[k][j]<dist[i][j]){
                        dist[i][j]=dist[i][k]+dist[k][j];
                    }
                }
            }
        }
        for(int i=0;i<n;++i){
            if(dist[i][i]<0){
                cout<<"圖中存在負迴圈\n";
                return;
            }
        }
        for(int i=0;i<n;++i){
            for(int j=0;j<n;++j){
                if(dist[i][j]==INF)cout<<"X ";
                else cout<<dist[i][j]<<" ";
            }
            cout<<'\n';
        }
    }
};

int main(){
    cout<<"測資一:綜合有權連通圖\n";
    Graph g1(10);
    g1.InsertEdge(0,1,4);
    g1.InsertEdge(0,7,8);
    g1.InsertEdge(1,2,12);
    g1.InsertEdge(1,7,11);
    g1.InsertEdge(2,3,7);
    g1.InsertEdge(2,8,2);
    g1.InsertEdge(2,5,5);
    g1.InsertEdge(3,4,9);
    g1.InsertEdge(3,5,14);
    g1.InsertEdge(4,5,10);
    g1.InsertEdge(5,6,3);
    g1.InsertEdge(6,7,1);
    g1.InsertEdge(6,8,6);
    g1.InsertEdge(7,8,13);
    g1.InsertEdge(8,9,15);
    g1.InsertEdge(2,9,16);
    g1.BFS_List(0);
    g1.BFS_Matrix(0);
    g1.DFS_List(0);
    g1.DFS_Matrix(0);
    g1.Kruskal_List();
    g1.Kruskal_Matrix();
    g1.Prim_List(0);
    g1.Prim_Matrix(0);
    g1.Dijkstra_List(0);
    g1.Dijkstra_Matrix(0);
    g1.BellmanFord_List(0);
    g1.BellmanFord_Matrix(0);
    g1.FloydWarshall_List();
    g1.FloydWarshall_Matrix();
    cout<<'\n';
    cout<<"測資二:捷徑有權連通圖\n";
    Graph g2(10);
    g2.InsertEdge(0,1,10);
    g2.InsertEdge(1,2,11);
    g2.InsertEdge(2,3,12);
    g2.InsertEdge(3,4,13);
    g2.InsertEdge(4,5,14);
    g2.InsertEdge(5,6,15);
    g2.InsertEdge(6,7,16);
    g2.InsertEdge(7,8,17);
    g2.InsertEdge(8,9,18);
    g2.InsertEdge(0,4,25); 
    g2.InsertEdge(4,9,45); 
    g2.InsertEdge(2,7,30); 
    g2.BFS_List(0);
    g2.BFS_Matrix(0);
    g2.DFS_List(0);
    g2.DFS_Matrix(0);
    g2.Kruskal_List();
    g2.Kruskal_Matrix();
    g2.Prim_List(0);
    g2.Prim_Matrix(0);
    g2.Dijkstra_List(0);
    g2.Dijkstra_Matrix(0);
    g2.BellmanFord_List(0);
    g2.BellmanFord_Matrix(0);
    g2.FloydWarshall_List();
    g2.FloydWarshall_Matrix();
    cout<<'\n';
    cout<<"測資三:負權(負迴圈)連通圖\n";
    Graph g3(10);
    g3.InsertEdge(0,1,5);
    g3.InsertEdge(1,2,8);
    g3.InsertEdge(2,3,12);
    g3.InsertEdge(3,4,6);
    g3.InsertEdge(4,5,-10); 
    g3.InsertEdge(5,6,3);
    g3.InsertEdge(6,7,7);
    g3.InsertEdge(7,8,19);
    g3.InsertEdge(8,9,4);
    g3.InsertEdge(1,9,20);
    g3.BFS_List(0);
    g3.BFS_Matrix(0);
    g3.DFS_List(0);
    g3.DFS_Matrix(0);
    g3.Kruskal_List();
    g3.Kruskal_Matrix();
    g3.Prim_List(0);
    g3.Prim_Matrix(0);
    //g3.Dijkstra_List(0);   會出錯導致無窮迴圈
    //g3.Dijkstra_Matrix(0); 會出錯導致無窮迴圈
    g3.BellmanFord_List(0); 
    g3.BellmanFord_Matrix(0); 
    g3.FloydWarshall_List();
    g3.FloydWarshall_Matrix(); 
    cout<<'\n';

    return 0;
}