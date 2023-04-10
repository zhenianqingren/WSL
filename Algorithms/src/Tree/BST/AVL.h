#include "./BST.h"
#define Balanced(x) (stature((x).lChild) == stature((x).rChild)) //理想平衡条件
#define BalFac(x) (stature((x).lChild) - stature((x).rChild)) //平衡因子
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2)) // AVL树平衡条件
// empty tree length -1

// 在左、右孩子中取更高者，若等高，则与父亲p同侧者优先
#define tallerChild(x)                                                         \
  (stature((x)->lChild) > stature((x)->rChild)                                 \
       ? (x)->lChild                                                           \
       : (stature((x)->rChild) > stature((x)->lChild)                          \
              ? (x)->rChild                                                    \
              : (IsLChild(*(x)) ? (x)->lChild : (x)->rChild)))

template <typename T> class AVL : public BST<T> {
public:
  BinNodePosi(T) insert(const T &e);
  bool remove(const T &e);
};

template <typename T> BinNodePosi(T) AVL<T>::insert(const T &e) {
  BinNodePosi(T) &x = this->search(e);
  if (x)
    return x; // 确认目标节点不存在
  x = new BinNode<T>(e, this->_hot);
  ++(this->_size);

  for (BinNodePosi(T) g = this->_hot; g; g = g->parent) {
    if (!AvlBalanced(g)) {
      FromParentTo(*g) =
          this->rotateAt(tallerChild(tallerChild(g))); //将该子树连接至父亲
      break; // g复衡后，局部子树高度必然复原；其祖先亦必如此，故调整随即结束
            // 高度不必再更新，因为并没有变
    } else
      this->updateHeight(g);
    // 即使在g中插入新节点后未失衡，高度依然增加
  }
  // 无论单旋还是双旋，经局部调整之后，不仅g能够重获平衡，而且局部子树高度(包括其祖先)也必将复原。
  return x;
}

template <typename T> bool AVL<T>::remove(const T &e) {
  BinNodePosi(T) &x = search(e);
  if (!x)
    return false;
  --this->_size;
  removeAt(x);
  for (BinNodePosi(T) g = this->_hot; g; g = g->parent) {
    if (!AvlBalanced(g))
      g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
    // FromParentTo(*g) -> g to prevent dead loop considering that g = g->parent
    this->updateHeight(g); //更新高度 即使在g中删除节点后未失衡 高度亦可能降低
  }
  return true;

  //与插入不同的是 删除操作可能引起失衡传播
  //考虑g本来就是更短的分支 删除操作后高度进一步减小 因此需要继续调整
}