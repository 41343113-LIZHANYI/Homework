#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
class Polynomial; 
class Term {
    friend Polynomial;
    friend ostream& operator<<(ostream& os,const Polynomial& poly);
private:
    float coef; //�Y��
    int exp; //����
};
class Polynomial {
private:
    Term *termArray; //���Ы��V�}�C�Ŷ� 
    int capacity; //�}�C�j�p 
    int terms; //�D�s���ƶq
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
Polynomial::Polynomial(){//�غc�l
        termArray=NULL;   //p(x)=0���n�D 
        capacity=0;
        terms=0;
}
Polynomial::Polynomial(const Polynomial &copy) {
    terms=copy.terms;
    capacity=copy.capacity;
    termArray=new Term[capacity]; //���t�s�Ŷ��A�קK��ӳ����V�P�ӪŶ� 
    for (int i=0;i<terms;++i)
        termArray[i]=copy.termArray[i]; //�v�@�ƻs 
}
Polynomial& Polynomial::operator=(Polynomial copy){ // �w�غc������ ��ȹB��l����  
    swap(termArray,copy.termArray); //����swap(�ƻs�غc�l�ʺA�t�m���s�Ŷ� ) 
    swap(capacity,copy.capacity);  
    swap(terms,copy.terms);  
    return *this;
}
void Polynomial::newTerm(float c,int e){
    if(c==0) 
		return;
    if(terms==capacity){//�p�G�Ŷ����� 
    	if(capacity)
        	capacity*=2; //�w�]capacity*2
        else
        	capacity=1; //�קK��غccapacity=0 
        Term *temp=new Term[capacity]; //�ʺA�t�m�s�Ŷ� 
        copy(termArray,termArray+terms,temp); //�ƻs 
        delete []termArray; //�R���ªŶ� 
        termArray=temp; //���V�s�Ŷ� 
    }
    termArray[terms].coef=c; //�[�Jc,e��̫� 
    termArray[terms++].exp=e;
}
Polynomial Polynomial::Add(Polynomial poly){
    Polynomial c; //�sArray c 
    int aPos = 0, bPos = 0; //a�Mb������ 
    while(aPos<terms && bPos<poly.terms){ //����a��b�M���� 
        if(termArray[aPos].exp==poly.termArray[bPos].exp){ //a�Mb���Ƭ۵�  
            float sum=termArray[aPos].coef+poly.termArray[bPos].coef;
            if(sum)//�P�_�����ۥ[�Y�ƬO�_��0 
                c.newTerm(sum,termArray[aPos].exp);//�s�W��c 
            aPos++; //a,b���������s�W��c,a�Mb����++ 
			bPos++;
        } 
        else if(termArray[aPos].exp>poly.termArray[bPos].exp){ //a���Ƥ���j   
            c.newTerm(termArray[aPos].coef,termArray[aPos].exp);//�s�W��c 
            aPos++; //a�����s�W��c,a����++ 
        } 
        else{  //b���Ƥ���j 
            c.newTerm(poly.termArray[bPos].coef,poly.termArray[bPos].exp);//�s�W��c 
            bPos++; //b�����s�W��c,b����++  
        }
    }
    //��a��b�S�M���������c 
    for (;aPos<terms;aPos++)
        c.newTerm(termArray[aPos].coef,termArray[aPos].exp);
    for (;bPos<poly.terms;bPos++)
        c.newTerm(poly.termArray[bPos].coef,poly.termArray[bPos].exp);
    return c;
}
Polynomial Polynomial::Mult(Polynomial poly){
    Polynomial c; //�sArray c 
    for (int i=0;i<terms;++i){ //i�M��a���� 
        Polynomial temp; //�{��Array temp 
        for (int j=0;j<poly.terms;++j){ //j�M��b���� 
            float newC=termArray[i].coef*poly.termArray[j].coef; //a*b�����Y�� 
            int newE=termArray[i].exp+poly.termArray[j].exp; //a*b�������� 
            temp.newTerm(newC,newE); //�����[��temp�� 
        }
        c=c.Add(temp); // ��Add()��C����X�Ӫ�temp�ۥ[ 
    }
    return c;
}
float Polynomial::Eval(float f){ //�N�ȱa�Jx���h�����B�� 
    float sum=0;
    for(int i=0;i<terms;++i)
        sum+=termArray[i].coef*pow(f,termArray[i].exp);//sum��pow�֥[ 
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
	if(poly.terms==0){ //�p�G�O0����return0 
        os<<"0";
        return os;
    }
    for (int i=0;i<poly.terms;++i) {
        if(i>0&&poly.termArray[i].coef>0)//���O�Ĥ@�ӿ�X�����B�Y�Ƥ����t�� 
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

