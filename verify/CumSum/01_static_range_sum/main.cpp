#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#define debug(...) debug_print::multi_print(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) ;
#endif

// clang-format off
#define rep(i, n) for (int i = 0; (i) < (int)(n); (i)++)
 
template<class T> bool chmax(T &a, const T &b) {if(a<b) {a=b; return true;} return false; }
template<class T> bool chmin(T &a, const T &b) {if(a>b) {a=b; return true;} return false; }
 
template<class T> istream& operator>>(istream& is, vector<T>& vec){ rep(i, vec.size()) is >> vec[i]; return is;}
template<class T> ostream& operator<<(ostream& os, vector<T>& vec){ rep(i, vec.size()) os << vec[i] << (i+1==(int)vec.size() ? "" : " "); return os;}
// clang-format on

using ll = long long;
using ull = unsigned long long;

// 累積和を求める
template <class T>
class CumSum {
  public:
   CumSum() {
   }

   CumSum(const vector<T>& init);

   // 区間[l, r)の和(=A_l + ... + A_r-1)を求める
   T query(const int l, const int r) const;

   // cum_sum_[i]: 区間[0, i)の和
   T operator[](size_t index) const {
      return query(0, index);
   }

   friend ostream& operator<<(ostream& os, CumSum<T>& cum_sum) {
      const auto& vec = cum_sum.cum_sum_;

      rep(i, vec.size()) os << vec[i] << (i + 1 == (int)vec.size() ? "" : " ");
      return os;
   }

   typename vector<T>::const_iterator begin() const {
      return cum_sum_.begin();
   }

   typename vector<T>::const_iterator end() const {
      return cum_sum_.end();
   }

  private:
   vector<T> cum_sum_;  // cum_sum_[i]: 区間[0, i)の和
};

template <class T>
CumSum<T>::CumSum(const vector<T>& init)
    : cum_sum_(init.size() + 1, 0) {
   int N = (int)init.size();

   for (int i = 0; i < N; i++) {
      cum_sum_[i + 1] = cum_sum_[i] + init[i];
   }
}

template <class T>
T CumSum<T>::query(const int l, const int r) const {
   assert(0 <= l && l <= r && r < (int)cum_sum_.size());
   return cum_sum_[r] - cum_sum_[l];
}

int main() {
   ll N, Q;
   cin >> N >> Q;

   vector<ll> al(N);
   cin >> al;

   CumSum<ll> cum_sum(al);

   rep(q, Q) {
      ll l, r;
      cin >> l >> r;

      auto ans = cum_sum.query(l, r);

      cout << ans << endl;
   }

   return 0;
}