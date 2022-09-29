#include "Graph.cpp"
using namespace std;
template <typename Tv>
struct Vertex
{ //�������Ϊ���������δ�ϸ��װ��
    Tv data;
    int inDegree, outDegree;
    VStatus status;   //���ݡ����������״̬
    int dTime, fTime; //ʱ���ǩ
    int parent;
    int priority;                 //�ڱ������еĸ��ڵ㡢���ȼ���
    Vertex(Tv const &d = (Tv)0) : //�����¶���
                                  data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
                                  dTime(-1), fTime(-1), parent(-1), priority(INT_MAX)
    {
    } //�ݲ�����Ȩ�����
};

template <typename Te>
struct Edge
{ //�߶���Ϊ���������δ�ϸ��װ��
    Te data;
    int weight;
    EStatus status;                                                        //���ݡ�Ȩ�ء�״̬
    Edge(Te const &d, int w) : data(d), weight(w), status(UNDETERMINED) {} //�����±�
};

template <typename Tv, typename Te> //�������͡�������
class GraphMatrix : public Graph<Tv, Te>
{ //�������������ڽӾ�����ʽʵ�ֵ�ͼ
private:
    vector<Vertex<Tv>> V;         //���㼯��������
    vector<vector<Edge<Te> *>> E; //�߼����ڽӾ���
public:
    GraphMatrix() { this->n = this->e = 0; } //����
    ~GraphMatrix()
    {                                         //����
        for (int j = 0; j < this->n; j++)     //���ж�̬������
            for (int k = 0; k < this->n; k++) //�߼�¼
                delete E[j][k];               //�������
    }
    // ����Ļ�����������ѯ��i�����㣨0 <= i < n��
    virtual Tv &vertex(int i) { return V[i].data; }             //����
    virtual int inDegree(int i) { return V[i].inDegree; }       //���
    virtual int outDegree(int i) { return V[i].outDegree; }     //����
    virtual int firstNbr(int i) { return nextNbr(i, this->n); } //�׸��ڽӶ���
    virtual int nextNbr(int i, int j)                           //����ڶ���j����һ�ڽӶ���
    {
        while ((-1 < j) && (!exists(i, --j)))
            ;
        return j;
    }                                                      //����������̽�������ڽӱ�����Ч�ʣ�
    virtual VStatus &status(int i) { return V[i].status; } //״̬
    virtual int &dTime(int i) { return V[i].dTime; }       //ʱ���ǩdTime
    virtual int &fTime(int i) { return V[i].fTime; }       //ʱ���ǩfTime
    virtual int &parent(int i) { return V[i].parent; }     //�ڱ������еĸ���
    virtual int &priority(int i) { return V[i].priority; } //�ڱ������е����ȼ���
    // ����Ķ�̬����
    virtual int push_back(Tv const &vertex)
    {                                     //���붥�㣬���ر��
        for (int j = 0; j < this->n; j++) // i->j
            E[j].push_back(NULL);         //ÿ������vector<Edge<Te>*>����һ��j���㼴
                                          //������Ԥ��һ��Ǳ�ڵĹ�����
        this->n++;
        E.push_back(vector<Edge<Te> *>(this->n, (Edge<Te> *)NULL)); //�����¶����Ӧ�ı����� ����һ��i
        V.push_back(Vertex<Tv>(vertex));                      //������������һ������
        return V[V.size() - 1];
    }
    virtual Tv remove(int i)
    {                                     //ɾ����i�����㼰������ߣ�0 <= i < n��
        for (int j = 0; j < this->n; j++) //���г���
            if (exists(i, j))
            {
                delete E[i][j];
                V[j].inDegree--;
            } //��������
        E.erase(E.begin() + i, E.begin() + i + 1);
        this->n--;                  //ɾ����i��
        for (int j = 0; j < this->n; j++) //���г���
            if (exists(j, i))
            {
                delete E[j][i];
                E[j][i] = NULL;
                E[j].erase(E[j].begin() + i, E.begin() + i + 1);
                V[j].outDegree--;
            } //��������
        Tv vBak = vertex(i);
        V.erase(V.begin() + i, V.begin() + i + 1); //ɾ������i
        return vBak;                               //���ر�ɾ���������Ϣ
    }
    // �ߵ�ȷ�ϲ���
    virtual bool exists(int i, int j) //��(i, j)�Ƿ����
    {
        return (0 <= i) && (i < this->n) && (0 <= j) && (j < this->n) && E[i][j] != NULL;
    }
    // �ߵĻ�����������ѯ����i��j֮������ߣ�0 <= i, j < n��exists(i, j)��

    virtual EStatus &status(int i, int j) { return E[i][j]->status; } //��(i, j)��״̬
    virtual Te &edge(int i, int j) { return E[i][j]->data; }          //��(i, j)������
    virtual int &weight(int i, int j) { return E[i][j]->weight; }     //��(i, j)��Ȩ��
    // �ߵĶ�̬����
    virtual void insert(Te const &edge, int w, int i, int j)
    { //����Ȩ��Ϊw�ı�e = (i, j)
        if (exists(i, j))
            return;                      //ȷ���ñ��в�����
        E[i][j] = new Edge<Te>(edge, w); //�����±�
        this->e++;
        V[i].outDegree++;
        V[j].inDegree++; //���±߼�������������Ķ���
    }
    virtual Te remove(int i, int j)
    { //ɾ������i��j֮������ߣ�exists(i, j)��
        Te eBak = edge(i, j);
        delete E[i][j];
        E[i][j] = NULL; //���ݺ�ɾ���߼�¼
        this->e--;
        V[i].outDegree--;
        V[j].inDegree--; //���±߼������������Ķ���
        return eBak;     //���ر�ɾ���ߵ���Ϣ
    }
};
