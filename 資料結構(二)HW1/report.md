# 41343113

資料結構(二)作業一

## 解題說明
### 問題描述
1. 實作MinPQ(ADT)，接著實作繼承於MinPQ的MinHeap類別，時間複雜度應該與 MaxHeap 對應函式的時間複雜度相同。

2. (a)撰寫初始為空的二元搜尋樹，進行 $n$ 次隨機插入，使用均勻亂數產生器取得插入值，測量樹高度，將此高度除以 $\log_2 n$針對
   
   $n = 100, 500, 1000, 2000, 3000, \cdots, 10,000$ 執行操作。
   
   將比值 $height/\log_2 n$ 繪製成 $n$ 的函數圖，該比值應該大約為2。
   
   (b)實做一個函式，從二元搜尋樹中刪除鍵值為 $k$ 的資料對 (pair)，問此函式的時間複雜度為何？
### 解題策略

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


## 程式實作
以下為主要程式碼：
#### (1) Max/Min Heap
``` c++
#include<iostream>
#include<chrono> //用來測執行時間的
#include<cstdlib> 
using namespace std;
template<class T>
class MaxPQ{ //MAXPQ的ADT
public:
    virtual ~MaxPQ(){}
    virtual bool IsEmpty() const=0;
    virtual const T& Top() const=0;
    virtual void Push(const T&)=0;
    virtual void Pop()=0;
};
template<class T>
class MinPQ{ //MINPQ的ADT
public:
    virtual ~MinPQ(){}
    virtual bool IsEmpty() const=0;
    virtual const T& Top() const=0;
    virtual void Push(const T&)=0;
    virtual void Pop() = 0;
};
template<class T>
class MaxHeap:public MaxPQ<T>{
private:
    T* heap;
    int heapSize; //大小
    int capacity; //容量
    // 動態記憶體配置
    void ChangeSize1D(T*& a,const int oldSize,const int newSize) {
        T* temp=new T[newSize];
        for(int i=0;i<oldSize;++i)
            temp[i]=a[i];
        delete[] a;
        a=temp;
    }
public:
    MaxHeap(int initCapacity=10):heapSize(0),capacity(initCapacity){ //沒參數預設Cap為10
        heap=new T[capacity + 1]; //1 based
    }
    ~MaxHeap(){ 
        delete[] heap; 
    }
    bool IsEmpty()const override{ //override做改寫基礎類別函式
         return heapSize==0; 
    }
    const T& Top() const override{
        if(IsEmpty()) 
            throw "Heap is empty.";
        return heap[1]; //root 即是 top
    }
    void Push(const T& e)override{
        if(heapSize==capacity){ //如果大小和容量一樣大就做擴展
            ChangeSize1D(heap,capacity+1,2*capacity+1);//新容量為*2+1
            capacity*=2;
        }
        int currentNode=++heapSize; //大小+1 紀錄新元素的位置
        while(currentNode!=1&&heap[currentNode/2]<e){ //如果新元素位置不在root且父節點比新元素小
            heap[currentNode]=heap[currentNode/2]; //叫父節點滾下去
            currentNode/=2; //新元素位置上位
        }
        heap[currentNode]=e; //將值放入
    }
    void Pop() override{ //pop root相當於把最後一個元素放到root開始重排
        if(IsEmpty())
            throw "Heap is empty.";
        T lastE=heap[heapSize--]; //最後一個元素
        if(heapSize==0)
            return;
        int currentNode=1; //當前節點=root(1)
        int child=2; //子節點ID=2
        while(child<=heapSize){ //如果左邊子節點存在
            if(child+1<=heapSize&&heap[child]<heap[child+1]) //如果右節點存在 且 右邊值更大
                child++; //設為右邊
            if(lastE>=heap[child]) //如果最後元素>=最大子節點
                break; //找到最後元素重排的位置
            heap[currentNode]=heap[child]; //子節點上位
            currentNode=child; //最後元素位置設為子節點的位置
            child*=2; //下一個左子節點的位置
        }
        heap[currentNode]=lastE; //將值放入
    }
    void Print() const{ //額外新增印出陣列的函式
        if(IsEmpty()){
            cout<<"Heap is empty.\n";
            return;
        }
        for(int i=1;i<=heapSize;++i)
            cout<<(i==1?"":" ")<<heap[i];
        cout<<"\n";
    }
};
template<class T>
class MinHeap:public MinPQ<T>{
private:
    T* heap;
    int heapSize;
    int capacity;
    //動態記憶體配置
    void ChangeSize1D(T*& a,const int oldSize,const int newSize){
        T* temp=new T[newSize];
        for(int i=0;i<oldSize;++i)
            temp[i]=a[i];
        delete[] a;
        a=temp;
    }
public:
    MinHeap(int initCapacity=10):heapSize(0),capacity(initCapacity){
        heap=new T[capacity+1];
    }
    ~MinHeap(){
        delete[] heap;
    }
    bool IsEmpty() const override{
        return heapSize==0;
    }
    const T& Top() const override{
        if (IsEmpty())
            throw "Heap is empty.";
        return heap[1];
    }
    void Push(const T& e) override{
        if(heapSize==capacity){
            ChangeSize1D(heap,capacity+1,2*capacity+1);
            capacity*=2;
        }
        int currentNode=++heapSize;
        while(currentNode!=1&& heap[currentNode/2]>e){ //如果新元素位置不在root且父節點比新元素大
            heap[currentNode]=heap[currentNode/2];
            currentNode/=2;
        }
        heap[currentNode]=e;
    }
    void Pop() override{
        if(IsEmpty())
            throw "Heap is empty.";
        T lastE=heap[heapSize--];
        if(heapSize==0)
            return;
        int currentNode=1;
        int child=2;
        while(child<=heapSize){
            if(child+1<=heapSize&&heap[child]>heap[child+1]) //如果右節點存在 且 右邊值更小
                child++;
            if(lastE<=heap[child]) //如果最後元素>=最小子節點
                break;
            heap[currentNode]=heap[child];
            currentNode=child;
            child*=2;
        }
        heap[currentNode]=lastE;
    }
    void Print() const{ //額外新增印出陣列的函式
        if(IsEmpty()){
            cout<<"Heap is empty.\n";
            return;
        }
        for(int i=1;i<=heapSize;++i)
            cout<<(i==1?"":" ")<<heap[i];
        cout<<"\n";
    }
};
int main(){
    MaxHeap<int> maxH;
    MinHeap<int> minH;
    int n;
    cout<<"輸入時間測試的隨機測資數n:";
    cin>>n;
    auto startMax=chrono::high_resolution_clock::now(); //取得MaxHeap執行開始時間
    for(int i=0;i<n;++i) 
        maxH.Push(rand()%n); //rand()%n隨機測資
    for(int i=0;i<n;++i)
        maxH.Pop();
    auto endMax=chrono::high_resolution_clock::now(); //取得MaxHeap執行結束時間
    chrono::duration<double> maxTime=endMax-startMax; //時間差計算
    auto startMin=chrono::high_resolution_clock::now(); //取得MinHeap執行開始時間
    for(int i=0;i<n;++i)
        minH.Push(rand()%n); 
    for(int i=0;i<n;++i)
        minH.Pop();
    auto endMin=chrono::high_resolution_clock::now(); //取得MinHeap執行結束時間
    chrono::duration<double> minTime=endMin-startMin;
    cout<<"MaxHeap 執行"<<n<<"次 Push && Pop耗費:"<< maxTime.count()<<"秒\n";
    cout<<"MinHeap 執行"<<n<<"次 Push && Pop耗費:"<< minTime.count()<<"秒\n";
    
    cout<<"手動輸入資料建樹\n";
    MaxHeap<int> maxh;
    MinHeap<int> minh;
    int temp;
    cout<<"請輸入要建樹的資料量 n: ";
    cin>>n;
    cout<<"輸入n個資料:";
    for(int i=0;i<n;++i){
        cin>>temp;
        maxh.Push(temp); 
        minh.Push(temp);
    }
    cout<<"Push()n次建樹的MaxHeap MinHeap輸出\n";
    maxh.Print();
    minh.Print();
    cout<<"Pop()執行後的MaxHeap MinHeap樹狀態輸出\n";
    maxh.Pop();
    minh.Pop();
    maxh.Print();
    minh.Print();
    cout<<"IsEmpty()測試輸出\n";
    cout<<maxh.IsEmpty()<<'\n';
    cout<<minh.IsEmpty()<<'\n';
    return 0;
}
```
#### (2) BST
``` c++
#include<iostream>
#include<cmath> //需要log2
#include<cstdlib> //隨機數值
#include<ctime> //取得當前系統時間
#include<utility> //pair<>
using namespace std;
template<class K,class E>
class Dictionary{ //BST的ADT 
public:
    virtual ~Dictionary(){} //解構
    virtual bool IsEmpty() const=0; //是否為空
    virtual pair<K, E>* Get(const K&) const=0; //取得Key對應的指標
    virtual void Insert(const pair<K, E>&)=0; //插入Key和值
    virtual void Delete(const K&)=0; //刪除指定Key值的節點
};
template<class K, class E>
class TreeNode{
public:
    TreeNode* left; //左子樹指標
    TreeNode* right; //右子樹指標
    pair<K, E> data; //鍵值對資料
    TreeNode(pair<K, E> e){ //建構子初始化
        this->data.first=e.first; 
        this->data.second=e.second; 
        this->left=NULL; 
        this->right=NULL; 
    }
};
template<class K, class E>
class BSTDictionary:public Dictionary<K, E>{
private:
    TreeNode<K, E>* root; //字典樹根節點
    TreeNode<K, E>* insert(TreeNode<K, E>* node,const pair<K, E>& e){ //遞迴插入
        if(!node)
            return new TreeNode<K, E>(e); //到底則建立新節點
        if(e.first<node->data.first)
            node->left=insert(node->left,e); //小於往左
        else if(e.first > node->data.first)
            node->right=insert(node->right,e); //大於往右
        else node->data.second=e.second; //若鍵值重複則更新元素內容
        return node; //回傳當前節點
    }
    int getHeight(TreeNode<K, E>* node) const{ //遞迴取得高度 part a 要求
        if(!node)
            return 0; //遞迴結束
        return 1+max(getHeight(node->left), getHeight(node->right)); //遞迴取最大子樹高+1
    }
    TreeNode<K, E>* findMin(TreeNode<K, E>* node) { //遞迴找最小值
        if (node->left == NULL) return node; //左邊為空代表是最小值
        else return findMin(node->left); //否則繼續往左遞迴
    }
    TreeNode<K, E>* remove(TreeNode<K, E>* node,const K& k){ 
        if(!node) 
            return node; //沒找到
        if(k<node->data.first)
            node->left=remove(node->left,k); //遞迴左子樹
        else if(k>node->data.first) 
            node->right=remove(node->right,k); //遞迴右子樹
        else{
            if(!node->left){ //若無左子樹
                TreeNode<K, E>* temp=node->right; //暫存右子樹
                delete node; //刪除節點
                return temp; //回傳右子樹接上
            }else if(!node->right){ //若無右子樹
                TreeNode<K, E>* temp=node->left; //暫存左子樹
                delete node; //刪除節點
                return temp; //回傳左子樹接上
            }
            TreeNode<K, E>* temp=findMin(node->right); //找右子樹中序後繼節點
            node->data=temp->data; //複製後繼節點資料至當前節點
            node->right=remove(node->right, temp->data.first); //遞迴刪除該後繼節點
        }
        return node; //回傳更新後的節點
    }
    pair<K, E>* get(TreeNode<K, E>* node,const K& k)const{ //遞迴查詢特定鍵值
        if(!node)
            return NULL; //找不到回傳空指標
        if(k<node->data.first)
            return get(node->left,k); //遞迴左子樹
        if(k>node->data.first)
            return get(node->right, k); //遞迴右子樹
        return &(node->data); //相等回傳指標
    }
    void destroy(TreeNode<K, E>* node){ //遞迴釋放記憶體
        if(!node)
            return; //空則返回
        destroy(node->left); //清空左
        destroy(node->right); //清空右
        delete node; //刪除自身
    }
public:
    BSTDictionary():root(NULL){} //初始化空字典
    ~BSTDictionary(){destroy(root);} //解構時清空整棵樹
    bool IsEmpty() const override{return root==NULL;} //實作IsEmpty
    pair<K, E>* Get(const K& k) const override{return get(root,k);} //實作Get
    void Insert(const pair<K, E>& e) override{root=insert(root,e);} //實作Insert
    void Delete(const K& k) override{root=remove(root,k);} //實作Delete
    int getHeight() const{return getHeight(root);} //公開高度介面(實驗用)
};
int main(){
    srand(time(0)); //設定亂數種子
    int ns[]={100,500,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000}; //測資陣列
    cout<<"n\tHeight\tRatio(Height/log2(n))\n"; //輸出col名 
    for(int n:ns){ //遍歷每個n
        BSTDictionary<int, int> dict; //建立字典樹
        for(int i=0;i<n;++i) { //執行n次插入
            int rnd=rand(); //產生亂數
            dict.Insert(make_pair(rnd, rnd)); //使用pair插入鍵與值
        }
        int height=dict.getHeight(); //取得樹高
        double ratio=height/log2(n); //計算比例
        cout<<n<<"\t"<<height<<"\t"<<ratio<<"\n"; //輸出結果
    }
    return 0;
}
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


### 測試案例

#### (1) Max/Min Heap
<table>
  <tr>
    <td align="center"><b>1.</b><br><img src="https://github.com/user-attachments/assets/224d2914-55cf-46cb-96b5-5c2b69290dc2" width="250"></td>
    <td align="center"><b>2.</b><br><img src="https://github.com/user-attachments/assets/e6f8a59d-4017-4925-8d88-ff265cc7a817" width="250"></td>
    <td align="center"><b>3.</b><br><img src="https://github.com/user-attachments/assets/c4fb585e-3354-402e-8b75-00f9c7a23490" width="250"></td>
  </tr>
</table>

### 輸入
| 測試項目 | 時間測試隨機測資數 n | 手動建樹資料量 n | 輸入 n 個資料 |
|-------------|--------------------------|--------------------|-------------------|
| 1: n個隨機資料時間測試&&基礎建樹測試 | 1000000 | 5 | 20 15 30 5 10 |
| 2: n個隨機資料時間測試&&遞增數列測試 | 2000000 | 6 | 10 20 30 40 50 60 |
| 3: n個隨機資料時間測試&&遞減數列測試 | 3000000 | 6 | 60 50 40 30 20 10 |

### 預期輸出
| 測試項目 | 執行n次 Push && Pop耗費 | Push()n次建樹的輸出 | Pop()執行後的樹狀態輸出 | IsEmpty()測試輸出 |
|-------------|-----------------------------|-------------------------|-----------------------------|-------------------|
| 1: n個隨機資料時間測試&&基礎建樹測試(MaxHeap) | ~0.2秒 | 30 15 20 5 10 | 20 15 10 5 | 0 |
| 1: n個隨機資料時間測試&&基礎建樹測試(MinHeap) | ~0.2秒 | 5 10 30 20 15 | 10 15 30 20 | 0 |
| 2: n個隨機資料時間測試&&遞增數列測試(MaxHeap) | ~0.45秒 | 60 40 50 10 30 20 | 50 40 20 10 30 | 0 |
| 2: n個隨機資料時間測試&&遞增數列測試(MinHeap) | ~0.45秒 | 10 20 30 40 50 60 | 20 40 30 60 50 | 0 |
| 3: n個隨機資料時間測試&&遞減數列測試(MaxHeap) | ~0.7秒 | 60 50 40 30 20 10 | 50 30 40 10 20 | 0 |
| 3: n個隨機資料時間測試&&遞減數列測試(MinHeap) | ~0.7秒 | 10 30 20 60 40 50 | 20 30 50 60 40 | 0 |

### 實際輸出
| 測試項目 | 執行n次 Push && Pop耗費 | Push()n次建樹的輸出 | Pop()執行後的樹狀態輸出 | IsEmpty()測試輸出 |
|-------------|-----------------------------|-------------------------|-----------------------------|-------------------|
| 1: n個隨機資料時間測試&&基礎建樹測試(MaxHeap) | 0.217252秒 | 30 15 20 5 10 | 20 15 10 5 | 0 |
| 1: n個隨機資料時間測試&&基礎建樹測試(MinHeap) | 0.199989秒 | 5 10 30 20 15 | 10 15 30 20 | 0 |
| 2: n個隨機資料時間測試&&遞增數列測試(MaxHeap) | 0.452087秒 | 60 40 50 10 30 20 | 50 40 20 10 30 | 0 |
| 2: n個隨機資料時間測試&&遞增數列測試(MinHeap) | 0.499431秒 | 10 20 30 40 50 60 | 20 40 30 60 50 | 0 |
| 3: n個隨機資料時間測試&&遞減數列測試(MaxHeap) | 0.713117秒 | 60 50 40 30 20 10 | 50 30 40 10 20 | 0 |
| 3: n個隨機資料時間測試&&遞減數列測試(MinHeap) | 0.715542秒 | 10 30 20 60 40 50 | 20 30 50 60 40 | 0 |
#### (2) BST


### 結論
  
## 申論及開發報告
### 程式分析
對當前寫的程式做優點以及資料結構&演算法分析，還有程式需注意的要點
#### (1) Ackermann 函數 — 遞迴
##### [選擇遞迴的原因]
1. 在本程式中，使用遞迴計算 Ackermann 函數主要原因如下：
   |函數定義|條件|
   |:---:|:---:|
   | $A(m, n) = n + 1$|**$m=0$**|
   |$A(m,n)=A(m-1,1)$|**$n=0$**|
   |$A(m,n)=A(m-1,A(m,n-1))$|**$otherwise$**|
   * 遞迴實作可直接對應數學公式，使邏輯一目了然，有利於團隊理解演算法意圖與驗證正確性。
2. 容易理解與實現

   遞迴避免手動管理堆疊或額外變數，每個函式呼叫即代表子問題可讀性高
   
   對程式競賽而言，程式碼短小且直觀，減少出錯機率，對專案協作，容易讓團隊快速理解與維護。
4. 遞迴語意清楚
   
   每次遞迴呼叫代表「子問題求解」，返回值逐層累加完成計算，這種設計降低中間要處理的狀態管理，對競程減少Bug，對專案增加程式可讀性。
##### [使用資料結構與演算法]
* 資料結構：基本型別 unsigned long long 用來乘載快速增長的數值
* 演算法：遞迴算法對應數學公式
##### [須注意的事]
遞迴深度隨m,n增長，速度極快，容易堆疊溢出。

僅適用小範圍m,n，可加入例外處理判斷來提升穩定性。
#### (2) Ackermann 函數 — 非遞迴
##### [選擇非遞迴的原因]
1. 避免堆疊溢位，對大範圍 m,n 使用封閉公式直接計算
   |      |     |
   |:---:|:---:|
   |m=0|A(m,n)=n+1|
   |m=1|A(m,n)=n+2|
   |m=2|A(m,n)=2n+3|
   |m=3|A(m,n)=2^(n+3)-3|
   |m=4,5|小範圍n直接數值對應|
2. 運算效率比起遞迴高非常多
##### [使用資料結構與演算法]
* 資料結構：unsigned long long+ullpow快速次方
* 演算法：條件判斷+封閉公式+位運算快速次方
##### [須注意的事]
超過 64-bit 範圍會溢位，應該加上提示或輸入的限制

非遞迴版本適合大範圍快速計算(Ex:Ack(3,60))

#### (1) Max/Min Heap 

##### [優化部分]


##### [遺留問題]

#### (2) BST


