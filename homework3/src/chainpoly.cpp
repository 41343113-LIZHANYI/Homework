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
