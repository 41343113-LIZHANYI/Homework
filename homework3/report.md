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
            os<<"+";
        if(it->coef<0)
            os<<"-";
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
|測試項目|$A$總項次|$A$多項次|$B$總項次|$B$多項次|$x$值|
|-------------|--------------|----------|----------|-------------|-------------|
|1:運算測試|3|3 2 2 1 1 0|3|1 2 -4 1 5 0|2|
|2:多項式相消|2|1 3 5 0|2|1 3 5 0|0|
|3:平方差中項抵消|2|1 1 1 0|2|1 1 -1 0|3|
|4:浮點係數相消|2|0.5 2 -2.5 0|2|-0.5 2 2.5 0|10|
### 預期輸出
|測試項目|$A$|$B$|加法|減法|乘法|a代入x|複製a|賦值a|
|----------------|--------------|----------|----------|----------|-------------|-------------|-------------|-------------|
|1:運算測試|3x^2+2x+1|x^2-4x+5|4x^2-2x+6|2x^2+6x-4|3x^4-10x^3+8x^2+6x+5|17|3x^2+2x+1|3x^2+2x+1|
|2:多項式相消|x^3+5|x^3+5|2x^3+10|0|x^6+10x^3+25|5|x^3+5|x^3+5|
|3:平方差中項抵消|x+1|x-1|2x|2|x^2-1|4|x+1|x+1|
|4:浮點係數相消|0.5x^2-2.5|-0.5x^2+2.5|0|x^2-5|-0.25x^4+2.5x^2-6.25|47.5|0.5x^2-2.5|0.5x^2-2.5|
|5:稀疏多項式|x^10+1|2x^10-5x|3x^10-5x+1|-x^10+5x+1|2x^20-5x^11+2x^10-5x|1025|x^10+1|x^10+1|
### 實際輸出
|測試項目|$A$|$B$|加法|減法|乘法|a代入x|複製a|賦值a|
|----------------|--------------|----------|----------|----------|-------------|-------------|-------------|-------------|
|1:運算測試|3x^2+2x+1|x^2-4x+5|4x^2-2x+6|2x^2+6x-4|3x^4-10x^3+8x^2+6x+5|17|3x^2+2x+1|3x^2+2x+1|
|2:多項式相消|x^3+5|x^3+5|2x^3+10|0|x^6+10x^3+25|5|x^3+5|x^3+5|
|3:平方差中項抵消|x+1|x-1|2x|2|x^2-1|4|x+1|x+1|
|4:浮點係數相消|0.5x^2-2.5|-0.5x^2+2.5|0|x^2-5|-0.25x^4+2.5x^2-6.25|47.5|0.5x^2-2.5|0.5x^2-2.5|
|5:稀疏多項式|x^10+1|2x^10-5x|3x^10-5x+1|-x^10+5x+1|2x^20-5x^11+2x^10-5x|1025|x^10+1|x^10+1|
### 結論
* 正確性驗證
	* 能夠使用帶有header node的環狀串列來正常建構與解構多項式		 	
	* 多項式的加法、減法、乘法、代入值、複製、賦值都可正常運作，且實際輸出和預期輸出一致
 	* 即使具有負係數、不同次方、項數不等、相和為零、浮點係數、稀疏多項式的程式依然可以正確處理
    * 乘法結果能夠正常合併同指數項次，而且不會少或多出項次
* 功能性測試
	* 輸入輸出運算子重載能正確用特定格式讀入和輸出，且係數和指數有特別情況也符合正常格式
 	* Polynomial(const Polynomial& a):能夠正確遍歷a並複製多項式，且沒有格式錯誤 
 	* operator=(const Polynomial& a):能夠正確clear()並遍歷a為多項式賦值，且沒有格式錯誤 
	* operator+(const Polynomial& b):能正確比較指數並逐一合併項次
 	* operator-(const Polynomial& b):可以在b項次新增時正確計算負號	 
 	* operator*(const Polynomial& b):能正確計算每對項次相乘結果並用temp多項式累加，同時避免排序問題
  	* Evaluate(float x):能正確代入x值並計算多項式結果
	
* 邊界 & 特殊情況
 	* 空多項式f(x)=0能正確輸入、輸出和計算
  	* 項次相加為0時能忽略該項次。
  	* 指數為零可以正常忽略該x輸出
  	* 係數為一時可以正常忽略係數輸出
## 申論及開發報告
### 程式分析
對當前寫的程式做優點以及資料結構&演算法分析，還有程式需注意的要點
### (1) Term & ChainNode 類別成員
[設計原因]

* Term類別封裝了多項式的基本單元(係數)coef和(指數)exp
* 透過friend 機制允許 Polynomial 直接存取私有成員
* ChainNode是鏈結串列的節點容器，設計為通用樣板，使其不僅限於儲存Term，也能擴充至其他資料型態

[使用資料結構與演算法]

資料結構：自訂類別與樣板

演算法：建構子初始化列表

[時間與空間複雜度]

* 時間複雜度：O(1) //變數賦值

* 空間複雜度：O(1) //固定變數
[須注意的事]

* Term 定義了operator!=主要是為了讓ChainNode在樣板實例化時，若有預設值比較的需求能正常運作

### (2) AvailableList::getBack 回收節點
[設計原因]

* 為了避免頻繁使用delete造成的成本，函式將不再使用的鏈結串列返還到av list中，以供後續重複利用

[使用資料結構與演算法]

資料結構：單向鏈結串列

演算法：遍歷尋找尾端節點將其next指向head

[時間與空間複雜度]

時間複雜度：O(n) //遍歷至尾端

空間複雜度：O(1)

[須注意的事]

* 必須處理傳入 nullptr 的情況
* 回收後，head會更新為這串鏈結串列的前端，達成堆疊

### (3) AvailableList::getNode 取得節點
[設計原因]

* Chain建構時優先從Av list中取用現成節點，若為空則呼叫new配置新記憶體
* 大幅減少了動態記憶體配置的次數

[使用資料結構與演算法]

資料結構：單向鏈結串列

演算法：指標操作

[時間與空間複雜度]

時間複雜度：O(1) 

空間複雜度：O(1) 

[須注意的事]

* 取出節點後，需將該節點的next設為nullptr，確保完全獨立，避免指向Av list其他節點導致邏輯錯誤
  
### (4) ChainIterator類別 運算子多載
[設計原因]

* 為了讓自定的Chain能使用for的方法遍歷，提供遞增運算子和比較運算子

[使用資料結構與演算法]

資料結構：迭代器模式

演算法：指標移動與取值

[時間與空間複雜度]

時間複雜度：O(1)

空間複雜度：O(1)

[須注意的事]

* 區分前置遞增++i(回傳參考)與後置遞增i++(複製物件)
  
### (5) Chain() 建構子
[設計原因]

* 初始化一個空的環狀鏈結串列，建立一個Header Node，並讓next指向自己，完成空串列的定義
* 有Header node可省去插入與刪除的邊界檢查
  
[使用資料結構與演算法]

資料結構：環狀鏈結串列

演算法：向av list請求一個節點作為Head

[時間與空間複雜度]

時間複雜度：O(1)

空間複雜度：O(1)

[須注意的事]

* last指標初始化時指向head
### (6) ~Chain() 解構子
[設計原因]

* 物件生命週期結束時，這裡不直接delete，將整串鏈結(非空)交給getback回收，實現資源循環。
  
[使用資料結構與演算法]

資料結構：鏈結串列

演算法：斷開環狀結構後呼叫 getBack

[時間與空間複雜度]

時間複雜度：O(n)//遍歷尋找尾端

空間複雜度：O(1)

[須注意的事]

* Header節點本身也需要被回收
* 需避免指標懸空，最後要將head設為nullptr
### (7) Chain::append 新增節點
[設計原因]

* 為了在多項式建立或運算時，能以 O(1) 效率將新項次加入尾端，類別中多了last指標，不須從頭遍歷
  
[使用資料結構與演算法]

資料結構：帶尾指標的環狀鏈結串列

演算法：last指標直接新增節點，並指向head維持環狀

[時間與空間複雜度]

時間複雜度：O(1) //last直接新增

空間複雜度：O(1)

[須注意的事]

* 每次必須更新last指標指向新的節點
### (8) Chain::clear 清空串列
[設計原因]

* 將串列恢復到初始狀態(只剩Head指向自己)不銷毀Chain物件本身
* 用於operator=賦值前的舊資料清除，或operator>>輸入前的重置
  
[使用資料結構與演算法]

資料結構：鏈結串列

演算法：保留Header，將header和整串鏈結切斷連接並透過getBack回收。

[時間與空間複雜度]

時間複雜度：O(n) //getback

空間複雜度：O(1)

[須注意的事]

* 如果串列本來就是空的直接return，避免錯誤操作
### (9) Chain::begin(),end()
[設計原因]

* 提供迭代器的起始與結束位置，支援for遍歷
  
[使用資料結構與演算法]

資料結構：迭代器生成

演算法：begin回傳head->next(第一個節點)；end回傳head(讓for可以正確遍歷)

[時間與空間複雜度]

時間複雜度：O(1)

空間複雜度：O(1)

[須注意的事]

* end()回傳的是head而非nullptr，因為這是環狀串列，遍歷回到head即代表繞行一圈結束
### (10) Polynomial() 建構子
[設計原因]

* 初始化一個空多項式，Chain的建構子會自動被呼叫
  
[使用資料結構與演算法]

資料結構：環狀鏈結串列

演算法：呼叫 Chain 建構子

[時間與空間複雜度]

時間複雜度：O(1)

空間複雜度：O(1)

[須注意的事]

* 確保 Chain 已正確初始化 Header
### (11) Polynomial(const Polynomial& a) 複製建構子
[設計原因]

* 當以一個多項式複製另一個多項式時，需避免兩個物件共用同一個鏈結串列導致解構時重複返回av list。
  
[使用資料結構與演算法]

資料結構：鏈結串列

演算法：透過迭代器遍歷來源多項式，將每個Term使用append新增到新物件中

[時間與空間複雜度]

時間複雜度：O(A) //A為多項式a的項數

空間複雜度：O(A) //和A多項式同空間

[須注意的事]

* 會先自動呼叫Chain建構子建立Header，這裡只需複製資料節點
### (12) ~Polynomial() 解構子
[設計原因]

* 多項式生命週期結束時，自動觸發Chain的解構子，自動返還av list
  
[使用資料結構與演算法]

資料結構：環狀鏈結串列

演算法：呼叫 ~Chain()

[時間與空間複雜度]

時間複雜度：O(A) //返還A節點

空間複雜度：O(A)

[須注意的事]

* 無須手動撰寫，全交由Chain解構子處理
### (13) operator=(const Polynomial& a) 賦值運算子
[設計原因]

* 支援物件間的賦值
* 必須先清空左側物件原有的內容，再從右側物件複製資料
  
[使用資料結構與演算法]

資料結構：鏈結串列

演算法：clear()->迴圈 append

[時間與空間複雜度]

時間複雜度：O(old_size+A) //清除舊資料 + 複製新資料

空間複雜度：O(1)

[須注意的事]

* if(this!=&a)非常重要，否則先執行 clear() 會把來源資料也刪除，導致資料遺失
### (14) operator+(const Polynomial& b) 加法
[設計原因]

* 實作多項式加法，依次比較項次指數來操作和新增
  
[使用資料結構與演算法]

資料結構：雙指標

演算法：同時遍歷A和B，指數大者先加入，相同就係數求和加入，加入剩下未遍歷完的項次

[時間與空間複雜度]

時間複雜度：O(A+B) //多項式各遍歷一次

空間複雜度：O(A+B) //最差情況指數都不同

[須注意的事]

* 相加後係數為0的項次不應加入多項式中
### (15) operator-(const Polynomial& b) 減法
[設計原因]

* 實作邏輯與加法相似，差別在於當單獨加入B項次時，係數需變號
  
[使用資料結構與演算法]

資料結構：雙迭代器

演算法：類似加法，但在 B 指數較大或指數相等係數相減時，對 B 的係數進行負號處理

[時間與空間複雜度]

時間複雜度：O(A+B)

空間複雜度：O(A+B)

[須注意的事]

* 相減後係數為0的項次不應加入多項式中
### (16) operator*(const Polynomial& b) 乘法
[設計原因]

* A的每一項都要與B的每一項相乘，並將結果累加
  
[使用資料結構與演算法]

資料結構：巢狀迴圈與臨時多項式

演算法：外層迴圈遍歷A的每一項，內層迴圈遍歷B產生暫存多項式temp，利用c=c+temp將結果累加

[時間與空間複雜度]

時間複雜度：O(A^2*B) //單純生成項次是A*B，但因反覆呼叫加法合併，複雜度會上升

空間複雜度：O(A*B) //項次最多為兩者項數乘積

[須注意的事]

* 用了已實作的加法來處理同次項合併的問題，犧牲了部分效能
### (17) Evaluate(float x) 代入求值
[設計原因]

* 利用pow計算多項式在特定值下的總和
  
[使用資料結構與演算法]

資料結構：累加變數

演算法：遍歷多項式，對每一項呼叫pow並乘以係數，累加至sum

[時間與空間複雜度]

時間複雜度：O(A) //遍歷多項式

空間複雜度：O(1)

[須注意的事]

* pow 函式運算需注意精度問題
### (18) operator>>(istream& is, Polynomial& poly) 輸入
[設計原因]

* 輸入讀取多項式，讀取項數，再依次讀取每一項的係數與指數
  
[使用資料結構與演算法]

資料結構：輸入串流

演算法：先呼叫clear清空舊資料，再跑迴圈append新增項次

[時間與空間複雜度]

時間複雜度：O(n) //迴圈至n

空間複雜度：O(n) //n個節點

[須注意的事]

* 必須先清空多項式，否則會變成附加新輸入到舊資料後方
* 假設輸入已排序，否則append後可能需要額外排序 
### (19) operator<<(ostream& os, const Polynomial& poly) 輸出
[設計原因]

* 將多項式以人類可讀的格式輸出，需處理符號、係數為1省略、指數為0或1的特殊顯示規則
  
[使用資料結構與演算法]

資料結構：輸出串流

演算法：遍歷串列

[時間與空間複雜度]

時間複雜度：O(A) //遍歷串列

空間複雜度：O(1)

[須注意的事]

* 若多項式為空，應輸出0

### 程式改進
* 針對原有Evaluate()、Mult()、~Chain()做出改進
#### Evaluate()—改用霍納法則
```c++
float Polynomial::Evaluate(float x)const{
    if(terms.begin()==terms.end())return 0;
    ChainIterator<Term> it=terms.begin();
    float result=it->coef; //初始為最高次項係數
    int currentExp=it->exp;
    ++it;
    for (;it!=terms.end();++it){
        result=result*pow(x,currentExp-it->exp)+it->coef;//計算指數差額
        currentExp=it->exp;
    }
    //補足最後一項到x^0的次數
    result*=pow(x,currentExp);
    return result;
}
```
#### [原有問題]
* 當指數很大時，pow要重複計算且效率低
* 容易產生浮點數誤差
#### [優化部分]

1.結合律提取公因數(霍納法則)

2.pow的指數參數從次方數優化為項次間的差值

結論:

* 時間複雜度降低為線性時間
* 減少了乘法運算的總次數，數值精確度提高
#### Mult()—拿掉臨時變數直接存結果進去
```c++
Polynomial Polynomial::operator*(const Polynomial& b)const{
    Polynomial c;
    if(terms.begin()==terms.end()||b.terms.begin()==b.terms.end())
		return c;
    for(ChainIterator<Term> ait=terms.begin();ait!=terms.end();++ait){
        for(ChainIterator<Term> bit=b.terms.begin();bit!= b.terms.end();++bit){
            float newCoef=ait->coef*bit->coef;
            int newExp=ait->exp+bit->exp;
			if (newCoef == 0) continue; // 係數為 0 不處理
            ChainNode<Term>* prev = c.terms.head;     
            ChainNode<Term>* curr = c.terms.head->next; 
            while(curr!=c.terms.head&&curr->element.exp>newExp){
                prev=curr;
                curr=curr->next;
            }
            if(curr!=c.terms.head&&curr->element.exp==newExp){
                curr->element.coef+=newCoef;
            }else{
                ChainNode<Term>* newNode=AvailableList<Term>::getNode();
                newNode->element=Term(newCoef,newExp);
                newNode->next=curr;
                prev->next=newNode;
                if(curr==c.terms.head){
                    c.terms.last=newNode;
				}
			}
        }
    }
    return c;
}
```
#### [原有問題]
* 原本的寫法在迴圈裡建立了一個temp多項式，完成計算把temp加到c，最後刪除temp，太過耗時
#### [優化部分]
1.不建立temp，算出係數和指數後，直接把這個新項次插入到c的正確位置

結論:

* 省下大量的時間，不用反覆建立和刪除物件
#### ~Chain()—利用last指標回收資源
```c++
static void getBack(ChainNode<T>* firstNode,ChainNode<T>* lastNode){
    if(!firstNode)
		return;
    lastNode->next=head; 
    head=firstNode;
}
~Chain(){
    if(head){
        if(head->next!=head){
            ChainNode<T>* first=head->next; 
            last->next=nullptr;
            AvailableList<T>::getBack(first,last); 
        }
        head->next=nullptr;
        AvailableList<T>::getBack(head); 
    }
    head=nullptr;
    last=nullptr;
}
```
#### [原有問題]
* 原本在回收串列時，get函式為了要把這串東西返還av list，必須從頭跑到尾找出最後一個節點
#### [優化部分]
1.我們的 Chain 類別裡本來就有存last。直接把頭跟尾都傳給回收函式，它就能馬上接好不用再跑迴圈

結論:
* 時間複雜度變為O(1)
