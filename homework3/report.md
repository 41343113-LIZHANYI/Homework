# 41343113

作業三

## 解題說明
### 問題描述
使用帶 header 節點的環狀鏈結串列，每個節點都含有以下三個資料(coef,exp,link)

來實作Polynomial類別

以下為題目要求:
* (a) 輸入運算子多載
* (b) 輸出運算子多載
* (c) 複製建構子
* (d) 賦值運算子
* (e) 解構子
* (f) 加法運算子多載
* (g) 減法運算子多載
* (h) 乘法運算子多載
* (i) x代入求值
  
並依序實作:
1. 實作ChainNode
2. 實作Chain
3. 實作ChainIterator
4. 實作多項式與環狀串列
5. 實作 Available Lists

### 解題策略
非題目要求必要含式:

newTerm()成員含式
   * 雖然題目沒有要求，但其他含式的操作總是需要新增項次在新Array後，所以我們新增newTerm()來做輔助
   * term可以直接作為索引來新增項次到Array後
   * term和capcity比較可以得知新增項次會不會超出陣列
   
     再動態配置記憶體來擴增大小

~Polynomial()解構子
   * 讓類別在生命周期結束時可以釋放記憶體空間
     
Polynomial(const Polynomial &copy)複製建構子
   * 讓複製時可以重新建立空間，不讓指標指向同個空間，導致重複的記憶體釋放

Polynomial& operator=(Polynomial copy)賦值運算子重載
   * Mult有使用已建構的物件做累加動作c=c.Add(temp)因此需要附值運算子重載

1. Polynomial()建構子
   * 根據註解 //Construct the polynomial p(x)=0
   * 根據註解提示 termArray 和 terms 都須為非零項，因此將指標指向nullptr，capacity和terms都設為0;
2. Add()成員函式
   * 我們假設三個Polynomial A,B,C(被加多項式、加多項式、新多項式)
   * 我們需要遍歷A和B，並不斷比較A和B誰指數較大，因此會有三種情況(A大、B大、相等)

     由此決定如何去移動數值到新多項式C
   * 在相等的情況還要考慮到A+B的係數是否為0
3. Mult()成員函式
   * 乘法產生的**項次並不按照順序**，因此要做**排序**以及**合併**的操作
   * 需要臨時的陣列存放亂序的項次
   * 乘完的項次要考慮加起來時是否為0
4. Eval()成員函式
   * 遍歷整個多項式，使用pow來累加回傳
5. 輸入運算子重載
   * 輸入格式為先輸入項次數，然後依序輸入係數和指數
   * 先cin一個n然後for n次依次讀入係數指數
6. 輸出的運算子重載
   * 格式為：係數X^指數 + ......
   * 需判斷是否為第一項以及係數是否大於0來決定是否加"+"
## 程式實作
以下為主要程式碼：
``` c++
#include <iostream>
#include <cmath> //x帶入值使用pow
using namespace std;
template <class T> class ChainNode; //類別宣告
template <class T> class Chain;
template <class T> class ChainIterator;
template <class T> class AvailableList;
class Polynomial;
class Term {
    friend class Polynomial; //讓多項式可以存取係數和指數
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
    friend istream& operator>>(istream& is, Polynomial& poly);
    friend class Chain<Term>; 
public:
    float coef; //係數
    int exp;    //指數
    Term(float c=0,int e=0):coef(c),exp(e){}
    bool operator!=(const Term& other) const { return exp != other.exp; }// 為了讓ChainNode可以賦值預設值
};
template <class T>
class ChainNode{
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& elem) : element(elem), next(nullptr) {}
    ChainNode(const T& elem, ChainNode<T>* nextNode) : element(elem), next(nextNode) {}
};
template <class T>
class AvailableList {
private:
    static ChainNode<T>* head; //static 共用head
public:
    static void getBack(ChainNode<T>* firstNode) {
        if(!firstNode)//如果空的直接return
            return;
        ChainNode<T>* last=firstNode;//這裡從firstnode開始遍歷chain來鎖定lastnode在哪
        while(last->next!=nullptr) 
            last=last->next;
        last->next=head; 
        head=firstNode;//讓firstnode成為新head
    }
    static ChainNode<T>* getNode(){
        if(head==nullptr)//如果av list沒東西new一個
            return new ChainNode<T>();
        ChainNode<T>* node=head;//node指定為av list現有的單一節點(head位置)
        head=head->next;//head指向下一個位置
        node->next=nullptr;//將node和head切斷
        return node;//return空間位置
    }
};
template <class T>
ChainNode<T>* AvailableList<T>::head=nullptr; //初始化static變數 使av list為空

template<class T>
class ChainIterator{
    friend class Chain<T>;//使chain 可以存取當前迭代的節點
private:
    ChainNode<T>* currentNode;//基本上是模擬指標
public:
    ChainIterator(ChainNode<T>* startNode=nullptr):currentNode(startNode){}//如果有參數指向startnode
    T& operator*()const{return currentNode->element;}//指標取值運算子
    T* operator->()const{return &currentNode->element;}//指標取址運算子
    ChainIterator& operator++(){ //前遞增運算子
        currentNode=currentNode->next;
        return *this;
    }
    ChainIterator operator++(int) { //後遞增運算子
        ChainIterator old=*this;
        currentNode=currentNode->next;
        return old;
    }
    bool operator!=(const ChainIterator& right)const{ //比較運算子
        return currentNode!=right.currentNode;
    }

    bool operator==(const ChainIterator& right)const{//比較運算子
        return currentNode==right.currentNode;
    }
};
template <class T>
class Chain{
    friend class ChainIterator<T>;
    friend class Polynomial;
private:
    ChainNode<T>* head;
    ChainNode<T>* last; 
public:
    Chain(){
        head=AvailableList<T>::getNode();//建構時從av list拿
        head->next=head; //head指向head(環狀)
        last=head; // 初始head=last
    }
    ~Chain(){ //解構要給av list
        if(head->next!=head){
            last->next=nullptr;//將環狀斷開不指向head
            AvailableList<T>::getBack(head);//歸還av list
        }else{//只有header
            head->next=nullptr; 
            AvailableList<T>::getBack(head);
        }
        head=nullptr;//空間轉移給av list要避免指標懸空
        last=nullptr;
    }
    void append(const T& element){
        ChainNode<T>* newNode=AvailableList<T>::getNode();//av list拿node空間
        newNode->element=element; //newnode值設定
        newNode->next=head; //維持環狀
        last->next=newNode; //last把最後node指向newnode上
        last=newNode;       //last變為newnode
    }
    
    void clear(){//使chain為空head->head(不是銷毀head->nullptr)
        if(head->next==head) return;//如果chain為空不動作
        ChainNode<T>* first=head->next;//將要返還給av list的首個節點設為head的下一個空間
        last->next=nullptr;//斷開環準備返還
        AvailableList<T>::getBack(first);
        head->next=head;//空chain定義 head->head
        last=head;
    }
    ChainIterator<T> begin()const{return ChainIterator<T>(head->next);}
    ChainIterator<T> end()const{return ChainIterator<T>(head);}//不能設為last會導致迭代少跑一項
};
class Polynomial {
    friend std::ostream& operator<<(std::ostream& os,const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is,Polynomial& poly);
private:
    Chain<Term> terms;
public:
    Polynomial(){}//會呼叫Chain的建構子
    Polynomial(const Polynomial& a){
        for (ChainIterator<Term> it = a.terms.begin(); it != a.terms.end(); ++it) 
            terms.append(*it);
    }
    ~Polynomial(){}//會觸發Chain的解構子
    const Polynomial& operator=(const Polynomial& a){
        if (this!=&a) {
            terms.clear();//清空
            for(ChainIterator<Term> it=a.terms.begin();it!=a.terms.end();++it) 
                terms.append(*it);
            
        }
        return *this;
    }
    Polynomial operator+(const Polynomial& b)const{
        Polynomial c;
        ChainIterator<Term> ait=this->terms.begin();
        ChainIterator<Term> bit=b.terms.begin();
        while(ait!=this->terms.end()&& bit!= b.terms.end()){//直到其中一個遍歷完
            if(ait->exp>bit->exp){//a的指數大
                c.terms.append(*ait);//新增a
                ait++;//指向下一個
            }else if(bit->exp>ait->exp){//b的指數大
                c.terms.append(*bit);//新增b
                bit++;//指向下一個
            }else{ //a和b指數相同
                float sum=ait->coef+bit->coef;//係數相加
                if(sum!=0)
                    c.terms.append(Term(sum,ait->exp));//不為0相加
                ait++;
                bit++;//a和b指向下一個
            }
        }
        while(ait!= this->terms.end()){//如果其中一個跑完另一個還沒，新增剩餘項次進c
            c.terms.append(*ait);
            ait++;
        }
        while(bit!= b.terms.end()){
            c.terms.append(*bit);
            bit++;
        }
        return c;
    }
    Polynomial operator-(const Polynomial& b)const{
        Polynomial c;
        ChainIterator<Term> ait=this->terms.begin();
        ChainIterator<Term> bit=b.terms.begin();
        while (ait!=this->terms.end()&&bit!=b.terms.end()) {
            if (ait->exp>bit->exp){//a指數大
                c.terms.append(*ait);//直接新增
                ait++;
            }else if(ait->exp<bit->exp){//b指數大
                c.terms.append(Term(-(bit->coef),bit->exp));//減法所以係數為負
                bit++;
            }else{
                float sum=ait->coef-bit->coef;//係數相減
                if(sum!=0)
                    c.terms.append(Term(sum,ait->exp));
                ait++;
                bit++;
            }
        }
        while(ait!=this->terms.end()){//如果還沒遍歷完就把剩下新增
            c.terms.append(*ait);
            ait++;
        }
        while(bit!=b.terms.end()){
            c.terms.append(Term(-(bit->coef),bit->exp));
            bit++;
        }
        return c;
    }
    Polynomial operator*(const Polynomial& b)const{
        Polynomial c;
        for(ChainIterator<Term> ait=this->terms.begin();ait!=this->terms.end();++ait){
            Polynomial temp;
            for(ChainIterator<Term> bit= b.terms.begin(); bit!=b.terms.end();++bit)
                temp.terms.append(Term(ait->coef*bit->coef,ait->exp+bit->exp));
            c=c+temp; 
        }
        return c;
    }
    float Evaluate(float x)const{
        float sum=0;
        for(ChainIterator<Term> it=terms.begin();it!=terms.end();++it) 
            sum+=it->coef*pow(x,it->exp);//帶入用pow計算
        return sum;
    }
};
istream& operator>>(istream& is, Polynomial& poly) {
    int n;
    is>>n; //輸入總項次
    poly.terms.clear(); //清空舊資料模擬覆蓋資料
    float c;
    int e;
    for (int i=0;i<n;++i){
        is>>c>>e;//迴圈輸入
        poly.terms.append(Term(c, e));//新增項次
    }
    return is;
}
ostream& operator<<(ostream& os, const Polynomial& poly) {
    ChainIterator<Term> it=poly.terms.begin();
    if (it== poly.terms.end()){//如果多項式為空
        os<<"0";
        return os;
    }
    bool first=true;//first旗標避免第一個項次前輸出+
    for (;it!=poly.terms.end();++it){
        if(!first&&it->coef>0) 
            os<<" + ";
        if(it->coef<0)
            os<< " - ";
        float absCoef=abs(it->coef);
        if(absCoef!=1.0||it->exp==0)//係數是否輸出 1x輸出x 1x^0輸出1
            os<<absCoef;
        if(it->exp!=0) 
            os<<"x";
        if(it->exp!=0&&it->exp!=1)
            os<<"^"<<it->exp;
        first=false;
    }
    return os;
}
int main(){
    Polynomial a,b;
    cout<<"請輸入多項式a"<<'\n'; //(a)輸入測試
    cin>>a;
    cout<<"a="<<a<<'\n';//(b)輸出測試
    cout<<"\n請輸入多項式b"<<'\n';
    cin>>b;
    cout<<"b="<<b<<'\n';
    cout<<"\n--- 運算結果"<<'\n';
    Polynomial sum=a+b;//(f)加法測試
    cout<<"加法:"<<sum<<'\n';
    Polynomial sub=a-b;//(g)減法測試
    cout<<"減法:"<<sub<<'\n';
    Polynomial mul=a*b;//(h)乘法測試
    cout<<"乘法:"<<mul<<'\n';
    float x;//(i)代入求值測試
    cout<<"輸入x:";
    cin>>x;
    cout<<"a代入x="<<x<<"的值:"<<a.Evaluate(x)<<'\n';
    Polynomial copy(a);//(c)複製建構子測試
    cout<<"複製a:"<<copy<<'\n';
    Polynomial assign;//(d)賦值運算子測試
    assign=a;
    cout<<"賦值a:"<<assign<<'\n';
    return 0;//(e)解構子測試
}
```
## 效能分析
#### (1) Polynomial() 建構子
  * 時間複雜度：O(1) //僅初始化termArray、capacity、terms，與輸入無關
  * 空間複雜度：O(1) 
#### (2) ~Polynomial() 解構子
  * 時間複雜度：O(1) //僅執行delete[]釋放記憶體，與輸入無關
  * 空間複雜度：O(1) 
#### (3) Polynomial(const Polynomial &copy) 複製建構子
  * 時間複雜度：O(n) //單層for迴圈複製n次
  * 空間複雜度：O(n) //new Term[capacity]分配新空間n大小
#### (4) Polynomial& operator=(Polynomial copy) 賦值運算子重載
  * 時間複雜度：O(n) //參數&copy在呼叫時會觸發複製建構子O(n)
  * 空間複雜度：O(n) //雖然只有swap，但整體因複製建構子仍為O(n)
#### (5) void newTerm(float c, int e) 項次新增含式
  * 時間複雜度：O(n) or O(1) //如果空間足夠O(1)，否則須重分配空間觸發copy含式O(n)
  * 空間複雜度：O(n) or O(1) //同理，不夠時重分配O(n)
#### (6) Polynomial Add(Polynomial poly) 多項式相加含式
  * 時間複雜度：O(m+n) //假設A項次=m B項次=n for迴圈各遍歷一次
  * 空間複雜度：O(m+n) //最多m+n項至C多項式 
#### (7) Polynomial Mult(Polynomial poly) 多項式相乘含式
  * 時間複雜度：O(mxn) //雙層for迴圈，外層m次，內層n次
  * 空間複雜度：O(mxn) //項次最多mxn項
#### (8) float Eval(float f) 數值帶入x含式
  * 時間複雜度：O(n) //單層for遍歷項次
  * 空間複雜度：O(1) //固定變數不受輸入引響
#### (9) istream& operator>>(istream& is, Polynomial& poly) 輸入運算子重載
  * 時間複雜度：O(n) //單層while，受輸入引響O(n)
  * 空間複雜度：O(n) //newTerm至poly最大O(n)
#### (10) ostream& operator<<(ostream& os, const Polynomial& poly) 輸出運算子重載
  * 時間複雜度：O(n) //for遍歷產生輸出
  * 空間複雜度：O(1) //固定變數不受輸入引響
## 測試與驗證
### 輸入
|測試項目|輸入參數 $A$總項次|輸入參數 $A$多項次|輸入參數 $B$總項次|輸入參數 $B$多項次|輸入參數 $x$值|
|-------------|--------------|----------|----------|-------------|-------------|
|1:基本加、乘法|3|2 2 1 1 3 0|3|1 2 1 1 1 0|2|
|2:負係數和不同次方|3|2 3 -1 2 3 0|2|1 1 4 0|2|
|3:項數不等|4|1 5 1 4 1 3 1 0|2|1 2 1 1|1|
|4:部分相加為0|3|3 2 -2 1 1 0|3|-3 2 2 1 5 0|1|
|5:空多項式|1|0 0|1|0 0|3|
### 預期輸出
|測試項目|預期輸出 $A$多項式|預期輸出 $B$多項式|預期輸出 $A+B$多項式|預期輸出 $AxB$多項式|預期輸出 $A$多項式帶入x|預期輸出 $B$多項式帶入x|
|----------------|--------------|----------|----------|----------|-------------|-------------|
|1:基本加、乘法|2X^3-1X^2+3X^0|1X^2+1X^1+1X^0|3X^2+2X^1+4X^0|2X^4+3X^3+6X^2+4X^1+3X^0|13|7|
|2:負係數和不同次方|2X^3-1X^2+3X^0|1X^1+4X^0|2X^3-1X^2+1X^1+7X^0|2X^4+7X^3-4X^2+3X^1+12X^0|15|6|
|3:項數不等|1X^5+1X^4+1X^3+1X^0|1X^2+1X^1|1X^5+1X^4+1X^3+1X^2+1X^1+1X^0|1X^7+2X^6+2X^5+1X^4+1X^2+1X^1|4|2|
|4:部分相加為0|3X^2-2X^1+1X^0|-3X^2+2X^1+5X^0|6X^0|-9X^4+12X^3+8X^2-8X^1+5X^0|2|4|
|5:空多項式|0|0|0|0|0|0|
### 實際輸出
|測試項目|實際輸出 $A$多項式|實際輸出 $B$多項式|實際輸出 $A+B$多項式|實際輸出 $AxB$多項式|實際輸出 $A$多項式帶入x|實際輸出 $B$多項式帶入x|
|----------------|--------------|----------|----------|----------|-------------|-------------|
|1:基本加、乘法|2X^3-1X^2+3X^0|1X^2+1X^1+1X^0|3X^2+2X^1+4X^0|2X^4+3X^3+6X^2+4X^1+3X^0|13|7|
|2:負係數和不同次方|2X^3-1X^2+3X^0|1X^1+4X^0|2X^3-1X^2+1X^1+7X^0|2X^4+7X^3-4X^2+3X^1+12X^0|15|6|
|3:項數不等|1X^5+1X^4+1X^3+1X^0|1X^2+1X^1|1X^5+1X^4+1X^3+1X^2+1X^1+1X^0|1X^7+2X^6+2X^5+1X^4+1X^2+1X^1|4|2|
|4:部分相加為0|3X^2-2X^1+1X^0|-3X^2+2X^1+5X^0|6X^0|-9X^4+12X^3+8X^2-8X^1+5X^0|2|4|
|5:空多項式|0|0|0|0|0|0|
### 結論
* 正確性驗證
	* 多項式的加法、乘法，以及代入值的計算，實際輸出和預期輸出一致
 	* 即使有負係數、不同次方、項數不等和空多項式的程式依然可以正確處理
    * 乘法結果能夠合併同指數項次，而且不會少或多出項次
* 功能性測試
	* Add()能正確比較指數並逐一合併項次
 	* Mult()能正確計算每對項次相乘結果並用temp多項式累加，同時避免排序問題
  	* Eval()能正確代入x值並計算多項式結果
	* 樹入輸出運算子重載能正確用特定格式讀入和輸出
* 邊界 & 特殊情況
 	* 空多項式f(x)=0能正確輸入、輸出和計算
  	* 項次相加為0時能忽略該項次。
## 申論及開發報告
### 程式分析
對當前寫的程式做優點以及資料結構&演算法分析，還有程式需注意的要點
### (1) Polynomial—建構子&複製建構子&解構子
[設計原因]

* 使用動態陣列 (Term*) 儲存多項式的非零項，避免靜態陣列浪費空間。
* 建構子 Polynomial() 初始化空多項式 p(x)=0，避免未定義指標。
* 複製建構子 Polynomial(const Polynomial &copy) 確保複製時重新分配記憶體，避免指標共用導致重複釋放。
* 解構子 ~Polynomial() 釋放動態陣列，防止記憶體洩漏。

[使用資料結構與演算法]

資料結構：動態陣列 (Term*)，儲存項次(coef,exp)

演算法：動態記憶體管理，swap賦值

[須注意的事]

* 注意容量與項數的管理
* newTerm()需動態分配來擴增陣列
* 防止加入係數為0的項次

### (2) Polynomial—Add()多項式加法
[設計原因]

* 加法需要遍歷兩個多項式，逐項比較指數大小，使多項式能按順序合併
* 遇到指數相等時，需檢查係數相加是否為0，避免0加入多項式

[使用資料結構與演算法]

資料結構：Term* 動態陣列

演算法：雙指標(aPos,bPos)遍歷、比較並合併至C

[時間與空間複雜度]

時間複雜度：O(n)

空間複雜度：O(n)

[須注意的事]

* 保持項次按指數由大到小排序
* 處理部分相加為0的項

### (3) Polynomial—Mult()多項式乘法
[設計原因]

* 每個項次都要和另一多項式的所有項次相乘，因此需使用雙層迴圈遍歷
* 乘法結果可能不按照順序，所以每次乘法後須使用Add()合併同次項，用C多項式累加temp。

[使用資料結構與演算法]

資料結構：Term* 動態陣列+臨時temp多項式存放中間的結果

演算法：雙層迴圈產生乘積項，Add()合併同指數次項

[時間與空間複雜度]

時間複雜度：O(nxm) //n與m為兩個多項式項數

空間複雜度：O(nxm) //臨時存放相乘結果

[須注意的事]

* 每次乘法後使用Add()合併，避免同次項重複出現
* 注意乘法結果可能為0
  
### (4) Polynomial — Eval(float f) 多項式帶入值
[設計原因]

* 遍歷計算coef*pow(f,exp)並累加求得到多項式數值

[使用資料結構與演算法]

資料結構：浮點數 sum 累加

演算法：單層迴圈遍歷多項式項

[時間與空間複雜度]

時間複雜度：O(n)

空間複雜度：O(1)

[須注意的事]

* 使用pow()計算大指數時注意精度
* sum初始值預設為0
  
### (5) Polynomial — 輸入/輸出運算子重載
[設計原因]

* operator>> 特定格式輸入，先輸入總項次數n，然後依次輸入係數和指數
* operator<< 特定格式輸出，多項式以(係數X^指數)顯示，第一項以及負係數避免加"+"
  
[使用資料結構與演算法]

資料結構：Term*

演算法：for遍歷項次，特定格式輸出

[時間與空間複雜度]

時間複雜度：O(n)

空間複雜度：O(1):output or O(n):input

[須注意的事]

* 處理係數正負號與第一項"+"
* 空多項式應輸出0

### 程式改進
* 對以上三個成員含式Add() & Mult() & Eval() 做改進
#### Add()-直接一次分配完空間和完全避免後續使用newTerm()
```c++
Polynomial Polynomial::Add(const Polynomial &poly){
    Polynomial c;
    c.capacity=terms+poly.terms; //預判最多項次的可能m+n
    c.termArray=new Term[c.capacity]; //為新多項式動態規劃
    int aPos=0,bPos=0;
    while(aPos<terms&&bPos<poly.terms){
        if(termArray[aPos].exp==poly.termArray[bPos].exp){
            float sum=termArray[aPos].coef+poly.termArray[bPos].coef;
            if(sum)
                c.termArray[c.terms++]={sum,termArray[aPos].exp}; //這裡都使用terms做索引新增，避免newTerm()呼叫
            aPos++;
			bPos++;
        }else if(termArray[aPos].exp>poly.termArray[bPos].exp){
            c.termArray[c.terms++]=termArray[aPos++];
        }else{
            c.termArray[c.terms++]=poly.termArray[bPos++];
        }
    }
    while(aPos<terms)//改用while
        c.termArray[c.terms++]=termArray[aPos++];
    while(bPos<poly.terms)
        c.termArray[c.terms++]=poly.termArray[bPos++];
    return c;
}
```
#### [原有問題]
* 每新增一項次都要呼叫newTerm()，要不斷檢查容量、重分配記憶體導致額外成本
* 多次newTerm()需要多次記憶體存取、複製
#### [優化部分]
1.一次性分配空間
```c++
c.capacity=terms+poly.terms;
c.termArray=new Term[c.capacity];
```
避免不斷分配空間

2.利用索引插入項次
```c++
c.termArray[c.terms++] = {sum, exp};
```
完全避免後續使用newTerm()，不會有額外的函式呼叫

3.改用while新增剩餘項至新空間

讓程式看起來更整潔

結論:
* 時間複雜度常數降低
* 動態記憶體分配次數從多次降為1次
#### Mult()-一次分配完空間和完全避免後續呼叫newTerm() & 避免使用Add()和臨時多項式temp
```c++
Polynomial Polynomial::Mult(const Polynomial &poly){
    if(terms==0||poly.terms==0)
		return Polynomial();//return f(x)=0
    int maxCap=terms*poly.terms;//預判最多項次可能 mxn
    Polynomial c;
    c.capacity=maxCap;
    c.termArray=new Term[maxCap];
    for(int i=0;i<terms;++i){
        for(int j=0;j<poly.terms;++j){
            float newC=termArray[i].coef*poly.termArray[j].coef;
            int newE=termArray[i].exp+poly.termArray[j].exp;
            bool found=false; //flag
            for(int k=0;k<c.terms;++k){ //每次都遍歷是否有相同指數
                if(c.termArray[k].exp==newE){
                    c.termArray[k].coef+=newC; //如果有相加
                    found=true;
                    break;
                }
            }
            if(!found&&newC!=0)
                c.termArray[c.terms++]={newC,newE};
        }
    }
    return c;
}
```
#### [原有問題]
* 乘法都要建立一個臨時多項式temp再用Add()累加，浪費時間與空間
#### [優化部分]
1.一次性分配空間
```c++
c.capacity = terms * poly.terms;
c.termArray = new Term[c.capacity];
```
避免不斷分配空間

2.就地合併同次方項
```c++
for(int k=0;k<c.terms;++k){
	if(c.termArray[k].exp == newE){
		c.termArray[k].coef += newC;
		found = true;
		break;
	}
}
if(!found && newC != 0)
	c.termArray[c.terms++] = {newC, newE};
```
完全避免使用newTerm() & Add()，不會有額外的函式呼叫，也不會多新增temp多項式

結論:
* 避免兩個成員含式呼叫，降低時間複雜度
* 避免建構temp多項式，降低空間複雜度
#### Mult()-改用霍納法則
```c++
float Polynomial::Eval(float f){
	float res=0;
	for(int i=0;i<terms;++i){
		res=res*f+termArray[i].coef;//霍納法則
	}
	return res;
}
```
#### [原有問題]
* 每個項次都呼叫pow()重複計算次方，成本高

#### [優化部分]

程式使用霍納法則的多項式算法(下圖)
<img width="1218" height="160" alt="image" src="https://github.com/user-attachments/assets/98ab414a-4d64-4b4d-b7ef-b4ec72d9092f" />

結論:
* 將原本時間複雜度O(n^2)降為O(n)
* 完全避免pow()執行時間大幅減少
