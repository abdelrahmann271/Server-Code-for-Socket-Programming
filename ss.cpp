#include <iostream>
#include <bits/stdc++.h>
#include <math.h>
#include <stdlib.h>
#include <limits>

#define EPS 1e-9
#define PI acos(-1.0)
#define  mp make_pair
#define  mt make_tuple
#define  pb push_back
#define  F first
#define  S second
#define  setzero(a) memset(a,0,sizeof(a))
#define  setdp(a) memset(a,-1,sizeof(a))
#define  MAX 100005
#define print(a)        for (auto x : a) cout << x << " "; cout << endl
#define print_upto(a, n)        for(int i=0;i<n;i++)    cout<<a[i]<<" "; cout<<endl
#define take(x, n)           for(int i=0;i<n;i++)  cin>>x[i];
#define watch(x) cout << (#x) << " is " << (x) << "\n"
#define watch2(x, y) cout <<(#x)<<" is "<<(x)<<" and "<<(#y)<<" is "<<(y)<<"\n"
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
const int M = 1e9 + 7;

ll mod(long long x) {
    return ((x % M + M) % M);
};

ll add(long long a, long long b) {
    return mod(mod(a) + mod(b));
};

ll mul(long long a, long long b) {
    return mod(mod(a) * mod(b));
};

ll modPow(ll a, ll b) {
    if (b == 0)
        return 1LL;
    if (b == 1)
        return a % M;
    ll res = 1;
    while (b) {
        if (b % 2 == 1)
            res = mul(res, a);
        a = mul(a, a);
        b = b / 2;
    }
    return res;
};

void solve() {

    return;
};

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    solve();
// #ifndef ONLINE_JUDGE
// 	freopen("input.txt", "r", stdin);
// 	freopen("output.txt", "w", stdout);
// 	freopen("error.txt" , "w" , stderr);
// #endif

    return 0;
}