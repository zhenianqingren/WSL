#ifndef BINNODE
#define BINNODE
#define BinNodePosi(T) BinNode<T> *         //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（不“空树高度为-1”的约定相统一）
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lChild))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x) || HasRChild(x))     //至少拥有一个孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //同时拥有两个孩子
#define IsLeaf(x) (!HasChild(x))
#define sibling(p) ( \
    IsLChild(*(p)) ? (p)->parent->rChild : (p)->parent->lChild) //兄弟
#define uncle(x) ( \
    IsLChild(*((x)->parent)) ? (x)->parent->parent->rChild : (x)->parent->parent->lChild) //叔叔

#define FromParentTo(x) ( \
    IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lChild : (x).parent->rChild)) //来自父亲的指针 可以改变父亲的某个指针的指向
#define max(x, y) x > y ? x : y
#include <stack>
#include <queue>
#include <random>
using namespace std;
typedef enum
{
    RB_RED,
    RB_BLACK
} RBColor; //节点颜色

template <typename T>
struct BinNode
{           //二叉树节点模板类
    T data; //数值
    BinNodePosi(T) parent;
    BinNodePosi(T) lChild;
    BinNodePosi(T) rChild; //父节点及左、右孩子
    int height;            //高度（通用）
    int npl;               // Null Path Length（左式堆，也可直接用height代替）
    RBColor color;         //颜色（红黑树）
    // 构造函数
    BinNode() : parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED) {}
    BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
            int h = 0, int l = 1, RBColor c = RB_RED)
        : data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) {}
    // 操作接口
    int size();                           //统计当前节点后代总数，亦即以其为根的子树的规模
    BinNodePosi(T) insertAsLC(T const &); //作为当前节点的左孩子插入新节点
    BinNodePosi(T) insertAsRC(T const &); //作为弼前节点的右孩子插入新节点
    BinNodePosi(T) succ();                //返回当前节点的直接后继
    template <typename VST>
    void travLevel(VST &); //子树层次遍历
    template <typename VST>
    void travPre(VST &); //子树先序遍历
    template <typename VST>
    void travIn(VST &); //子树中序遍历
    template <typename VST>
    void travPost(VST &); //子树后序遍历
    // 比较器、判别器（各列其一，其余自行补充）
    bool operator<(BinNode const &bn) { return data < bn.data; }   //小于
    bool operator==(BinNode const &bn) { return data == bn.data; } //等于
};

template <typename T> //将e作为当前节点的左孩子插入二叉树
BinNodePosi(T) BinNode<T>::insertAsLC(T const &e)
{
    return lChild = new BinNode(e, this);
}

template <typename T> //将e作为当前节点的右孩子插入二叉树
BinNodePosi(T) BinNode<T>::insertAsRC(T const &e)
{
    return rChild = new BinNode(e, this);
}

template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    visit(x->data);
    travPre_R(x->lChild, visit);
    travPre_R(x->rChild, visit);
}

template <typename T, typename VST>
void visitAlongLeftBranch(BinNodePosi(T) root, VST &visit, stack<BinNodePosi(T)> &s)
{
    while (root)
    {
        visit(root->data);
        s.push(root->rChild);
        root = root->lChild;
    }
}
template <typename T, typename VST>
void travPre_I1(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> s;
    while (true)
    {
        visitAlongLeftBranch(x, visit, s);
        if (s.empty())
            break;
        x = s.top();
        s.pop();
    }
}

template <typename T>
template <typename VST> //元素类型、操作器
void BinNode<T>::travPre(VST &visit)
{ //二叉树中序遍历算法统一入口
    switch (rand() % 5)
    { //此处暂随机选择以做测试，共五种选择
    case 1:
        travPre_I1(this, visit);
        break; //迭代版#1
    case 2:
        travPre_I2(this, visit);
        break; //迭代版#2
    case 3:
        travPre_I3(this, visit);
        break; //迭代版#3
    case 4:
        travPre_I4(this, visit);
        break; //迭代版#4
    default:
        travPre_R(this, visit);
        break; //递归版
    }
}

template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travPre_R(x->lChild, visit);
    visit(x->data);
    travPre_R(x->rChild, visit);
}

template <typename T, typename VST>
void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)> &s)
{
    while (x)
    {
        s.push(x);
        x = x->lChild;
    }
}

template <typename T, typename VST>
void travIn_I1(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> s;
    while (true)
    {
        goAlongLeftBranch(x, visit);
        if (s.empty())
            break;
        x = s.top();
        s.pop();
        visit(x->data);
        x = x->rChild;
    }
}

template <typename T>
BinNodePosi(T) BinNode<T>::succ()
{
    BinNodePosi(T) s = this;
    if (rChild)
    {
        s = rChild;
        while (HasLChild(*s))
            s = s->lChild;
    }
    else
    {
        while (IsRChild(*s))
            s = s->parent;
        s = s->parent;
    }
}

template <typename T, typename VST>
void travIn_I2(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> s;
    while (true)
    {
        if (x)
        {
            s.push(x);
            x = x->lChild;
        }
        else
        {
            if (s.empty())
                break;
            x = s.top();
            s.pop();
            visit(x->data);
            x = x->rChild;
        }
    }
}

template <typename T, typename VST>
void travIn_I3(BinNodePosi(T) x, VST &visit)
{
    bool backtrack = false;
    while (true)
    {
        if (!backtrack && HasLChild(*x))
            x = x->lChild;
        else
        {
            visit(x->data);
            if (HasRChild(*x))
            {
                x = x->RChild;
                backtrack = false;
            }
            else
            {
                if (!(x = x->succ()))
                    break;
                backtrack = true;
            }
        }
    }
}

template <typename T>
template <typename VST> //元素类型、操作器
void BinNode<T>::travIn(VST &visit)
{ //二叉树中序遍历算法统一入口
    switch (rand() % 5)
    { //此处暂随机选择以做测试，共五种选择
    case 1:
        travIn_I1(this, visit);
        break; //迭代版#1
    case 2:
        travIn_I2(this, visit);
        break; //迭代版#2
    case 3:
        travIn_I3(this, visit);
        break; //迭代版#3
    case 4:
        travIn_I4(this, visit);
        break; //迭代版#4
    default:
        travIn_R(this, visit);
        break; //递归版
    }
}

template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travPre_R(x->lChild, visit);
    travPre_R(x->rChild, visit);
    visit(x->data);
}

template <typename T>
void gotoHLVFL(stack<BinNodePosi(T)> &s)
{
    while (BinNodePosi(T) x = s.top())
    {
        if (HasLChild(*x))
        {
            if (HasRChild(*x))
                s.push(x->rChild);
            s.push(x->lChild);
        }
        else
            s.push(x->rChild);
    }
}

template <typename T, typename VST>
void travPost_I1(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> s;
    if (x)
        s.push(x);
    while (!s.empty())
    {
        if (s.top() != x->parent)
            gotoHLVFL(s);
        x = s.top();
        s.pop();
        visit(x->data);
    }
}

template <typename T>
template <typename VST> //元素类型、操作器
void BinNode<T>::travPost(VST &visit)
{ //二叉树中序遍历算法统一入口
    switch (rand() % 5)
    { //此处暂随机选择以做测试，共五种选择
    case 1:
        travPost_I1(this, visit);
        break; //迭代版#1
    case 2:
        travPost_I2(this, visit);
        break; //迭代版#2
    case 3:
        travPost_I3(this, visit);
        break; //迭代版#3
    case 4:
        travPost_I4(this, visit);
        break; //迭代版#4
    default:
        travPost_R(this, visit);
        break; //递归版
    }
}

template <typename T>
template <typename VST>
void BinNode<T>::travLevel(VST &visit)
{
    queue<BinNodePosi(T)> q;
    q.push(this);
    BinNodePosi(T) temp = nullptr;
    while (!q.empty())
    {
        temp = q.front();
        q.pop();
        visit(temp->data);
        if (HasLChild(*temp))
            q.push(temp->lChild);
        if (HasRChild(*temp))
            q.push(temp->rChild);
    }
}
#endif