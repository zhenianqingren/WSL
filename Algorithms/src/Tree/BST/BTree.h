#include "./BTNode.h"
#include <algorithm>

template <typename T>
class BTree //水平方向对应的是RAM内存操作 垂直方向对应的是外存IO操作
{
protected:
    int _size;  //关键码总数
    int _order; //阶次 树根为0 一直到外部节点而非叶子节点的高度 (外部节点可能链接其他B树 此处关注一处局部)
    //最大高度与最小高度详见教材
    BTNodePosi(T) _root;
    BTNodePosi(T) _hot;
    void solveOverflow(BTNodePosi(T));
    void solveUnderflow(BTNodePosi(T));

public:
    BTNodePosi(T) search(const T &e);
    bool insert(const T &e);
    bool remove(const T &e);
};

template <typename T>
static int k_search(const vector<T> &v, const T &e) //鉴于内外存的差异 采用二分查找的效率优化效果微乎其微 甚至可能负优化
{
    //个人猜想:IO操作(中断或DMA方式 如果对于某个节点(对应一个页) 查询速度过快可能要等待中断查询)
    //(每一次微操作后会进行中断查询)
    int pos;
    for (pos = 0; pos < v.size(); ++pos)
    {
        if (v[pos] == e || (pos < v.size() - 1 && v[pos] < e && v[pos + 1] > e))
            break;
        else if (v[pos] > e)
            return -1;
    }
    return pos;
}

template <typename T>
BTNodePosi(T) BTree<T>::search(const T &e)
{
    BTNodePosi(T) v = _root;
    _hot = nullptr;
    while (v)
    {
        int rank = k_search(v->key, e);
        if (rank >= 0 && v->key[rank] == e)
            return v;

        _hot = v;
        v = v->child[rank + 1];
    }
    return nullptr;
}

template <typename T>
bool BTree<T>::insert(const T &e)
{
    BTNodePosi(T) v = this->search(e);
    if (v)
        return false;
    int rank = search(_hot->key, e);
    _hot->key.insert(rank + 1, e);
    _hot->child.insert(rank + 2, nullptr);
    ++_size;
    solveOverflow(_hot);
    return true;
}

//此处中位数亦称中值 在向量中就是秩居中的元素 例如对于vec size=n
// the middle value -> vec[n/2(下确界)]

template <typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v)
{
    if (_order > v->key.size())
        return;

    int s = _order >> 1;
    BTNodePosi(T) u = new BTNode<T>();
    u->key.resize(_order - s);
    u->child.resize(_order - s + 1);
    std::copy(v->key.begin() + s + 1, v->key.end(), u->key.begin());
    std::copy(v->child.begin() + s + 1, v->child.end(), u->child.begin());
    v->key.resize(s + 1);
    v->child.resize(s + 1);

    if (u->child[0]) //插入操作总是在最底层 之后逐渐向上 因此若第一个child不为nullptr 那么必然不是叶子节点 那么必然它的每个孩子都存在
        std::for_each(u->child.begin(), u->child.end(), [=](BTNodePosi(T) & e)
                      { e->parent = u; });

    BTNodePosi(T) p = v->parent;
    if (!p)
    {
        _root = p = new BTNodePosi(T);
        p->child[0] = v;
        v->parent = p;
    }

    int rank = 1 + k_search(p->key, v->key[0]);
    p->key.insert(rank, v->key[s]);
    v->key.pop_back();
    p->child.insert(rank + 1, u);
    u->parent = p;
    solveOverflow(p);
}

template <typename T>
bool BTree<T>::remove(const T &e)
{
    BTNodePosi(T) v = search(e);
    if (!v)
        return false;
    int rank = k_search(v->key, e);
    if (v->child[0])
    {
        BTNodePosi(T) u = v->child[rank + 1];
        while (u->child[0])
            u = u->child[0];
        v->key[rank] = u->key[0];
        rank = 0;
        v = u;
    }
    v->key.erase(v->key.begin());
    v->child.pop_back();
    --this->_size;
    solveUnderflow(v);
    return true;
}

template <typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v)
{
    if (((_order - 1) >> 1) <= v->key.size())
        return;
    BTNodePosi(T) p = v->parent;
    if (!p) //递归基 已到根节点而没有子节点的下限
    {
        if (!v->key.size() && v->child[0])
        {
            _root = v->child[0]; //倘若树根v已不含关键码 却又唯一的非空子节点
            _root->parent = nullptr;
            v->child[0] = nullptr;
            delete v;
        } //如果为非空的情况 那么也是允许的(作为根节点)
        return;
    }
    int rank = 0;
    while (p->child[rank] != v)
        ++rank;
    if (rank > 0) //如果r不是v的第一个孩子
    {
        BTNodePosi(T) ls = p->child[rank - 1]; //从左兄弟处借
        if (ls->key.size() > ((_order - 1) >> 1))
        {
            v->key.insert(0, p->key[rank - 1]);
            p->key[rank - 1] = ls->key[ls->key.size() - 1];
            ls->key.pop_back();
            v->child.insert(0, p->child.back());
            p->child.pop_back();
            if (v->child[0])
                v->child[0]->parent = v;
            return;
        }
    }
    if (rank != p->child.size() - 1) //如果左兄弟数量不足或者没有左兄弟节点
    {
        BTNodePosi(T) rs = p->child[rank + 1];
        if (rs->key.size() > ((_order - 1) >> 1))
        {
            v->key.push_back(p->key[rank]);
            p->key[rank - 1] = rs->key[0];
            rs->key.erase(rs->key.begin());
            v->child.push_back(rs->child.front());
            rs->child.erase(rs->child.begin());
            if (v->child[v->child.size() - 1])
                v->child[v->child.size() - 1]->parent = v;
            return;
        }
    }
    int offset;
    if (rank > 0) //左右兄弟都无法相借 尝试合并左兄弟
    {
        BTNodePosi(T) ls = p->child[rank - 1];
        ls->key.push_back(p->key[rank - 1]);
        p->key.erase(p->key.begin() + rank - 1);
        offset = ls->key.size();
        ls->key.resize(ls->key.size() + v->key.size());
        std::copy(v->key.begin(), v->key.end(), ls->key.begin() + offset);
        offset = ls->child.size();
        ls->child.resize(ls->child.size() + v->child.size());
        std::copy(v->child.begin(), v->child.end(), ls->child.begin() + offset);
        delete v;
    }
    else //但是v是第一个孩子节点 于是只能合并右兄弟
    {
        BTNodePosi(T) rs = p->child[rank + 1];
        v->key.push_back(p->key[rank]);
        p->key.erase(p->key.begin() + rank);
        offset = v->key.size();
        v->key.resize(v->key.size() + rs->key.size());
        std::copy(rs->key.begin(), rs->key.end(), v->key.begin() + offset);
        offset = v->child.size();
        v->child.resize(v->child.size() + rs->child.size());
        std::copy(rs->child.begin(), rs->child.end(), v->child.begin() + offset);
        delete rs;
    }

    solveUnderflow(p);
}
