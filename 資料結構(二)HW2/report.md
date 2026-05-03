# 41343113 李展毅 41343144 廖峻德 (第六組 HW2)

資料結構(二)作業二 第六組  

## 解題說明
### 問題描述
1. 使用有向、無向、有權重、無權重的四種 Graph 並實現以下算法
   1. Elementary Graph Operations (基本圖運算)
      1. Depth First Search (深度優先搜尋)
      2. Breadth First Search (廣度優先搜尋)
      3. Connected Components (連通元件)
      4. Spanning Trees (生成樹)
      5. Biconnected Components (雙連通單元)
   2. Minimum Cost Spanning Trees (最小生成樹)
      1. Kruskal’s Algorithm (克魯斯克爾演算法)
      2. Prim’s Algorithm (普林演算法)
   3. Shortest Paths and Transitive Closure (最短路徑和傳遞閉包)
      1. Single Source/All Destination: Nonnegative Edge Costs (一對多:無負權)
      2. Single Source/All Destination: General Weights (一對多:有負權)
      3. All-Pairs Shortest Paths (多對多)
   4. Activity Networks (活動網絡)
      1. Activity on Vertex (AOV) Networks (頂點活動網路)
      2. Activity on Edge (AOV) Networks (邊活動網路)

### 解題策略
各算法有限制圖的類別與情況

共分四個cpp檔案 (ud and uw.cpp、ud and w.cpp、d and uw.cpp、d and w.cpp)

有權重、無權重、有向、無向的組合並分配以下算法

1. DFS & BFS (四種圖皆可)
2. 生成樹 (無向無權)
3. 連通元件 & 雙連通單元 (無向無權)
4. Kruskal’s Algorithm & Prim’s Algorithm (無向有權)
5. 一對多最短路徑:無負權 Dijkstra's Algorithm (有向有權、無向有權)
6. 一對多最短路徑:有負權 Bellman-Ford Algorithm (有向有權、無向有權)
7. 多對多最短路徑 Floyd-Warshall Algorithm (有向有權、無向有權)
8. 頂點活動網路 (有向無權)
9. 邊活動網路 (有向有權)

由於我們**兩種表示法會同時存在於同個程式**

我們會在部分函式操作完後用if(adj Matrix==adj list)

來驗證兩種操作的結果相同

#### (1) Max/Min Heap
1. ChangeSize1D()

   當陣列容量(capacity)不足時，配置一個大小為原本兩倍加一(2*capacity+1)的新陣列，將舊資料複製過去後釋放舊記憶體，指向新空間，達成動態記憶體配置。

2. Push(const T& e)

   先檢查容量是否不夠，需要擴充陣列，接著將新元素放置到樹的末端。
   
   利用向上浮動的方式，比較當前節點與父節點的大小。
   
   若新元素大於(MaxHeap)或小於父節點(MinHeap)，則將父節點的值往下拉，直到找到符合Heap特性的位置，最後才將值放入。

3. Pop()

   先檢查堆積是否為空。將最後一個元素暫存，並準備將其放入根節點。
   
   接著利用逐一向下的方式，比較當前節點和兩個子節點。
   
   挑選較大(MaxHeap)或較小(MinHeap)的子節點往上移，反覆執行直到最後一個元素大於等於(MaxHeap)或小於等於(MinHeap)其子節點，或是到達葉節點停止，最後將值放入該位置。
#### (2) BST
1. BSTInsert(const pair<K, E>& e)
   
   利用遞迴的方式，將傳入的鍵值對與當前節點進行比較。
   
   若鍵值小於當前節點往左子樹尋找，大於則往右子樹尋找。
   
   遇到空指標時即為插入位置，動態配置新節點並回傳，若鍵值已存在則更新該節點的內容。
   
2. Delete(const K& k)
   
   實作二元搜尋樹的節點刪除，需先以遞迴找到目標鍵值的位置。
   
   刪除情況分三種：無子節點直接刪除、單一子節點以其子節點取代、兩個子節點則尋找右子樹的最小值（中序後繼）替換。
   
   替換資料後，再針對該後繼節點進行遞迴刪除，確保樹的結構維持二元搜尋樹的性質。

3. getHeight(TreeNode<K, E>* node)
   
   利用遞迴走訪計算樹的高度，若節點為空則回傳0，
   
   分別取得左子樹與右子樹的高度後，取最大值加一，即為當前節點的樹高，
   
   最終將根節點算出的高度除以 $\log_2 n$ 求得比值，完成題目的驗證要求。

## 程式實作
以下為主要程式碼：
#### (1) ud and uw(無向、無權重:BFS、DFS、生成樹、連通元件、雙連通單元)
``` c++
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
```
#### (2) ud and w(無向、無權重:BFS、DFS、Kruskal、Prim、Dijkstra、Bellman-Ford、Floyd)
``` c++
#include <iostream>
#include <vector>
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
```
#### (3) d and uw()
``` c++

```
#### (4) d and w()
``` c++

```
## 效能分析
#### (1) ud and uw (無向、無權重)
   1. ChangeSize1D(T*& a, const int oldSize, const int newSize)
      * 時間複雜度： $O(n)$ // 必須將原陣列的$n$個元素複製到新陣列中
      * 空間複雜度： $O(n)$ // 需額外配置大小為新容量的動態陣列
   2. Push(const T& e)
      * 時間複雜度：最壞情況 $O(\log n)$ // 需從樹葉向上遍歷至樹根；平攤複雜度約為 $O(1)$
      * 空間複雜度： $O(1)$ // 除了可能觸發陣列擴充外，僅需固定數量的指標與變數
   3. Pop()
      * 時間複雜度：最壞情況 $O(\log n)$ // 需從樹根向下遍歷至樹葉
      * 空間複雜度： $O(1)$ // 僅進行常數變數操作
   4. Top()/IsEmpty()
      * 時間複雜度： $O(1)$ // 陣列取值與簡單條件判斷
      * 空間複雜度： $O(1)$
   5. MaxHeap()/MinHeap() 建構與解構
      * 時間複雜度： $O(1)$ // 初始化與釋放記憶體
      * 空間複雜度： $O(1)$
#### (2) ud and w (無向、有權重)
#### (3) d and uw (有向、無權重)
#### (4) d and w (有向、有權重)

## 測試案例
### (1) ud and uw (無向、無權重)
####  測資一：綜合連通圖 
| 原圖  | BFS 廣度優先搜尋 | DFS 深度優先搜尋 |
| :---: | :---: | :---: |
| <img src="https://github.com/user-attachments/assets/46e66031-a326-4fa1-a7f2-882d4a187ad3" width="100%" alt="tc1_original"> | <img src="https://github.com/user-attachments/assets/7bc3924a-d98e-447e-a255-f46a5ab67755" width="100%" alt="tc1_bfs"> | <img src="https://github.com/user-attachments/assets/5bffe9db-0fa7-43c3-bcee-974615cbd31b" width="100%" alt="tc1_dfs"> |
| 生成樹 | 連通元件 | 雙連通元件 |
| <img width="2460" height="1859" alt="tc1_tree" src="https://github.com/user-attachments/assets/828c60d7-8fba-485e-ad1c-ef8a41a126cf" /> | <img width="2460" height="1859" alt="tc1_cc" src="https://github.com/user-attachments/assets/ebaa68cd-0c0a-41f8-9829-a41ed69fb0ca" />| <img width="2460" height="1859" alt="tc1_bcc" src="https://github.com/user-attachments/assets/aed053dc-da01-4b6d-bd15-5c9027cdf796" />|

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | 0 1 2 3 4 5 6 | 0 1 2 3 4 5 6 |
| **DFS** | 0 1 2 3 4 5 6 | 0 1 2 3 4 5 6 |
| **生成樹** | (0,1) (1,2) (2,3) (3,4) (4,5) (5,6) | (0,1) (1,2) (2,3) (3,4) (4,5) (5,6) |
| **連通元件 (CC)** | 群組 1: 0 1 2 3 4 5 6 | 群組 1: 0 1 2 3 4 5 6 |
| **雙連通單元 (BCC)** | BCC 群組 1: (5,6)<br>BCC 群組 2: (5,3) (4,5) (3,4)<br>BCC 群組 3: (2,3)<br>BCC 群組 4: (2,0) (1,2) (0,1) | BCC 群組 1: (5,6)<br>BCC 群組 2: (5,3) (4,5) (3,4)<br>BCC 群組 3: (2,3)<br>BCC 群組 4: (2,0) (1,2) (0,1) |
#### 測資二：離散森林
| 原圖  | BFS 廣度優先搜尋 | DFS 深度優先搜尋 |
| :---: | :---: | :---: |
| <img width="2460" height="1859" alt="tc2_original" src="https://github.com/user-attachments/assets/5c054e29-10ac-4aab-a3c7-63d79a5527d1" />|<img width="2460" height="1859" alt="tc2_bfs" src="https://github.com/user-attachments/assets/144347ed-6a55-4034-bb17-a415947f7fea" />| <img width="2460" height="1859" alt="tc2_dfs" src="https://github.com/user-attachments/assets/82bf285d-d581-427b-ae47-b8a09423ea07" />|
| 生成樹 | 連通元件 | 雙連通元件 |
|<img width="2460" height="1859" alt="tc2_tree" src="https://github.com/user-attachments/assets/1895ff8d-8ac3-4f12-b8da-ab88a8901817" />|<img width="2460" height="1859" alt="tc2_cc" src="https://github.com/user-attachments/assets/01effc7d-e777-4a01-8c98-91862d1a9ccf" />|<img width="2460" height="1859" alt="tc2_bcc" src="https://github.com/user-attachments/assets/d2123c10-25fc-45f5-8a5a-8166982573aa" />|

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | 0 1 2 | 0 1 2 |
| **DFS** | 0 1 2 | 0 1 2 |
| **生成樹** | (0,1) (1,2)<br>(3,4) (4,5)<br>(6,7) | (0,1) (1,2)<br>(3,4) (4,5)<br>(6,7) |
| **連通元件 (CC)** | 群組 1: 0 1 2<br>群組 2: 3 4 5<br>群組 3: 6 7 | 群組 1: 0 1 2<br>群組 2: 3 4 5<br>群組 3: 6 7 |
| **雙連通單元 (BCC)** | BCC 群組 1: (1,2)<br>BCC 群組 2: (0,1)<br>BCC 群組 3: (5,3) (4,5) (3,4)<br>BCC 群組 4: (6,7) | BCC 群組 1: (1,2)<br>BCC 群組 2: (0,1)<br>BCC 群組 3: (5,3) (4,5) (3,4)<br>BCC 群組 4: (6,7) |
#### 測資三：星狀圖
| 原圖  | BFS 廣度優先搜尋 | DFS 深度優先搜尋 |
| :---: | :---: | :---: |
|<img width="2460" height="1859" alt="tc3_original" src="https://github.com/user-attachments/assets/379eb4f6-7578-4ca8-bcd5-83dfe3e6cb7c" />|<img width="2460" height="1859" alt="tc3_bfs" src="https://github.com/user-attachments/assets/2560d5a5-3274-4880-a91c-dffde495660a" />|<img width="2460" height="1859" alt="tc3_dfs" src="https://github.com/user-attachments/assets/1bb0694b-c118-43be-ae3b-a9ef52a51985" />|
| 生成樹 | 連通元件 | 雙連通元件 |
|<img width="2460" height="1859" alt="tc3_tree" src="https://github.com/user-attachments/assets/66e78cfb-6e1a-48be-9e09-fc1a412c104e" />|<img width="2460" height="1859" alt="tc3_cc" src="https://github.com/user-attachments/assets/c519a107-e9a5-46fe-b8fe-e114d9a7ba57" />|<img width="2460" height="1859" alt="tc3_bcc" src="https://github.com/user-attachments/assets/e34eb32a-ea76-474a-b123-c97d53c9b57b" />|

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | 0 1 2 3 4 | 0 1 2 3 4 |
| **DFS** | 0 1 2 3 4 | 0 1 2 3 4 |
| **生成樹** | (0,1) (0,2) (0,3) (0,4) | (0,1) (0,2) (0,3) (0,4) |
| **連通元件 (CC)** | 群組 1: 0 1 2 3 4 | 群組 1: 0 1 2 3 4 |
| **雙連通單元 (BCC)** | BCC 群組 1: (0,2)<br>BCC 群組 2: (0,3)<br>BCC 群組 3: (0,4)<br>BCC 群組 4: (0,1) | BCC 群組 1: (0,2)<br>BCC 群組 2: (0,3)<br>BCC 群組 3: (0,4)<br>BCC 群組 4: (0,1) |

### (2) ud and w (無向、有權重)
#### 測資一：綜合有權連通圖
| 原圖 (Original) | BFS 廣度優先搜尋 | DFS 深度優先搜尋 |
| :---: | :---: | :---: |
| <img width="3000" height="1800" alt="tc1_original" src="https://github.com/user-attachments/assets/a284f5c7-5d29-4451-ba40-f3a5cee8ebdf" />| <img width="3000" height="1800" alt="tc1_bfs" src="https://github.com/user-attachments/assets/16eef40b-0083-4e77-8584-c37f1bd6d846" />| <img width="3000" height="1800" alt="tc1_dfs" src="https://github.com/user-attachments/assets/ccbf77a8-0f2d-4bda-b49c-e68a81464863" />|

| Kruskal 最小生成樹 | Prim 最小生成樹 |
| :---: | :---: |
| <img width="3000" height="1800" alt="tc1_kruskal" src="https://github.com/user-attachments/assets/c730c39e-56fb-4049-bdea-183c17c1c2f2" />| <img width="3000" height="1800" alt="tc1_prim" src="https://github.com/user-attachments/assets/a374916b-6c93-4d42-8e92-17c075d69a5b" />| 

|Dijkstra 最短路徑| Bellman-Ford 最短路徑 | Floyd-Warshall 全點對路徑 |
| :---: | :---: | :--- |
|<img width="3000" height="1800" alt="tc1_dijkstra" src="https://github.com/user-attachments/assets/12c25918-98c5-4f74-a580-332d7c23ed90" />|<img width="3000" height="1800" alt="tc1_bellman" src="https://github.com/user-attachments/assets/49cb9072-3747-49ac-b82e-a0ea1c357351" /> |<img width="3000" height="1800" alt="tc1_floyd" src="https://github.com/user-attachments/assets/a84650d4-8c70-4003-ae66-3f124f07bcee" />|
#### 測資二：捷徑有權連通圖

| 原圖 (Original) | BFS 廣度優先搜尋 | DFS 深度優先搜尋 |
| :---: | :---: | :---: |
| <img width="3000" height="1805" alt="tc2_original" src="https://github.com/user-attachments/assets/f4050354-b0f0-4af9-8653-95ef88a67cab" />|<img width="3000" height="1805" alt="tc2_bfs" src="https://github.com/user-attachments/assets/0a39e6ff-5f3f-45fa-b1fb-561c9b5a0d30" />|<img width="3000" height="1805" alt="tc2_dfs" src="https://github.com/user-attachments/assets/7f0c8f23-cdce-4d46-9cfd-397acd8224c7" />|

| Kruskal 最小生成樹 | Prim 最小生成樹 |
| :---: | :---: |
| <img width="3000" height="1805" alt="tc2_kruskal" src="https://github.com/user-attachments/assets/fa31a62b-99d4-4e45-8cb3-700ed55e5d08" />| <img width="3000" height="1805" alt="tc2_prim" src="https://github.com/user-attachments/assets/98f6da25-eda6-4d3b-8a69-ba7f95c7b054" />| 

|Dijkstra 最短路徑| Bellman-Ford 最短路徑 | Floyd-Warshall 全點對路徑 |
| :---: | :---: | :--- |
|<img width="3000" height="1805" alt="tc2_dijkstra" src="https://github.com/user-attachments/assets/a9ad032c-b515-4da7-94e6-cdf61bf842ee" />|<img width="3000" height="1800" alt="tc2_bellman" src="https://github.com/user-attachments/assets/765400e2-2db2-4daa-b58b-ccfdef971809" />|<img width="3000" height="1800" alt="tc2_floyd" src="https://github.com/user-attachments/assets/9ed799f5-aed7-41e5-a16d-53cb997c4ae0" />|

#### 測資三：負權連通圖與負權迴圈
**測試重點**：故意埋入致命的「負權重邊」，展示 Dijkstra 的侷限性，以及防呆專武演算法的糾錯能力。

| 原圖 (Original) | BFS 廣度優先搜尋 | DFS 深度優先搜尋 |
| :---: | :---: | :---: |
| <img width="3000" height="1805" alt="tc3_original" src="https://github.com/user-attachments/assets/558480a1-1f53-4680-a5cf-91babd35a666" />| <img width="3000" height="1805" alt="tc3_bfs" src="https://github.com/user-attachments/assets/5f552240-9b04-48d0-af25-2a30cc46f418" />| <img width="3000" height="1805" alt="tc3_dfs" src="https://github.com/user-attachments/assets/52368588-389f-472b-8ad8-d1ca41762606" />|

| Kruskal 最小生成樹 | Prim 最小生成樹 |
| :---: | :---: |
|<img width="3000" height="1805" alt="tc3_kruskal" src="https://github.com/user-attachments/assets/df5b2c0f-6254-4c50-a163-e05b8a2633a4" />|<img width="3000" height="1805" alt="tc3_prim" src="https://github.com/user-attachments/assets/4bf4c50b-4548-496c-bead-65b6fd25f946" />| 

|Dijkstra 最短路徑| Bellman-Ford 最短路徑 | Floyd-Warshall 全點對路徑 |
| :---: | :---: | :--- |
|<img width="3000" height="1805" alt="tc3_dijkstra" src="https://github.com/user-attachments/assets/e097e1ca-1480-4c16-a2f5-dc0eeb913f35" />|<img width="3000" height="1800" alt="tc3_bellman" src="https://github.com/user-attachments/assets/2186a8f7-3613-47d3-b0da-9faaeed62c91" />|<img width="3000" height="1800" alt="tc3_floyd" src="https://github.com/user-attachments/assets/58e2e91b-8599-4401-86b4-0ec639ef05db" />|
|Dijkstra 陷入無窮迴圈|Bellman-Ford dp到最後檢測出負環|Floyd-Warshall dp到最後檢測出負環|
### (3) d and uw (有向、無權重)
### (4) d and w (有向、有權重)
### 結論
1. Heap 的效率在 $n=10^6$ 的測試下，MaxHeap與MinHeap的執行時間幾乎一致，證明演算法對稱性與複雜度完全相同，均符合 $O(\log n)$。

2. 測試輸出結果顯示 $height / \log_2 n$ 的比值落在2.0到2.6之間，符合題目預期。
## 申論及開發報告
### 程式分析
對當前寫的程式做優點以及資料結構&演算法分析，還有程式需注意的要點
#### (1) Max/Min Heap

##### [使用資料結構與演算法]
* 資料結構： 一維陣列模擬的完全二元樹
* 演算法：
  1. 利用向上浮動與向下沉降進行節點比較與位置交換
  2. 並在容量不足時動態配置兩倍加一的新陣列
##### [須注意的事]
1. 程式跳過heap[0]使用1 based，使父節點$i$的子節點直接為 $2i$ 和 $2i+1$
#### (2) BST
##### [使用資料結構與演算法]
* 資料結構： 指標鏈結的二元搜尋樹
* 演算法： 遞迴樹狀走訪
##### [須注意的事]
1. 迴深度過大會引發堆疊溢位，這點會在優化時處理
### 程式改進
對目前的程式片段做優化處理
#### (1) Max/Min Heap — 由下而上建樹 
```c++
MaxHeap(T* initArray,int n){
    heapSize=n; //設定初始大小
    capacity=n+10; //預留空間
    heap=new T[capacity+1]; //配置1based動態陣列
    for(int i=1;i<=n;++i)
        heap[i]=initArray[i-1]; //將外部陣列資料複製進heap
    for (int i=heapSize/2;i>=1;--i){ //從最後一個非葉節點向前遍歷
        int currentNode=i;
        int child=2*i;
        T temp=heap[currentNode]; //暫存當前準備向下的節點值
        while(child<=heapSize){
            if(child< heapSize&&heap[child]<heap[child+1])
                child++; //挑選子節點中較大者
            if(temp>=heap[child])
                break;
            heap[currentNode]=heap[child]; //子節點上移
            currentNode=child; //更新位置
            child*=2; //檢查下一層左子節點
        }
        heap[currentNode]=temp; //將值放入正確位置
    }
}
```
##### [原有問題]
   * 原本需要一個個呼叫Push()插入資料，每次插入最壞情況需$O(\log n)$。
   * 若要建立包含$n$個元素的堆積，總時間複雜度會高達 $O(n \log n)$，對於已知全部測資的初始建樹過程非常缺乏效率。
##### [優化部分]
   1. 不逐一觸發向上浮動，而是先將 $n$ 筆資料直接填入陣列。
   2. 從最後一個非葉節點往前遍歷至根節點，逐一執行向下沉降。
##### [結論]
   1. 時間複雜度降低為線性時間 $O(n)$。
   2. 因為底層節點多但沉降距離短，上層節點少沉降距離長，整體大幅減少了節點比較的總次數，顯著提升初始建樹效能。
#### (2) BST — 非遞迴版本的Insert和Get
``` c++
void InsertIterative(const pair<K,E>& e){
    TreeNode<K, E>* newNode=new TreeNode<K,E>(e); //預建新節點
    if(!root){
        root=newNode; // 如果樹為空則直接當根節點
        return;
    }
    TreeNode<K,E>* current=root;
    TreeNode<K,E>* parent=NULL; // 用來記錄current的父節點
    while(current){
        parent=current; //先記錄父節點
        if(e.first<current->data.first)
            current=current->left; // 較小則往左走
        else if(e.first > current->data.first)
            current=current->right; // 較大則往右走
        else{
            current->data.second=e.second; //若key已存在則更新資料
            delete newNode; //刪除多餘節點
            return;
        }
    }
    if(e.first<parent->data.first)
        parent->left=newNode; //接在父節點的左邊
    else
        parent->right=newNode; //接在父節點的右邊
}
pair<K,E>* GetIterative(const K& k)const{
    TreeNode<K,E>* current=root; //從root開始搜尋
    while(current){
        if(k<current->data.first)
            current=current->left; // key較小往左尋找
        else if(k>current->data.first)
            current=current->right; //較大往右尋找
        else
            return &(current->data); //找到目標 回傳該節點指標
    }
    return NULL; //仍未找到回傳空指標
}
```
##### [原有問題]
   * 原本寫法高度依賴遞迴，每一次往下走訪都會在記憶體中產生一層呼叫堆疊。
   * 當插入已排序資料導致樹極度不平衡（退化成斜曲樹）時，遞迴深度等於節點數 $n$，空間複雜度達 $O(n)$，極易引發堆疊溢位導致程式崩潰。
##### [優化部分]
   1. 放棄遞迴呼叫，改用while迴圈搭配暫存指標(current與parent)來進行樹狀結構的走訪與更新。
##### [結論]
   1. 空間複雜度完美降低為 $O(1)$。消除了堆疊溢位的風險，同時也省下了處理大量函式呼叫與返回的額外的時間成本。

