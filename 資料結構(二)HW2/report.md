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

#### (1) ud and uw (無向、無權重)
1. InsertEdge(int u, int v)

   先進行邊界檢查，確認頂點編號合法，並透過 ExistsEdge() 確認邊尚未存在
   
   同步更新 Adjacency List 與 Adjacency Matrix 兩種資料結構
   
   將 v 加入 adj[u]、u 加入 adj[v]，並將matrix[u][v]與matrix[v][u]設為1，最後將邊數e加1

2. DeleteVertex(int v)

   先遍歷所有與v相鄰的節點，從Adjacency List裡面移除v，並對每條被刪除的邊將e減1
   
   接著將adj陣列中v之後的元素逐一往前搬移覆蓋，並對所有大於v的頂點編號減1維持節點ID正確
   
   Adjacency Matrix則直接刪除第v列與第v行，最後將頂點數 n 減1

3. BFS_List(int start) / BFS_Matrix(int start)

   使用queue實作BFS，初始將起點標記為已拜訪並推入佇列
   
   List版本每次取出佇列節點，依序走訪其Adjacency List中尚未拜訪的鄰居並推入佇列
   
   Matrix版本則為逐行掃描第u列，找出值為1且未拜訪的節點推入佇列，直到佇列為空為止

4. DFS_List(int start) / DFS_Matrix(int start)

   使用遞迴實作DFS，進入節點時即標記為已拜訪並輸出
   
   List版本依Adjacency List順序遞迴走訪未拜訪的鄰居
   
   Matrix版本則依序掃描矩陣該列，找到值為1且未拜訪的節點後遞迴進入，直到所有可達節點皆被拜訪
   
6. ST_List() / ST_Matrix()

   以DFS為建構生成樹，走訪中每發現未拜訪的節點，即輸出該樹邊(u,v)
   
   外層對所有頂點檢查，確保非連通圖每個皆能產生各自的生成樹

7. CC_List() / CC_Matrix()
   
   對所有頂點進行掃描，如果該頂點尚未被拜訪，則表示發現新的連通分量
   
   啟動一次完整的 DFS 走訪，將同一群組內所有節點一次拜訪完畢
   
   每次啟動新的DFS前，將群組計數器加一並輸出群組編號，達到區分各連通分量的效果

8. BCC_List() / BCC_Matrix()
   
   Tarjan演算法，維護每個節點的dfn(發現時間)與low(最早時間戳)，並以堆疊紀錄當前走訪過的邊
   
   當判斷某節點u為關節點時，將堆疊中屬於該雙連通分量的所有邊一次彈出並輸出
   
   結束後，堆疊中仍有剩餘的邊，代表屬於最後一個雙連通分量，一併輸出
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
#### (2) ud and w(無向、有權重:BFS、DFS、Kruskal、Prim、Dijkstra、Bellman-Ford、Floyd)
``` c++
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
```
#### (3) d and uw(有向、無權重:BFS、DFS、頂點活動網路)
``` c++
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
```
#### (4) d and w(有向、有權重:BFS、DFS、Dijkstra、Bellman-Ford、Floyd、邊活動網路)
``` c++
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
```
## 效能分析
#### (1) ud and uw（無向、無權重）
   1. InsertEdge(int u, int v)
      * 時間複雜度：$O(1)$（Adjacency List）/ $O(1)$（Adjacency Matrix）// 直接對陣列索引與 vector 尾端插入，為常數時間操作
      * 空間複雜度：$O(1)$ // 僅新增固定數量的邊資料，不需額外配置大型結構
   2. DeleteVertex(int v)
      * 時間複雜度：$O(n + e)$ // 需走訪所有相鄰節點刪除邊（$O(e)$），並對 adj 陣列做搬移與重新編號（$O(n)$）
      * 空間複雜度：$O(1)$ // 原地修改，不需額外空間
   3. BFS_List / BFS_Matrix
      * 時間複雜度：$O(n + e)$（List）/ $O(n^2)$（Matrix）// List 版本僅走訪實際存在的邊；Matrix 版本需掃描整列 $n$ 個格子
      * 空間複雜度：$O(n)$ // 需維護 visited 陣列與 queue
   4. DFS_List / DFS_Matrix
      * 時間複雜度：$O(n + e)$（List）/ $O(n^2)$（Matrix）// 與 BFS 理由相同
      * 空間複雜度：$O(n)$ // 需維護 visited 陣列與遞迴呼叫堆疊
   5. ST_List / ST_Matrix（生成樹）
      * 時間複雜度：$O(n + e)$（List）/ $O(n^2)$（Matrix）// 本質為一次完整 DFS 走訪
      * 空間複雜度：$O(n)$ // 同 DFS
   6. CC_List / CC_Matrix（連通元件）
      * 時間複雜度：$O(n + e)$（List）/ $O(n^2)$（Matrix）// 對全圖做完整 DFS，每個節點與邊各處理一次
      * 空間複雜度：$O(n)$ // 同 DFS
   7. BCC_List / BCC_Matrix（雙連通單元）
      * 時間複雜度：$O(n + e)$（List）/ $O(n^2)$（Matrix）// Tarjan 演算法在 DFS 框架下對每條邊處理常數次
      * 空間複雜度：$O(n + e)$ // 維護dfn、low、parent陣列以及邊堆疊
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

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | 0 1 7 2 6 8 3 5 9 4 | 0 1 7 2 6 8 3 5 9 4 |
| **DFS** | 0 1 2 3 4 5 6 7 8 9 | 0 1 2 3 4 5 6 7 8 9 |
| **Kruskal 最小生成樹** | (6,7) (2,8) (5,6) (0,1) (2,5) (2,3) (0,7) (3,4) (8,9)<br>**總權重: 54** | (6,7) (2,8) (5,6) (0,1) (2,5) (2,3) (0,7) (3,4) (8,9)<br>**總權重: 54** |
| **Prim 最小生成樹**<br>*(起點 0)* | (0,1) (0,7) (7,6) (6,5) (5,2) (2,8) (2,3) (3,4) (8,9)<br>**總權重: 54** | (0,1) (0,7) (7,6) (6,5) (5,2) (2,8) (2,3) (3,4) (8,9)<br>**總權重: 54** |
| **Dijkstra 最短路徑**<br>*(起點 0)* | 到 0: 0, 到 1: 4, 到 2: 16, 到 3: 23, 到 4: 22<br>到 5: 12, 到 6: 9, 到 7: 8, 到 8: 15, 到 9: 30 | 到 0: 0, 到 1: 4, 到 2: 16, 到 3: 23, 到 4: 22<br>到 5: 12, 到 6: 9, 到 7: 8, 到 8: 15, 到 9: 30 |
| **Bellman-Ford 最短路徑**<br>*(起點 0)* | 到 0: 0, 到 1: 4, 到 2: 16, 到 3: 23, 到 4: 22<br>到 5: 12, 到 6: 9, 到 7: 8, 到 8: 15, 到 9: 30 | 到 0: 0, 到 1: 4, 到 2: 16, 到 3: 23, 到 4: 22<br>到 5: 12, 到 6: 9, 到 7: 8, 到 8: 15, 到 9: 30 |
| **Floyd-Warshall**<br>*(任兩點最短路徑矩陣)* | `0  4  16 23 22 12 9  8  15 30`<br>`4  0  12 19 25 15 12 11 14 28`<br>`16 12 0  7  15 5  8  9  2  16`<br>`23 19 7  0  9  12 15 16 9  23`<br>`22 25 15 9  0  10 13 14 17 31`<br>`12 15 5  12 10 0  3  4  7  21`<br>`9  12 8  15 13 3  0  1  6  21`<br>`8  11 9  16 14 4  1  0  7  22`<br>`15 14 2  9  17 7  6  7  0  15`<br>`30 28 16 23 31 21 21 22 15 0` | `0  4  16 23 22 12 9  8  15 30`<br>`4  0  12 19 25 15 12 11 14 28`<br>`16 12 0  7  15 5  8  9  2  16`<br>`23 19 7  0  9  12 15 16 9  23`<br>`22 25 15 9  0  10 13 14 17 31`<br>`12 15 5  12 10 0  3  4  7  21`<br>`9  12 8  15 13 3  0  1  6  21`<br>`8  11 9  16 14 4  1  0  7  22`<br>`15 14 2  9  17 7  6  7  0  15`<br>`30 28 16 23 31 21 21 22 15 0` |
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

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | 0 1 4 2 3 5 9 7 6 8 | 0 1 4 2 3 5 9 7 6 8 |
| **DFS** | 0 1 2 3 4 5 6 7 8 9 | 0 1 2 3 4 5 6 7 8 9 |
| **Kruskal 最小生成樹** | (0,1) (1,2) (2,3) (3,4) (4,5) (5,6) (6,7) (7,8) (8,9)<br>**總權重: 126** | (0,1) (1,2) (2,3) (3,4) (4,5) (5,6) (6,7) (7,8) (8,9)<br>**總權重: 126** |
| **Prim 最小生成樹**<br>*(起點 0)* | (0,1) (1,2) (2,3) (3,4) (4,5) (5,6) (6,7) (7,8) (8,9)<br>**總權重: 126** | (0,1) (1,2) (2,3) (3,4) (4,5) (5,6) (6,7) (7,8) (8,9)<br>**總權重: 126** |
| **Dijkstra 最短路徑**<br>*(起點 0)* | 到 0: 0, 到 1: 10, 到 2: 21, 到 3: 33, 到 4: 25<br>到 5: 39, 到 6: 54, 到 7: 51, 到 8: 68, 到 9: 70 | 到 0: 0, 到 1: 10, 到 2: 21, 到 3: 33, 到 4: 25<br>到 5: 39, 到 6: 54, 到 7: 51, 到 8: 68, 到 9: 70 |
| **Bellman-Ford 最短路徑**<br>*(起點 0)* | 到 0: 0, 到 1: 10, 到 2: 21, 到 3: 33, 到 4: 25<br>到 5: 39, 到 6: 54, 到 7: 51, 到 8: 68, 到 9: 70 | 到 0: 0, 到 1: 10, 到 2: 21, 到 3: 33, 到 4: 25<br>到 5: 39, 到 6: 54, 到 7: 51, 到 8: 68, 到 9: 70 |
| **Floyd-Warshall**<br>*(任兩點最短路徑矩陣)* | `0  10 21 33 25 39 54 51 68 70`<br>`10 0  11 23 35 49 57 41 58 76`<br>`21 11 0  12 25 39 46 30 47 65`<br>`33 23 12 0  13 27 42 42 59 58`<br>`25 35 25 13 0  14 29 45 62 45`<br>`39 49 39 27 14 0  15 31 48 59`<br>`54 57 46 42 29 15 0  16 33 51`<br>`51 41 30 42 45 31 16 0  17 35`<br>`68 58 47 59 62 48 33 17 0  18`<br>`70 76 65 58 45 59 51 35 18 0` | `0  10 21 33 25 39 54 51 68 70`<br>`10 0  11 23 35 49 57 41 58 76`<br>`21 11 0  12 25 39 46 30 47 65`<br>`33 23 12 0  13 27 42 42 59 58`<br>`25 35 25 13 0  14 29 45 62 45`<br>`39 49 39 27 14 0  15 31 48 59`<br>`54 57 46 42 29 15 0  16 33 51`<br>`51 41 30 42 45 31 16 0  17 35`<br>`68 58 47 59 62 48 33 17 0  18`<br>`70 76 65 58 45 59 51 35 18 0` |
#### 測資三：負權(負迴圈)連通圖
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

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | 0 1 2 9 3 8 4 7 5 6 | 0 1 2 9 3 8 4 7 5 6 |
| **DFS** | 0 1 2 3 4 5 6 7 8 9 | 0 1 2 3 4 5 6 7 8 9 |
| **Kruskal 最小生成樹** | (4,5) (5,6) (8,9) (0,1) (3,4) (6,7) (1,2) (2,3) (7,8)<br>**總權重: 54** | (4,5) (5,6) (8,9) (0,1) (3,4) (6,7) (1,2) (2,3) (7,8)<br>**總權重: 54** |
| **Prim 最小生成樹**<br>*(起點 0)* | (0,1) (1,2) (2,3) (3,4) (4,5) (5,6) (6,7) (7,8) (8,9)<br>**總權重: 54** | (0,1) (1,2) (2,3) (3,4) (4,5) (5,6) (6,7) (7,8) (8,9)<br>**總權重: 54** |
| **Dijkstra 最短路徑**<br>*(起點 0)* | (未使用，因負迴圈會導致無窮迴圈卡死) | (未使用，因負迴圈會導致無窮迴圈卡死) |
| **Bellman-Ford 最短路徑**<br>*(起點 0)* | 圖中存在負迴圈 | 圖中存在負迴圈 |
| **Floyd-Warshall**<br>*(任兩點最短路徑矩陣)* | 圖中存在負迴圈 | 圖中存在負迴圈 |
### (3) d and uw (有向、無權重)
#### 測資一：標準有向無環圖測試
| 原圖 | AOV 拓撲排序 |
| :---: | :---: |
| <img width="3060" height="1859" alt="tc1_original" src="https://github.com/user-attachments/assets/20dc1cf1-1591-4708-bbaa-4edb89f8a7be" />|<img width="3060" height="1772" alt="tc1_standard_aov" src="https://github.com/user-attachments/assets/70c647d5-7718-4ca7-becc-04d1d4419ca1" />|
| **BFS 廣度優先搜尋** | **DFS 深度優先搜尋** |
| <img width="3060" height="1859" alt="tc1_standard_bfs" src="https://github.com/user-attachments/assets/dde0647f-53e1-4c36-9e3f-0598f049f6c4" /> | <img width="3060" height="1859" alt="tc1_standard_dfs" src="https://github.com/user-attachments/assets/340ad0d5-c862-4083-8515-26486e28e005" />|

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | `0 2 3 4 5 6 7 8 9` | `0 2 3 4 5 6 7 8 9` |
| **DFS** | `0 2 3 5 8 9 6 4 7` | `0 2 3 5 8 9 6 4 7` |
| **AOV (拓撲排序)** | `0 1 2 3 4 5 6 7 8 9` | `0 1 2 3 4 5 6 7 8 9` |
#### 測資二：多源圖測試
| 原圖 | AOV 拓撲排序 |
| :---: | :---: |
| <img width="3060" height="1859" alt="tc2_multisource_original" src="https://github.com/user-attachments/assets/5f2fc738-44b9-442e-9332-2faa338b0ef9" />| <img width="3060" height="1772" alt="tc2_multisource_aov" src="https://github.com/user-attachments/assets/df9d07b8-9598-4dad-a1e3-4e04da63869a" />|
| **BFS 廣度優先搜尋** | **DFS 深度優先搜尋** |
| <img width="3060" height="1859" alt="tc2_multisource_bfs" src="https://github.com/user-attachments/assets/e02daca3-cdc9-4ba5-8219-a6e297ed5cab" />|<img width="3060" height="1859" alt="tc2_multisource_dfs" src="https://github.com/user-attachments/assets/c5966b52-c556-48a7-914b-cf1eaed59f70" />|

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | `0 3 5 7 9` | `0 3 5 7 9` |
| **DFS** | `0 3 5 7 9` | `0 3 5 7 9` |
| **AOV (拓撲排序)** | `0 1 2 3 4 5 6 7 8 9` | `0 1 2 3 4 5 6 7 8 9` |
#### 測資三：交錯相依圖測試
| 原圖 | AOV 拓撲排序 |
| :---: | :---: |
| <img width="3060" height="1859" alt="tc3_interdependent_original" src="https://github.com/user-attachments/assets/e4d75c01-a89b-47aa-95c3-feb9f69b4f4a" />| <img width="3060" height="1849" alt="tc3_interdependent_aov" src="https://github.com/user-attachments/assets/4fd4162c-256d-41ab-bd45-341bfd8ccf34" />|
| **BFS 廣度優先搜尋** | **DFS 深度優先搜尋** |
| <img width="3060" height="1859" alt="tc3_interdependent_bfs" src="https://github.com/user-attachments/assets/3780cc9b-29de-4fe0-a99a-6e51da857ed6" />| <img width="3060" height="1859" alt="tc3_interdependent_dfs" src="https://github.com/user-attachments/assets/6470c9c2-64b1-465e-92ea-b71f9386e3d9" />|

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | `0 8 5 2 9 1 4 3 6 7` | `0 5 8 1 4 2 9 7 3 6` |
| **DFS** | `0 8 2 3 7 9 6 5 1 4` | `0 5 1 7 4 8 2 3 9 6` |
| **AOV (拓撲排序)** | `0 8 5 2 9 1 4 3 6 7` | `0 5 8 1 4 2 9 3 6 7` |
### (4) d and w (有向、有權重)
#### 測資一：標準有向無環圖測試
| 原圖 | **BFS 廣度優先搜尋** | **DFS 深度優先搜尋** |
| :---: | :---: | :---: |
| <img width="3000" height="1800" alt="tc1_aoe_multiple_original" src="https://github.com/user-attachments/assets/425388e7-7a84-4f41-88c5-1e5cc689a889" />|<img width="3000" height="1800" alt="tc1_aoe_multiple_bfs" src="https://github.com/user-attachments/assets/3f95f2d7-0226-41e8-b902-477df92b1fc2" />|<img width="3000" height="1800" alt="tc1_aoe_multiple_dfs" src="https://github.com/user-attachments/assets/aab21ad1-e03e-4002-95d5-87f56295a1bb" />|

|Dijkstra 最短路徑| Bellman-Ford 最短路徑 | Floyd-Warshall 全點對路徑 |
| :---: | :---: | :---: |
|<img width="3000" height="1800" alt="tc1_aoe_multiple_dijkstra" src="https://github.com/user-attachments/assets/9990e0f3-2e04-4e01-a6a8-1ad87105978a" />|<img width="3000" height="1800" alt="tc1_aoe_multiple_bellman" src="https://github.com/user-attachments/assets/5076b26f-9360-4d24-86f6-9e272b114161" />|<img width="3000" height="1800" alt="tc1_aoe_multiple_floyd" src="https://github.com/user-attachments/assets/ab7e7edf-2406-43ab-904a-997396ee1c63" />|

|AOE 關鍵路徑|
| :---: |
|<img width="3000" height="1800" alt="tc1_aoe_multiple_aoe" src="https://github.com/user-attachments/assets/539eeac3-13fd-4359-8997-2e9c1e8848cd" />|

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | `0 1 2 3 4 5 6 7 8 9` | `0 1 2 3 4 5 6 7 8 9` |
| **DFS** | `0 1 4 6 8 9 7 2 3 5` | `0 1 4 6 8 9 7 2 3 5` |
| **Dijkstra 最短路徑**<br>*(起點 0)* | 到 0: 0, 到 1: 3, 到 2: 2, 到 3: 5, 到 4: 7<br>到 5: 7, 到 6: 10, 到 7: 12, 到 8: 14, 到 9: 17 | 到 0: 0, 到 1: 3, 到 2: 2, 到 3: 5, 到 4: 7<br>到 5: 7, 到 6: 10, 到 7: 12, 到 8: 14, 到 9: 17 |
| **Bellman-Ford 最短路徑**<br>*(起點 0)* | 到 0: 0, 到 1: 3, 到 2: 2, 到 3: 5, 到 4: 7<br>到 5: 7, 到 6: 10, 到 7: 12, 到 8: 14, 到 9: 17 | 到 0: 0, 到 1: 3, 到 2: 2, 到 3: 5, 到 4: 7<br>到 5: 7, 到 6: 10, 到 7: 12, 到 8: 14, 到 9: 17 |
| **Floyd-Warshall**<br>*(任兩點最短路徑矩陣)* | `0  3  2  5  7  7  10 12 14 17`<br>`X  0  X  X  4  X  7  9  11 14`<br>`X  X  0  X  6  X  9  11 13 16`<br>`X  X  X  0  X  2  6  X  10 13`<br>`X  X  X  X  0  X  3  5  7  10`<br>`X  X  X  X  X  0  4  X  8  11`<br>`X  X  X  X  X  X  0  X  4  7`<br>`X  X  X  X  X  X  X  0  2  5`<br>`X  X  X  X  X  X  X  X  0  3`<br>`X  X  X  X  X  X  X  X  X  0` | `0  3  2  5  7  7  10 12 14 17`<br>`X  0  X  X  4  X  7  9  11 14`<br>`X  X  0  X  6  X  9  11 13 16`<br>`X  X  X  0  X  2  6  X  10 13`<br>`X  X  X  X  0  X  3  5  7  10`<br>`X  X  X  X  X  0  4  X  8  11`<br>`X  X  X  X  X  X  0  X  4  7`<br>`X  X  X  X  X  X  X  0  2  5`<br>`X  X  X  X  X  X  X  X  0  3`<br>`X  X  X  X  X  X  X  X  X  0` |
| **AOE 關鍵路徑** | 關鍵活動: `(0,2) (0,3) (2,4) (3,5) (4,6) (4,7) (5,6) (6,8) (7,8) (8,9)`<br>**最短專案完成時間: 18** | 關鍵活動: `(0,2) (0,3) (2,4) (3,5) (4,6) (4,7) (5,6) (6,8) (7,8) (8,9)`<br>**最短專案完成時間: 18** |

#### 測資二：標準有向無環圖測試
| 原圖 | **BFS 廣度優先搜尋** | **DFS 深度優先搜尋** |
| :---: | :---: | :---: |
| <img width="3000" height="1793" alt="tc2_aoe_contrast_original" src="https://github.com/user-attachments/assets/587b61b2-db37-4dca-9826-f86e3c6f3db3" />|<img width="3000" height="1793" alt="tc2_aoe_contrast_bfs" src="https://github.com/user-attachments/assets/d86cfd22-3c33-4330-9f9a-abdca0e9a3f3" />|<img width="3000" height="1793" alt="tc2_aoe_contrast_dfs" src="https://github.com/user-attachments/assets/c09c7e84-bfc5-44ff-8521-7fb930345d93" />|

|Dijkstra 最短路徑| Bellman-Ford 最短路徑 | Floyd-Warshall 全點對路徑 |
| :---: | :---: | :---: |
|<img width="3000" height="1793" alt="tc2_aoe_contrast_dijkstra" src="https://github.com/user-attachments/assets/1a6eee11-1c88-4221-883c-91c7f9cda507" />|<img width="3000" height="1800" alt="tc2_aoe_contrast_bellman" src="https://github.com/user-attachments/assets/3f36225c-2a51-4a21-bfbc-8b82d53f47ec" />|<img width="3000" height="1800" alt="tc2_aoe_contrast_floyd" src="https://github.com/user-attachments/assets/d88722e1-73ec-4771-b8e8-b09ac7ea6c53" />|

|AOE 關鍵路徑|
| :---: |
|<img width="3000" height="1793" alt="tc2_aoe_contrast_aoe" src="https://github.com/user-attachments/assets/4e085f9e-6f85-4d11-8414-cf07fb997890" />|

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | `0 1 2 4 3 5 6 8 9 7` | `0 1 2 3 4 6 5 7 8 9` |
| **DFS** | `0 1 4 5 8 9 3 6 7 2` | `0 1 3 4 5 8 9 6 7 2` |
| **Dijkstra 最短路徑**<br>*(起點 0)* | 到 0: 0, 到 1: 10, 到 2: 2, 到 3: 5, 到 4: 6<br>到 5: 12, 到 6: 7, 到 7: 10, 到 8: 12, 到 9: 13 | 到 0: 0, 到 1: 10, 到 2: 2, 到 3: 5, 到 4: 6<br>到 5: 12, 到 6: 7, 到 7: 10, 到 8: 12, 到 9: 13 |
| **Bellman-Ford 最短路徑**<br>*(起點 0)* | 到 0: 0, 到 1: 10, 到 2: 2, 到 3: 5, 到 4: 6<br>到 5: 12, 到 6: 7, 到 7: 10, 到 8: 12, 到 9: 13 | 到 0: 0, 到 1: 10, 到 2: 2, 到 3: 5, 到 4: 6<br>到 5: 12, 到 6: 7, 到 7: 10, 到 8: 12, 到 9: 13 |
| **Floyd-Warshall**<br>*(任兩點最短路徑矩陣)* | `0  10 2  5  6  12 7  10 12 13`<br>`X  0  X  1  2  8  3  6  8  9`<br>`X  X  0  3  4  10 5  8  10 11`<br>`X  X  X  0  1  7  2  5  7  8`<br>`X  X  X  X  0  6  X  X  7  8`<br>`X  X  X  X  X  0  X  X  1  2`<br>`X  X  X  X  X  X  0  3  5  6`<br>`X  X  X  X  X  X  X  0  2  3`<br>`X  X  X  X  X  X  X  X  0  1`<br>`X  X  X  X  X  X  X  X  X  0` | `0  10 2  5  6  12 7  10 12 13`<br>`X  0  X  1  2  8  3  6  8  9`<br>`X  X  0  3  4  10 5  8  10 11`<br>`X  X  X  0  1  7  2  5  7  8`<br>`X  X  X  X  0  6  X  X  7  8`<br>`X  X  X  X  X  0  X  X  1  2`<br>`X  X  X  X  X  X  0  3  5  6`<br>`X  X  X  X  X  X  X  0  2  3`<br>`X  X  X  X  X  X  X  X  0  1`<br>`X  X  X  X  X  X  X  X  X  0` |
| **AOE 關鍵路徑** | 關鍵活動: `(0,1) (1,4) (4,5) (5,9)`<br>**最短專案完成時間: 34** | 關鍵活動: `(0,1) (1,4) (4,5) (5,9)`<br>**最短專案完成時間: 34** |
#### 測資三：標準有向無環圖測試
| 原圖 | **BFS 廣度優先搜尋** | **DFS 深度優先搜尋** |
| :---: | :---: | :---: |
| <img width="3000" height="1800" alt="tc3_aoe_negative_cycle_original" src="https://github.com/user-attachments/assets/55d931de-9906-4bc9-9793-e7ddde6fdd2c" />|<img width="3000" height="1800" alt="tc3_aoe_negative_cycle_bfs" src="https://github.com/user-attachments/assets/4c1b5a95-19c7-4f3b-bc03-2db095f84172" />|<img width="3000" height="1800" alt="tc3_aoe_negative_cycle_dfs" src="https://github.com/user-attachments/assets/fa9f1f5a-ddb2-4f2d-b21e-f5b9e57764d7" />|

|Dijkstra 最短路徑| Bellman-Ford 最短路徑 | Floyd-Warshall 全點對路徑 |
| :---: | :---: | :---: |
|<img width="3000" height="1800" alt="tc3_aoe_negative_cycle_dijkstra" src="https://github.com/user-attachments/assets/5ec44448-047e-4957-bb29-5cb7dc34b463" />|<img width="3000" height="1800" alt="tc3_aoe_negative_cycle_bellman" src="https://github.com/user-attachments/assets/5db1cfd6-1e88-41a7-84db-b741470cfa73" />|<img width="3000" height="1800" alt="tc3_aoe_negative_cycle_floyd" src="https://github.com/user-attachments/assets/993d1a26-26f7-4ebb-8a32-0050e3c33019" />|
|Dijkstra 陷入無窮迴圈|Bellman-Ford dp到最後檢測出負環|Floyd-Warshall dp到最後檢測出負環|

|AOE 關鍵路徑|
| :---: |
|<img width="3000" height="1800" alt="tc3_aoe_negative_cycle_aoe" src="https://github.com/user-attachments/assets/bedc3374-806a-421e-acf9-b0d04ed11c70" />|
|AOE 陷入無窮迴圈|

| 演算法 | Adjacency List 實作結果 | Adjacency Matrix 實作結果 |
| :--- | :--- | :--- |
| **BFS** | `0 1 7 2 8 3 9 4 5 6` | `0 1 7 2 8 3 9 4 5 6` |
| **DFS** | `0 1 2 3 4 5 6 7 8 9` | `0 1 2 3 4 5 6 7 8 9` |
| **Dijkstra 最短路徑**<br>*(起點 0)* | (未使用，因負迴圈會導致無窮迴圈卡死) | (未使用，因負迴圈會導致無窮迴圈卡死) |
| **Bellman-Ford 最短路徑**<br>*(起點 0)* | 圖中存在負迴圈 | 圖中存在負迴圈 |
| **Floyd-Warshall**<br>*(任兩點最短路徑矩陣)* | 圖中存在負迴圈 | 圖中存在負迴圈 |
| **AOE 關鍵路徑** | 圖中存在迴圈，無法計算關鍵路徑 | 圖中存在迴圈，無法計算關鍵路徑 |
### 結論
1. Heap 的效率在 $n=10^6$ 的測試下，MaxHeap與MinHeap的執行時間幾乎一致，證明演算法對稱性與複雜度完全相同，均符合 $O(\log n)$。

2. 測試輸出結果顯示 $height / \log_2 n$ 的比值落在2.0到2.6之間，符合題目預期。
## 申論及開發報告
### 程式分析
對當前寫的程式做優點以及資料結構&演算法分析，還有程式需注意的要點
#### (1) ud and uw（無向、無權重）
##### [使用資料結構與演算法]
* 資料結構：Adjacency List（`vector<int>[]`）與 Adjacency Matrix（`vector<vector<bool>>`）雙軌並行維護
* 演算法：
  1. BFS：利用 queue 實現逐層走訪
  2. DFS：利用遞迴實現深度優先走訪，並作為生成樹、連通元件的基礎
  3. Tarjan 演算法：利用 dfn 與 low 值配合邊堆疊，識別雙連通單元
##### [須注意的事]
1. 每次 InsertEdge / DeleteEdge 皆同步更新 List 與 Matrix，確保兩者一致；`ExistsEdge()` 中加入驗證機制，若兩者不一致會拋出例外。
2. DeleteVertex 需對 adj 陣列做搬移並對所有大於 v 的編號減一，需注意順序避免覆蓋錯誤。
3. BCC 中的關節點判斷條件分兩種情況：根節點的子樹數大於 1，以及非根節點的子節點 low 值大於等於自身 dfn，需分別處理。
#### (2) BST
##### [使用資料結構與演算法]
* 資料結構： 指標鏈結的二元搜尋樹
* 演算法： 遞迴樹狀走訪
##### [須注意的事]
1. 迴深度過大會引發堆疊溢位，這點會在優化時處理
### 程式改進
對目前的程式片段做優化處理
#### (1) ud and uw — 迭代式 DFS 取代遞迴
```c++
void DFS_List_Iterative(int start){
    cout <<"[List] DFS : ";
    vector visited(n,0);
    stack st;
    st.push(start);
    while(!st.empty()){
        int u=st.top();
        st.pop();
        if(visited[u])
            continue;
        visited[u]=1;
        cout<<u<<" ";
        for(int i=adj[u].size()-1;i>=0;--i) {
            if(!visited[adj[u][i]])
                st.push(adj[u][i]);
        }
    }
    cout << '\n';
}
```
##### [原有問題]
   * 原本的遞迴DFS在圖的規模極大或深度極深時，可能因系統呼叫堆疊空間有限而導致堆疊溢出
   * 每層遞迴皆需保存函式的活動紀錄:return address、區域變數等，對記憶體造成較大壓力

##### [優化部分]
   1. 取代遞迴堆疊，手動控制走訪狀態
   2. 反向推入鄰居節點，確保彈出順序與遞迴版本順序一致

##### [結論]
   1. 時間複雜度維持 $O(n+e)$，結果相同
   2. 消除了遞迴深度的風險，在處理大型稠密圖或鏈狀圖時提升穩定性
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

