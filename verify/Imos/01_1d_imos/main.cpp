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

// 1次元のいもす法
template <class T>
class Imos {
  public:
   Imos(const size_t N);

   // 閉区間[l, r]に値を設定する(0-indexed)
   // @pre 0 <= l < N, l <= r(rはN以上でも可)
   void SetValue(const size_t l, const size_t r, const T& value);

   // 区間の累積値を明示的に計算する
   // @note []演算子でアクセスする際に自動で展開される。明示的に計算することを示す際に使用する。
   // 計算量: O(N)
   void Expand();

   // 累積値を取得する(0-indexed)
   // @note 取得はO(1)だが累積値が未計算の場合は自動で計算されO(N)かかる。
   const T& operator[](size_t i);

   // 要素数を返す
   size_t size() const {
      return data_.size();
   }

   // 展開前の配列を取得する
   const vector<T>& GetInOutArray() const {
      return in_out_;
   }

   // 展開後の配列を取得する
   // @note 自動でExpandは行わない
   const vector<T>& GetDataArray() const {
      if (!expand_) cerr << "Warning: not expanded" << endl;
      return data_;
   }

   friend ostream& operator<<(ostream& os, const Imos<T>& rhs) {
      auto vec = rhs.GetDataArray();
      os << vec;
      return os;
   }

   typename vector<T>::const_iterator begin() {
      if (!expand_) Expand();
      return data_.begin();
   }

   typename vector<T>::const_iterator end() {
      return data_.end();
   }

  private:
   size_t N_;          // 区間長
   bool expand_;       // 展開済かのフラグ
   vector<T> in_out_;  // 展開前のin/out情報を格納する
   vector<T> data_;    // 展開後のデータを格納する
};

template <class T>
Imos<T>::Imos(const size_t N)
    : N_(N), expand_(false), in_out_(N, 0), data_(N, 0) {
}

template <class T>
void Imos<T>::SetValue(const size_t l, const size_t r, const T& value) {
   assert(0 <= l && l < N_);
   assert(l <= r);

   expand_ = false;

   in_out_[l] += value;

   if (r + 1 < N_)
      in_out_[r + 1] += -value;
}

template <class T>
void Imos<T>::Expand() {
   if (expand_)
      return;

   expand_ = true;

   data_[0] = in_out_[0];

   for (size_t i = 1; i < N_; i++) {
      data_[i] = data_[i - 1] + in_out_[i];
   }
}

template <class T>
const T& Imos<T>::operator[](size_t i) {
   Expand();
   assert(0 <= i && i < N_);

   return data_[i];
}

int main() {
   int N, T;
   cin >> N >> T;

   Imos<int> imos(T + 1);

   rep(i, N) {
      int l, r;
      cin >> l >> r;

      imos.SetValue(l, r - 1, 1);
   }

   int ans = 0;

   for (auto v : imos) {
      chmax(ans, v);
   }

   cout << ans << endl;

   return 0;
}