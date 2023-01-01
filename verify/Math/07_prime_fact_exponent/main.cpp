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

// 素因数分解を行う
// @note N=prod p_i ^ k_iに分解し(p_i, k_i)のリストに記録する
// 計算量: O(sqrt{N})
vector<pair<long long, long long>> PrimeFactorization(long long N) {
   assert(N >= 2);

   vector<pair<long long, long long>> factorization;

   // sqrt{N}以下の数で分解
   for (long long p = 2; p * p <= N; p++) {
      if (N % p != 0) {
         continue;
      }

      // 割れる限り割り続ける
      long long n = 0;

      while (N % p == 0) {
         n++;
         N /= p;
      }

      factorization.emplace_back(p, n);
   }

   // 残りの因数pがあったとするとそれは素数でp^1の形になる。
   if (N != 1) {
      factorization.emplace_back(N, 1);
   }

   return factorization;
}

// ToDo: 素因数分解を実装後にVerify https://atcoder.jp/contests/abc280/tasks/abc280_d
// N!の素数pの指数部を求める
// @note ルジャンドルの定理に基づく
// @note 計算量: O(log N)
long long CalcExponentOfFactorial(long long p, long long N) {
   assert(p >= 2);
   long long cnt = 0;
   long long q = p;

   while (N >= q) {
      cnt += N / q;

      if (N / q < p) {
         break;
      }

      q *= p;
   }

   return cnt;
}

using P = pair<ll, ll>;

bool solve(ll N, const vector<P>& k_fact_list) {
   for (auto [p, cnt] : k_fact_list) {
      auto n_cnt = CalcExponentOfFactorial(p, N);
      if (n_cnt < cnt) return false;
   }

   return true;
}

int main() {
   ll K;
   cin >> K;

   ll ng = 1;
   ll ok = K;

   auto k_fact_list = PrimeFactorization(K);

   while (ok - ng > 1) {
      ll m = (ok + ng) / 2;

      if (solve(m, k_fact_list)) {
         ok = m;
      } else {
         ng = m;
      }
   }

   cout << ok << endl;

   return 0;
}