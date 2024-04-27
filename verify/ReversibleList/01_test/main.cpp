#pragma GCC target("arch=skylake-avx512")
#pragma GCC optimize("O3")

#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#define debug(...) debug_print::multi_print(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) ;
#endif

// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)
#define repp(i, s, n) for (int i = s; (i) < (int)(n); (i)++)

#define ALL(v) begin(v),end(v)
#define RALL(v) rbegin(v),rend(v)

template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }

template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

using ll = long long;
using ull = unsigned long long;

using T = long long;
using E = long long;

T f(T a, T b) { return a + b; }
T g(T a, E b) { return a + b; }
E h(E a, E b) { return a + b; }
T ts(T a) { return a; }

int main() {
   vector<ll> v{1, 2, 3, 4, 5};
   ReversibleList<ll, ll, f, g, h, ts> rev_list(v);

   rev_list.Apply(0, 3, 100);
   rev_list.Set(2, 1000);
   rev_list.Reverse(2, 4);
   int val = rev_list.Query(0, 5);
   cerr << rev_list << endl;

   return 0;
}
