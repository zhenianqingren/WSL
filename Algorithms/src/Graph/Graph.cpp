#define INT_MAX 0x79999999
#define min(a, b) a > b ? b : a
#include <stack>
#include <queue>
#include <vector>
#include <iostream>
using namespace std;
typedef enum
{
    UNDISCOVERED,
    DISCOVERED,
    VISITED
} VStatus; //顶点状态

typedef enum
{
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
} EStatus;                          //边状态
template <typename Tv, typename Te> //顶点类型、边类型
class Graph
{ //图Graph模板类
private:
    void reset()
    { //所有顶点、边的辅助信息复位
        for (int i = 0; i < n; i++)
        { //所有顶点的
            status(i) = UNDISCOVERED;
            dTime(i) = fTime(i) = -1; //状态，时间标签
            parent(i) = -1;
            priority(i) = INT_MAX;      //（在遍历树中的）父节点，优先级数
            for (int j = 0; j < n; j++) //所有边的
                if (exists(i, j))
                    status(i, j) = UNDETERMINED; //状态
            outDegree(i) = 0;
            for (int j = 0; j < n; j++)
                if (exists(i, j))
                    outDegree(i)++;
            inDegree(i) = 0;
            for (int j = 0; j < n; j++)
                if (exists(j, i))
                    inDegree(i)++;
        }
    }
    void BFS(int, int &);                //（连通域）广度优先搜索算法
    void DFS(int, int &);                //（连通域）深度优先搜索算法
    void BCC(int, int &, stack<int> &);  //（连通域）基于DFS的双连通分量分解算法
    bool TSort(int, int &, stack<Tv> &); //（连通域）基于DFS的拓扑排序算法
    template <typename PU>
    void PFS(int, PU); //（连通域）优先级搜索框架

public:
    // 顶点
    int n;                              //顶点总数
    virtual int insert(Tv const &) = 0; //插入顶点，返回编号
    virtual Tv remove(int) = 0;         //删除顶点及其关联边，返回该顶点信息
    virtual Tv &vertex(int) = 0;        //顶点v的数据（该顶点的确存在）
    virtual int &inDegree(int) = 0;     //顶点v的入度（该顶点的确存在）
    virtual int &outDegree(int) = 0;    //顶点v的出度（该顶点的确存在）
    virtual int firstNbr(int) = 0;      //顶点v的首个邻接顶点
    virtual int nextNbr(int, int) = 0;  //顶点v的（相对于顶点j的）下一邻接顶点
    virtual VStatus &status(int) = 0;   //顶点v的状态
    virtual int &dTime(int) = 0;        //顶点v的时间标签dTime
    virtual int &fTime(int) = 0;        //顶点v的时间标签fTime
    virtual int &parent(int) = 0;       //顶点v在遍历树中的父亲
    virtual int &priority(int) = 0;     //顶点v在遍历树中的优先级数
    // 边：这里约定，无向边均统一转化为方向互逆的一对有向边，从而将无向图规作有向图的特例
    int e;                                              //边总数
    virtual bool exists(int, int) = 0;                  //边(v, u)是否存在
    virtual void insert(Te const &, int, int, int) = 0; //在顶点v和u之间插入权重为w的边e
    virtual Te remove(int, int) = 0;                    //删除顶点v和u之间的边e，返回该边信息
    virtual EStatus &status(int, int) = 0;              //边(v, u)的状态
    virtual Te &edge(int, int) = 0;                     //边(v, u)的数据（该边的确存在）
    virtual int &weight(int, int) = 0;                  //边(v, u)的权重
    // 算法
    void bfs(int);         //广度优先搜索算法
    void dfs(int);         //深度优先搜索算法
    void bcc(int);         //基于DFS的双连通分量分解算法
    stack<Tv> *tSort(int); //基于DFS的拓扑排序算法
    void prim(int);        //最小支撑树Prim算法
    void dijkstra(int);    //最短路径Dijkstra算法
    template <typename PU>
    void pfs(int, PU); //优先级搜索框架
    bool TopSort();
    void Floyd();
};

template <typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s)
{
    reset();
    int v = s;
    int clock = 0;
    do //逐一检查所有顶点
    {
        if (status(v) == UNDISCOVERED) //一旦遇到尚未发现的顶点
            BFS(v, clock);             //即从该顶点出发启动一次BFS
    } while (s != (v = (++v % n)));    //按序号检查 不重不漏
}

template <typename Tv, typename Te> //广度优先搜索
void Graph<Tv, Te>::BFS(int v, int &clock)
{
    queue<int> q;
    status(v) = DISCOVERED;
    q.push(v);
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        dTime(v) = ++clock;
        //取出队首节点
        for (int u = firstNbr(v); u >= 0; u = nextNbr(v, u))
            if (status(u) == UNDISCOVERED)
            {
                status(u) = DISCOVERED;
                q.push(u);
                status(v, u) = TREE;
                parent(u) = v;
            }
            else
                status(v, u) = CROSS;
        status(v) = VISITED;
    }
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s)
{
    reset();
    int clock = 0;
    int v = s;
    do
    {
        if (UNDISCOVERED == status(v))
            DFS(v, clock);
    } while (s != (v = (++v % n)));
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int v, int &clock)
{
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    for (int u = firstNbr(v); u >= 0; u = nextNbr(v, u))
        switch (status(u))
        {
        case UNDISCOVERED:
            status(v, u) = TREE;
            parent(u) = v;
            DFS(u, clock);
            break;
        case DISCOVERED:
            status(v, u) = BACKWARD;
            break;
        default:
            status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    status(v) = VISITED;
    fTime(v) = ++clock;
}

template <typename Tv, typename Te>
stack<Tv> *Graph<Tv, Te>::tSort(int s) //基于DFS的拓扑排序算法
{
    reset();
    int clock = 0;
    int v = s;
    stack<Tv> ss; //用栈记录排序顶点
    do
    {
        if (UNDISCOVERED == status(v))
            if (!TSort(v, clock, ss)) // clock并非必需
            {
                while (!ss.empty())
                    ss.pop();
                break;
            }
    } while (s != (v = (++v % n)));
    return s;
}

template <typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(int v, int &clock, stack<Tv> &ss)
{
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    for (int u = firstNbr(v); u >= 0; u = nextNbr(v, u))
    {
        switch (status(u))
        {
        case UNDISCOVERED:
            status(v, u) = TREE;
            parent(u) = v;
            if (!TSort(u, clock, ss))
                return false;
            break;
        case DISCOVERED:
            status(v, u) = BACKWARD;
            return false;
        default:
            status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    }
    status(v) = VISITED;
    ss.push(vertex(v));
    return true;
}

template <typename Tv, typename Te>
bool Graph<Tv, Te>::TopSort()
{
    stack<int> ss;
    int count = 0;
    for (int i = 0; i < n; i++)
        if (inDegree(i) == 0)
            ss.push(i);
    while (!ss.empty())
    {
        int v = ss.top();
        cout << vertex(v) << " ";
        count++;
        for (int u = firstNbr(v); u >= 0; u = nextNbr(v, u))
        {
            inDegree(u)--;
            if (inDegree(u) == 0)
                ss.push(u);
        }
    }
    return count == n ? true : false;
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s)
{
    reset();
    int clock = 0;
    int v = s;
    stack<int> ss; //栈s用于记录已访问的顶点
    do
    {
        if (UNDISCOVERED == status(v)) //一旦发现未发现的顶点
        {
            BCC(v, clock, ss); //从该顶点出发启动一次BCC
            s.pop();           //遍历返回后 弹出栈中最后一个顶点
        }
    } while (s != (v = (++v % n)));
}
// DFS树中 叶节点绝对不是关节点 假设叶节点是关节点 那么它在图中一定有两个分支 且这两个分支只能通过它相连 这种情况下它不可能成为叶
// 节点 同样 DFS树中 若根节点有两个及以上的分支 它绝对是关节点
//
//
#define hca(x) (fTime(x)) //利用此处闲置的fTime[]充当hca[] highest-connected-ancestor
//此处的dTime可以另类的看作一种深度 越小越靠前
template <typename Tv, typename Te>
void Graph<Tv, Te>::BCC(int v, int &clock, stack<int> &ss)
{
    hca(v) = dTime(v) = ++clock;
    status(v) = DISCOVERED;
    ss.push(v);
    // 顶点被发现并入栈
    for (int u = firstNbr(v); u >= 0; u = nextNbr(v, u)) //对于v的所有邻居u
        switch (status(u))
        {
        case UNDISCOVERED: //如果没被发现
            status(v, u) = TREE;
            parent(u) = v;
            BCC(u, clock, ss);                //常规操作
            if (hca(u) < dTime(v))            //此时u作为v的下一个孩子返回 观察u是否连接到了更高的祖先
                hca(v) = min(hca(v), hca(u)); //如果是 则更新 因为v可以通过u与更高的祖先相连
            else
                while (u != ss.top()) //如果它的孩子遍历完回来后连接不到更高的祖先 那么v就是关节点
                    ss.pop();
            ss.pop();
            cout << vertex(v) << " ";
            break;
        case DISCOVERED: //如果已经发现 那么u必定在v之前
            status(v, u) = BACKWARD;
            if (u != parent(v))                 //如果不是无意义的逆回去找v 及u->v
                hca(v) = min(hca(v), dTime(u)); //说明u可以与更高的祖先相连 更新hca 此处应该是dTime(u)而不是hca(u) 因为u很可能是关节点
            break;
        default:
            status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    status(v) = VISITED;
}
#undef hca

template <typename Tv, typename Te>
template <typename PU>
void Graph<Tv, Te>::pfs(int s, PU prioUpdater)
{
    reset();
    int v = s;
    do
    {
        if (status(v) == UNDISCOVERED)
            PFS(v, prioUpdater);
    } while (s != (v = (++v % n)));
}

template <typename Tv, typename Te>
template <typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater)
{
    priority(s) = 0;
    status(s) = VISITED;
    parent(s) = -1;
    while (true)
    { //将下一顶点和边添加到PFS树中
        for (int w = firstNbr(s); w >= 0; w = nextNbr(s, w))
            prioUpdater(this, s, w); //对于选出的所有邻居w 更新优先级
        for (int shortest = INT_MAX, w = 0; w < n; w++)
            if (status(w) == UNDISCOVERED)
                if (shortest > priority(w))
                {
                    shortest = priority(w);
                    s = w;
                }
        if (VISITED == status(s))
            break;
        status(s) = VISITED;
        status(parent(s), s) = TREE;
    }
}
//通过预定义的优先级更新策略(Dijkstra算法选出某个点后判断源点到另一个点的距离与源点到被选出的点之间的距离加上被选出点与另一点距离的大小)
//(Prim算法则更新新加入的点与已有点之间的关系)
template <typename Tv, typename Te>
struct DijstraPU
{
    virtual void operator()(Graph<Tv, Te> *g, int uk, int v)
    {
        if (g->status(v) == UNDISCOVERED)
            if (g->priority(v) > g->priority(uk) + g->weight(uk, v))
            {
                g->priority(v) = g->priority(uk) + g->weight(uk, v);
                g->parent(v) = uk;
            }
    }
};

template <typename Tv, typename Te>
struct PrimPU
{
    virtual void operator()(Graph<Tv, Te> *g, int uk, int v)
    {
        if (g->status(v) == UNDISCOVERED)
            if (g->priority(v) > g->priority(uk))
            {
                g->priority(v) = g->priority(uk);
                g->parent(v) = uk;
            }
    }
};
// priority在不同算法中代表的具体含义不同
//在Dijstra中 它代表到给定源点的距离
//在Prim中 它代表到目前这颗最小树的距离 因此更新的策略也就不同
template <typename Tv, typename Te>
void Graph<Tv, Te>::Floyd()
{
    for (int m = 0; m < this->n; m++)
        for (int i = 0; i < this->n; i++)
            for (int j = 0; j < this->n; j++)
                if (i != m && j != m && weight(i, j) > weight(i, m) + weight(m, j))
                    weight(i, j) = weight(i, m) + weight(m, j);
}
