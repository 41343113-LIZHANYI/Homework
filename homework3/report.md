# 41343113

作業三

## 解題說明
### 問題描述
開發一個Polynomial類別使用具備首節點(header nodes) 的環狀鏈結串列
，每個多項式都是有header node的環狀串列。

為了有效地刪除多項式，我們需要使用可用空間串列 (available-space list)
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
* Term類別
  
  作為項次類別需要coef(係數)和exp(指數)資料成員
  
* ChainNode類別
  
  節點定義為一個有element(元素)和next(指標)的類別
  
* AvailableList類別

  因為頻繁地向作業系統呼叫new和delete效率低，

  所以有了AvailableList來做資源的重複利用。
  1. getBack(ChainNode<T>* firstNode)
  	 
	 返還av list需要先判斷串列是否為空，為空直接return，不為空原先Chain類別的解構子會把原本環狀段開，

	 所以只要遍歷至nullptr就可以接上原本av list的header，隨後把header更新為firstnode即完成更新。
  2. getNode()
  	
	 提取av list的node需要先判斷串列是否為空，如果為空就new新建一個ChainNode，
	 
	 如果需要把第一個節點提取來用(同時為header指向)，因此須將header指向下一個，
	 
	 並切斷和第一個節點的連接，才可釋出節點。
* ChainIterator類別
 
  迭代Chain需要一個currentNode(指標)成員遍歷和存取Chain的資料，

  同時該類別需要運算子多載來讓呼叫時for可以使用it++。
  
* Chain類別

  我們使用為了方便存取前端和尾端節點需要head和last(指標)，避免存取尾端節點需要O(n)的情況。

  1. Chain()

	 我們建構時需要使用av list的節點，並把head和last都指向head完成初始化。
  2. ~Chain()

	 我們解構時則需要返還給av list，我們要先做Chain是否為空的判斷，

	 以此來判斷是否須將環狀斷開，讓av list方便處理，

	 最後將兩個指標設為nullptr避免懸空。
  3. append(const T& element)
  
     從av list取出一個節點newnode，先將原本尾部節點改為指向newnode，
	 
	 設定該節點元素並將newnode指向header完成環狀，並把last指向newnode使該節點成為尾節點。
  4. clear()
  	
     使Chain清空，要注意空的定義是header->header而非header->nullptr(銷毀)，

	 先定義first為header的下個空間避免header被返還av list，

	 接下來將header和first連接切斷執行返還，接下來重新初始化header和last指標。
  5. begin()
  
     要回傳header->next，因為header node本身不存資料。
  6. end()

	 不能回傳last應回傳header，避免迭代時略過最後一個元素。
* Polynomial類別
  我們使用Chain來完成Polynomial，這樣可以使用Chain的成員來完成Polynomial的操作。
  1. Polynomial()
  
  	 會呼叫Chain的建構子
  2. Polynomial(const Polynomial& a)
  	 
	 複製建構子用ChainIterator類別去做迭代，然後使用Chain的append來完成依項的複製。
  3. ~Polynomial()
  
  	 會呼叫Chain的解構子
  4. operator+(const Polynomial& b)

  	 利用ChainIterator去遍歷a,b的多項式，指數大的先新增，一樣大就做係數相加再新增，

	 如果其中一個多項式遍歷完，就把另一個多項式剩餘的全部新增。
  5. operator-(const Polynomial& b)

     利用ChainIterator去遍歷a,b的多項式，如果a指數大先新增，
	 
	 如果b指數大由於b為減數因此係數要多負號，如果指數相同就係數相加新增，
	 
	 如果其中一個多項式遍歷完，就把另一個多項式剩餘的全部新增。
  6. operator*(const Polynomial& b)

     新增一個多項式temp，雙迴圈遍歷a,b多項式，讓逐次讓a的單一項次去遍歷整個b多項次相乘相加到temp，

	 遍歷完再將temp加到c多項式上，當a遍歷完即完成多項式乘法。
  7. Evaluate(float x)
  
	 利用ChainIterator去遍歷並使用pow來完成帶入值計算相加。
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
private:
    float coef; //係數
    int exp;    //指數
public:
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
#### (1) Term & ChainNode類別成員
  * 時間複雜度：O(1) //皆為簡單資料儲存和賦值
  * 空間複雜度：O(1) //不受參數影響
#### (2) getBack(ChainNode<T>* firstNode)
  * 時間複雜度：O(n) //需要遍歷串列得到尾端節點
  * 空間複雜度：O(1) //不受參數影響
#### (3) getNode() 
  * 時間複雜度：O(1) //固定指標操作
  * 空間複雜度：O(1) //固定變數
#### (4) ChainIterator類別 運算子多載
  * 時間複雜度：O(1) //都是極為簡短的指標操作
  * 空間複雜度：O(1) 
#### (5) Chain()
  * 時間複雜度：O(1) //只建立一個head並初始化
  * 空間複雜度：O(1) 
#### (6) ~Chain()
  * 時間複雜度：O(n) //getBack需遍歷串列
  * 空間複雜度：O(1) 
#### (7) append(const T& element)
  * 時間複雜度：O(1) //有last指標不需要遍歷
  * 空間複雜度：O(1) 
#### (8) clear()
  * 時間複雜度：O(n) //getBack需遍歷串列
  * 空間複雜度：O(1) 
#### (9) begin(), end()
  * 時間複雜度：O(1) //都直接回傳位置
  * 空間複雜度：O(1) 
#### (10) Polynomial()
  * 時間複雜度：O(1) //呼叫Chain建構子複雜度相同
  * 空間複雜度：O(1)
#### (11) Polynomial(const Polynomial& a)
  * 時間複雜度：O(A) //遍歷a的節點
  * 空間複雜度：O(A) //和a相同空間
#### (12) ~Polynomial()
  * 時間複雜度：O(A) //呼叫Chain解構複雜度相同
  * 空間複雜度：O(1) 
#### (13) operator=(const Polynomial& a)
  * 時間複雜度：O(A+old_size) //先呼叫 clear()清除再遍歷a進行複製
  * 空間複雜度：O(A)//相同大小 
#### (14) operator+(const Polynomial& b)
  * 時間複雜度：O(A+B) //a,b各遍歷一次
  * 空間複雜度：O(A+B) //最壞情況項次不重複
#### (15) operator-(const Polynomial& b)
  * 時間複雜度：O(A+B) //和加法流程大致相同
  * 空間複雜度：O(A+B)
#### (16) operator*(const Polynomial& b)
  * 時間複雜度：O(A^2*B) //A和B遍歷的巢狀迴圈還要考慮多項式c=c+temp相加問題
  * 空間複雜度：O(A*B)// 乘法項次最大情況
#### (17) Evaluate(float x)
  * 時間複雜度：O(A) //遍歷A做pow相加
  * 空間複雜度：O(1)
#### (18) operator>>(istream& is, Polynomial& poly)
  * 時間複雜度：O(n) //遍歷做輸入
  * 空間複雜度：O(n) //n個節點
#### (19) operator<<(ostream& os, const Polynomial& poly) 
  * 時間複雜度：O(A) //遍歷A做輸出
  * 空間複雜度：O(1)
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
