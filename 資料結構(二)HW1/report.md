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
#### (2) Ackermann 函數 — 非遞迴
  - 由於輸出的增長速度超級快 使用unsigned long long
  - 已知Ackmann m 的規則
    - when m=0 Ackmann(m,n)= $n+1$ 
    - when m=1 Ackmann(m,n)= $n+2$
    - when m=2 Ackmann(m,n)= $2n+3$
    - when m=3 Ackmann(m,n)= $2^{n+3}-3$
    - when m=4 Ackmann(m,n)= $2↑↑(n+3)-3$ (超指數成長)
  - when (m>5) or (m=5 and n>0) or (m=4 and n>1) unsigned long long 必定溢位 [參考Ackmann wiki](https://en.wikipedia.org/wiki/Ackermann_function)
  - <img width="1518" height="760" alt="image" src="https://github.com/user-attachments/assets/98d7c9fb-8f8b-4a6e-b5d5-f3ee4499a685" />

#### (3) 子集合生成 — 遞迴
  - 用回溯法來生成特定長度的組合
  - 當前字串達到特定長度作為遞迴的結束條件
  - 在主程式for(int i=0;i<=len;++i)依次呼叫遞迴函式使子集合由小到大生成
## 程式實作
以下為主要程式碼：
#### (1) Max/Min Heap
``` c++
#include <iostream>
#include <chrono> //用來測執行時間的
#include <cstdlib> 
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
#### (2) Ackermann 函數 — 非遞迴
``` c++
#include <iostream>
using namespace std;
unsigned long long ullpow(int b,int exp){ //由於pow return double 必須寫一個ullpow來用
    unsigned long long ans=1;
    unsigned long long B=(unsigned long long)b;
    unsigned long long EXP=(unsigned long long)exp;
    while(EXP){
        if (EXP&1) 
            ans*=B;
        B*=B;
        EXP>>=1;
    }
    return ans;
}
unsigned long long Ack(int m,int n){
    if(m==0) 
        return n+1;
    if(m==1) 
        return n+2;
    if(m==2) 
        return 2*n+3;
    if(m==3)  
        return ullpow(2,n+3)-3;
    if(m==4) {
        if(n== 0)
          return 13; //A(4,0)=13
        if(n==1)
          return 65533; //A(4,1)=2^16-3=65533
    }
    if (m==5 && n==0)
        return 65533; //A(5,0)=65533
    return 0; //超過範圍或溢位
}
int main(){
    int m,n;
    while(cin>>m>>n)
        cout<<Ack(m,n)<<'\n';
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
      * 時間複雜度： $O(1)$ // 初始配置與釋放記憶體
      * 空間複雜度： $O(1)$
#### (2) Ackermann 函數 — 非遞迴
  * 時間複雜度：O(1) 或 O(log n),when m=3
  * 空間複雜度：O(1)
#### (3) 子集合生成 — 遞迴
  * 時間複雜度：O(n·2ⁿ) //長度為n的集合，每次取或不取=2ⁿ，n為重建now字串
  * 空間複雜度：O($n^2$) // 每層now都要複製，最多n層，每層now最長=n
## 測試與驗證

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
#### (2) Ackermann 函數 — 非遞迴
|測試案例|輸入參數 $m$|輸入參數 $n$|預期輸出|實際輸出|
|----------|--------------|----------|----------|----------|
|測試一|$m=0$|$n=1$|2|2|
|測試二|$m=1$|$n=1$|3|3|
|測試三|$m=2$|$n=1$|5|5|
|測試四|$m=3$|$n=1$|13|13|
|測試五|$m=3$|$n=61$|18446744073709551613|18446744073709551613|
|測試六|$m=3$|$n=62$|36893488147419103229|18446744073709551613(溢位)|
|測試七|$m=4$|$n=1$|65533|65533|
|測試八|$m=4$|$n=2$|0(超過範圍)|0(超過範圍)|
|測試九|$m=5$|$n=0$|65533|65533|
|測試十|$m=5$|$n=1$|0(超過範圍)|0(超過範圍)|
|測試十一|$m=-1$|$n=0$|0(超過範圍)|0(超過範圍)|
|測試十二|$m=0$|$n=-1$|0(超過範圍)|0(超過範圍)|
#### (3) 子集合生成 — 遞迴
|測試案例|輸入參數 $s$|預期輸出|實際輸出|
|----------|--------------|----------|----------|
|測試一|s="a b c"|"{{},{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}}"|"{{},{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}}"|
|測試二|s="abc"|"{{},{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}}"|"{{},{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}}"|
|測試三|s="1 2 3"|"{{},{1},{2},{3},{1,2},{1,3},{2,3},{1,2,3}}"|"{{},{1},{2},{3},{1,2},{1,3},{2,3},{1,2,3}}"|
|測試四|s="123"|"{{},{1},{2},{3},{1,2},{1,3},{2,3},{1,2,3}}"|"{{},{1},{2},{3},{1,2},{1,3},{2,3},{1,2,3}}"|
|測試五|s=""|"" (continue)|"" (continue)|
### 結論
  * Ackermann-遞迴：
    * 可正確計算小m,n但是容易因堆疊溢位
  * Ackermann-非遞迴：
    * 比起Ackermann遞迴能計算的範圍更廣更快，但超過64bit任然會溢位
  * 子集合生成：
    * 能正確列出所有Subset，輸出結果正確，邊界和空集合測試通過。
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
#### (3) 子集合生成 — 遞迴
##### [選擇遞迴的原因]
1. 回溯法實作上十分直觀每個元素有選或不選兩種可能增加可讀性：
2. allsubset(s,now+s[i],i+1,sublen)
   回溯法概念，遞迴每個節點代表當前子集合，遍歷完成或達到目標長度就回溯上一層
3. 易於排序生成
##### [使用資料結構與演算法]
* 資料結構：string存儲集合元素及子集合
* 演算法：遞迴+回溯法
##### [須注意的事]
遞迴深度與集合大小n線性相關n大時要注意記憶體消耗

輸出格式需刪除多餘逗號，保持輸出格式正確
### 程式改進
對以上三個程式做後續延伸和改進
#### (1) Ackermann 函數 — 遞迴
```c++
#include <iostream>
using namespace std;
unsigned long long dp[5][65533]={0};//dp記憶化，初始化0
unsigned long long Ack(int m, int n) {
    if (dp[m][n])
        return dp[m][n]; //有值的話直接返回
    unsigned long long ans=0;
    if(m==0) 
        ans=n+1;
    else if(n==0) 
        ans=Ack(m-1,1);
    else 
        ans=Ack(m-1,Ack(m,n-1));
    dp[m][n]=ans; //記錄這個值之後就不用重複做
    return ans;
}
int main(){
    int m,n;
    while(cin>>m>>n){
        cout<<Ack(m,n)<<endl;
    }
}
```
##### [優化部分]
  這裡改用了動態規劃(Dynamic Programming)的記憶化(Memoization)+遞迴的寫法
  
  已經計算過的值會被記住
  
  比起原本單純遞迴的方式可以避免重複計算
  
  呼叫Ack(m,n)可能包含多個Ack(x,y)
  
  當他算出一次Ack(x,y)之後都不用再次計算
  
  達到剪枝(Pruning)，避免了堆疊溢位。

##### [遺留問題]
  記憶陣列dp[][]其實直接影響了可計算的Ack範圍
  
  $$Ack(3, 3) = Ack(2, 29) = Ack(1, 59) = Ack(0, 60)$$
  
  Ack(3,3)存取dp[0][60]在dp[6][100]不會越界

  $$Ack(3, 4) = Ack(2, 61) = Ack(1, 123) = Ack(0, 124)$$
  
  Ack(3,4)存取dp[0][124]在dp[6][100]將會越界存取
  
  因此我要存取Ack(4,1)時
  
  $$Ack(4, 1) = Ack(3, 13) = Ack(2, 65531) = Ack(1, 65531) = Ack(0, 65532)$$
  
  陣列dp要有至少[5][65533]的大小。
#### (2) Ackermann 函數 — 非遞迴
  在當前程式直接使用查表法已經最佳化，無法再優化
  * 時間複雜度在m=0,1,2為 $O(1)$
    
    時間複雜度在m=3為 $O(log(n))$ //二進制指數法
    
    時間複雜度在m=4,5為 $O(1)$ //極小範圍直接對應
  * 空間複雜度 總是 $O(1)$ //輸入不影響變數個數
#### (3) 子集合生成 — 遞迴
  ```c++
    void allsubset(const string &s, string now,int start,int sublen) {
      if (now.length()==sublen){ //當滿足sublen長度(遞迴結束)
          cout<<"{";
          for(int i=0;i<now.length();++i){ //把整個now輸出
              cout<<now[i];
              if(i<now.length()-1) //如果還不是最後就輸出逗號
                  cout<<",";
          }
          cout << "}";
          cout << ","; //這裡要注意最後一個集合會多一個逗號
      }
      for(int i=start;i<s.length();++i) 
          allsubset(s,now+s[i],i+1,sublen); //遞迴組合生成
  }
  ```
##### [優化部分]
  在原先的程式沒有return導致遞迴滿足元素要求個數，仍然會執行直到集合尾端。

  原程式string now使用傳參考導致每一次呼叫都要複製一個now+s[i]

  這裡我們改成string &now傳參考
  
  因為now相同所以操作會互相干擾
  
  我們使用now.pop_back(); 來主動回溯
  
  並使用now.push_back(s[i]);替代掉now+=s[i]; 

  雖然效率相同
  
  但使新增單一元素的操作和模擬堆疊更加的直觀
  ``` c++
  #include <iostream>
  #include <string>
  using namespace std;
  void allsubset(const string &s,string &now,int start,int sublen) { //now傳參考
      if (now.length()==sublen){ 
          cout<<"{";
          for(int i=0;i<now.length();++i){ 
              cout<<now[i];
              if(i<now.length()-1) 
                  cout<<",";
          }
          cout<<"}";
          cout<<","; 
          return; //如果達成元素個數要求直接中止返回
      }
      for(int i=start;i<s.length();++i){
          now.push_back(s[i]);
          allsubset(s,now,i+1,sublen);
          now.pop_back(); 
      }
  }
  int main(){
      string s; 
      while(getline(cin,s)){
          string Sets="";
          for(char c:s) 
              if(c!=' '&&c!='\t'&&c!='\r'&&c!='\n') 
                  Sets+=c;
          if (Sets.empty()) 
              continue; 
          cout<<"{"; 
          int len=Sets.length(); 
          string now; //傳參考字串now
          for (int i=0;i<=len;++i) 
              allsubset(Sets,now,0,i); //改為變數now
          cout<<"\b}\n\n"; 
      }
  }
  ```
  * 時間複雜度從O(n·2ⁿ)->O(2ⁿ)
  * 空間複雜度從O($n^2$)->O(n)
  都少了原本now的開銷
