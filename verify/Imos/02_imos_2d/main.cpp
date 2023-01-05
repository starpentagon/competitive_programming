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

// 2次元のいもす法
template <class T>
class Imos2D {
  public:
   // H行, W列のGridを生成する
   Imos2D(const size_t H, const size_t W);

   // 左上(i1, j1), 右下(i2, j2)(1-index, 閉領域)を頂点とする長方形領域に値を設定する
   // @pre 1 <= i1 <= H, 1 <= j1 <= W
   // @pre i1 <= i2, j1 <= j2
   void SetValue(const size_t i1, const size_t j1, const size_t i2, const size_t j2, const T& value);

   // 区間の累積値を明示的に計算する
   // @note []演算子でアクセスする際に自動で展開される。明示的に計算することを示す際に使用する。
   // 計算量: O(HW)
   void Expand();

   // 累積値を取得する(1-indexed)
   // @note 取得はO(1)だが累積値が未計算の場合は自動で計算されO(HW)かかる。
   // @pre 1 <= i <= H, 1 <= j <= W
   const T& get(const size_t i, const size_t j);

  private:
   size_t H_;                  // 行数
   size_t W_;                  // 列数
   bool expand_;               // 展開済かのフラグ
   vector<vector<T>> in_out_;  // 展開前のin/out情報を格納する
   vector<vector<T>> data_;    // 展開後のデータを格納する
};

template <class T>
Imos2D<T>::Imos2D(const size_t H, const size_t W)
    : H_(H), W_(W), expand_(false), in_out_(H + 1, vector<T>(W + 1, 0)), data_(H + 1, vector<T>(W + 1, 0)) {
}

template <class T>
void Imos2D<T>::SetValue(const size_t i1, const size_t j1, const size_t i2, const size_t j2, const T& value) {
   assert(1 <= i1 && i1 <= H_);
   assert(1 <= j1 && j1 <= W_);
   assert(i1 <= i2 && j1 <= j2);

   expand_ = false;

   in_out_[i1][j1] += value;

   if (j2 + 1 <= W_) {
      in_out_[i1][j2 + 1] += -value;
   }

   if (i2 + 1 <= H_) {
      in_out_[i2 + 1][j1] += -value;

      if (j2 + 1 <= W_) {
         in_out_[i2 + 1][j2 + 1] += value;
      }
   }
}

template <class T>
void Imos2D<T>::Expand() {
   if (expand_)
      return;

   expand_ = true;

   // 横方向の更新
   for (size_t i = 1; i <= H_; i++) {
      for (size_t j = 1; j <= W_; j++) {
         data_[i][j] = data_[i][j - 1] + in_out_[i][j];
      }
   }

   // 縦方向の更新
   for (size_t i = 2; i <= H_; i++) {
      for (size_t j = 1; j <= W_; j++) {
         data_[i][j] += data_[i - 1][j];
      }
   }
}

template <class T>
const T& Imos2D<T>::get(const size_t i, const size_t j) {
   Expand();
   assert(1 <= i && i <= H_);
   assert(1 <= j && j <= W_);

   return data_[i][j];
}

int main() {
   int N;
   cin >> N;

   Imos2D<int> imos(1005, 1005);

   rep(i, N) {
      int x1, y1, x2, y2;
      cin >> x1 >> y1 >> x2 >> y2;

      x1++;
      y1++;
      x2++;
      y2++;

      imos.SetValue(x1, y1, x2 - 1, y2 - 1, 1);
   }

   int ans = 0;

   for (int x = 1; x <= 1001; x++) {
      for (int y = 1; y <= 1001; y++) {
         int cnt = imos.get(x, y);
         chmax(ans, cnt);
      }
   }

   cout << ans << endl;

   return 0;
}