#include "./BST.h"
#define Balanced(x) (stature((x).lChild) == stature((x).rChild)) //鐞嗘兂骞宠　鏉′欢
#define BalFac(x) (stature((x).lChild) - stature((x).rChild))    //骞宠　鍥犲瓙
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))
// empty tree length -1

#define tallerChild(x) ( \
    stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild : (stature((x)->rChild) > stature((x)->lChild) ? (x)->rChild : (IsLChild(*(x)) ? (x)->lChild : (x)->rChild)))

template <typename T>
class AVL : public BST<T>
{
public:
    BinNodePosi(T) insert(const T &e);
    bool remove(const T &e);
};

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T &e)
{
    BinNodePosi(T) &x = this->search(e);
    if (x)
        return x;
    x = new BinNode<T>(e, this->_hot);
    ++_size;

    for (BinNodePosi(T) g = this->_hot; g; g = g->parent)
    {
        if (!AvlBalanced(g))
        {
            FromParentTo(*g) = this->rotateAt(tallerChild(tallerChild(g)));
            break;
        }
        else
            this->updateHeight(g);
        //鍗充娇g鏈け琛�? 楂樺害浜﹀彲鑳藉鍔�?
        //浣嗘槸骞舵病鏈夊皢g鍙婂叾绁栧厛height鏇存柊鐨勫師鍥犲嵆涓烘棆杞皟鏁村悗楂樺害宸笉锟�?([-1,1]) 楂樺害涓嶄細鍙戠敓鍙樺寲
    }

    return x;
}

template <typename T>
bool AVL<T>::remove(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if (!x)
        return false;
    --this->_size;
    removeAt(x);
    for (BinNodePosi(T) g = this->_hot; g; g = g->parent)
    {
        if (!AvlBalanced(g))
            g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
        //FromParentTo(*g) -> g to prevent dead loop considering that g = g->parent
        this->updateHeight(g);
    }
    return true;
}