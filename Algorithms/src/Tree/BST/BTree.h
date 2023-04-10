#include "./BTNode.h"
#include <algorithm>

template <typename T>
class BTree //姘村钩鏂瑰悜瀵瑰簲鐨勬槸RAM鍐呭瓨鎿嶄綔 鍨傜洿鏂瑰悜瀵瑰簲鐨勬槸澶栧瓨IO鎿嶄綔
{
protected:
    int _size;  //鍏抽敭鐮佹€绘暟
    int _order; //闃舵 鏍戞牴涓�0 涓€鐩村埌澶栭儴鑺傜偣鑰岄潪鍙跺瓙鑺傜偣鐨勯珮搴� (澶栭儴鑺傜偣鍙兘閾炬帴鍏朵粬B鏍� 姝ゅ鍏虫敞涓€澶勫眬閮�)
    //鏈€澶ч珮搴︿笌鏈€灏忛珮搴﹁瑙佹暀鏉�
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
static int k_search(const vector<T> &v, const T &e) //閴翠簬鍐呭瀛樼殑宸紓 閲囩敤浜屽垎鏌ユ壘鐨勬晥鐜囦紭鍖栨晥鏋滃井涔庡叾寰� 鐢氳嚦鍙兘璐熶紭鍖�
{
    //涓汉鐚滄兂:IO鎿嶄綔(涓柇鎴朌MA鏂瑰紡 濡傛灉瀵逛簬鏌愪釜鑺傜偣(瀵瑰簲涓€涓〉) 鏌ヨ閫熷害杩囧揩鍙兘瑕佺瓑寰呬腑鏂煡璇�)
    //(姣忎竴娆″井鎿嶄綔鍚庝細杩涜涓柇鏌ヨ)
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

//姝ゅ涓綅鏁颁害绉颁腑鍊� 鍦ㄥ悜閲忎腑灏辨槸绉╁眳涓殑鍏冪礌 渚嬪瀵逛簬vec size=n
// the middle value -> vec[n/2(涓嬬‘鐣�)]

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

    if (u->child[0]) //鎻掑叆鎿嶄綔鎬绘槸鍦ㄦ渶搴曞眰 涔嬪悗閫愭笎鍚戜笂 鍥犳鑻ョ涓€涓猚hild涓嶄负nullptr 閭ｄ箞蹇呯劧涓嶆槸鍙跺瓙鑺傜偣 閭ｄ箞蹇呯劧瀹冪殑姣忎釜瀛╁瓙閮藉瓨鍦�
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
    if (!p) //閫掑綊鍩� 宸插埌鏍硅妭鐐硅€屾病鏈夊瓙鑺傜偣鐨勪笅闄�
    {
        if (!v->key.size() && v->child[0])
        {
            _root = v->child[0]; //鍊樿嫢鏍戞牴v宸蹭笉鍚叧閿爜 鍗村張鍞竴鐨勯潪绌哄瓙鑺傜偣
            _root->parent = nullptr;
            v->child[0] = nullptr;
            delete v;
        } //濡傛灉涓洪潪绌虹殑鎯呭喌 閭ｄ箞涔熸槸鍏佽鐨�(浣滀负鏍硅妭鐐�)
        return;
    }
    int rank = 0;
    while (p->child[rank] != v)
        ++rank;
    if (rank > 0) //濡傛灉r涓嶆槸v鐨勭涓€涓瀛�
    {
        BTNodePosi(T) ls = p->child[rank - 1]; //浠庡乏鍏勫紵澶勫€�
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
    if (rank != p->child.size() - 1) //濡傛灉宸﹀厔寮熸暟閲忎笉瓒虫垨鑰呮病鏈夊乏鍏勫紵鑺傜偣
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
    if (rank > 0) //宸﹀彸鍏勫紵閮芥棤娉曠浉鍊� 灏濊瘯鍚堝苟宸﹀厔寮�
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
    else //浣嗘槸v鏄涓€涓瀛愯妭鐐� 浜庢槸鍙兘鍚堝苟鍙冲厔寮�
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
