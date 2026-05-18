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
    int L = 5e5, R = 5e5, d = 5e3,testcnt = 5;
    for(int i=L;i<=R;i+=d){
        res1 = 0; res2 = 0; rate = 0;
        for(int j=0;j<testcnt;j++)
            solve(i);
        res1/=testcnt; res2/=testcnt; rate/=testcnt;
        printf("%d	%.9f\n",i,rate);
        printf("%.5f %.5f\n",res1,res2);
    }
    return 0;
}