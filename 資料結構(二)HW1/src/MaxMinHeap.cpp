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