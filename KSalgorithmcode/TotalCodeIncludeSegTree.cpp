#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define printFuncName() printf("%s\n", __FUNCTION__)
#define nl '\n'
#define all(v) v.begin(), v.end()
#define compress(v) sort(all(v)), v.erase(unique(all(v)), v.end())

string tab = "	";
void dbg(){puts("STILL WORKING");}

struct Point
{
    int x, y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    Point operator-(Point &tmp) { return Point(x - tmp.x, y - tmp.y); }
    ll sq() { return x * 1LL * x + y * 1LL * y; }
    bool operator<(const Point &r) const { return x == r.x ? y < r.y : x < r.x; }
    bool operator==(const Point &r) const { return x == r.x && y == r.y; }
};

double res1,res2;
double rate;

//Time 출력할 때 double은 %f로 출력해야 한다.
void printTime(clock_t s, clock_t e)
{
    printf("	%.5f", ((double)e - s) / CLOCKS_PER_SEC);
};
void printTime(function<void(void)> func)
{
    clock_t start1, end1;
    start1 = clock();
    func();
    end1 = clock();
    printf("%.5f\n", ((double)end1 - start1) / CLOCKS_PER_SEC);
};
void printTime(function<void(void)> func,double&res)
{
    clock_t start1, end1;
    start1 = clock();
    func();
    end1 = clock();
    res += ((double)end1 - (double)start1) / CLOCKS_PER_SEC;
    //printTime(start1,end1);
};
vector<Point> point, PCHP;
int range = 1000;
const int W = 5000;
/*
0. Point Generator
1. Graham Scan
2. Monotone Chain
3. Segment Tree struct
4. Segment Tree way
5. Fenwick Tree Way
6. Continuous Sum Way
7. Dynamic Segment Tree Way
*/

void setRandomPoint(int n)
{
    mt19937_64 gen(time(NULL));
    uniform_int_distribution<int> dis(1, range);
    for (int i = 0; i < n; i++)
        point.emplace_back(dis(gen), dis(gen));
    compress(point);
}

void printPoints(vector<Point> &points)
{
    for (Point &p : points)
    {
        printf("%d %d\n", p.x, p.y);
    }
}

Point P0;
//a에서 바라 봤을 때 b->c가 counter clock wise인지 체크
int ccw(Point a, Point b, Point c)
{ 
    ll res = (b.x - a.x) * 1LL * (c.y - a.y) - (b.y - a.y) * 1LL * (c.x - a.x);
    if (res != 0) res = (res > 0 ? 1 : -1);
    return res;
}
//P0,P1,P2가 ccw인지 체크
//P0,P1,P2가 일직선 상에 있다면 작은 것을 앞에 있을 경우 참을 리턴
bool cmp(Point P1, Point P2)
{
    int res = ccw(P0, P1, P2);
    if (res == 0) return (P1 - P0).sq() < (P2 - P0).sq();
    return res > 0;
}
//Stack의 top 전에 있는 원소를 리턴
Point nextToTop(stack<Point> &st)
{
    Point p = st.top();
    st.pop();
    Point res = st.top();
    st.push(p);
    return res;
}

//좌하 점에 대해 반시계 방향으로 점들을 정렬
void recoordinate(vector<Point> &P)
{
    if(P.size()<2) return;
    P0 = P[0];
    int n = P.size(), id = 0;
    for (int i = 1; i < n; i++)
        if (P[i] < P0)
            P0 = P[i], id = i;
    P[id] = P[0];
    P[0] = P0;
    sort(P.begin() + 1, P.end(), cmp);
    int m = 1;
    for (int i = 1; i < n; i++)
    {
        while (i < n - 1 && ccw(P0, P[i], P[i + 1]) == 0) i++;
        P[m++] = P[i];
    }
    P.resize(m);   
}

//점집합을 리스트로 받아 Convex Hull 점집합의 리스트를 리턴
vector<Point> GrahamScan(vector<Point> P)
{
    recoordinate(P);
    if(P.size()<3) return P;
    stack<Point> st;
    st.push(P[0]); st.push(P[1]); st.push(P[2]);
    for (int i = 3; i < P.size(); i++)
    {
        while (ccw(nextToTop(st), st.top(), P[i]) <= 0) st.pop();
        st.push(P[i]);
    }
    vector<Point> ans;
    while (!st.empty()) ans.push_back(st.top()), st.pop();
    return ans;
}

void GrahamWay()
{
    vector<Point> CH_Old = GrahamScan(point);
    //printFuncName();
}


vector<Point> MonotoneChain(vector<Point> P)
{
    compress(P);
    if (P.size() < 3)
        return P;
    vector<Point> up, dn;
    int u(0), d(0);
    sort(all(P));
    for (auto &t : P)
    {
        while (u >= 2 && ccw(up[u - 2], up[u - 1], t) >= 0)
            --u, up.pop_back();
        up.push_back(t);
        ++u;
        while (d >= 2 && ccw(dn[d - 2], dn[d - 1], t) <= 0)
            --d, dn.pop_back();
        dn.push_back(t);
        ++d;
    }
    dn.insert(dn.end(), ++up.begin(), --up.end());
    return dn;
}

void AndrewWay()
{
    vector<Point> CH = MonotoneChain(point);
    printFuncName();
}
/*
Seg Tree 1
*/
//const variable is faster
constexpr int blk = 64;
struct SegmentTree
{
    int base = 1;
    ll *tree;
    SegmentTree(int n = 0)
    {
        printf("Using bitset Segment Tree, ");
        n += 3;
        int size = 0;
        while (base < n * n)
            size += base, base *= 4;
        size += base;
        size_t sz = sizeof(ll) * (size + 1) / blk + 1;
        tree = (ll *)malloc(sz);
        memset(tree, 0, sz);
    }
    void update(int nd, int sx, int sy, int ex, int ey, int px, int py)
    {
        if (px < sx || px > ex || py < sy || py > ey)
            return;
        // printf("%d %d %d %d\n",sx,sy,ex,ey);
        tree[nd / blk] |= 1LL << (nd % blk);
        if (sx != ex || sy != ey)
        {
            int mx = (sx + ex) >> 1, my = (sy + ey) >> 1;
            update(nd * 4 - 2, sx, sy, mx, my, px, py);
            update(nd * 4 - 1, sx, my + 1, mx, ey, px, py);
            update(nd * 4, mx + 1, sy, ex, my, px, py);
            update(nd * 4 + 1, mx + 1, my + 1, ex, ey, px, py);
        }
    }
    bool exist(int nd, int sx, int sy, int ex, int ey, int ax, int ay, int bx, int by)
    {
        if (ex < ax || ey < ay || sx > bx || sy > by)
            return 0;
        if ((ax <= sx && ex <= bx) && (ay <= sy && ey <= by))
            return tree[nd / blk] & (1LL << (nd % blk));
        int mx = (sx + ex) >> 1, my = (sy + ey) >> 1;
        return exist(nd * 4 - 1, sx, my + 1, mx, ey, ax, ay, bx, by) || exist(nd * 4 - 2, sx, sy, mx, my, ax, ay, bx, by) || exist(nd * 4, mx + 1, sy, ex, my, ax, ay, bx, by) || exist(nd * 4 + 1, mx + 1, my + 1, ex, ey, ax, ay, bx, by);
    }
    void update(int px, int py)
    {
        update(1, 1, 1, range, range, px, py);
    }
    bool exist(int ax, int ay, int bx, int by)
    {
        return exist(1, 1, 1, range, range, ax, ay, bx, by);
    }
    ~SegmentTree() { free(tree); }
};
/*
Bool Segment Tree
*/
struct boolSegmentTree
{
    int base = 1;
    bool *tree;
    boolSegmentTree(int n = 0)
    {
        printf("Using Bool Segment Tree, ");
        n += 3;
        int size = 0;
        while (base < n * n)
            size += base, base *= 4;
        size += base;
        ++size;
        size_t sz = sizeof(bool) * size;
        tree = (bool *)malloc(sz);
        memset(tree, 0, sz);
    }
    void update(int nd, int sx, int sy, int ex, int ey, int px, int py)
    {
        if (px < sx || px > ex || py < sy || py > ey)
            return;
        // printf("%d %d %d %d\n",sx,sy,ex,ey);
        tree[nd] = 1;
        if (sx != ex || sy != ey)
        {
            int mx = (sx + ex) / 2, my = (sy + ey) / 2;
            update(nd * 4 - 2, sx, sy, mx, my, px, py);
            update(nd * 4 - 1, sx, my + 1, mx, ey, px, py);
            update(nd * 4, mx + 1, sy, ex, my, px, py);
            update(nd * 4 + 1, mx + 1, my + 1, ex, ey, px, py);
        }
    }
    bool exist(int nd, int sx, int sy, int ex, int ey, int ax, int ay, int bx, int by)
    {
        if (ex < ax || ey < ay || sx > bx || sy > by)
            return 0;
        if ((ax <= sx && ex <= bx) && (ay <= sy && ey <= by))
            return tree[nd];
        int ans = 0, mx = (sx + ex) >> 1, my = (sy + ey) >> 1;
        return exist(nd * 4 - 1, sx, my + 1, mx, ey, ax, ay, bx, by) || exist(nd * 4 - 2, sx, sy, mx, my, ax, ay, bx, by) || exist(nd * 4, mx + 1, sy, ex, my, ax, ay, bx, by) || exist(nd * 4 + 1, mx + 1, my + 1, ex, ey, ax, ay, bx, by);
    }
    void update(int px, int py)
    {
        update(1, 1, 1, range, range, px, py);
    }
    bool exist(int ax, int ay, int bx, int by)
    {
        return exist(1, 1, 1, range, range, ax, ay, bx, by);
    }
    ~boolSegmentTree() { free(tree); }
};
template <typename T>
void SegmentTreeWay()
{
    T Seg(range);
    PCHP.clear();
    for (auto &p : point)
        Seg.update(p.x, p.y);
    for (auto &p : point)
    {
        int x, y;
        x = p.x, y = p.y; //1~range
        int halfrange = range / 2;
        if (x >= halfrange && y >= halfrange)
        {
            if (!Seg.exist(x, y + 1, range, range) || !Seg.exist(1, y, x - 1, range) || !Seg.exist(x + 1, 1, range, y) || !Seg.exist(1, 1, x, y - 1))
            {
                PCHP.push_back(p);
            }
        }
        else if (x >= halfrange && y < halfrange)
        {
            if (!Seg.exist(x + 1, 1, range, y) || !Seg.exist(1, 1, x, y - 1) || !Seg.exist(x, y + 1, range, range) || !Seg.exist(1, y, x - 1, range))
            {
                PCHP.push_back(p);
            }
        }
        else if (x < halfrange && y >= halfrange)
        {
            if (!Seg.exist(1, y, x - 1, range) || !Seg.exist(x, y + 1, range, range) || !Seg.exist(1, 1, x, y - 1) || !Seg.exist(x + 1, 1, range, y))
            {
                PCHP.push_back(p);
            }
        }
        else if (x < halfrange && y < halfrange)
        {
            if (!Seg.exist(1, 1, x, y - 1) || !Seg.exist(1, y, x - 1, range) || !Seg.exist(x + 1, 1, range, y) || !Seg.exist(x, y + 1, range, range))
            {
                PCHP.push_back(p);
            }
        }
    }
    vector<Point> CH_New = MonotoneChain(PCHP);
    printFuncName();
    // printPoints(PCHP); dbg();
    // printPoints(CH_New);
}
/*
Fenwick Tree Way
*/
struct BIT
{
    int n, m;
    vector<vector<int>> v;
    BIT(int a = 0, int b = 0)
    {
        n = a;
        m = b;
        v.resize(n + 3, vector<int>(m + 3));
    }
    int get_sum(int x, int y)
    {
        int sum = 0;
        for (int i = x; i > 0; i -= i & -i)
            for (int j = y; j > 0; j -= j & -j)
                sum += v[i][j];
        return sum;
    }
    void update(int x, int y, int val)
    {
        for (int i = x; i <= n; i += i & -i)
            for (int j = y; j <= m; j += j & -j)
                v[i][j] += val;
    }
    bool exist(int sx, int sy, int ex, int ey)
    {
        int sum = get_sum(ex, ey) - get_sum(ex, sy - 1) - get_sum(sx - 1, ey) + get_sum(sx - 1, sy - 1);
        return sum > 0;
    }
};
/*
Fenwick Tree Way
*/
void FenwickTreeWay()
{
    PCHP.clear();
    BIT f(range, range);
    for (auto &p : point)
        f.update(p.x, p.y, 1);
    for (auto &p : point)
    {
        int x, y;
        x = p.x;
        y = p.y;
        if (!f.exist(x, y + 1, range, range) || !f.exist(1, y, x - 1, range) || !f.exist(x + 1, 1, range, y) || !f.exist(1, 1, x, y - 1))
        {
            PCHP.push_back(p);
        }
    }
    vector<Point> CH_New = MonotoneChain(PCHP);
    printFuncName();
    // printPoints(PCHP);
    // printPoints(CH_New);

}


/*
Continuous Sum way
Only available at range <= 5000
*/
int table[W + 10][W + 10];
void ContinuousSumWay()
{
    for (Point &p : point)
        ++table[p.x][p.y];
    for (int i = 1; i <= range; i++)
        for (int j = 1; j <= range; j++)
            table[i][j] += table[i - 1][j] + table[i][j - 1] - table[i - 1][j - 1];
    auto Sum = [](int x1, int y1, int x2, int y2) -> int {
        return table[x2][y2] - table[x2][y1 - 1] - table[x1 - 1][y2] + table[x1 - 1][y1 - 1];
    };
    for (Point &p : point)
    {
        int x = p.x, y = p.y;
        if (!Sum(x, y + 1, range, range) || !Sum(1, y, x - 1, range) || !Sum(x + 1, 1, range, y) || !Sum(1, 1, x, y-1))
            PCHP.push_back(p);
    }
    vector<Point> CH_New = GrahamScan(PCHP);
    //printFuncName();
}
/*
Dynamic Segment Tree Way
좌표계의 한계를 극복!
*/
/*
TODO
1. 인덱스기반 DST로 바꾸기
2. 노드에서 v값 없애기 (존재성 자체가 v=1을 뜻함)
*/
struct DST
{
    int range;
    struct Node
    {
        Node *lu, *ld, *ru, *rd; //양쪽 자식
        bool v;                  //구간 존재성
        Node()
        {
            lu = ld = ru = rd = NULL;
            v = 0;
        }
    } * root; //root 동적할당 필수!
    DST(int R = 0) : range(R) { root = new Node(); }
    void update(Node *node, int sx, int sy, int ex, int ey, int px, int py)
    {
        if (px < sx || px > ex || py < sy || py > ey)
            return;
        if (sx == ex && sy == ey)
        { //리프 노드
            node->v = 1;
            return;
        }
        if (sx != ex || sy != ey)
        {
            int mx = (sx + ex) >> 1, my = (sy + ey) >> 1;
            if (px <= mx && py <= my)
            {
                if (!node->lu)
                    node->lu = new Node();
                update(node->lu, sx, sy, mx, my, px, py);
            }
            else if (px <= mx && py > my)
            {
                if (!node->ru)
                    node->ru = new Node();
                update(node->ru, sx, my + 1, mx, ey, px, py);
            }
            else if (px > mx && py <= my)
            {
                if (!node->ld)
                    node->ld = new Node();
                update(node->ld, mx + 1, sy, ex, my, px, py);
            }
            else if (px > mx && py > my)
            {
                if (!node->rd)
                    node->rd = new Node();
                update(node->rd, mx + 1, my + 1, ex, ey, px, py);
            }
        }
        if (node->lu || node->ru || node->ld || node->rd)
            node->v = 1;
    }
    void update(int px, int py)
    {
        update(this->root, 1, 1, range, range, px, py);
    }
    bool exist(Node *node, int sx, int sy, int ex, int ey, int ax, int ay, int bx, int by)
    {
        if (!node)
            return 0;
        if (ex < ax || ey < ay || sx > bx || sy > by)
            return 0;
        if ((ax <= sx && ex <= bx) && (ay <= sy && ey <= by))
            return node->v;
        int mx = (sx + ex) >> 1, my = (sy + ey) >> 1;
        return exist(node->lu, sx, sy, mx, my, ax, ay, bx, by) || exist(node->ru, sx, my + 1, mx, ey, ax, ay, bx, by) || exist(node->ld, mx + 1, sy, ex, my, ax, ay, bx, by) || exist(node->rd, mx + 1, my + 1, ex, ey, ax, ay, bx, by);
    }
    bool exist(int ax, int ay, int bx, int by)
    {
        return exist(this->root, 1, 1, range, range, ax, ay, bx, by);
    }
    void freeAll(Node *node)
    {
        if (!node)
            return;
        freeAll(node->ld);
        freeAll(node->lu);
        freeAll(node->rd);
        freeAll(node->ru);
        delete node;
    }
    ~DST() { freeAll(this->root); }
};
void DSTWay()
{
    DST d(range);
    PCHP.clear();
    for (auto &p : point)
        d.update(p.x, p.y);
    for (auto &p : point)
    {
        int x, y;
        x = p.x, y = p.y; //1~range
        int halfrange = range / 2;
        if (x >= halfrange && y >= halfrange)
        {
            if (!d.exist(x, y + 1, range, range) || !d.exist(1, y, x - 1, range) || !d.exist(x + 1, 1, range, y) || !d.exist(1, 1, x, y - 1))
            {
                PCHP.push_back(p);
            }
        }
        else if (x >= halfrange && y < halfrange)
        {
            if (!d.exist(x + 1, 1, range, y) || !d.exist(1, 1, x, y - 1) || !d.exist(x, y + 1, range, range) || !d.exist(1, y, x - 1, range))
            {
                PCHP.push_back(p);
            }
        }
        else if (x < halfrange && y >= halfrange)
        {
            if (!d.exist(1, y, x - 1, range) || !d.exist(x, y + 1, range, range) || !d.exist(1, 1, x, y - 1) || !d.exist(x + 1, 1, range, y))
            {
                PCHP.push_back(p);
            }
        }
        else if (x < halfrange && y < halfrange)
        {
            if (!d.exist(1, 1, x, y - 1) || !d.exist(1, y, x - 1, range) || !d.exist(x + 1, 1, range, y) || !d.exist(x, y + 1, range, range))
            {
                PCHP.push_back(p);
            }
        }
    }
    // printPoints(PCHP);
    vector<Point> CH_New = MonotoneChain(PCHP);
    printFuncName();
    // printPoints(CH_New);
}

void setIO(){
    //freopen("Elimination.txt","w",stdout);
}

void init(){
    point.clear();
    PCHP.clear();
    memset(table,0,sizeof(table));
}

/*
main
*/

void solve(int n=0){
    //puts("Number of Points?");
    //scanf("%d", &n);
    init();
    setRandomPoint(n);
    int All = point.size();
    printTime(GrahamWay,res1);
    printTime(ContinuousSumWay,res2);
    rate += (All-PCHP.size())*1.00/All*100;
}

int main()
{
    //set Range
    //scanf("%d",&range);
    setIO();
    range = 2000;
    int L = 5e5, R = 5e5, d = 5e3,cnt = 5;
    for(int i=L;i<=R;i+=d){
        res1 = 0; res2 = 0; rate = 0;
        for(int j=0;j<cnt;j++)
            solve(i);
        res1/=cnt; res2/=cnt; rate/=cnt;
        printf("%d	%.9f\n",i,rate);
        printf("%.5f %.5f\n",res1,res2);
    }
    printTime(AndrewWay);

    printTime(SegmentTreeWay<SegmentTree>);

    printTime(SegmentTreeWay<boolSegmentTree>);

    printTime(FenwickTreeWay);

    printTime(DSTWay);
    return 0;
}