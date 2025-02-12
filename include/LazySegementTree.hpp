#pragma once

#include <vector>
#include <limits>
#include <functional>

using namespace std;

// [Start] Lazy Segment Tree
// [Prefix] laz-seg-tree-class
// [Verified] Range Update: N, Q<=10^5, Range Update Query(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_D&lang=ja)
// [Verified] Range Add: N, Q<=10^5, Range Add Query(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_E&lang=ja)
// [Verified] Range Update and Range Min: N, Q<=10^5, RMQ and RUQ(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F&lang=ja)
// [Verified] Range Add and Range Sum: N, Q<=10^5, RSQ and RAQ(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G&lang=ja)

// 遅延評価セグメント木
// 計算量
// - 初期構築 O(N)
// - 区間更新 or 区間加算 O(log N)
// - 添え字区間[a, b)の要素に対する演算結果 O(log N)
//
// 前提
// X: セグメント木のデータ
// M: 遅延伝搬させるデータ
// Xと演算operはモノイド(結合則と単位元が存在)
// Mと演算laz_opはモノイド(結合則と単位元が存在): M上での評価ルール(更新/加算etc)に対応
// X x M -> Xの演算子eval_opについて
// - x * e_m = x(e_mはMの単位元)
// - (x1 oper x2) * p(m,n) = (x1 * p(m,n/2)) oper (x2 * p(m,n/2))
// - (x * m1) * m2 = x * (m1 laz_op m2)
// が成立すること。(MからXへの遅延評価ルール(更新/加算etc)に対応)
// ここで p(m,n)は区間作用素でmをn回laz_opを作用させたもの(区間和などを実現するために利用)
template <class X, class M>
class LazySegmentTree {
  public:
   // @param val_list セグメント木を構築する配列
   // @param oper X上の二項演算子(最大、最小、XORなど)
   // @param eval_op X x M -> Xの二項演算子(遅延データを反映させる演算子)
   // @param laz_op M上の二項演算子
   // @param laz_p_op laz_opをn回行う演算子(区間加算など区間に比例した演算を行う場合に利用)
   // @param unit_x Xの単位元
   // @param unit_m Mの単位元
   LazySegmentTree(const vector<X>& val_list,
                   function<X(X, X)> oper, function<X(X, M)> eval_op, function<M(M, M)> laz_op, function<M(M, size_t)> laz_p_op,
                   const X unit_x, const M unit_m);

   // 区間[a, b)にmを作用(更新/加算など)させる
   void update(size_t a, size_t b, M m) {
      update(a, b, m, 0, 0, leaf_size_);
   }

   // 添え字区間[a, b), 0-indexedの要素に対するX上の二項演算結果を返す
   X query(size_t a, size_t b) {
      return sub_query(a, b,       // クエリを行う区間
                       0,          // ルートノード
                       0,          // ルートノードが表す範囲の始点(0番目以上)
                       leaf_size_  // ルートノードが表す範囲の終点(n番目未満)
      );
   }

   // 添え字区間[a, b)におけるx以下の最小の添え字を返す(存在しない場合は-1を返す)
   // @note operがminであること
   int find_leftest_leq(size_t a, size_t b, X x) {
      return sub_find_leftest_leq(a, b,
                                  x,
                                  0,          // ルートノード
                                  0,          // 0番目以上
                                  leaf_size_  // n番目未満
      );
   }

   // 添え字区間[a, b)におけるx以下の最大の添え字を返す(存在しない場合は-1を返す)
   // @note operがminであること
   // [Verified] N, Q<= 2*10^5, ABC_389「F - Rated Range」(https://atcoder.jp/contests/abc389/tasks/abc389_f)
   int find_rightest_leq(size_t a, size_t b, X x) {
      return sub_find_rightest_leq(a, b,
                                   x,
                                   0,          // ルートノード
                                   0,          // 0番目以上
                                   leaf_size_  // n番目未満
      );
   }

   // 添え字区間[a, b)におけるx以上の最小の添え字を返す(存在しない場合は-1を返す)
   // @note operがmaxであること
   int find_leftest_geq(size_t a, size_t b, X x) {
      return sub_find_leftest_geq(a, b,
                                  x,
                                  0,          // ルートノード
                                  0,          // 0番目以上
                                  leaf_size_  // n番目未満
      );
   }

   // 添え字区間[a, b)におけるx以上の最大の添え字を返す(存在しない場合は-1を返す)
   // @note operがmaxであること
   int find_rightest_geq(size_t a, size_t b, X x) {
      return sub_find_rightest_geq(a, b,
                                   x,
                                   0,          // ルートノード
                                   0,          // 0番目以上
                                   leaf_size_  // n番目未満
      );
   }

   // 添え字区間[a, b)における[a, i]の合計がx以上になる最小の添え字を返す(存在しない場合は-1を返す)
   // @note operがsumであること, 各要素が非負であること
   int find_leftest_sum_geq(size_t a, size_t b, X x) {
      return sub_find_leftest_sum_geq(a, b,
                                      x,
                                      0,          // ルートノード
                                      0,          // 0番目以上
                                      leaf_size_  // n番目未満
      );
   }

  private:
   void update(size_t a, size_t b, M m, size_t k, size_t l, size_t r);

   // 遅延評価を行う
   void eval(size_t k, size_t len);

   // i番目の要素の値を設定する
   // @note 親ノードの更新は行わない。初期構築時に利用。
   void set(size_t i, X x) {
      data_[leaf_index(i)] = x;
   }

   // セグメント木の初期構築
   // @pre leafの値が設定されていること
   void build();

   // i番目の要素の完全二分木配列data_でのleaf位置を返す
   size_t leaf_index(size_t i) const {
      return i + leaf_size_ - 1;
   }

   // 添え字区間[a, b)の要素に対する二項演算結果を返す
   // @param k 現在のノード位置
   // @param [l, r) data_[k]が表している区間
   X sub_query(size_t a, size_t b, size_t k, size_t l, size_t r);

   // 添え字区間[a, b)におけるx以下の最小の添え字を返す
   int sub_find_leftest_leq(size_t a, size_t b, X x, size_t k, size_t l, size_t r);

   // 添え字区間[a, b)におけるx以下の最大の添え字を返す
   int sub_find_rightest_leq(size_t a, size_t b, X x, size_t k, size_t l, size_t r);

   // 添え字区間[a, b)におけるx以上の最小の添え字を返す
   int sub_find_leftest_geq(size_t a, size_t b, X x, size_t k, size_t l, size_t r);

   // 添え字区間[a, b)におけるx以上の最大の添え字を返す
   int sub_find_rightest_geq(size_t a, size_t b, X x, size_t k, size_t l, size_t r);

   // 添え字区間[a, b)における[a, i)の合計がx以上になる最小の添え字を返す
   int sub_find_leftest_sum_geq(size_t a, size_t b, X x, size_t k, size_t l, size_t r);

   vector<X> data_;  // データを完全二分木を格納する配列
   vector<M> lazy_;  // 遅延評価する値を格納する配列

   function<X(X, X)> oper_;           // X上の二項演算子(最大、最小、XORなど)
   function<X(X, M)> eval_op_;        // X x M -> Xの二項演算子
   function<M(M, M)> laz_op_;         // M上の二項演算子
   function<M(M, size_t)> laz_p_op_;  // M上でlaz_op_をn回作用させた演算子

   X unit_x_;  // X上の単位元
   M unit_m_;  // M上の単位元

   size_t leaf_size_;  // 管理する配列のサイズ(=セグメント木のリーフ数)
};

template <class X, class M>
LazySegmentTree<X, M>::LazySegmentTree(const vector<X>& val_list,
                                       function<X(X, X)> oper, function<X(X, M)> eval_op, function<M(M, M)> laz_op, function<M(M, size_t)> laz_p_op,
                                       const X unit_x, const M unit_m)
    : oper_(oper), eval_op_(eval_op), laz_op_(laz_op), laz_p_op_(laz_p_op), unit_x_(unit_x), unit_m_(unit_m), leaf_size_(1) {
   // val_listの要素をleafとする完全二分木を作る
   while (val_list.size() > leaf_size_) {
      leaf_size_ *= 2;
   }

   size_t tree_size = 2 * leaf_size_;

   data_.resize(tree_size);
   fill(data_.begin(), data_.end(), unit_x);

   lazy_.resize(tree_size);
   fill(lazy_.begin(), lazy_.end(), unit_m);

   for (size_t i = 0; i < val_list.size(); i++) {
      set(i, val_list[i]);
   }

   build();
}

template <class X, class M>
void LazySegmentTree<X, M>::build() {
   for (ll i = (ll)leaf_size_ - 2; i >= 0; i--) {
      data_[i] = oper_(data_[2 * i + 1], data_[2 * i + 2]);
   }
}

template <class X, class M>
void LazySegmentTree<X, M>::eval(size_t k, size_t len) {
   if (lazy_[k] == unit_m_) {
      // 更新するものがない場合は終了する
      return;
   }

   if (k < leaf_size_ - 1) {
      // leafでなければ子に伝搬させる
      lazy_[2 * k + 1] = laz_op_(lazy_[2 * k + 1], lazy_[k]);
      lazy_[2 * k + 2] = laz_op_(lazy_[2 * k + 2], lazy_[k]);
   }

   // 自身を更新する
   data_[k] = eval_op_(data_[k], laz_p_op_(lazy_[k], len));
   lazy_[k] = unit_m_;
}

template <class X, class M>
void LazySegmentTree<X, M>::update(size_t a, size_t b, M m, size_t k, size_t l, size_t r) {
   eval(k, r - l);

   if (a <= l && r <= b) {
      // 完全に内側の場合
      lazy_[k] = laz_op_(lazy_[k], m);
      eval(k, r - l);
   } else if (a < r && l < b) {
      // 一部区間がかぶる場合

      // 左の子
      update(a, b, m, 2 * k + 1, l, (l + r) / 2);

      // 右の子
      update(a, b, m, 2 * k + 2, (l + r) / 2, r);

      data_[k] = oper_(data_[2 * k + 1], data_[2 * k + 2]);
   }
}

template <class X, class M>
X LazySegmentTree<X, M>::sub_query(size_t a, size_t b, size_t k, size_t l, size_t r) {
   // 遅延評価を反映させる
   eval(k, r - l);

   // 演算を行う範囲[a, b)と今いるノードkが表してる範囲[r, l)を比較する
   if (r <= a || b <= l) {
      // 自身の範囲が演算を行う範囲[a, b)外
      // -> 演算しなくて良いので単位元を返して終了
      return unit_x_;
   } else if (a <= l && r <= b) {
      // 自身の範囲が演算を行う範囲[a, b)に含まれる
      // -> 自身の値を返す
      return data_[k];
   } else {
      // 自身の範囲が演算を行う範囲[a, b)に一部含まれる
      // -> より範囲の狭い子ノードの結果を使う
      X val_l = sub_query(a, b, 2 * k + 1, l, (l + r) / 2);
      X val_r = sub_query(a, b, 2 * k + 2, (l + r) / 2, r);

      return oper_(val_l, val_r);
   }
}

template <class X, class M>
int LazySegmentTree<X, M>::sub_find_leftest_leq(size_t a, size_t b, X x, size_t k, size_t l, size_t r) {
   // 遅延評価を反映させる
   eval(k, r - l);

   if (!(data_[k] <= x) || r <= a || b <= l) {
      // 自分の値がxより大きい or [a, b)が[l, r)の範囲外
      return -1;
   } else if (k >= leaf_size_ - 1) {
      // 自分が葉ノード
      return k - leaf_size_ + 1;
   } else {
      // 左の部分木を見てa-1以外ならその値を返す
      int val_l = sub_find_leftest_leq(a, b, x, k * 2 + 1, l, (l + r) / 2);

      if (val_l != -1) {
         return val_l;
      }

      return sub_find_leftest_leq(a, b, x, k * 2 + 2, (l + r) / 2, r);
   }
}

template <class X, class M>
int LazySegmentTree<X, M>::sub_find_rightest_leq(size_t a, size_t b, X x, size_t k, size_t l, size_t r) {
   // 遅延評価を反映させる
   eval(k, r - l);

   if (!(data_[k] <= x) || r <= a || b <= l) {
      // 自分の値がxより大きい or [a, b)が[l, r)の範囲外
      return -1;
   } else if (k >= leaf_size_ - 1) {
      // 自分が葉ノード
      return k - leaf_size_ + 1;
   } else {
      // 右の部分木を見てa-1以外ならその値を返す
      int val_r = sub_find_rightest_leq(a, b, x, k * 2 + 2, (l + r) / 2, r);

      if (val_r != -1) {
         return val_r;
      }

      return sub_find_rightest_leq(a, b, x, k * 2 + 1, l, (l + r) / 2);
   }
}

template <class X, class M>
int LazySegmentTree<X, M>::sub_find_leftest_geq(size_t a, size_t b, X x, size_t k, size_t l, size_t r) {
   // 遅延評価を反映させる
   eval(k, r - l);

   if (!(data_[k] >= x) || r <= a || b <= l) {
      // 自分の値がxより小さい or [a, b)が[l, r)の範囲外
      return -1;
   } else if (k >= leaf_size_ - 1) {
      // 自分が葉ノード
      return k - leaf_size_ + 1;
   } else {
      // 左の部分木を見てa-1以外ならその値を返す
      int val_l = sub_find_leftest_geq(a, b, x, k * 2 + 1, l, (l + r) / 2);

      if (val_l != -1) {
         return val_l;
      }

      return sub_find_leftest_geq(a, b, x, k * 2 + 2, (l + r) / 2, r);
   }
}

template <class X, class M>
int LazySegmentTree<X, M>::sub_find_rightest_geq(size_t a, size_t b, X x, size_t k, size_t l, size_t r) {
   // 遅延評価を反映させる
   eval(k, r - l);

   if (!(data_[k] >= x) || r <= a || b <= l) {
      // 自分の値がxより小さい or [a, b)が[l, r)の範囲外
      return -1;
   } else if (k >= leaf_size_ - 1) {
      // 自分が葉ノード
      return k - leaf_size_ + 1;
   } else {
      // 右の部分木を見てa-1以外ならその値を返す
      int val_r = sub_find_rightest_geq(a, b, x, k * 2 + 2, (l + r) / 2, r);

      if (val_r != -1) {
         return val_r;
      }

      return sub_find_rightest_geq(a, b, x, k * 2 + 1, l, (l + r) / 2);
   }
}

template <class X, class M>
int LazySegmentTree<X, M>::sub_find_leftest_sum_geq(size_t a, size_t b, X x, size_t k, size_t l, size_t r) {
   // 遅延評価を反映させる
   eval(k, r - l);

   if (!(data_[k] >= x) || r <= a || b <= l) {
      // 自分の値がxより小さい or [a, b)が[l, r)の範囲外
      return -1;
   } else if (k >= leaf_size_ - 1) {
      // 自分が葉ノード
      return k - leaf_size_ + 1;
   } else {
      // 左の部分木を見てa-1以外ならその値を返す
      int val_l = sub_find_leftest_sum_geq(a, b, x, k * 2 + 1, l, (l + r) / 2);

      if (val_l != -1) {
         return val_l;
      }

      X sub_sum = sub_query(a, b, k * 2 + 1, l, (l + r) / 2);
      return sub_find_leftest_sum_geq(a, b, x - sub_sum, k * 2 + 2, (l + r) / 2, r);
   }
}

// [End] Lazy Segment Tree

void LazySegOper() {
   {
      // clang-format off
      // [Start] Lazy Segment Tree(Sum)
      // [Prefix] laz-seg-oper-sum-inline
using X = long long;

// 合計
auto oper = [](X x, X y) -> X { return x + y; };
const X unit_x = 0;  // Xの単位元
// [End] Lazy Segment Tree(Sum)
      // clang-format on
   }
   {
      // clang-format off
      // [Start] Lazy Segment Tree(Min)
      // [Prefix] laz-seg-oper-min-inline
using X = long long;

// 最小値
auto oper = [](X x, X y) -> X { return min(x, y); };
const X unit_x = numeric_limits<X>::max();  // Xの単位元
// [End] Lazy Segment Tree(Min)
      // clang-format on
   }
   {
      // clang-format off
      // [Start] Lazy Segment Tree(Max)
      // [Prefix] laz-seg-oper-max-inline
using X = long long;

// 最大値
auto oper = [](X x, X y) -> X { return max(x, y); };
const X unit_x = numeric_limits<X>::min();  // Xの単位元
      // [End] Lazy Segment Tree(Max)
                                                  // clang-format on
   }
   {
      // clang-format off
      // [Start] Lazy Segment Tree(XOR)
      // [Prefix] laz-seg-oper-xor-inline
using X = long long;

// XOR
auto oper = [](X x, X y) -> X { return x ^ y; };
const X unit_x = 0;  // Xの単位元
      // [End] Lazy Segment Tree(XOR)
                           // clang-format on
   }
}

void LazySegUpdateOper() {
   {
      vector<long long> init_array;
      using X = long long;
      auto oper = [](X x, X y) -> X { return x ^ y; };
      const X unit_x = 0;  // Xの単位元

      // clang-format off
      // [Start] Lazy Segment Tree(range update)
      // [Prefix] laz-seg-range-update-inline
// 区間更新
using M = long long;

auto eval_op = [](X x, M m) -> X { return m; };
const M unit_m = numeric_limits<M>::max();  // Mの単位元

auto lazy_op = [](M m1, M m2) -> M { return m2; };
auto lazy_p_op = [](M m, size_t n) -> M { return; };  // operがsumの場合は m * nとする, min/maxは区間に依存しないのでmを返す, XORはnが偶なら0, 奇ならm

LazySegmentTree<X, M> laz_seg_tree(init_array, oper, eval_op, lazy_op, lazy_p_op, unit_x, unit_m);
      // [End] Lazy Segment Tree(range update)
      // clang-format on
   }
   {
      vector<long long> init_array;
      using X = long long;
      auto oper = [](X x, X y) -> X { return x ^ y; };
      const X unit_x = 0;  // Xの単位元

      // clang-format off
      // [Start] Lazy Segment Tree(range add)
      // [Prefix] laz-seg-range-add-inline
// 区間加算
using M = long long;

auto eval_op = [](X x, M m) -> X { return x + m; };
const M unit_m = 0;  // Mの単位元

auto lazy_op = [](M m1, M m2) -> M { return m1 + m2; };
auto lazy_p_op = [](M m, size_t n) -> M { return; };  // operがsumの場合は m * nとする, min/maxは区間に依存しないのでmを返す, XORはnが偶なら0, 奇ならm

LazySegmentTree<X, M> laz_seg_tree(init_array, oper, eval_op, lazy_op, lazy_p_op, unit_x, unit_m);
      // [End] Lazy Segment Tree(range add)
      // clang-format on
   }
}