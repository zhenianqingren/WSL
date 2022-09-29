#ifndef BINTREE
#define BINTREE
#include "BinNode.h" //引入二叉树节点类
template <typename T>
class BinTree
{ //二叉树模板类
protected:
    int _size;                                  //规模
    BinNodePosi(T) _root;                       //根节点
    virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);   //更新节点x及其祖先的高度
public:
    BinTree() : _size(0), _root(NULL) {} //构造函数
    ~BinTree()
    {
    }                                                        //析构函数
    int size() const { return _size; }                       //规模
    bool empty() const { return !_root; }                    //判空
    BinNodePosi(T) root() const { return _root; }            //树根
    BinNodePosi(T) insertAsRoot(T const &e);                 //插入根节点
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e); // e作为x的左孩子（原无）插入

    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e);        // e作为x的右孩子（原无）插入
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T> *&tree); // T作为x左子树接入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T> *&tree); // T作为x右子树接入
    int remove(BinNodePosi(T) x);                                   //删除以位置x处节点为根的子树，返回该子树原先的规模
    BinTree<T> *secede(BinNodePosi(T) x);                           //将子树x从当前树中摘除，并将其转换为一棵独立子树
    template <typename VST>                                         //操作器
    void travLevel(VST &visit)
    {
        if (_root)
            _root->travLevel(visit);
    }                       //层次遍历
    template <typename VST> //操作器
    void travPre(VST &visit)
    {
        if (_root)
            _root->travPre(visit);
    }                       //先序遍历
    template <typename VST> //操作器
    void travIn(VST &visit)
    {
        if (_root)
            _root->travIn(visit);
    }                       //中序遍历
    template <typename VST> //操作器
    void travPost(VST &visit)
    {
        if (_root)
            _root->travPost(visit);
    } //后序遍历
    // 比较器、判等器（各列其一，其余自行补充）
    bool operator<(BinTree<T> const &t) { return _root && t._root && lt(_root, t._root); }
    bool operator==(BinTree<T> const &t) { return _root && t._root && (_root == t._root); }
}; // BinTree

template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //更新节点x高度
{
    return x->height = 1 + max(stature(x->lChild), stature(x->rChild));//not backtrace beacause we always intend to use it from the back to the top
                                                                //so the son height definitely be calculated
} //具体规则因树不同而异

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) //更新v及祖先的高度
{
    while (x)
    {
        updateHeight(x);
        x = x->parent;
    }
} //可优化：一旦高度未发，即可终止

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const &e)
{
    _size = 1;
    return _root = new BinNode<T>(e);
} //将e当作根节点插入空的二叉树

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const &e)
{
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lChild;
} // e插入为x的左孩子

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const &e)
{
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rChild;
} // e插入为x的右孩子

template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
int BinTree<T>::remove(BinNodePosi(T) x)
{                                 // assert: x为二叉树中的合法位置
    FromParentTo(*x) = NULL;      //切断来自父节点的指针
    updateHeightAbove(x->parent); //更新祖先高度
    int n = removeAt(x);
    _size -= n;
    return n; //删除子树x，更新规模，返回删除节点总数
}

template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
static int removeAt(BinNodePosi(T) x)
{ // assert: x为二叉树中的合法位置
    if (!x)
        return 0;                                          //逑弻基：空树
    int n = 1 + removeAt(x->lChild) + removeAt(x->rChild); //逑弻释放左、右子树
    delete x;
    return n; //释放被摘除节点，并返回删除节点总数3
}
#endif
