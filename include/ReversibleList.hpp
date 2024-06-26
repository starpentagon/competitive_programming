#pragma once

#include <memory>
#include <vector>

// [Start] Reversible List
// [Prefix] reversible-list-class
// [Verified] N <= 5 * 10^5: ABC 350 F - Transpose(https://atcoder.jp/contests/abc350/tasks/abc350_f)
// 遅延伝搬反転可能乱択平衡二分木
// @ref https://nyaannyaan.github.io/library/rbst/lazy-reversible-rbst.hpp.html
template <typename Node>
struct RBSTBase {
   using Ptr = Node *;
   vector<Ptr> node_ptr_list;

   template <typename... Args>
   inline Ptr my_new(Args... args) {
      Ptr ret = new Node(args...);
      node_ptr_list.push_back(ret);

      return ret;
   }

   inline void my_del(Ptr t) {
   }

   inline Ptr make_tree() const {
      return nullptr;
   }

   inline void clear() {
      for (auto ptr : node_ptr_list) {
         delete ptr;
      }
      node_ptr_list.clear();
   }

   int size(Ptr t) const {
      return count(t);
   }

   Ptr merge(Ptr l, Ptr r) {
      if (!l || !r) return l ? l : r;
      if (int((rng() * (l->cnt + r->cnt)) >> 32) < l->cnt) {
         push(l);
         l->r = merge(l->r, r);
         return update(l);
      } else {
         push(r);
         r->l = merge(l, r->l);
         return update(r);
      }
   }

   pair<Ptr, Ptr> split(Ptr t, int k) {
      if (!t) return {nullptr, nullptr};
      push(t);
      if (k <= count(t->l)) {
         auto s = split(t->l, k);
         t->l = s.second;
         return {s.first, update(t)};
      } else {
         auto s = split(t->r, k - count(t->l) - 1);
         t->r = s.first;
         return {update(t), s.second};
      }
   }

   Ptr build(int l, int r, const vector<decltype(Node::key)> &v) {
      if (l + 1 == r) return my_new(v[l]);
      int m = (l + r) >> 1;
      Ptr pm = my_new(v[m]);
      if (l < m) pm->l = build(l, m, v);
      if (m + 1 < r) pm->r = build(m + 1, r, v);
      return update(pm);
   }

   Ptr build(const vector<decltype(Node::key)> &v) {
      return build(0, (int)v.size(), v);
   }

   template <typename... Args>
   void insert(Ptr &t, int k, const Args &...args) {
      auto x = split(t, k);
      t = merge(merge(x.first, my_new(args...)), x.second);
   }

   void erase(Ptr &t, int k) {
      auto x = split(t, k);
      auto y = split(x.second, 1);
      my_del(y.first);
      t = merge(x.first, y.second);
   }

  protected:
   static uint64_t rng() {
      static uint64_t x_ = 88172645463325252ULL;
      return x_ ^= x_ << 7, x_ ^= x_ >> 9, x_ & 0xFFFFFFFFull;
   }

   inline int count(const Ptr t) const {
      return t ? t->cnt : 0;
   }

   virtual void push(Ptr) = 0;

   virtual Ptr update(Ptr) = 0;
};

template <typename T, typename E>
struct LazyReversibleRBSTNode {
   typename RBSTBase<LazyReversibleRBSTNode>::Ptr l, r;
   T key, sum;
   E lazy;
   int cnt;
   bool rev;

   LazyReversibleRBSTNode(const T &t = T(), const E &e = E())
       : l(), r(), key(t), sum(t), lazy(e), cnt(1), rev(false) {
   }
};

template <typename T, typename E, T (*f)(T, T), T (*g)(T, E), E (*h)(E, E),
          T (*ts)(T)>
class ReversibleList : RBSTBase<LazyReversibleRBSTNode<T, E>> {
   using Node = LazyReversibleRBSTNode<T, E>;
   using base = RBSTBase<LazyReversibleRBSTNode<T, E>>;
   using base::merge;
   using base::split;
   using typename base::Ptr;

  public:
   ReversibleList(const vector<decltype(Node::key)> &v)
       : root_(nullptr) {
      root_ = base::build(v);
   }

   ~ReversibleList() {
      base::clear();
   }

   int size() {
      int ret = base::count(root_);
      return ret;
   }

   // k番目の要素を取得する
   // 計算量: O(log N)
   T Get(int k) {
      assert(0 <= k && k < size());
      return Query(k, k + 1);
   }

   // k番目の要素をvalに更新する
   // 計算量: O(log N)
   void Set(int k, const T &val) {
      assert(0 <= k && k < size());
      base::erase(root_, k);
      base::insert(root_, k, val);
   }

   // k番目の要素を削除する
   // 計算量: O(log N)
   void Erase(int k) {
      assert(0 <= k && k < size());
      base::erase(root_, k);
   }

   // 区間[a, b)の和を求める
   // 計算量: O(log N)
   T Query(int a, int b) {
      auto x = split(root_, a);
      auto y = split(x.second, b - a);
      auto ret = sum(y.first);
      root_ = merge(x.first, merge(y.first, y.second));
      return ret;
   }

   // 区間[a, b)を反転する
   // 計算量: O(log N)
   void Reverse(int a, int b) {
      auto x = split(root_, a);
      auto y = split(x.second, b - a);
      toggle(y.first);
      root_ = merge(x.first, merge(y.first, y.second));
   }

   // 区間[a, b)に作用素eを適用する
   // 計算量: O(log N)
   void Apply(int a, int b, const E &e) {
      auto x = split(root_, a);
      auto y = split(x.second, b - a);
      propagate(y.first, e);
      root_ = merge(x.first, merge(y.first, y.second));
   }

   // ostream出力
   // Note: debugマクロだとエラーになるが cerr << rev_list << endl; として使う
   friend ostream &operator<<(ostream &os, ReversibleList &rev_list) {
      int L = rev_list.size();
      for (int i = 0; i < L; ++i) {
         os << (int)rev_list.Get(i) << (i + 1 == L ? "" : " ");
      }
      return os;
   }

  protected:
   // 全体を反転する
   // 計算量: O(1)
   void toggle(Ptr t) {
      if (!t) return;
      swap(t->l, t->r);
      t->sum = ts(t->sum);
      t->rev ^= true;
   }

   inline T sum(const Ptr t) const {
      return t ? t->sum : T();
   }

   Ptr update(Ptr t) override {
      push(t);
      t->cnt = 1;
      t->sum = t->key;
      if (t->l) t->cnt += t->l->cnt, t->sum = f(t->l->sum, t->sum);
      if (t->r) t->cnt += t->r->cnt, t->sum = f(t->sum, t->r->sum);
      return t;
   }

   void push(Ptr t) override {
      if (t->rev) {
         if (t->l) toggle(t->l);
         if (t->r) toggle(t->r);
         t->rev = false;
      }
      if (t->lazy != E()) {
         if (t->l) propagate(t->l, t->lazy);
         if (t->r) propagate(t->r, t->lazy);
         t->lazy = E();
      }
   }

   void propagate(Ptr t, const E &x) {
      t->lazy = h(t->lazy, x);
      t->key = g(t->key, x);
      t->sum = g(t->sum, x);
   }

   Ptr root_;
};

// 区間和, 区間加算を行う場合は以下のようにする
// using T = long long;
// using E = long long;
// T f(T a, T b) { return a + b; }
// T g(T a, E b) { return a + b; }
// E h(E a, E b) { return a + b; }
// T ts(T a) { return a; }
// vector<ll> v{1, 2, 3, 4, 5};
// ReversibleList<ll, ll, f, g, h, ts> rev_list(v);
// [End] Reversible List
