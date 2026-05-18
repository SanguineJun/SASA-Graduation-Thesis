#include <cstdio>
#include <random>
#include <ctime>
#include <cstring>
#include <algorithm>
using namespace std;
const int SIZE=1<<20;
namespace in{
	char buf[SIZE];
	int idx=0,ridx=0;
	inline char read(){
		if(idx==ridx){
			ridx=fread(buf,1,SIZE,stdin);
			idx=buf[ridx]=0;
		}
		return buf[idx++];
	}
	inline int R(){
		int sum=0,flag=1;
		register char now=read();
		while(now<=32) now=read();
		if(now=='-') flag=0,now=read();
		while(now>=48&&now<=57){
			sum=(sum<<3)+(sum<<1)+(now&15);
			now=read();
		}
		return flag?sum:-sum;
	}
}
namespace out{
	char buf[SIZE],tmp[20];
	int idx;
	inline void flush(){
		fwrite(buf,1,idx,stdout);
	}
	inline void write(register char c){
		if(idx==SIZE){
			flush();
			idx=0;
		}
		buf[idx++]=c;
	}
	inline void W(int ans){
		if(ans<0) write('-'),ans*=-1;
		int cnt=0;
	    do{
	        tmp[cnt++]=(ans%10)|48;
	        ans/=10;
	    }while(ans>0);
	    for(;cnt--;) write(tmp[cnt]);
	    write(' ');
	}
}
void dbg()
{
    puts("STILL WORKING");
}
struct Point
{
    int x, y;
    Point() {}
    Point(int x_, int y_) : x(x_), y(y_) {}
    Point operator-(Point &tmp)
    {
        return Point(x - tmp.x, y - tmp.y);
    }
    bool operator<(Point &r){
        if(x==r.x)return y<r.y;
        return x<r.x;
    }
    int sq() { return x * x + y * y; }
    void print(){
        printf("%d %d\n",x,y);
    }
};
int range;
random_device rd;
mt19937 g(rd());
void setRandomPoint(int n);
void printPoints(vector<Point> &P);
void printLocation(vector<Point> &P);
vector<Point> point;
void init(){
    freopen("PointInfo.txt","w",stdout);
    srand(time(NULL));
}
using namespace in;
using namespace out;
int main()
{
    int T,n;
    T=30;
    init();
    W(T); write('\n');
    for(int i=1;i<=5;i++)
    {
        range=3000;
        n=(range*range)*i/30;
        for(int j=0;j<6;j++){
            setRandomPoint(n);
            printLocation(point);
        }
    }
    flush();
    return 0;
}

void setRandomPoint(int n)
{
    srand(time(NULL));
    size_t sz = sizeof(bool) * range * range;
    bool *arr = (bool *)malloc(sz);
    memset(arr, 0, sz);
    for(int i=0;i<n;i++) arr[rand()%range*range+rand()%range]=1;
    for (int i = 0; i < range; i++)
        for (int j = 0; j < range; j++)
            if(arr[i*range+j])
                point.emplace_back(i+1,j+1);
    W(range); W(point.size()); write('\n');
    shuffle(point.begin(), point.end(), g);
    free(arr);
}
void printLocation(vector<Point> &P){
    for(auto&p : P){
        W(p.x); W(p.y); write('\n');
    }
}
void printPoints(vector<Point> &P)
{
    size_t sz = sizeof(bool) * range * range;
    bool *arr = (bool *)malloc(sz);
    memset(arr, 0, sz);
    for (auto &p : P)
        arr[(p.x - 1) * range + (p.y - 1)] = 1;
    for (int i = 0; i < range; i++, puts(""))
        for (int j = 0; j < range; j++)
        {
            int k = i * range + j;
            printf("%d ", arr[k]);
        }
    puts("--------------------------------");
    free(arr);
}