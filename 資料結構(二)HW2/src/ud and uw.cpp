#include <iostream> //這份是無向無權圖
#include <vector>   //包含 BFS、DFS、生成樹、連通元件、雙連通單元
#include <algorithm>
#include <iterator>
#include <queue> //BFS使用
#include <stack>
using namespace std;
struct Edge{
    int u,v;//點到點
};
class Graph{
private:
    int n;//頂點數
    int e;//邊數
    vector<int>* adj;//鄰接表
    vector<vector<bool>> matrix; //鄰接矩陣
public:
    Graph(int nodes):n(nodes), e(0) {
        adj=new vector<int>[n];
        matrix.resize(n, vector<bool>(n,0));
    }
    virtual ~Graph(){
        delete[] adj; //vec
    }
    bool IsEmpty() const{
        return n==0;
    }
    int NumberOfVertices() const{
        return n;
    }
    int NumberOfEdges() const{
        return e;
    }
    virtual int Degree(int u) const{ //回傳度數 
        if(u<0||u>=n) 
            return 0;
        int adj_list_degree=adj[u].size(); //以adj list度數
        
        int matrix_degree=0;
        for(int i=0;i<n;++i){
            if(matrix[u][i]==1)
                ++matrix_degree;  //以matrix計算度數
        }
        
        if(adj_list_degree!=matrix_degree) //驗證結果相同
            throw "操作錯誤:度數不相等";

        return adj_list_degree;
    }
    virtual bool ExistsEdge(int u,int v) const{
        if(u<0||u>=n||v<0||v>=n) //邊界檢查
            return 0;

        bool adj_list_exist=0; //adj list檢查
        for(int nei:adj[u]){
            if(nei==v){
                adj_list_exist=1;
                break;
            }
        }

        bool matrix_exist=matrix[u][v]; //matrix檢查

        if(adj_list_exist!=matrix_exist)
            throw "操作錯誤:存在狀態不一";

        return adj_list_exist;
    }
    virtual void InsertVertex(int v){ //加入頂點
        if(v>=n){ //如果v大於頂點數才動態記憶體配置,否則就已經包含v
            int new_n=v+1;

            vector<int>* new_adj=new vector<int>[new_n]; //adj list操作
            for(int i=0;i<n;++i){
                new_adj[i]=adj[i];
            }
            delete[] adj;
            adj=new_adj;

            matrix.resize(new_n); //matrix操作
            for(int i=0;i<new_n;++i)
                matrix[i].resize(new_n,0);

            n=new_n;
        }
    }
    virtual void InsertEdge(int u,int v){ //加入邊
        if(u<0||u>=n||v<0||v>=n)
            return;
        if(!ExistsEdge(u,v)){ //邊不存在才加入
            adj[u].push_back(v); //adj list更新
            adj[v].push_back(u);
            matrix[u][v]=1; //matrix更新
            matrix[v][u]=1;
            e++;
        }
    }
    virtual void DeleteVertex(int v){
        if(v<0||v>=n)//邊界條件
            return;

        for (int u:adj[v]){//頂點和v的邊 adj list操作
            auto it=find(adj[u].begin(),adj[u].end(),v);
            if(it!=adj[u].end()){ 
                adj[u].erase(it);
                e--;
            }
        }
        for(int i=v;i<n-1;++i)//往前移覆蓋要刪除的v
            adj[i]=adj[i+1];
        for(int i=0;i<n-1;++i){ 
            for(int& target:adj[i]){
                if(target>v) 
                    target--;
            }
        }

        matrix.erase(matrix.begin()+v);//matrix 操作
        for(int i=0;i<matrix.size();++i)
            matrix[i].erase(matrix[i].begin()+v);

        n--;
    }
    virtual void DeleteEdge(int u,int v){
        if(u<0||u>=n||v<0||v>=n)
            return;
        bool edgeFound=0;

        auto it_u=find(adj[u].begin(),adj[u].end(),v);//u刪v  
        if(it_u!=adj[u].end()){ //adj list 更新
            adj[u].erase(it_u); 
            edgeFound=1;
        }
        auto it_v=find(adj[v].begin(),adj[v].end(),u);//v刪u
        if(it_v!=adj[v].end()){
            adj[v].erase(it_v);
            edgeFound=1;
        }

        if(edgeFound){ //matrix 更新 
            matrix[u][v]=0; //矩陣更新
            matrix[v][u]=0;
            e--;
        }
    }
    void showMartix(){
        for(int k=0;k<n;k++){ //這裡直接用martix show
            cout<<k<<" ";
        }
        cout<<'\n';
        for(int i=0;i<n;++i){
            for(int j=0;j<n;j++){
                if(ExistsEdge(i,j))
                    cout<<"1 ";
                else
                    cout<<"0 ";
            }
            cout<<'\n';
        }
    }
    void showArraylist(){
        for(int i=0;i<n;++i){ //直接用adj list show
            cout<<i<<" : ";
            for(int nei:adj[i])
                cout<<nei<<" ";
            cout<<'\n';
        }
    }
    //1.BFS
    void BFS_List(int start){
            cout<<"[Adj List] BFS: ";
            if(start<0||start>=n)
                return;
            vector<bool> visited(n,0);
            queue<int> q;
            visited[start]=1;
            q.push(start);
            while(!q.empty()){
                int u=q.front(); 
                q.pop();
                cout<<u<<" ";
                for(int v:adj[u]){ 
                    if(!visited[v]){//如果沒造訪過把鄰居推入queue
                        visited[v]=1;
                        q.push(v);
                    }
                }
            }
            cout<<'\n';
        }

    void BFS_Matrix(int start){
        cout << "[Matrix] BFS: ";
        if(start<0||start>=n)
            return;
        vector<bool> visited(n,0);
        queue<int> q;
        
        visited[start]=1;
        q.push(start);
        
        while (!q.empty()) {
            int u = q.front();
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
    //2.DFS
    void DFSUtil_List(int u,vector<bool>& visited){
        visited[u]=1;
        cout<<u<<" ";    
        for(int v:adj[u]){ 
            if(!visited[v])
                DFSUtil_List(v,visited); 
        }
    }
    void DFS_List(int start){
        cout<<"[List] DFS: ";
        vector<bool> visited(n, 0);
        DFSUtil_List(start, visited);
        cout<<'\n';
    }
    void DFSUtil_Matrix(int u, vector<bool>& visited) {
        visited[u]=1;
        cout<<u<<" ";    
        for(int v=0;v<n;++v){
            if(matrix[u][v]&&!visited[v])
                DFSUtil_Matrix(v,visited);
        }
    }
    void DFS_Matrix(int start){
        cout<<"[Matrix] DFS: ";
        vector<bool> visited(n,0);
        DFSUtil_Matrix(start,visited);
        cout<<'\n';
    }
    //3.生成樹
    void STUtil_List(int u,vector<bool>& visited){
        visited[u]=1;
        for(int v:adj[u]){
            if(!visited[v]){
                cout<<"("<<u<<","<<v<<") ";
                STUtil_List(v, visited);
            }
        }
    }
    void ST_List(){
        cout<<"[List] 生成樹: \n";
        vector<bool> visited(n,0);
        for (int i=0;i<n;++i){ //預防是不連通圖
            if(!visited[i]){
                STUtil_List(i,visited);
                cout<<'\n'; 
            }
        }
    }
    void STUtil_Matrix(int u,vector<bool>& visited){
        visited[u]=1;
        for(int v=0;v<n;++v){
            if(matrix[u][v]&&!visited[v]){
                cout<<"("<<u<<","<<v<<") "; 
                STUtil_Matrix(v,visited);
            }
        }
    }
    void ST_Matrix(){
        cout<<"[Matrix] 生成樹: \n";
        vector<bool> visited(n,0);
        for(int i=0;i<n;++i){
            if(!visited[i]){
                STUtil_Matrix(i,visited);
                cout<<'\n';
            }
        }
    }
    //4.連通元件
    void CC_List(){
        cout<<"[List] 連通元件: \n";
        vector<bool> visited(n, 0);
        int count=0;
        for (int i=0;i<n;++i) {
            if (!visited[i]){ // 發現新群組
                cout<<"群組 "<<++count<<": ";
                DFSUtil_List(i, visited);
                cout<< '\n'; // 感染整塊島嶼
            }
        }
    }

    void CC_Matrix() {
        cout<<"[Matrix] 連通元件: \n";
        vector<bool> visited(n,0);
        int count=0;
        for (int i=0;i<n;++i){
            if(!visited[i]){
                cout<<"群組 "<<++count<<": ";
                DFSUtil_Matrix(i, visited);
                cout<<'\n';
            }
        }
    }
    //5.雙連通單元
    void BCCUtil_List(int u,vector<int>& dfn,vector<int>& low,vector<int>& parent,stack<Edge>& st,int& time,int& count){
        dfn[u]=low[u]=++time; 
        int child=0; // 紀錄 DFS 樹上的小孩數
        for (int v:adj[u]) {
            if (dfn[v]==-1) { // 沒走過 (Tree Edge)
                child++;
                parent[v]=u;
                st.push({u,v}); // 把邊推入堆疊
                BCCUtil_List(v, dfn, low, parent, st, time,count); // 遞迴往下一代走
                low[u] = min(low[u],low[v]); // 更新自己的 low 值
                
                // 判斷關節點：(1)是起點且小孩>1 (2)非起點且小孩的low >= 自己的dfn
                if((parent[u]==-1&&child>1)||(parent[u]!=-1&&low[v]>=dfn[u])){
                    count++;
                    cout << "BCC 群組"<<count<<": ";
                    while (st.top().u != u || st.top().v != v) {
                        cout << "(" << st.top().u << "," << st.top().v << ") ";
                        st.pop();
                    }
                    cout << "(" << st.top().u << "," << st.top().v << ")\n";
                    st.pop(); // 把屬於這個 BCC 的邊全部彈出
                }
            } else if (v != parent[u]&&dfn[v]<dfn[u]){
                low[u] = min(low[u], dfn[v]); 
                st.push({u, v}); 
            }
        }
    }
    void BCC_List() {
        cout << "[List] 雙連通單元: \n";
        vector<int> dfn(n, -1), low(n, -1), parent(n, -1);
        stack<Edge> st;
        int time=0;
        int count=0;
        for(int i=0;i<n;++i){
            if(dfn[i]==-1){
                BCCUtil_List(i,dfn,low,parent,st,time,count);
                bool hasEdge=0;
                while(!st.empty()){ //刪除堆疊剩餘的邊
                    if(!hasEdge){
                        ++count;
                        cout<<"BCC 群組"<<count<<": "; 
                        hasEdge=1;
                    }
                    cout << "("<<st.top().u<<","<< st.top().v << ") ";
                    st.pop();
                }
                if(hasEdge)
                    cout<<'\n';
            }
        }
    }

    void BCCUtil_Matrix(int u,vector<int>& dfn,vector<int>& low,vector<int>& parent,stack<Edge>& st,int& time,int& count) {
        dfn[u]=low[u]=++time; 
        int child=0;         
        for(int v=0;v<n;++v){
            if(matrix[u][v]){ 
                if(dfn[v]==-1){ 
                    child++;
                    parent[v]=u;
                    st.push({u,v}); 
                    BCCUtil_Matrix(v,dfn,low,parent,st,time,count); 
                    low[u]=min(low[u],low[v]); 
                    if((parent[u]==-1&&child>1)||(parent[u]!=-1&&low[v]>=dfn[u])){
                        ++count;
                        cout<<"BCC 群組"<<count<<": ";
                        while(st.top().u != u || st.top().v != v){
                            cout<<"("<<st.top().u<<","<<st.top().v<<") ";
                            st.pop();
                        }
                        cout<<"("<<st.top().u<<","<<st.top().v<<")\n";
                        st.pop();
                    }
                }else if(v!=parent[u]&&dfn[v]<dfn[u]){ 
                    low[u]=min(low[u],dfn[v]); 
                    st.push({u,v}); 
                }
            }
        }
    }
    void BCC_Matrix() {
        cout<<"[Matrix] 雙連通單元: \n";
        vector<int> dfn(n,-1), low(n,-1), parent(n,-1);
        stack<Edge> st;
        int time=0;
        int count=0;
        for (int i=0;i<n;++i){
            if(dfn[i]==-1){
                BCCUtil_Matrix(i,dfn,low,parent,st,time,count);
                bool hasEdge=0;
                while(!st.empty()){
                    if(!hasEdge){
                        ++count;
                        cout<<"BCC 群組"<<count<<": ";
                        hasEdge=1; 
                    }
                    cout<<"("<<st.top().u<<","<<st.top().v<<") ";
                    st.pop();
                }
                if(hasEdge)
                    cout<<'\n';
            }
        }
    }
};
int main(){
    cout<<"測資一 綜合連通圖\n";
    Graph g1(7);
    g1.InsertEdge(0,1); 
    g1.InsertEdge(1,2); 
    g1.InsertEdge(0,2); 
    g1.InsertEdge(2,3); 
    g1.InsertEdge(3,4); 
    g1.InsertEdge(4,5); 
    g1.InsertEdge(3,5); 
    g1.InsertEdge(5,6); 
    cout<<"BFS與DFS 測試\n";
    g1.BFS_List(0); 
    g1.BFS_Matrix(0); 
    g1.DFS_List(0); 
    g1.DFS_Matrix(0); 
    cout<<"生成樹 測試\n";
    g1.ST_List();   
    g1.ST_Matrix();  
    cout<<"連通元件 測試\n";
    g1.CC_List();   
    g1.CC_Matrix();   
    cout<<"雙連通單元 測試\n";
    g1.BCC_List(); 
    g1.BCC_Matrix();  
    cout<<"測資二 離散森林\n";
    Graph g2(8);
    g2.InsertEdge(0,1);
    g2.InsertEdge(1,2);                    
    g2.InsertEdge(3,4);
    g2.InsertEdge(4,5);
    g2.InsertEdge(3,5); 
    g2.InsertEdge(6,7);                                   
    cout<<"BFS與DFS測試\n";
    g2.BFS_List(0);
    g2.BFS_Matrix(0);
    g2.DFS_List(0);
    g2.DFS_Matrix(0); 
    cout<<"生成樹 測試\n";
    g2.ST_List();
    g2.ST_Matrix(); 
    cout<<"連通元件 測試\n";
    g2.CC_List();
    g2.CC_Matrix();
    cout<<"雙連通單元 測試\n";
    g2.BCC_List(); 
    g2.BCC_Matrix();
    cout<<"(測資三 星狀圖)\n";
    Graph g3(5);
    g3.InsertEdge(0,1); 
    g3.InsertEdge(0,2); 
    g3.InsertEdge(0,3); 
    g3.InsertEdge(0,4); 
    cout<<"BFS與DFS 測試\n";
    g3.BFS_List(0); 
    g3.BFS_Matrix(0); 
    g3.DFS_List(0); 
    g3.DFS_Matrix(0); 
    cout<<"生成樹 測試\n";
    g3.ST_List();
    g3.ST_Matrix();
    cout<<"連通元件 測試\n";
    g3.CC_List();
    g3.CC_Matrix();
    cout<<"雙連通單元 測試\n";
    g3.BCC_List(); 
    g3.BCC_Matrix();
    return 0;
}