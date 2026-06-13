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

// Quick Sort 遞迴主體
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

// 合併兩個已排序的子陣列
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

// 迭代版 Merge Sort
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

// 真・複合排序 (底層塊狀 Insertion 搭配上層 Merge)
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

// 產生插入排序的Worst case
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

//產生Merge的 Worstcase
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

// 計算最差執行時間
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

// 計算平均執行時間
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