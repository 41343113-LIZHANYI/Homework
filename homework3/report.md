# 41343113

作業三

## 解題說明
### 問題描述
使用帶 header 節點的環狀鏈結串列，每個節點都含有以下
三個資料(coef,exp,link)來實作Polynomial類別

以下為題目要求:
* (a) 輸入運算子多載
* (b) 輸出運算子多載
* (c) 複製建構子
* (d) 指派運算子
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
