# 41343113 李展毅 41343144 廖峻德 

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
6. 一對多最短路徑:有負權 Bellman-Ford Algorithm (有向有權)
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

```
#### (2) ud and w()
``` c++

```
#### (3) d and uw()
``` c++

```
#### (4) d and w()
``` c++

```
## 效能分析
#### (1) Max/Min Heap 
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
#### (2) BST
   1. Insert(const pair<K, E>& e)
      * 時間複雜度： $O(\log n)$ //最壞情況 $O(n)$
      * 空間複雜度： $O(h)$ // $h$為樹高
   2. Delete(const K& k)
      * 時間複雜度： $O(\log n)$ //最壞情況 $O(n)$需要遍歷至目標節點並可能尋找後繼節點
      * 空間複雜度： $O(h)$ 
   3. getHeight(TreeNode<K, E> node)
      * 時間複雜度： $O(n)$ //必須走訪每一個節點
      * 空間複雜度： $O(h)$ //遞迴深度等於樹高
   4. Get(const K& k)
      * 時間複雜度： $O(\log n)$ //最壞情況 $O(n)$
      * 空間複雜度： $O(h)$ 

## 測試案例
### ud and uw (無向、無權重)
#### (1) 測資一：綜合連通圖 
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
#### (2) BST 輸出
<img width="412" height="337" alt="image" src="https://github.com/user-attachments/assets/3f3296c8-26d7-4bee-8d51-8a8f522cee1f" />

|n       |Height  |Ratio(Height/log2(n))|
|--------|--------|-------|
|100     |15      |2.25772|
|500     |18      |2.00763|
|1000    |24      |2.40824|
|2000    |26      |2.37101|
|3000    |24      |2.07779|
|4000    |24      |2.00572|
|5000    |27      |2.19732|
|6000    |31      |2.46997|
|7000    |30      |2.34868|
|8000    |28      |2.15953|
|9000    |31      |2.35998|
|10000   |34      |2.55875|

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

