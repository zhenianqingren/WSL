#include "../BinNode.h"
#include "../BinTree.h"

template <typename T> class BST : public BinTree<T> {
protected:
  BinNodePosi(T) _hot; // BST::search()最后访问的非空（除非树空）的节点位置
  BinNodePosi(T) connect34( //按照“3 + 4”结构，联接3个节点及四棵子树
      BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
      BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
  BinNodePosi(T) rotateAt(BinNodePosi(T)); //对x及其父亲、祖父做统一旋转调整
public:
  virtual BinNodePosi(T) & search(const T &e);
  virtual BinNodePosi(T) insert(const T &e);
  virtual bool remove(const T &e);
};

template <typename T>
    static BinNodePosi(T) &
    searchIn(BinNodePosi(T) & v, const T &e, BinNodePosi(T) & hot) {
  if (!v || (e == v->data))
    return v;
  hot = v;
  return searchIn(((e < v->data) ? v->lChild : v->rChild), e, hot);
} //在搜索树中查找某个值

template <typename T> BinNodePosi(T) & BST<T>::search(const T &e) {
  return searchIn(this->_root, e, _hot = nullptr);
}

template <typename T> BinNodePosi(T) BST<T>::insert(const T &e) {
  BinNodePosi(T) x = search(e);
  if (x)
    return x;
  x = new BinNode<T>(e, _hot);
  this->_size++;
  updateHeightAbove(x);
  return x;
}

// BST删除节点
//返回值指向实际被删除节点的后继 hot指向实际被删除节点的父亲 二者均有可能是NULL
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) & x, BinNodePosi(T) & hot) {
  BinNodePosi(T) w = x;
  BinNodePosi(T) succ = nullptr;
  if (!HasLChild(*x))
    succ = x = x->rChild;
  else if (!HasRChild(*x))
    succ = x = x->lChild;
  else {
    w = w->succ();
    //既然是双分支的情况下 被删除节点一定会有右孩子
    // 那么根据succ()的特性 w一定没有左孩子 它要么是个叶节点 要么只有右孩子
    swap(x->data, w->data);
    //此时交换删除节点与后继节点的值
    BinNodePosi(T) u = w->parent;
    //此时被删除节点变成了w 但是值已经交换了
    ((u == x) ? u->rChild : u->lChild) = succ = w->rChild;
    //删除w时 需要将w父亲的指向指到w的右孩子(如果w是个叶节点
    //那么说明w是u的右孩子 否则就是u的左孩子) 隔离节点u
  }
  hot = w->parent;
  if (succ)
    succ->parent = hot;
  //改变w右孩子的父亲指向
  delete (w->data);
  delete (w);
  return succ;
}

template <typename T> bool BST<T>::remove(const T &e) {
  BinNodePosi(T) &x = search(e);
  if (!x)
    return false;
  removeAt(x, _hot);
  return true;
}

template <typename T>
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b,
                                 BinNodePosi(T) c, BinNodePosi(T) T0,
                                 BinNodePosi(T) T1, BinNodePosi(T) T2,
                                 BinNodePosi(T) T3) {
  a->lChild = T0;
  if (T0)
    T0->parent = a;
  a->rChild = T1;
  if (T1)
    T1->parent = a;
  updateHeight(a);

  c->lChild = T2;
  if (T2)
    T2->parent = c;
  c->rChild = T3;
  if (T3)
    T3->parent = c;
  updateHeight(c);

  b->lChild = a;
  b->rChild = c;
  a->parent = b;
  c->parent = b;

  updateHeight(b);
  return b;
}

template <typename T> BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v) {
  BinNodePosi(T) p = v->parent;
  BinNodePosi(T) g = p->parent;

  if (IsLChild(*p)) {
    if (IsLChild(*v)) {
      p->parent = g->parent;
      return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
    } else {
      v->parent = g->parent;
      return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
    }
  }

  else {
    if (IsRChild(*v)) {
      p->parent = g->parent;
      return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
    }

    else {
      v->parent = g->parent;
      return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
    }
  }
}
