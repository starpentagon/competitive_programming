#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>

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

// セグメント木
// 計算量
// - 初期構築 O(N)
// - 一点更新 O(log N)
// - 添え字区間[a, b)の要素に対する演算結果 O(log N)
template <class X>
class SegmentTree {
  public:
   // @param val_list セグメント木を構築する配列
   // @param oper 二項演算子(最大、最小、XORなど)
   // @param unit 単位元(max: 0, min: INF, XOR: 0)
   SegmentTree(const vector<X>& val_list, function<X(X, X)> oper, const X unit);

   // i番目の要素の値を取得する(0-indexed)
   X get(size_t i) const {
      return data_[leaf_index(i)];
   }

   // i番目の要素の値を更新する
   void update(size_t i, X val);

   // i番目の要素の値に加算する
   void add(size_t i, X val) {
      auto v = get(i);
      v += val;

      update(i, v);
   }

   // 添え字区間[a, b)の要素に対する二項演算結果を返す
   X query(size_t a, size_t b) const {
      return sub_query(a, b,
                       0,          // ルートノード
                       0,          // 0番目以上
                       leaf_size_  // n番目未満
      );
   }

  private:
   // i番目の要素の値を設定する
   // @note 親ノードの更新は行わない。初期構築時に利用。
   void set(size_t i, X val) {
      data_[leaf_index(i)] = val;
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
   X sub_query(size_t a, size_t b, size_t k, size_t l, size_t r) const;

   vector<X> data_;  // 完全二分木を格納する配列
   function<X(X, X)> oper_;
   X unit_;            // 二項演算子の単位元
   size_t leaf_size_;  // 管理する配列のサイズ(=セグメント木のリーフ数)
};

template <class X>
SegmentTree<X>::SegmentTree(const vector<X>& val_list, std::function<X(X, X)> oper, const X unit)
    : oper_(oper), unit_(unit), leaf_size_(1) {
   // val_listの要素をleafとする完全二分木を作る
   while (val_list.size() > leaf_size_) {
      leaf_size_ *= 2;
   }

   size_t tree_size = 2 * leaf_size_;
   data_.resize(tree_size);
   fill(data_.begin(), data_.end(), unit);

   for (size_t i = 0; i < val_list.size(); i++) {
      set(i, val_list[i]);
   }

   build();
}

template <class X>
void SegmentTree<X>::build() {
   for (ll i = (ll)leaf_size_ - 2; i >= 0; i--) {
      data_[i] = oper_(data_[2 * i + 1], data_[2 * i + 2]);
   }
}

template <class X>
void SegmentTree<X>::update(size_t i, X val) {
   set(i, val);

   auto index = leaf_index(i);

   while (index > 0) {
      // 親ノードのindex
      index = (index - 1) / 2;
      data_[index] = oper_(data_[index * 2 + 1], data_[index * 2 + 2]);
   }
}

template <class X>
X SegmentTree<X>::sub_query(size_t a, size_t b, size_t k, size_t l, size_t r) const {
   // 演算を行う範囲[a, b)と今いるノードkが表してる範囲[r, l)を比較する
   if (r <= a || b <= l) {
      // 自身の範囲が演算を行う範囲[a, b)外
      // -> 演算しなくて良いので単位元を返して終了
      return unit_;
   } else if (a <= l && r <= b) {
      // 自身の範囲が演算を行う範囲[a, b)に含まれる
      // -> 自身の値を返す
      return data_[k];
   } else {
      // 自身の範囲が演算を行う範囲[a, b)に一部含まれる
      // -> より範囲の狭い子ノードの結果を使う
      X val_l = sub_query(a, b, k * 2 + 1, l, (l + r) / 2);
      X val_r = sub_query(a, b, k * 2 + 2, (l + r) / 2, r);

      return oper_(val_l, val_r);
   }
}

int main() {
   ll N, Q;
   cin >> N >> Q;

   vector<ll> al(N);
   cin >> al;

   // - 合計
   using X = long long;

   auto oper = [](X x, X y) -> X { return x + y; };
   const X unit_x = 0;  // Xの単位元

   SegmentTree<X> seg_tree(al, oper, unit_x);

   rep(q, Q) {
      ll t;
      cin >> t;

      if (t == 0) {
         ll p, x;
         cin >> p >> x;

         seg_tree.add(p, x);
      } else {
         ll l, r;
         cin >> l >> r;

         ll ans = seg_tree.query(l, r);
         cout << ans << endl;
      }
   }

   return 0;
}