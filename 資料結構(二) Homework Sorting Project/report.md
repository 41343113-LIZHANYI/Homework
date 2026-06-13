# 41343113 李展毅 41343144 廖峻德 (第六組 Sorting Project)
## 解題說明
### 問題描述
1. 實作四種指定的排序演算法：Insertion Sort, Quick Sort (需使用 Median-of-three), Merge Sort (需使用 Iterative method), Heap Sort。
2. 測量以上四種演算法在最壞情況 (Worst-case) 與平均情況 (Average-case) 下的執行時間。測資大小 n 分別為 500, 1000, 2000, 3000, 4000, 5000。
3. 若執行時間低於時鐘精確度，需重複執行多次取平均以獲得精確數值。
4. 根據測量結果，設計一個在「最壞情況標準 (Worst-time criterion)」下表現最優的複合排序函式 (Composite Sort)。
5. 將所有測試結果繪製成趨勢圖，並撰寫效能分析報告。

### 解題策略
#### (1) 最壞測資產生策略
1. **Insertion Sort:** 直接產生反向遞減數列 (n 到 1)。
2. **Merge Sort:** 實作 `unmerge` 函式，利用遞迴將陣列切半，偶數索引放左邊、奇數索引放右邊進行反向交叉。強迫演算法在合併時發生最多次數的交錯比對。
3. **Quick Sort & Heap Sort:** 依題目要求，使用 `randData` 隨機產生 20 組測資，並取執行時間之最大值 (Max) 來逼近最壞情況。

#### (2) 克服時鐘精確度
針對 Insertion 與 Merge 的最壞情況測試，在外層套用迴圈跑 100 次，最後將總耗時除以 100，藉此消除 `<chrono>` 函式庫在處理極小測資時的測量誤差。

#### (3) 複合排序 (Composite Sort) 設計
為保證 Worst-case 效能，底層不使用可能退化成 $O(n^2)$ 的 Quick Sort。而是將大陣列切割為長度 20 的小區塊 (Block)，針對每個區塊執行常數時間極小的 Insertion Sort，最後再以 Iterative Merge Sort 疊加合併。此舉能大幅省去深層遞迴與處理極小陣列的記憶體開銷。

## 程式實作
以下為主要程式碼：

```cpp
#include<iostream>
#include<chrono>
#include<iomanip>
#include<algorithm>
#include<cstdlib>
using namespace std;
using namespace std::chrono;

// 插入排序
void insertSort(int* a,int n){
    for(int i=1;i<n;++i){
        int t=a[i];
        int j=i-1;
        while(j>=0&&a[j]>t){
            a[j+1]=a[j];
            j--;
        }
        a[j+1]=t;
    }
}

// 三數取中法
int med3(int* a,int l,int r){
    int m=(l+r)/2;
    if(a[l]>a[m])
        swap(a[l],a[m]);
    if(a[l]>a[r])
        swap(a[l],a[r]);
    if(a[m]>a[r])
        swap(a[m],a[r]);
    swap(a[m],a[r-1]);
    return a[r-1];
}

//Quick Sort遞迴主體
void qSortRec(int* a,int l,int r){
    if(l+10<=r){
        int p=med3(a,l,r);
        int i=l;
        int j=r-1;
        while(1){
            while(a[++i]<p);
            while(a[--j]>p);
            if(i<j)
                swap(a[i],a[j]);
            else
                break;
        }
        swap(a[i],a[r-1]);
        qSortRec(a,l,i-1);
        qSortRec(a,i+1,r);
    }else{
        insertSort(a+l,r-l+1);
    }
}

void quickSort(int* a,int n){
    qSortRec(a,0,n-1);
}

//合併兩個已排序的子陣列
void merge(int* a,int* tmp,int l,int r,int rend){
    int lend=r-1;
    int p=l;
    int num=rend-l+1;
    while(l<=lend&&r<=rend){
        if(a[l]<=a[r])
            tmp[p++]=a[l++];
        else
            tmp[p++]=a[r++];
    }
    while(l<=lend)
        tmp[p++]=a[l++];
    while(r<=rend)
        tmp[p++]=a[r++];
    for(int i=0;i<num;++i,rend--)
        a[rend]=tmp[rend];
}

// 迭代 Merge Sort
void mergeSort(int* a,int n){
    int* tmp=new int[n];
    for(int w=1;w<n;w*=2)
        for(int i=0;i<n;i+=2*w)
            if(min(i+w,n)<n)
                merge(a,tmp,i,min(i+w,n),min(i+2*w-1,n-1));
    delete[] tmp;
}

// 向下沉降
void percDown(int* a,int i,int n){
    int c;
    int t=a[i];
    for(;2*i+1<n;i=c){
        c=2*i+1;
        if(c!=n-1&&a[c]<a[c+1])
            c++;
        if(t<a[c])
            a[i]=a[c];
        else
            break;
    }
    a[i]=t;
}

// Heap Sort 主體
void heapSort(int* a,int n){
    for(int i=n/2-1;i>=0;--i)
        percDown(a,i,n);
    for(int j=n-1;j>0;--j){
        swap(a[0],a[j]);
        percDown(a,0,j);
    }
}

// 複合排序 (底層Insertion搭配上層Merge)
void compSort(int* a,int n){
    int b=20;
    for(int i=0;i<n;i+=b)
        insertSort(a+i,min(b,n-i));
    int* tmp=new int[n];
    for(int w=b;w<n;w*=2)
        for(int i=0;i<n;i+=2*w)
            if(min(i+w,n)<n)
                merge(a,tmp,i,min(i+w,n),min(i+2*w-1,n-1));
    delete[] tmp;
}

// 產生 Insertion Sort 的 Worst-case
void worstInsert(int* a,int n){
    for(int i=0;i<n;++i)
        a[i]=n-i;
}

// 遞迴反向交叉
void unmerge(int* a,int l,int r,int* tmp){
    if(l>=r)
        return;
    int m=l+(r-l)/2;
    int p=0;
    for(int i=l;i<=r;i+=2)
        tmp[p++]=a[i];
    for(int i=l+1;i<=r;i+=2)
        tmp[p++]=a[i];
    for(int i=l;i<=r;++i)
        a[i]=tmp[i-l];
    unmerge(a,l,m,tmp);
    unmerge(a,m+1,r,tmp);
}

// 產生 Merge Sort 的 Worst-case
void worstMerge(int* a,int n){
    for(int i=0;i<n;++i)
        a[i]=i+1;
    int* tmp=new int[n];
    unmerge(a,0,n-1,tmp);
    delete[] tmp;
}

// 產生隨機排列
void randData(int* a,int n){
    for(int i=0;i<n;++i)
        a[i]=i+1;
    for(int i=n-1;i>=1;i--)
        swap(a[rand()%(i+1)],a[i]);
}

// 計算 Worst-case 執行時間
double getWorst(void(*f)(int*,int),int* a,int n,int type){
    double maxT=0;
    if(type<2){
        auto st=high_resolution_clock::now();
        for(int r=0;r<100;++r){
            if(type==0)
                worstInsert(a,n);
            else
                worstMerge(a,n);
            f(a,n);
        }
        return duration<double,milli>(high_resolution_clock::now()-st).count()/100.0;
    }
    for(int p=0;p<20;++p){
        randData(a,n);
        auto st=high_resolution_clock::now();
        f(a,n);
        double d=duration<double,milli>(high_resolution_clock::now()-st).count();
        if(d>maxT)
            maxT=d;
    }
    return maxT;
}

// 計算 Average-case 執行時間
double getAvg(void(*f)(int*,int),int* a,int n){
    auto st=high_resolution_clock::now();
    for(int p=0;p<50;++p){
        randData(a,n);
        f(a,n);
    }
    return duration<double,milli>(high_resolution_clock::now()-st).count()/50.0;
}

int main(){
    srand(time(0));
    int sz[]={500,1000,2000,3000,4000,5000};
    
    cout<<"--- Worst-case (ms) ---\n";
    cout<<setw(6)<<"n"<<setw(12)<<"Insert"<<setw(12)<<"Merge"<<setw(12)<<"Quick"<<setw(12)<<"Heap"<<setw(12)<<"Comp\n";
    for(int n:sz){
        int* a=new int[n];
        cout<<fixed<<setprecision(4)<<setw(6)<<n;
        cout<<setw(12)<<getWorst(insertSort,a,n,0);
        cout<<setw(12)<<getWorst(mergeSort,a,n,1);
        cout<<setw(12)<<getWorst(quickSort,a,n,2);
        cout<<setw(12)<<getWorst(heapSort,a,n,2);
        cout<<setw(12)<<getWorst(compSort,a,n,1)<<"\n";
        delete[] a;
    }
    
    cout<<"\n--- Average-case (ms) ---\n";
    cout<<setw(6)<<"n"<<setw(12)<<"Insert"<<setw(12)<<"Merge"<<setw(12)<<"Quick"<<setw(12)<<"Heap"<<setw(12)<<"Comp\n";
    for(int n:sz){
        int* a=new int[n];
        cout<<setw(6)<<n;
        cout<<setw(12)<<getAvg(insertSort,a,n);
        cout<<setw(12)<<getAvg(mergeSort,a,n);
        cout<<setw(12)<<getAvg(quickSort,a,n);
        cout<<setw(12)<<getAvg(heapSort,a,n);
        cout<<setw(12)<<getAvg(compSort,a,n)<<"\n";
        delete[] a;
    }
    return 0;
}
```
## 效能分析
#### 理論時間與空間複雜度
1. **Insertion Sort:**
   * 時間複雜度：最壞與平均皆為 $O(n^2)$。
   * 空間複雜度：$O(1)$ (In-place)。
2. **Quick Sort (Median-of-three):**
   * 時間複雜度：平均 $O(n \log n)$，最壞情況理論上為 $O(n^2)$，但在三數取中法保護下極難退化。
   * 空間複雜度：$O(\log n)$ (遞迴堆疊)。
3. **Merge Sort (Iterative):**
   * 時間複雜度：最壞與平均皆保證為 $O(n \log n)$。
   * 空間複雜度：$O(n)$ (需額外陣列 `tmp`)。
4. **Heap Sort:**
   * 時間複雜度：最壞與平均皆保證為 $O(n \log n)$。
   * 空間複雜度：$O(1)$ (In-place)。
5. **Composite Sort:**
   * 時間複雜度：最壞與平均皆為 $O(n \log n)$，但常數時間小於純粹的 Merge Sort。
   * 空間複雜度：$O(n)$ (需額外陣列 `tmp`)。

### 測試案例

#### 圖表輸出

<img width="1751" height="831" alt="image" src="https://github.com/user-attachments/assets/eaba2fb6-d5ae-433d-b1c5-d600fac821ab" />

<img width="1745" height="830" alt="image" src="https://github.com/user-attachments/assets/08dcd625-f3a2-44df-8cea-ea80eda13c54" />
#### 實際輸出 (執行時間表 ms)

**Worst-case (ms)**
| n | Insert | Merge | Quick | Heap | Comp |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 500 | 0.2703 | 0.0462 | 0.0567 | 0.1074 | 0.0420 |
| 1000 | 1.1602 | 0.1032 | 0.1019 | 0.1396 | 0.0929 |
| 2000 | 4.3181 | 0.2168 | 0.2735 | 0.3542 | 0.1998 |
| 3000 | 10.1749 | 0.3439 | 0.2795 | 0.4506 | 0.3054 |
| 4000 | 17.2094 | 0.4575 | 0.3487 | 0.6962 | 0.4082 |
| 5000 | 25.8687 | 0.6052 | 0.4369 | 0.7405 | 0.5341 |

**Average-case (ms)**
| n | Insert | Merge | Quick | Heap | Comp |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 500 | 0.1406 | 0.0677 | 0.0398 | 0.0544 | 0.0661 |
| 1000 | 0.6135 | 0.1267 | 0.0893 | 0.1189 | 0.1100 |
| 2000 | 2.2960 | 0.2600 | 0.2018 | 0.2661 | 0.2257 |
| 3000 | 4.8889 | 0.3645 | 0.3162 | 0.4078 | 0.3220 |
| 4000 | 8.5367 | 0.5249 | 0.4252 | 0.5421 | 0.4565 |
| 5000 | 13.0953 | 0.6567 | 0.5205 | 0.7010 | 0.5506 |

### 結論
1. **$O(n^2)$ 成長曲線驗證：** 如圖表藍線所示，Insertion Sort 呈現拋物線暴增，在 n=5000 時高達 25.8ms，完美印證了 $O(n^2)$ 處理大量資料的劣勢。
2. **$O(n \log n)$ 家族壓制力：** Merge, Quick, Heap 等演算法面對大資料量時，皆維持在平緩的線性對數增長（全低於 1ms內），與 Insertion Sort 形成鮮明對比。
3. **Quick Sort 未退化之因：** 雖然理論上存在 $O(n^2)$ 致命缺陷，但實作 Median-of-three 完美防範了反向或已排序的極端測資。在隨機抽樣 20 次取 Max 的規則下，極難觸發真實退化，使其憑藉優秀的 Cache Locality 仍保持高速。
4. **複合排序 (Composite Sort) 的成功：** 從圖表中可明顯看出紫色虛線 (Comp) 穩穩壓在橘色實線 (Merge) 下方。這證明了「底層分塊交由 Insertion Sort 處理」確實能省去深層遞迴與處理極小陣列的額外開銷，達成效能超越。

## 申論及開發報告
### 程式分析
##### [使用資料結構與演算法]
* **Worst-case Generator (unmerge):** 這是 Merge Sort 最困難的部分。程式利用反向推導邏輯，遞迴將陣列分為偶數與奇數索引分置左右，強制使合併過程中發生最多次的跨區段元素比對。
* **Composite Sort:** 融合 $O(n^2)$ 常數極小的特性與 $O(n \log n)$ 的穩定保證，實作類似實務上的 Timsort 區塊優化邏輯。
##### [須注意的事]
* **時鐘精確度 (Clock accuracy):** 面對 n=500 或小陣列時，單次執行時間極短，容易被計時器判定為 0。故在 `getWorst` 函式中採用迴圈執行 100 次再取平均，有效解決精確度問題。
* **記憶體開銷與 In-place 特性:** Merge Sort 雖然能穩定保證 $O(n \log n)$，但每次遞迴或合併皆須要 `new` 出額外空間，並反覆複製資料。這巨大的記憶體搬運成本，是其常數時間大於 Quick Sort 的主因。

### 程式改進
#### Composite Sort — 底層分塊優化
##### [原有問題]
直覺上的複合排序常寫為以下形式，只在最外層判斷一次：
```cpp
void compSort(int* a,int n){
    if(n<20)
        insertSort(a,n);
    else
        mergeSort(a,n);
}
```
但此設計導致測資 $n \ge 500$ 時，程式完全退化為純 Merge Sort，無法發揮任何加速效果。
##### [優化部分]
改寫排序邏輯，預先將整個大陣列切割成 Size=20 的小區塊 (Block)。
對每一個小區塊優先執行常數極小的 Insertion Sort 進行局部排序，最後再用 Iterative Merge Sort 疊加合併這些已排序的區塊。
```c++
void compSort(int* a,int n){
    int b=20;
    for(int i=0;i<n;i+=b)
        insertSort(a+i,min(b,n-i));
    int* tmp=new int[n];
    for(int w=b;w<n;w*=2)
        for(int i=0;i<n;i+=2*w)
            if(min(i+w,n)<n)
                merge(a,tmp,i,min(i+w,n),min(i+2*w-1,n-1));
    delete[] tmp;
}
```
##### [結論]
優化後的複合排序在 Worst-case (n=5000) 時，時間從 0.6052ms 降至 0.5341ms，成功且穩定地擊敗了純粹的 Merge Sort。此改進完美契合了實務上 Timsort 截長補短的核心精神。
