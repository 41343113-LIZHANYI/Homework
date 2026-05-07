#include <iostream> //這份是有向有權圖
#include <vector> //包含BFS、DFS、Dijkstra、Bellman-Ford、Floyd、邊活動網路
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
    virtual int OutDegree(int u) const{ 
        if(u<0||u>=n)
            return 0;
        int adj_list_degree=adj[u].size(); 
        int matrix_degree=0;
        for(int i=0;i<n;++i){
            if(matrix[u][i]!=INF&&i!=u) 
                ++matrix_degree;  
        }
        if(adj_list_degree!=matrix_degree)
            throw "操作錯誤:度數不相等";
        return adj_list_degree;
    }
    virtual int InDegree(int u) const{
        if(u<0||u>=n)
            return 0;
        int matrix_degree=0;
        for(int i=0;i<n;++i){
            if(matrix[i][u]!=INF&&i!=u)
                ++matrix_degree;
        }
        int adj_list_degree=0;
        for(int i=0;i<n;++i){
            for(auto& nei:adj[i]){
                if(nei.first==u)
                    ++adj_list_degree;
            }
        }
        if(adj_list_degree!=matrix_degree)
            throw "操作錯誤:度數不相等";
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
        if(u<0||u>=n||v<0||v>=n)
            return;
        if(!ExistsEdge(u,v)){ 
            adj[u].push_back({v,w}); 
            matrix[u][v]=w; 
            ++e;
        }
    }
    virtual void DeleteVertex(int v){
        if(v<0||v>=n)
            return;
        int removedEdges=adj[v].size(); 
        for(int i=0;i<n;++i){
            if(i!=v){
                for(auto it=adj[i].begin();it!=adj[i].end();){
                    if(it->first==v){
                        it=adj[i].erase(it);
                        removedEdges++;
                    }else{
                        ++it;
                    }
                }
            }
        }
        e-=removedEdges;
        for(int i=v;i<n-1;++i)
            adj[i]=adj[i+1];
        for(int i=0;i<n-1;++i){ 
            for(auto& target:adj[i]){
                if(target.first>v)
                    --target.first;
            }
        }
        matrix.erase(matrix.begin()+v);
        for(int i=0;i<matrix.size();++i)matrix[i].erase(matrix[i].begin()+v);
        --n;
    }
    virtual void DeleteEdge(int u,int v){
        if(u<0||u>=n||v<0||v>=n)
            return;
        bool edgeFound=0;
        for(auto it=adj[u].begin();it!=adj[u].end();++it){
            if(it->first==v){
                adj[u].erase(it);
                edgeFound=1;
                break;
            }
        }
        if(edgeFound){ 
            matrix[u][v]=INF; //只需單邊設為INF
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
    // 3.Dijkstra
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
    // 4.Bellman-Ford
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
    // 5.Floyd-Warshall
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
    // 6.AOE(關鍵路徑)
    void CriticalPath_List(){
        cout<<"[Adj List] AOE(關鍵路徑):\n";
        if(n==0)return;
        vector<int> in_degree(n,0);
        for(int i=0;i<n;++i){
            for(auto& nei:adj[i])in_degree[nei.first]++;
        }
        queue<int> q;
        for(int i=0;i<n;++i){
            if(in_degree[i]==0)q.push(i);
        }
        vector<int> ve(n,0); 
        vector<int> topo_order;
        while(!q.empty()){
            int u=q.front();
            q.pop();
            topo_order.push_back(u);
            for(auto& nei:adj[u]){
                int v=nei.first,w=nei.second;
                if(ve[u]+w>ve[v])ve[v]=ve[u]+w; // 順向找最大值
                if(--in_degree[v]==0)q.push(v);
            }
        }
        if(topo_order.size()!=n){
            cout<<"圖中存在迴圈，無法計算關鍵路徑\n";
            return;
        }
        int max_ve=0;
        for(int i=0;i<n;++i)if(ve[i]>max_ve)max_ve=ve[i];
        
        vector<int> vl(n,max_ve); 
        for(int i=n-1;i>=0;--i){ // 逆拓撲順序推導
            int u=topo_order[i];
            for(auto& nei:adj[u]){
                int v=nei.first,w=nei.second;
                if(vl[v]-w<vl[u])vl[u]=vl[v]-w; // 逆向找最小值
            }
        }
        cout<<"關鍵活動: ";
        for(int u=0;u<n;++u){
            for(auto& nei:adj[u]){
                int v=nei.first,w=nei.second;
                int e=ve[u],l=vl[v]-w;
                if(e==l)cout<<"("<<u<<","<<v<<") ";
            }
        }
        cout<<"\n最短專案完成時間: "<<max_ve<<"\n";
    }
    void CriticalPath_Matrix(){
        cout<<"[Matrix] AOE 關鍵路徑:\n";
        if(n==0)return;
        vector<int> in_degree(n,0);
        for(int i=0;i<n;++i){
            for(int j=0;j<n;++j){
                if(matrix[i][j]!=INF&&matrix[i][j]!=0)
                in_degree[j]++;
            }
        }
        queue<int> q;
        for(int i=0;i<n;++i){
            if(in_degree[i]==0)q.push(i);
        }
        vector<int> ve(n,0);
        vector<int> topo_order;
        while(!q.empty()){
            int u=q.front();
            q.pop();
            topo_order.push_back(u);
            for(int v=0;v<n;++v){
                if(matrix[u][v]!=INF&&matrix[u][v]!=0){
                    if(ve[u]+matrix[u][v]>ve[v])
                        ve[v]=ve[u]+matrix[u][v];
                    if(--in_degree[v]==0)
                        q.push(v);
                }
            }
        }
        if(topo_order.size()!=n){
            cout<<"圖中存在迴圈，無法計算關鍵路徑\n";
            return;
        }
        int max_ve=0;
        for(int i=0;i<n;++i)if(ve[i]>max_ve)max_ve=ve[i];
        
        vector<int> vl(n,max_ve);
        for(int i=n-1;i>=0;--i){
            int u=topo_order[i];
            for(int v=0;v<n;++v){
                if(matrix[u][v]!=INF&&matrix[u][v]!=0){
                    if(vl[v]-matrix[u][v]<vl[u])vl[u]=vl[v]-matrix[u][v];
                }
            }
        }
        cout<<"關鍵活動: ";
        for(int u=0;u<n;++u){
            for(int v=0;v<n;++v){
                if(matrix[u][v]!=INF&&matrix[u][v]!=0){
                    int e=ve[u],l=vl[v]-matrix[u][v];
                    if(e==l)cout<<"("<<u<<","<<v<<") ";
                }
            }
        }
        cout<<"\n最短專案完成時間: "<<max_ve<<"\n";
    }
};
int main(){
    cout<<"測資一:多重交匯圖\n";
    Graph g1(10);
    g1.InsertEdge(0,1,3); 
    g1.InsertEdge(0,2,2); 
    g1.InsertEdge(0,3,5);
    g1.InsertEdge(1,4,4); 
    g1.InsertEdge(2,4,6); 
    g1.InsertEdge(3,5,2);
    g1.InsertEdge(4,6,3); 
    g1.InsertEdge(5,6,4); 
    g1.InsertEdge(4,7,5);
    g1.InsertEdge(6,8,4); 
    g1.InsertEdge(7,8,2);
    g1.InsertEdge(8,9,3);
    g1.BFS_List(0); 
    g1.BFS_Matrix(0);
    g1.DFS_List(0); 
    g1.DFS_Matrix(0);
    g1.Dijkstra_List(0); 
    g1.Dijkstra_Matrix(0);
    g1.BellmanFord_List(0); 
    g1.BellmanFord_Matrix(0);
    g1.FloydWarshall_List(); 
    g1.FloydWarshall_Matrix();
    g1.CriticalPath_List(); 
    g1.CriticalPath_Matrix();
    cout<<'\n';
    cout<<"測資二: 最短與最長路徑圖\n";
    Graph g2(10);
    g2.InsertEdge(0,1,10); 
    g2.InsertEdge(0,2,2);
    g2.InsertEdge(1,4,8);  
    g2.InsertEdge(1,3,1); 
    g2.InsertEdge(2,3,3);
    g2.InsertEdge(3,4,1);  
    g2.InsertEdge(3,6,2);
    g2.InsertEdge(4,5,6);  
    g2.InsertEdge(5,8,1);  
    g2.InsertEdge(5,9,10);
    g2.InsertEdge(6,7,3);  
    g2.InsertEdge(7,8,2);  
    g2.InsertEdge(8,9,1);
    g2.BFS_List(0); 
    g2.BFS_Matrix(0);
    g2.DFS_List(0); 
    g2.DFS_Matrix(0);
    g2.Dijkstra_List(0); 
    g2.Dijkstra_Matrix(0);
    g2.BellmanFord_List(0); 
    g2.BellmanFord_Matrix(0);
    g2.FloydWarshall_List(); 
    g2.FloydWarshall_Matrix();
    g2.CriticalPath_List(); 
    g2.CriticalPath_Matrix();
    cout<<'\n';
    cout<<"測資三:負權(負迴圈)連通圖\n";
    Graph g3(10);
    g3.InsertEdge(0,1,5);  
    g3.InsertEdge(0,7,10);
    g3.InsertEdge(1,2,4);  
    g3.InsertEdge(2,3,-10); 
    g3.InsertEdge(3,4,2);  
    g3.InsertEdge(4,1,1);  
    g3.InsertEdge(4,5,8);  
    g3.InsertEdge(5,6,2);
    g3.InsertEdge(7,8,3);  
    g3.InsertEdge(8,9,5);
    g3.BFS_List(0); 
    g3.BFS_Matrix(0);
    g3.DFS_List(0); 
    g3.DFS_Matrix(0);
    // g3.Dijkstra_List(0); Dijkstra 陷入無窮迴圈
    // g3.Dijkstra_Matrix(0); 
    g3.BellmanFord_List(0); 
    g3.BellmanFord_Matrix(0);
    g3.FloydWarshall_List(); 
    g3.FloydWarshall_Matrix();
    g3.CriticalPath_List(); 
    g3.CriticalPath_Matrix(); // 觸發迴圈警告
    cout<<'\n';
    return 0;
}