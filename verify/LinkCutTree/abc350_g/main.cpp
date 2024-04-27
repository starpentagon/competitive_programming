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

// @ref https://ei1333.github.io/luzhiled/snippets/structure/link-cut-tree.html
// @ref https://ei1333.github.io/library/structure/lct/link-cut-tree.hpp.html
template <typename Monoid = long long, typename OperatorMonoid = Monoid>
class LinkCutTree {
  public:
   using F = function<Monoid(Monoid, Monoid)>;                          // 2つの区間をマージする二項演算子
   using G = function<Monoid(Monoid, OperatorMonoid, int)>;             // 要素と作用素をマージする二項演算子
   using H = function<OperatorMonoid(OperatorMonoid, OperatorMonoid)>;  // 作用素同士をマージする二項演算子
   using S = function<Monoid(Monoid)>;                                  // 要素を反転する演算子

   // ノードの値を加算、作用素がない場合
   // auto f = [](int a, int b) { return a + b; };
   // auto g = [](int a, int b, int l) { return 0; };
   // auto h = [](int a, int b) { return 0; };
   // auto s = [](int a) { return a; };
   // LinkCutTree<int> lct(f, g, h, s, 0, 0);

   struct Node {
      int idx;
      Monoid key, sum;
      OperatorMonoid lazy;

      int sz;
      Node *l, *r, *p;
      bool rev;

      bool is_root() {
         return !p || (p->l != this && p->r != this);
      }

      Node(int idx, const Monoid &key, const OperatorMonoid &om)
          : idx(idx), key(key), sum(key), lazy(om), sz(1), l(nullptr), r(nullptr), p(nullptr), rev(false) {
      }
   };

   const F f;
   const G g;
   const H h;
   const S s;
   const Monoid M1;
   const OperatorMonoid OM0;

   LinkCutTree()
       : LinkCutTree([](Monoid a, Monoid b) { return a + b; }, [](Monoid a) { return a; }, Monoid()) {
      node_ptr_list_.push_back(nullptr);  // 1-indexedにするために先頭にdummyを追加
   }

   LinkCutTree(const F &f, const S &s, const Monoid &M1)
       : LinkCutTree(f, G(), H(), s, M1, OperatorMonoid()) {
      node_ptr_list_.push_back(nullptr);  // 1-indexedにするために先頭にdummyを追加
   }

   LinkCutTree(const F &f, const G &g, const H &h, const S &s,
               const Monoid &M1, const OperatorMonoid &OM0)
       : f(f), g(g), h(h), s(s), M1(M1), OM0(OM0) {
      node_ptr_list_.push_back(nullptr);  // 1-indexedにするために先頭にdummyを追加
   }

   ~LinkCutTree() {
      for (Node *node : node_ptr_list_) delete node;
   }

   // ノードを追加する
   // v: ノードの値
   int MakeNode(const Monoid &v = Monoid()) {
      int n = node_ptr_list_.size();
      node_ptr_list_.push_back(make_node(n, v));
      return n;
   }

   // child_node_idのノードをparent_node_idのノードに接続する
   // ならし計算量: O(log N)
   void Link(int child_node_id, int parent_node_id) {
      Node *child_node = GetNode(child_node_id);
      Node *parent_node = GetNode(parent_node_id);
      assert(get_root(child_node) != get_root(parent_node));

      evert(child_node);  // childはもともとの木の根である必要がある
      link(child_node, parent_node);
   }

   // child_nodeと親ノードを切り離す
   // ならし計算量: O(log N)
   void Cut(int child_node_id) {
      Node *child_node = GetNode(child_node_id);
      assert(child_node->l);  // 親ノードが存在することを確認

      cut(child_node);
   }

   // nodeを根にする
   // ならし計算量: O(log N)
   void SetRoot(int node_id) {
      evert(GetNode(node_id));
   }

   // 根を求める
   // ならし計算量: O(log N)
   int GetRoot(int node_id) {
      return get_root(GetNode(node_id))->idx;
   }

   // 根までにパスに出現する頂点リストを返す
   // 計算量: O(N)
   vector<int> GetPathToRoot(int node_id) {
      return get_path(GetNode(node_id));
   }

   // node_idから親までのパスに出現する頂点を並べたときにk番目に出現する頂点を求める
   // k=0ならnode_idを返す, kに対応するノードが無ければ-1を返す
   // ならし計算量: O(log N)
   int GetKthNode(int node_id, int k) {
      Node *node = GetNode(node_id);
      Node *res = get_kth(node, k);
      if (res == nullptr) return -1;
      return res->idx;
   }

   // uからvまでのパス上に出現する頂点を並べたときにk番目に出現する頂点を求める
   // 同じ木に所属していない場合は-1を返す
   int GetKthNode(int u, int v, int k) {
      if (!IsConnected(u, v)) return -1;

      Node *v_node = GetNode(v);
      evert(v_node);
      return GetKthNode(u, k);
   }

   // 同じ木に属するか判定する
   // ならし計算量: O(log N)
   bool IsConnected(int u, int v) {
      return get_root(GetNode(u)) == get_root(GetNode(v));
   }

   // LCAを求める
   // ならし計算量: O(log N)
   // u, vが違う木に所属する場合は-1を返す
   int GetLCA(int u, int v) {
      Node *u_node = GetNode(u);
      Node *v_node = GetNode(v);
      if (get_root(u_node) != get_root(v_node)) return -1;

      Node *lca_node = lca(u_node, v_node);
      return lca_node->idx;
   }

   // node_idのノードの値を更新する
   void SetNodeVal(int node_id, const Monoid &val) {
      Node *node = GetNode(node_id);

      expose(node);
      node->key = val;
      update(GetNode(node_id));
   }

   // uから根までのパス上の頂点の値を二項演算子でまとめた結果を返す
   Monoid Query(int u) {
      Node *node = GetNode(u);
      expose(node);
      return node->sum;
   }

   // uからvまでのパス上の頂点の値を二項演算子でまとめた結果を返す
   Monoid Query(int u, int v) {
      evert(GetNode(u));
      return Query(v);
   }

  protected:
   Node *GetNode(int node_id) {
      assert(1 <= node_id && node_id < (int)node_ptr_list_.size());
      return node_ptr_list_[node_id];
   }

   Node *make_node(int idx, const Monoid &v = Monoid()) {
      return new Node(idx, v, OM0);
   }

   void propagate(Node *t, const OperatorMonoid &x) {
      t->lazy = h(t->lazy, x);
      t->key = g(t->key, x, 1);
      t->sum = g(t->sum, x, t->sz);
   }

   void toggle(Node *t) {
      assert(t);
      swap(t->l, t->r);
      t->sum = s(t->sum);
      t->rev ^= true;
   }

   void push(Node *t) {
      if (t->lazy != OM0) {
         if (t->l) propagate(t->l, t->lazy);
         if (t->r) propagate(t->r, t->lazy);
         t->lazy = OM0;
      }
      if (t->rev) {
         if (t->l) toggle(t->l);
         if (t->r) toggle(t->r);
         t->rev = false;
      }
   }

   void update(Node *t) {
      t->sz = 1;
      t->sum = t->key;
      if (t->l) t->sz += t->l->sz, t->sum = f(t->l->sum, t->sum);
      if (t->r) t->sz += t->r->sz, t->sum = f(t->sum, t->r->sum);
   }

   void rotr(Node *t) {
      auto *x = t->p, *y = x->p;
      if ((x->l = t->r)) t->r->p = x;
      t->r = x, x->p = t;
      update(x), update(t);
      if ((t->p = y)) {
         if (y->l == x) y->l = t;
         if (y->r == x) y->r = t;
         update(y);
      }
   }

   void rotl(Node *t) {
      auto *x = t->p, *y = x->p;
      if ((x->r = t->l)) t->l->p = x;
      t->l = x, x->p = t;
      update(x), update(t);
      if ((t->p = y)) {
         if (y->l == x) y->l = t;
         if (y->r == x) y->r = t;
         update(y);
      }
   }

   void splay(Node *t) {
      push(t);
      while (!t->is_root()) {
         auto *q = t->p;
         if (q->is_root()) {
            push(q), push(t);
            if (q->l == t)
               rotr(t);
            else
               rotl(t);
         } else {
            auto *r = q->p;
            push(r), push(q), push(t);
            if (r->l == q) {
               if (q->l == t)
                  rotr(q), rotr(t);
               else
                  rotl(t), rotr(t);
            } else {
               if (q->r == t)
                  rotl(q), rotl(t);
               else
                  rotr(t), rotl(t);
            }
         }
      }
   }

   Node *expose(Node *t) {
      Node *rp = nullptr;
      for (Node *cur = t; cur; cur = cur->p) {
         splay(cur);
         cur->r = rp;
         update(cur);
         rp = cur;
      }
      splay(t);
      return rp;
   }

   void link(Node *child, Node *parent) {
      expose(child);
      expose(parent);
      child->p = parent;
      parent->r = child;
      update(parent);
   }

   void cut(Node *child) {
      expose(child);
      auto *parent = child->l;
      child->l = nullptr;
      parent->p = nullptr;
      update(child);
   }

   void evert(Node *t) {
      expose(t);
      toggle(t);
      push(t);
   }

   Node *lca(Node *u, Node *v) {
      if (get_root(u) != get_root(v)) return nullptr;
      expose(u);
      return expose(v);
   }

   vector<int> get_path(Node *x) {
      vector<int> vs;
      function<void(Node *)> dfs = [&](Node *cur) {
         if (!cur) return;
         push(cur);
         dfs(cur->r);
         vs.push_back(cur->idx);
         dfs(cur->l);
      };
      expose(x);
      dfs(x);
      return vs;
   }

   void set_propagate(Node *t, const OperatorMonoid &x) {
      expose(t);
      propagate(t, x);
      push(t);
   }

   Node *get_kth(Node *x, int k) {
      expose(x);
      while (x) {
         push(x);
         if (x->r && x->r->sz > k) {
            x = x->r;
         } else {
            if (x->r) k -= x->r->sz;
            if (k == 0) return x;
            k -= 1;
            x = x->l;
         }
      }
      return nullptr;
   }

   Node *get_root(Node *x) {
      expose(x);
      while (x->l) {
         push(x);
         x = x->l;
      }
      return x;
   }

   vector<Node *> node_ptr_list_;  // 生成したノードポインタを管理する
};

int main() {
   int N, Q;
   cin >> N >> Q;

   auto f = [](int a, int b) { return a + b; };
   auto g = [](int a, int b, int l) { return 0; };
   auto h = [](int a, int b) { return 0; };
   auto s = [](int a) { return a; };

   LinkCutTree<int> lct(f, g, h, s, 0, 0);

   rep(i, N) {
      lct.MakeNode(i);
   }

   auto solve = [&](int u, int v) {
      int p_p_u = lct.GetKthNode(u, v, 2);
      if (p_p_u == v) {
         return lct.GetKthNode(u, v, 1);
      }
      return 0;
   };

   ll x = 0;

   rep(q, Q) {
      ll a, b, c;
      cin >> a >> b >> c;

      ll A = 1 + (a * (1 + x) % 998244353) % 2;
      ll B = 1 + (b * (1 + x) % 998244353) % N;
      ll C = 1 + (c * (1 + x) % 998244353) % N;

      if (A == 1) {
         lct.Link(B, C);
      } else {
         ll ans = solve(B, C);
         cout << ans << endl;
         x = ans;
      }
   }

   cout << flush;

   return 0;
}
