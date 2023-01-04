#pragma once

#include <cassert>
#include <vector>
#include <functional>

using namespace std;

// [Start] Sparse Table
// [Prefix] sp-table-class
// [Verified] N, Q<=5*10^5, Static RMQ(https://judge.yosupo.jp/problem/staticrmq)
// SparseTable(min/max/gcd/lcmなど結合則と冪等性があれば適用可能)
// 前処理: 計算量、空間量 O(N log N)
// クエリ: 計算量O(1)
// @ref https://ikatakos.com/pot/programming_algorithm/data_structure/sparse_table
// @ref https://kopricky.github.io/code/DataStructure_Basic/sparse_table.html
template <class T>
class SparseTable {
  public:
   SparseTable(const vector<T>& seq, const function<T(T, T)>& fx);

   // 半開区間[l, r)(l, rは0-indexed)の演算結果を返す
   // @pre 0<=l<r, r<=N
   T query(const size_t l, const size_t r) const;

  private:
   // log_table_[i]: 2^kのうちiを越えない最大のもの(i=1,2,...,n)
   vector<size_t> log_table_;

   // table[l][i]: 添え字iを起点に長さ2^lの区間の最小値を記録
   vector<vector<T>> table_;

   // X * X -> Xとなる演算子(ex. min, max, gcd, lcd)
   function<T(T, T)> fx_;
};

template <class T>
SparseTable<T>::SparseTable(const vector<T>& seq, const function<T(T, T)>& fx)
    : fx_(fx) {
   assert(!seq.empty());

   const size_t N = seq.size();

   // 2^kのうちiを越えない最大のものを計算しておく
   log_table_.resize(N + 1);
   log_table_[0] = log_table_[1] = 0;

   for (size_t i = 2; i <= N; i++) {
      log_table_[i] = log_table_[i >> 1] + 1;
   }

   table_.resize(log_table_[N] + 1, vector<T>(N, 0));

   // 事前計算しておく区間(l=1, 2,.., 2^k<N)幅の最大
   for (size_t i = 0; i < N; i++) {
      table_[0][i] = seq[i];
   }

   const size_t max_log_index = log_table_[N];

   for (size_t l = 1; l <= max_log_index; l++) {
      const size_t L = 1ULL << l;
      const size_t L_prev = 1ULL << (l - 1);

      for (size_t i = 0; i < N; i++) {
         if (!(i + L <= N)) {
            break;
         }

         table_[l][i] = fx_(table_[l - 1][i], table_[l - 1][i + L_prev]);
      }
   }
}

template <class T>
T SparseTable<T>::query(const size_t l, const size_t r) const {
   assert(0 <= l && l < r);

   // 区間幅([l, r)に含まれる最大の2^k)
   const auto L_ind = log_table_[r - l];
   const auto L = 1ULL << L_ind;

   // 始点がlで区間幅Lと終点がr(=始点がr-L)で区間幅Lを重ねたものが求めたい区間になる
   const auto i1 = l;      // 始点 l, 区間幅 L
   const auto i2 = r - L;  // 終点 r(=始点 r-L), 区間幅 L

   auto value = fx_(table_[L_ind][i1], table_[L_ind][i2]);

   return value;
}
// [End] Sparse Table

void SparseTableMinOper() {
   using ll = long long;
   // [Start] Min operator(Sparse Table)
   // [Prefix] sp-table-min-oper-inline
   auto fx = [](ll x, ll y) -> ll { return min(x, y); };
   // [End] Min operator(Sparse Table)
}

void SparseTableMaxOper() {
   using ll = long long;
   // [Start] Max operator(Sparse Table)
   // [Prefix] sp-table-max-oper-inline
   auto fx = [](ll x, ll y) -> ll { return max(x, y); };
   // [End] Max operator(Sparse Table)
}
