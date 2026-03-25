#include<iostream>
#include<cmath> //需要log2
#include<cstdlib> //隨機數值
#include<ctime> //取得當前系統時間
#include<utility> //pair<>
using namespace std;
template<class K,class E>
class Dictionary{ //BST的ADT 
public:
    virtual ~Dictionary(){} //解構
    virtual bool IsEmpty() const=0; //是否為空
    virtual pair<K, E>* Get(const K&) const=0; //取得Key對應的指標
    virtual void Insert(const pair<K, E>&)=0; //插入Key和值
    virtual void Delete(const K&)=0; //刪除指定Key值的節點
};
template<class K, class E>
class TreeNode{
public:
    TreeNode* left; //左子樹指標
    TreeNode* right; //右子樹指標
    pair<K, E> data; //鍵值對資料
    TreeNode(pair<K, E> e){ //建構子初始化
        this->data.first=e.first; 
        this->data.second=e.second; 
        this->left=NULL; 
        this->right=NULL; 
    }
};
template<class K, class E>
class BSTDictionary:public Dictionary<K, E>{
private:
    TreeNode<K, E>* root; //字典樹根節
    TreeNode<K, E>* insert(TreeNode<K, E>* node,const pair<K, E>& e){ 
        if(!node)
            return new TreeNode<K, E>(e); //到底則建立新節點
        if(e.first<node->data.first)
            node->left=insert(node->left,e); //小於往左
        else if(e.first>node->data.first)
            node->right=insert(node->right,e); //大於往右
        else node->data.second=e.second; //若鍵值重複則更新元素內容
        return node; //回傳當前節點
    }
    int getHeight(TreeNode<K, E>* node) const{ //遞迴取得高度 part a 要求
        if(!node)
            return 0; //遞迴結束
        return 1+max(getHeight(node->left), getHeight(node->right)); //遞迴取最大子樹高+1
    }
    TreeNode<K, E>* findMin(TreeNode<K, E>* node){ 
        if(node->left==NULL) 
            return node; //找到葉節點
        else 
            return findMin(node->left); //不為空繼續往左跑
    }
    TreeNode<K, E>* remove(TreeNode<K, E>* node,const K& k){ 
        if(!node) 
            return node; //沒找到
        if(k<node->data.first)
            node->left=remove(node->left,k); //遞迴左子樹
        else if(k>node->data.first) 
            node->right=remove(node->right,k); //遞迴右子樹
        else{
            if(!node->left){ //若無左子樹
                TreeNode<K, E>* temp=node->right; //暫存右子樹
                delete node; //刪除節點
                return temp; //回傳右子樹接上
            }else if(!node->right){ //若無右子樹
                TreeNode<K, E>* temp=node->left; //暫存左子樹
                delete node; //刪除節點
                return temp; //回傳左子樹接上
            }
            TreeNode<K, E>* temp=findMin(node->right); //找右子樹中序後繼節點
            node->data=temp->data; //複製後繼節點資料至當前節點
            node->right=remove(node->right, temp->data.first); //遞迴刪除該後繼節點
        }
        return node; //回傳更新後的節點
    }
    pair<K, E>* get(TreeNode<K, E>* node,const K& k)const{ //遞迴查詢特定鍵值
        if(!node)
            return NULL; //找不到回傳空指標
        if(k<node->data.first)
            return get(node->left,k); //遞迴左子樹
        if(k>node->data.first)
            return get(node->right, k); //遞迴右子樹
        return &(node->data); //相等回傳指標
    }
    void destroy(TreeNode<K, E>* node){ 
        if(!node)
            return; //空則返回
        destroy(node->left); //清空左
        destroy(node->right); //清空右
        delete node; //刪除自己
    }
public:
    BSTDictionary():root(NULL){} 
    ~BSTDictionary(){destroy(root);} 
    bool IsEmpty() const override{return root==NULL;} 
    pair<K, E>* Get(const K& k) const override{return get(root,k);} 
    void Insert(const pair<K, E>& e) override{root=insert(root,e);} 
    void Delete(const K& k) override{root=remove(root,k);} 
    int getHeight() const{return getHeight(root);} 
};
int main(){
    srand(time(0)); //設定亂數種子
    int ns[]={100,500,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000};
    cout<<"n\tHeight\tRatio(Height/log2(n))\n"; //輸出col名 
    for(int n:ns){ 
        BSTDictionary<int, int> dict; 
        for(int i=0;i<n;++i) { //n次插入
            int rnd=rand(); //產生亂數
            dict.Insert(make_pair(rnd, rnd)); //使用pair插入鍵與值
        }
        int height=dict.getHeight(); 
        double ratio=height/log2(n); //比值
        cout<<n<<"\t"<<height<<"\t"<<ratio<<"\n"; 
    }
    return 0;
}