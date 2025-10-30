#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
class Polynomial; 
class Term {
    friend Polynomial;
    friend ostream& operator<<(ostream& os,const Polynomial& poly);
private:
    float coef; //係數
    int exp; //指數
};
class Polynomial {
private:
    Term *termArray; //指標指向陣列空間 
    int capacity; //陣列大小 
    int terms; //非零項數量
public:
    Polynomial();                  
    Polynomial(const Polynomial &copy);
    Polynomial& operator=(Polynomial copy);
	~Polynomial(){delete[] termArray;}
    Polynomial Add(Polynomial poly); 
    Polynomial Mult(Polynomial poly); 
    float Eval(float f);
    void newTerm(float c,int e);
    friend istream& operator>>(istream& is,Polynomial& poly);
    friend ostream& operator<<(ostream& os,const Polynomial& poly);
};
Polynomial::Polynomial(){//建構子
        termArray=NULL;   //p(x)=0的要求 
        capacity=0;
        terms=0;
}
Polynomial::Polynomial(const Polynomial &copy) {
    terms=copy.terms;
    capacity=copy.capacity;
    termArray=new Term[capacity]; //分配新空間，避免兩個都指向同個空間 
    for (int i=0;i<terms;++i)
        termArray[i]=copy.termArray[i]; //逐一複製 
}
Polynomial& Polynomial::operator=(Polynomial copy){ // 已建構的物件 賦值運算子重載  
    swap(termArray,copy.termArray); //直接swap(複製建構子動態配置的新空間 ) 
    swap(capacity,copy.capacity);  
    swap(terms,copy.terms);  
    return *this;
}
void Polynomial::newTerm(float c,int e){
    if(c==0) 
		return;
    if(terms==capacity){//如果空間不夠 
    	if(capacity)
        	capacity*=2; //預設capacity*2
        else
        	capacity=1; //避免剛建構capacity=0 
        Term *temp=new Term[capacity]; //動態配置新空間 
        copy(termArray,termArray+terms,temp); //複製 
        delete []termArray; //刪掉舊空間 
        termArray=temp; //指向新空間 
    }
    termArray[terms].coef=c; //加入c,e到最後 
    termArray[terms++].exp=e;
}
Polynomial Polynomial::Add(Polynomial poly){
    Polynomial c; //新Array c 
    int aPos = 0, bPos = 0; //a和b的索引 
    while(aPos<terms && bPos<poly.terms){ //直到a或b遍歷完 
        if(termArray[aPos].exp==poly.termArray[bPos].exp){ //a和b指數相等  
            float sum=termArray[aPos].coef+poly.termArray[bPos].coef;
            if(sum)//判斷項次相加係數是否為0 
                c.newTerm(sum,termArray[aPos].exp);//新增到c 
            aPos++; //a,b的項次都新增到c,a和b索引++ 
			bPos++;
        } 
        else if(termArray[aPos].exp>poly.termArray[bPos].exp){ //a指數比較大   
            c.newTerm(termArray[aPos].coef,termArray[aPos].exp);//新增到c 
            aPos++; //a項次新增到c,a索引++ 
        } 
        else{  //b指數比較大 
            c.newTerm(poly.termArray[bPos].coef,poly.termArray[bPos].exp);//新增到c 
            bPos++; //b項次新增到c,b索引++  
        }
    }
    //把a或b沒遍歷完的放到c 
    for (;aPos<terms;aPos++)
        c.newTerm(termArray[aPos].coef,termArray[aPos].exp);
    for (;bPos<poly.terms;bPos++)
        c.newTerm(poly.termArray[bPos].coef,poly.termArray[bPos].exp);
    return c;
}
Polynomial Polynomial::Mult(Polynomial poly){
    Polynomial c; //新Array c 
    for (int i=0;i<terms;++i){ //i遍歷a項次 
        Polynomial temp; //臨時Array temp 
        for (int j=0;j<poly.terms;++j){ //j遍歷b項次 
            float newC=termArray[i].coef*poly.termArray[j].coef; //a*b項次係數 
            int newE=termArray[i].exp+poly.termArray[j].exp; //a*b項次指數 
            temp.newTerm(newC,newE); //項次加到temp後 
        }
        c=c.Add(temp); // 用Add()把每次算出來的temp相加 
    }
    return c;
}
float Polynomial::Eval(float f){ //將值帶入x做多項式運算 
    float sum=0;
    for(int i=0;i<terms;++i)
        sum+=termArray[i].coef*pow(f,termArray[i].exp);//sum用pow累加 
    return sum;
}
istream& operator>>(istream& is,Polynomial& poly){
    float coef;
    int exp,n;
    is>>n;
    while(n--){
        is>>coef>>exp;
        poly.newTerm(coef,exp);
    }
    return is;
}
ostream& operator<<(ostream& os,const Polynomial& poly){
	if(poly.terms==0){ //如果是0直接return0 
        os<<"0";
        return os;
    }
    for (int i=0;i<poly.terms;++i) {
        if(i>0&&poly.termArray[i].coef>0)//不是第一個輸出項次且係數不為負數 
			os<<"+";
        os<<poly.termArray[i].coef<<"X^"<< poly.termArray[i].exp;
    }
    return os;
}
int main() {
    Polynomial A,B;
    float x;
    cin>>A;
    cin>>B;
    cin>>x;
    cout<<A<<'\n';
    cout<<B<<'\n';
    cout<<A.Add(B)<<'\n';
    cout<<A.Mult(B)<<'\n';
    cout<<A.Eval(x)<<'\n';
	cout<<B.Eval(x)<<'\n';
    return 0;
}

