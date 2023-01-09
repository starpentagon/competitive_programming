#include <unordered_set>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)
 
template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }
 
template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

using ll = long long;
using ull = unsigned long long;

// 平方根のfloor, ceilを算出する
pair<long long, long long> sqrt_int(long long n) {
   assert(n >= 0);
   if (n == 0) return {0, 0};

   long long x = (long long)round(sqrtl(n));
   if ((n % x) == 0 && x == n / x) return {x, x};

   // long longの範囲で平方根のfloorの最大値はsqrt(9223372036854775807)=3037000499
   static constexpr long long kFloorMax = 3037000499;
   x = min(x, kFloorMax);

   long long xx = x * x;  // x <= kFloorMaxの範囲では x*xはlong longに収まる
   if (xx > n) x--;
   return {x, x + 1};
}

int main() {
   string a, b;
   cin >> a >> b;
   string c = a + b;
   int n = stoi(c);

   auto [lx, ux] = sqrt_int(n);

   if (lx == ux) {
      cout << "Yes" << endl;
   } else {
      cout << "No" << endl;
   }

   return 0;
}