#include "./BST.h"
// ephemeral data structure
// support accesss to all edition in history

// 树根必为黑色
// 外部节点：均为黑色
// 其余节点：若为红色 则它的后代必定为黑色
// 外部节点到根节点：途中经过的黑色节点数目相同

//提升各个红节点 使之与其黑父亲等高 于是每棵红黑树都对应于一棵(2,4)B-树
#define IsBlack(p) (!(p) || (RB_BLACK == (p)->color)) //外部节点也规作黑节点
#define IsRed(p) (!IsBlack(p))                        //非黑即红
#define BlackHeightUpdated(x) (                     \
    (stature((x).lChild) == stature((x).rChild)) && \
    ((x).height == (IsRed(&x) ? stature((x).lChild) : stature((x).lChild) + 1))) // RedBlack高度更新条件
template <typename T>
class RedBlack : public BST<T>
{ // RedBlack树模板类
protected:
    void solveDoubleRed(BinNodePosi(T) x);   //双红修正
    void solveDoubleBlack(BinNodePosi(T) x); //双黑修正
    int updateHeight(BinNodePosi(T) x);      //更新节点x的高度 此处的高度并不是常规意义上的高度 而是通路黑色节点的高度
public:
    BinNodePosi(T) insert(const T &e); //插入（重写）
    bool remove(const T &e);           //删除（重写）
    // BST::search()等其余接口可直接沿用
};

template <typename T>
int RedBlack<T>::updateHeight(BinNodePosi(T) x)
{
    x->height = max(stature(x->lChild), stature(x->rChild));
    return IsBlack(x) ? ++x->height : x->height;
}

template <typename T>
BinNodePosi(T) RedBlack<T>::insert(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if (x)
        return x;
    x = new BinNodePosi(T)(e, this->_hot, nullptr, nullptr, -1);
    solveDoubleRed(x);
    ++this->_size;
    return x;
}

template <typename T>
void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x)
{
    if (IsRoot(*x)) //若已递归至树根 则将其转黑 整树黑高度也随之增加
    {
        (this->_root)->color = RB_BLACK;
        ++(this->_root->height);
        return;
    }

    BinNodePosi(T) p = x->parent;
    if (IsBlack(p))
        return;
    BinNodePosi(T) g = p->parent;
    BinNodePosi(T) u = uncle(x);
    // p为红则g与u必然存在 且g必为黑
    if (IsBlack(u)) //若u为黑色
    {
        if (IsLChild(*x) == IsLChild(*p))
            p->color = RB_BLACK;
        else
            x->color = RB_BLACK;
        g->color = RB_RED;
        BinNodePosi(T) gg = g->parent;
        BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x); // 3+4 调整
        r->parent = gg;
    }
    else //若u为红色
    {
        p->color = RB_BLACK;
        ++p->height;
        g->color = RB_RED;
        u->color = RB_BLACK;
        ++u->height;
        solveDoubleRed(g);
    }
}

template <typename T>
bool RedBlack<T>::remove(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if (!x)
        return false;
    BinNodePosi(T) r = removeAt(x, this->_hot);//此处之后关注的是r的局部而不是x 注意不要混淆
    if (--this->_size <= 0)
        return true;
    if (!(this->_hot))
    {
        this->_root->color = RB_BLACK;
        updateHeight(this->_root);
        return true;
    }

    if (BlackHeightUpdated(*((this->_hot))))
        return true;
    if (IsRed(r)) //说明实际被删的是黑色w(详见removeAt) 高度减少后再将r染回来即可
    {
        r->color = RB_BLACK;
        ++r->height;
        return true;
    }


    solveDoubleBlack(x);
    return true;
}

template <typename T>
void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r)
{
    BinNodePosi(T) p = r ? r->parent : (this->_hot);
    if (!p)
        return;
    BinNodePosi(T) s = (r == p->lChild) ? p->rChild : p->lChild;
    if (IsBlack(s))
    {
        BinNodePosi(T) t = nullptr;
        if (HasLChild(*s) && IsRed(s->lChild))
            t = s->lChild;
        else if (HasRChild(*s) && IsRed(s->rChild))
            t = s->rChild;
        if (t)
        {
            RBColor oldColor = p->color;
            BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t);
            if (HasLChild(*b))
                b->lChild->color = RB_BLACK;
            updateHeight(b->lChild);
            if (HasRChild(*b))
                b->rChild->color = RB_BLACK;
            updateHeight(b->rChild);
            b->color = oldColor;
            updateHeight(b);
        }
        else
        {
            s->color = RB_RED;
            --s->height;
            if (IsRed(p))
                p->color = RB_BLACK;
            else
            {
                --p->height;
                solveDoubleBlack(p);
            }
        }
    }
    else
    {
        s->color = RB_BLACK;
        p->color = RB_RED;
        (this->_hot) = p;
        BinNodePosi(T) t = IsLChild(*s) ? s->lChild : s->rChild;
        FromParentTo(*p) = rotateAt(t);
        solveDoubleBlack(r);
    }
}