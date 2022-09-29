#define Parent(i) ((i - 1) >> 1)
#define LChild(i) (1 + ((i) << 1))
#define RChild(i) (((i) + 1) << 1)
#define InHeap(n, i) (((-1) < (i)) && ((i) < (n)))                                                                             //判断PQ[i]是否合法
#define LChildValid(n, i) InHeap(n, LChild(i))                                                                                 //判断PQ[i]是否有一个（左）孩子
#define RChildValid(n, i) InHeap(n, RChild(i))                                                                                 //判断PQ[i]是否有两个孩子
#define Bigger(PQ, i, j) (lt(PQ[i], PQ[j]) ? j : i)                                                                            //取大者（等时前者优先）
#define ProperParent(PQ, n, i)                                                                                                 /*父子（至多）三者中的大者*/ \
    (RChildValid(n, i) ? Bigger(PQ, Bigger(PQ, i, LChild(i)), RChild(i)) : (LChildValid(n, i) ? Bigger(PQ, i, LChild(i)) : i)) //相等时父节点优先，如此可避免不必要的交换
#define ParentValid(i) (i > 0)
template <typename T>
struct PQ
{
    virtual void insert(T) = 0;
    virtual T max() = 0;
    virtual T delmax() = 0;
};
