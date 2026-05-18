#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <ctime>
#include <functional>
using namespace std;
using ll = long long;
#define printFuncName() printf("%s\n", __FUNCTION__)
#define nl '\n'
#define all(v) v.begin(), v.end()
#define compress(v) sort(all(v)), v.erase(unique(all(v)), v.end())

struct Point
{
    int x, y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    Point operator-(Point &tmp) { return Point(x - tmp.x, y - tmp.y); }
    ll sq() { return x * 1LL * x + y * 1LL * y; }
    bool operator<(const Point &r) const { return x == r.x ? y < r.y : x < r.x; }
    bool operator==(const Point &r) const { return x == r.x && y == r.y; }
};