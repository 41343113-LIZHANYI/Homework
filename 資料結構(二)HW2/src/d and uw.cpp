#include <iostream> //這份是有向無權圖
#include <vector>   //包含 BFS、DFS、頂點活動網路
#include <algorithm> 
#include <iterator>
#include <queue> 
#include <stack>
using namespace std;
struct Edge{
    int u,v;
};
class Graph{
private:
    int n;
    int e;
    vector<int>* adj;
    vector<vector<bool>> matrix; 
public:
    Graph(int nodes):n(nodes),e(0){
        adj=new vector<int>[n];
        matrix.resize(n,vector<bool>(n,0));
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
        for(int i=0;i<n;++i)
            if(matrix[u][i]==1)++matrix_degree;  
        if(adj_list_degree!=matrix_degree)
            throw "操作錯誤:度數不相等";
        return adj_list_degree;
    }
    virtual int InDegree(int u) const{
        if(u<0||u>=n)
            return 0;
        int matrix_degree=0;
        for(int i=0;i<n;++i){
            if(matrix[i][u]==1)
                ++matrix_degree;
        }
        int adj_list_degree=0;
        for(int i=0;i<n;++i){
            for(int v:adj[i]){
                if(v==u)
                    ++adj_list_degree; 
            }
        }
        if(adj_list_degree!=matrix_degree)
            throw "操作錯誤:度數不相等";
        return adj_list_degree;
    }

    virtual bool ExistsEdge(int u,int v) const{
        if(u<0||u>=n||v<0||v>=n)
            return 0;
        bool adj_list_exist=0; 
        for(int nei:adj[u]){
            if(nei==v){
                adj_list_exist=1;
                break;
            }
        }
        bool matrix_exist=matrix[u][v]; 
        if(adj_list_exist!=matrix_exist)
            throw "操作錯誤:存在狀態不一";
        return adj_list_exist;
    }
    virtual void InsertVertex(int v){ 
        if(v>=n){ 
            int new_n=v+1;
            vector<int>* new_adj=new vector<int>[new_n]; 
            for(int i=0;i<n;++i)
                new_adj[i]=adj[i];
            delete[] adj;
            adj=new_adj;
            matrix.resize(new_n); 
            for(int i=0;i<new_n;++i)
                matrix[i].resize(new_n,0);
            n=new_n;
        }
    }
    virtual void InsertEdge(int u,int v){ 
        if(u<0||u>=n||v<0||v>=n)
            return;
        if(!ExistsEdge(u,v)){ //【有向圖只需單邊
            adj[u].push_back(v); 
            matrix[u][v]=1; 
            e++;
        }
    }
    virtual void DeleteVertex(int v){
        if(v<0||v>=n)
            return;
        int removedEdges=adj[v].size(); 
        for(int i=0;i<n;++i){
            if(i!=v){
                auto it=find(adj[i].begin(),adj[i].end(),v);
                if(it!=adj[i].end()){
                    adj[i].erase(it);
                    removedEdges++;
                }
            }
        }
        e-=removedEdges;
        for(int i=v;i<n-1;++i)
            adj[i]=adj[i+1];
        for(int i=0;i<n-1;++i){ 
            for(int& target:adj[i]){
                if(target>v)
                    target--;
            }
        }
        matrix.erase(matrix.begin()+v);
        for(int i=0;i<matrix.size();++i)
            matrix[i].erase(matrix[i].begin()+v);
        n--;
    }

    virtual void DeleteEdge(int u,int v){
        if(u<0||u>=n||v<0||v>=n)
            return;
        bool edgeFound=0;
        auto it_u=find(adj[u].begin(),adj[u].end(),v);
        if(it_u!=adj[u].end()){ 
            adj[u].erase(it_u); 
            edgeFound=1;
        }
        if(edgeFound){ 
            matrix[u][v]=0; 
            e--;
        }
    }
    void showMartix(){
        for(int k=0;k<n;k++)cout<<k<<" ";
        cout<<'\n';
        for(int i=0;i<n;++i){
            for(int j=0;j<n;j++){
                if(ExistsEdge(i,j))cout<<"1 ";
                else cout<<"0 ";
            }
            cout<<'\n';
        }
    }
    void showArraylist(){
        for(int i=0;i<n;++i){ 
            cout<<i<<" : ";
            for(int nei:adj[i])cout<<nei<<" ";
            cout<<'\n';
        }
    }
    // 1.BFS
    void BFS_List(int start){
        cout<<"[Adj List] BFS: ";
        if(start<0||start>=n)return;
        vector<bool> visited(n,0);
        queue<int> q;
        visited[start]=1;
        q.push(start);
        while(!q.empty()){
            int u=q.front(); 
            q.pop();
            cout<<u<<" ";
            for(int v:adj[u]){ 
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
        if(start<0||start>=n)return;
        vector<bool> visited(n,0);
        queue<int> q;
        visited[start]=1;
        q.push(start);
        while(!q.empty()){
            int u=q.front();
            q.pop();
            cout<<u<<" ";
            for(int v=0;v<n;++v){ 
                if(matrix[u][v]&&!visited[v]){
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
        for(int v:adj[u]){ 
            if(!visited[v])DFSUtil_List(v,visited); 
        }
    }
    void DFS_List(int start){
        cout<<"[List] DFS: ";
        vector<bool> visited(n,0);
        DFSUtil_List(start,visited);
        cout<<'\n';
    }

    void DFSUtil_Matrix(int u,vector<bool>& visited){
        visited[u]=1;
        cout<<u<<" ";    
        for(int v=0;v<n;++v){
            if(matrix[u][v]&&!visited[v])DFSUtil_Matrix(v,visited);
        }
    }
    void DFS_Matrix(int start){
        cout<<"[Matrix] DFS: ";
        vector<bool> visited(n,0);
        DFSUtil_Matrix(start,visited);
        cout<<'\n';
    }
    // 3.AOV(Topological Sort)
    void TopologicalSort_List(){
        cout<<"[Adj List] AOV(拓撲排序): ";
        if(n==0)
            return;
        vector<int> in_degree(n,0);
        for(int i=0;i<n;++i){
            for(int v:adj[i])
                in_degree[v]++;
        }
        queue<int> q;
        for(int i=0;i<n;++i){
            if(in_degree[i]==0)
                q.push(i);
        }
        int count=0;
        vector<int> topo_order;
        while(!q.empty()){
            int u=q.front();
            q.pop();
            topo_order.push_back(u);
            for(int v:adj[u]){
                if(--in_degree[v]==0)
                    q.push(v);
            }
            count++;
        }
        if(count!=n){
            cout<<"圖中存在迴圈，無法進行拓撲排序\n";
            return;
        }
        for(int v:topo_order)
            cout<<v<<" ";
        cout<<'\n';
    }

    void TopologicalSort_Matrix(){
        cout<<"[Matrix] AOV(拓撲排序): ";
        if(n==0)
            return;
        vector<int> in_degree(n,0);
        for(int i=0;i<n;++i){
            for(int j=0;j<n;++j){
                if(matrix[i][j])
                    in_degree[j]++;
            }
        }
        queue<int> q;
        for(int i=0;i<n;++i){
            if(in_degree[i]==0)
                q.push(i);
        }
        int count=0;
        vector<int> topo_order;
        while(!q.empty()){
            int u=q.front();
            q.pop();
            topo_order.push_back(u);
            for(int v=0;v<n;++v){
                if(matrix[u][v]){
                    if(--in_degree[v]==0)q.push(v);
                }
            }
            count++;
        }
        if(count!=n){
            cout<<"圖中存在迴圈，無法進行拓撲排序\n";
            return;
        }
        for(int v:topo_order)
            cout<<v<<" ";
        cout<<'\n';
    }
};
int main(){
    cout<<"測資一:標準有向無環圖測試\n";
    Graph g1(10);
    g1.InsertEdge(0,2);
    g1.InsertEdge(1,2);
    g1.InsertEdge(2,3);
    g1.InsertEdge(2,4);
    g1.InsertEdge(3,5);
    g1.InsertEdge(3,6);
    g1.InsertEdge(4,6);
    g1.InsertEdge(4,7);
    g1.InsertEdge(5,8);
    g1.InsertEdge(6,8);
    g1.InsertEdge(7,8);
    g1.InsertEdge(8,9);
    g1.BFS_List(0);
    g1.BFS_Matrix(0);
    g1.DFS_List(0);
    g1.DFS_Matrix(0);
    g1.TopologicalSort_List();
    g1.TopologicalSort_Matrix();
    cout<<'\n';
    cout<<"測資二:多源圖測試\n";
    Graph g2(10);
    g2.InsertEdge(0,3);
    g2.InsertEdge(1,3);
    g2.InsertEdge(1,4);
    g2.InsertEdge(2,4);
    g2.InsertEdge(3,5);
    g2.InsertEdge(4,5);
    g2.InsertEdge(4,6);
    g2.InsertEdge(5,7);
    g2.InsertEdge(6,8);
    g2.InsertEdge(7,9);
    g2.InsertEdge(8,9);
    g2.BFS_List(0); 
    g2.BFS_Matrix(0);
    g2.DFS_List(0);
    g2.DFS_Matrix(0);
    g2.TopologicalSort_List();
    g2.TopologicalSort_Matrix();
    cout<<'\n';
    cout<<"測資三:交錯相依圖測試\n";
    Graph g3(10);
    g3.InsertEdge(0,8); 
    g3.InsertEdge(0,5); 
    g3.InsertEdge(8,2); 
    g3.InsertEdge(8,9); 
    g3.InsertEdge(5,1); 
    g3.InsertEdge(5,4); 
    g3.InsertEdge(2,3); 
    g3.InsertEdge(9,6); 
    g3.InsertEdge(1,7); 
    g3.InsertEdge(4,7); 
    g3.InsertEdge(3,7); 
    g3.InsertEdge(6,7);
    g3.BFS_List(0); 
    g3.BFS_Matrix(0);
    g3.DFS_List(0); 
    g3.DFS_Matrix(0);
    g3.TopologicalSort_List(); 
    g3.TopologicalSort_Matrix();
    cout<<'\n';
}