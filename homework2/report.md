# 41343113

作業二

## 解題說明
### 問題描述
實作Polynomial的類別

以下為題目要求:
* 建構Polynomial物件的建構子
* 將Polynomial物件相加並回傳一個Polynomial物件的Add成員函式
* 將Polynomial物件相乘並回傳一個Polynomial物件的Mult成員函式
* 將Polynomial物件帶入x值並回傳結果值的Eval成員函式
* 實作Polynomial的輸入和輸出的運算子重載
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
#### (1) Ackermann 函數 — 遞迴
  * 時間複雜度：O(A(m,n))
  * 空間複雜度：O(A(m,n))
#### (2) Ackermann 函數 — 非遞迴
  * 時間複雜度：O(1) 或 O(log n),when m=3
  * 空間複雜度：O(1)
#### (3) 子集合生成 — 遞迴
  * 時間複雜度：O(n·2ⁿ) //長度為n的集合，每次取或不取=2ⁿ，n為重建now字串
  * 空間複雜度：O($n^2$) // 每層now都要複製，最多n層，每層now最長=n
## 測試與驗證

### 測試案例

#### (1) Ackermann 函數 — 遞迴
|測試案例|輸入參數 $m$|輸入參數 $n$|預期輸出|實際輸出|
|----------|--------------|----------|----------|----------|
|測試一|$m=0$|$n=1$|2|2|
|測試二|$m=1$|$n=1$|3|3|
|測試三|$m=2$|$n=1$|5|5|
|測試四|$m=3$|$n=1$|13|13|
|測試五|$m=3$|$n=12$|32765|32765|
|測試六|$m=3$|$n=13$|65533|異常拋出|
|測試七|$m=4$|$n=1$|65533|異常拋出|
|測試八|$m=-1$|$n=0$|異常拋出|異常拋出|
|測試九|$m=0$|$n=-1$|異常拋出|異常拋出|
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
