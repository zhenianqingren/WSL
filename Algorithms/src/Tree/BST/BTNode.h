#include <vector>
using std::vector;
/* 平衡的多路搜索树
    经过适当合并得超级节点
    每d代合并 m=2^d路 m-1个关键码

    对于m B-树 除根节点外(>=2个分支) 其余每个超级节点都应有最多m-1个关键码节点以及最多m个分支 最少m/2(上确界)个分支
*/
#define BTNodePosi(T) BTNode<T> *
template <typename T>
class BTNode
{
    BTNodePosi(T) parent;
    vector<T> key;
    vector<BTNodePosi(T)> child;

public:
    BTNode() : parent(nullptr), child({nullptr}), key({}) {}
    BTNode(T e, BTNodePosi(T) lc = nullptr, BTNodePosi(T) rc = nullptr) : parent(nullptr), key({e}), child({lc, rc}) {}
};
