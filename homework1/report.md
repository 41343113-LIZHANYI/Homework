# 41343113

作業一

## 解題說明
### 問題描述
* **Problem 1:** 實作遞迴與非遞迴的 Ackermann 函數。
  |函數定義|條件|
  |:---:|:---:|
  | $A(m, n) = n + 1$|**$m=0$**|
  |$A(m,n)=A(m-1,1)$|**$n=0$**|
  |$A(m,n)=A(m-1,A(m,n-1))$|**$otherwise$**|
* **Problem 2:** 實作遞迴生成集合的所有子集合。
### 解題策略

#### (1) Ackermann 函數 — 遞迴
  - 由於輸出的增長速度超級快 使用unsigned long long
  - 當m=0時返回n+1作為遞迴結束條件
  - 主程式呼叫遞迴函式，並輸出遞迴回傳數值
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
#### (1) Ackermann 函數 — 遞迴
``` c++
#include <iostream>
using namespace std;
unsigned long long Ack(unsigned long long m,unsigned long long n){
    if(m==0)
        return n+1; //遞迴結束條件
    if(n==0)
        return Ack(m-1,1);
    return Ack(m-1,Ack(m,n-1));
}
int main(){
    int m,n;
    while(cin>>m>>n)
        cout<<Ack(m,n)<<'\n';
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
#### (3) 子集合生成 — 遞迴
```c++
#include <iostream>
#include <string>
using namespace std;
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
int main(){
    string s; 
    while(getline(cin,s)){
        string Sets="";
        for(char c:s) //迭代輸入字串
            if(c!=' '&&c!='\t'&&c!='\r'&&c!='\n') //如果字元為有效
                Sets+=c;
        if (Sets.empty()) 
            continue; //如果沒輸入就繼續    
        cout << "{"; //初始左大括號
        int len = Sets.length(); 
        for (int i=0;i<=len;++i) //依照子集合小到大生成
            allsubset(Sets,"",0,i); //now設為空
        cout<<"\b}\n\n"; //最後輸出多的逗號刪掉 最後右大括號 換行
    }
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
  * 時間複雜度：O(n·2ⁿ)
  * 空間複雜度：O(n)
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
### (1) Ackermann 函數 — 遞迴
#### 選擇遞迴的原因
1. 在本程式中，使用遞迴計算 Ackermann 函數主要原因如下：
   |函數定義|條件|
   |:---:|:---:|
   | $A(m, n) = n + 1$|**$m=0$**|
   |$A(m,n)=A(m-1,1)$|**$n=0$**|
   |$A(m,n)=A(m-1,A(m,n-1))$|**$otherwise$**|
   * 遞迴實作可直接對應數學公式，使邏輯一目了然，有利於團隊理解演算法意圖與驗證正確性。
2. 易於理解與實現
   遞迴避免手動管理堆疊或額外變數，每個函式呼叫即代表子問題可讀性高
   對程式競賽而言，程式碼短小且直觀，減少出錯機率，對專案協作，容易讓團隊快速理解與維護。
3. 遞迴語意清楚
   每次遞迴呼叫代表「子問題求解」，返回值逐層累加完成計算，這種設計降低中間狀態管理，對競程減少Bug，對專案增加程式可讀性。
#### 使用資料結構與演算法
* 資料結構：基本型別 unsigned long long 用來乘載快速增長的數值
* 演算法：遞迴算法對應數學公式
#### 注意事項
遞迴深度隨m,n增長，速度極快，容易堆疊溢出。
僅適用小範圍m,n，可加入例外處理判斷來提升穩定性。
### (2) Ackermann 函數 — 非遞迴
#### 選擇非遞迴的原因
1. 避免堆疊溢位，對大範圍 m,n 使用封閉公式直接計算
   |      |     |
   |:---:|:---:|
   |m=0|A(m,n)=n+1|
   |m=1|A(m,n)=n+2|
   |m=2|A(m,n)=2n+3|
   |m=3|A(m,n)=2^(n+3)-3|
   |m=4,5|小範圍n直接數值對應|
2. 運算效率比起遞迴高非常多
#### 使用資料結構與演算法
資料結構：unsigned long long+ullpow快速次方
演算法：條件判斷+封閉公式+位運算快速次方
#### 注意事項
超過 64-bit 範圍會溢位，需加明確提示或輸入限制
非遞迴版本適合大範圍快速計算
### (3) 子集合生成 — 遞迴
#### 選擇遞迴的原因
1. 回溯法直觀每個元素有選或不選兩種可能：
2. allsubset(s,now+s[i],i+1,sublen)
   回溯法概念，遞迴每個節點代表當前子集合，遍歷完成或達到目標長度則回溯上一層
3. 易於排序生成
#### 使用資料結構與演算法
資料結構：string存儲集合元素及子集合
演算法：遞迴+回溯法
#### 注意事項
遞迴深度與集合大小n線性相關n大時注意記憶體消耗
輸出格式需刪除多餘逗號，保持集合正確性
