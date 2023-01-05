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

// 2次元配列上の縦横斜めの累積和/矩形和を求める
template <class T>
class CumSum2D {
  public:
   CumSum2D(const vector<vector<T>>& init);

   // 2点P1, P2間の和を求める(P1, P2をともに含む閉区間で計算する)
   // @note 2点が縦横斜めの直線上であれば配列の範囲外でも動作可能
   T query_line(const pair<int, int>& p1, const pair<int, int>& p2) const;

   // (a, b), (c, d)を頂点とする矩形の和を返す
   T query_area(const pair<int, int>& p1, const pair<int, int>& p2) const;

   friend ostream& operator<<(ostream& os, CumSum2D<T>& cum_sum) {
      const auto& vec = cum_sum.cum_sum_;

      rep(i, vec.size()) {
         rep(j, vec[i].size()) {
            os << vec[i][j] << (j + 1 == (int)vec[i].size() ? "" : " ");
         }
         os << endl;
      }
      return os;
   }

  private:
   // 右方向のクエリ
   T query_right(const pair<int, int>& p1, const pair<int, int>& p2) const;

   // 下方向のクエリ
   T query_down(const pair<int, int>& p1, const pair<int, int>& p2) const;

   // 右下方向のクエリ
   T query_right_down(const pair<int, int>& p1, const pair<int, int>& p2) const;

   // 左下方向のクエリ
   T query_left_down(const pair<int, int>& p1, const pair<int, int>& p2) const;

   int H_;  // 2次元配列の高さ
   int W_;  // 2次元配列の幅

   vector<vector<T>> right_;         // 右方向 right_[h][w]: init[h][0]からinit[h][w-1]までの和
   vector<vector<T>> down_;          // 下方向 down_[w][h] : init[0][w]からinit[h-1][w]までの和
   vector<vector<T>> right_down_h_;  // 右下方向(h-w>=0)
   vector<vector<T>> right_down_w_;  // 右下方向(w-h>=0)
   vector<vector<T>> left_down_h_;   // 左下方向(0<=h+w<=W-1)
   vector<vector<T>> left_down_w_;   // 左下方向(h+w>=W-1)

   vector<vector<T>> cum_sum_;  // (0, 0), (a, b)を2頂点とする矩形の和
};

template <class T>
CumSum2D<T>::CumSum2D(const vector<vector<T>>& init) {
   assert(!init.empty());

   H_ = (int)init.size();
   W_ = (int)init[0].size();

   // 累積和
   cum_sum_.resize(H_);

   rep(i, H_) {
      cum_sum_[i].resize(W_, 0);
   }

   rep(j, W_) {
      if (j == 0)
         cum_sum_[0][j] = init[0][j];
      else
         cum_sum_[0][j] = cum_sum_[0][j - 1] + init[0][j];
   }

   for (int i = 1; i < H_; i++) {
      rep(j, W_) {
         if (j == 0)
            cum_sum_[i][j] = cum_sum_[i - 1][j] + init[i][j];
         else
            cum_sum_[i][j] = cum_sum_[i - 1][j] + cum_sum_[i][j - 1] - cum_sum_[i - 1][j - 1] + init[i][j];
      }
   }

   // 右方向
   right_.resize(H_);

   rep(h, H_) {
      right_[h].resize(W_ + 1);

      rep(w, W_) {
         right_[h][w + 1] = right_[h][w] + init[h][w];
      }
   }

   // 下方向
   down_.resize(W_);

   rep(w, W_) {
      down_[w].resize(H_ + 1);

      rep(h, H_) {
         down_[w][h + 1] = down_[w][h] + init[h][w];
      }
   }

   // 右下方向(h-w=k, k=0,...,H-1)
   right_down_h_.resize(H_);

   rep(h, H_) {
      // (h+d, d), d=0,...,D-1を動く直線
      int D = min(W_, H_ - h);
      right_down_h_[h].resize(D + 1);

      rep(d, D) {
         int hi = h + d;
         int wi = d;
         right_down_h_[h][d + 1] = right_down_h_[h][d] + init[hi][wi];
      }
   }

   // 右下方向(w-h=k, k=0,...,W-1)
   right_down_w_.resize(W_);

   rep(w, W_) {
      // (d, w+d), d=0,...,D-1を動く直線
      int D = min(H_, W_ - w);
      right_down_w_[w].resize(D + 1);

      rep(d, D) {
         int hi = d;
         int wi = w + d;
         right_down_w_[w][d + 1] = right_down_w_[w][d] + init[hi][wi];
      }
   }

   // 左下方向(h+w=k, 0<= k <=W-1)
   left_down_w_.resize(W_);

   rep(w, W_) {
      // (d, w-d), d=0,...,D-1を動く直線
      int D = min(w + 1, H_);
      left_down_w_[w].resize(D + 1);

      rep(d, D) {
         int hi = d;
         int wi = w - d;

         left_down_w_[w][d + 1] = left_down_w_[w][d] + init[hi][wi];
      }
   }

   // 左下方向(h+w=k, W-1 <= k <= W+H-2)
   left_down_h_.resize(H_);

   rep(h, H_) {
      // (h+d, W-d-1), d=0,...,D-1を動く直線
      int D = min(W_, H_ - h);
      left_down_h_[h].resize(D + 1);

      rep(d, D) {
         int hi = h + d;
         int wi = W_ - 1 - d;

         left_down_h_[h][d + 1] = left_down_h_[h][d] + init[hi][wi];
      }
   }
}

template <class T>
T CumSum2D<T>::query_area(const pair<int, int>& p1, const pair<int, int>& p2) const {
   auto [a, b] = p1;
   auto [c, d] = p2;

   assert(a <= c && b <= d);

   T count = cum_sum_[c][d];

   if (a - 1 >= 0) {
      count -= cum_sum_[a - 1][d];
   }

   if (b - 1 >= 0) {
      count -= cum_sum_[c][b - 1];
   }

   if (a - 1 >= 0 && b - 1 >= 0) {
      count += cum_sum_[a - 1][b - 1];
   }

   return count;
}

template <class T>
T CumSum2D<T>::query_line(const pair<int, int>& p1, const pair<int, int>& p2) const {
   auto [h1, w1] = p1;
   auto [h2, w2] = p2;

   auto dh = h2 - h1, dw = w2 - w1;

   if (dh == 0) {
      // 横方向
      return query_right(p1, p2);
   } else if (dw == 0) {
      // 縦方向
      return query_down(p1, p2);
   } else if (dh + dw == 0) {
      // 左下方向
      return query_left_down(p1, p2);
   } else {
      // 右下方向
      assert(dh - dw == 0);
      return query_right_down(p1, p2);
   }
}

template <class T>
T CumSum2D<T>::query_right(const pair<int, int>& p1, const pair<int, int>& p2) const {
   auto [h1, w1] = p1;
   auto [h2, w2] = p2;

   if (!(0 <= h1 && h1 < H_)) {
      return 0;
   }

   int w_l = min(w1, w2);
   int w_r = max(w1, w2);

   w_l = max(w_l, 0);
   w_r = min(w_r, W_ - 1);

   return right_[h1][w_r + 1] - right_[h1][w_l];
}

template <class T>
T CumSum2D<T>::query_down(const pair<int, int>& p1, const pair<int, int>& p2) const {
   auto [h1, w1] = p1;
   auto [h2, w2] = p2;

   if (!(0 <= w1 && w1 < W_)) {
      return 0;
   }

   int h_l = min(h1, h2);
   int h_r = max(h1, h2);

   h_l = max(h_l, 0);
   h_r = min(h_r, H_ - 1);

   return down_[w1][h_r + 1] - down_[w1][h_l];
}

template <class T>
T CumSum2D<T>::query_right_down(const pair<int, int>& p1, const pair<int, int>& p2) const {
   auto [h1, w1] = p1;
   auto [h2, w2] = p2;

   int k = h1 - w1;

   if (k >= 0) {
      if (k >= H_) {
         return 0;
      }

      int d_l = min(w1, w2);
      int d_r = max(w1, w2);
      int D = min(W_, H_ - k);

      d_l = max(d_l, 0);
      d_r = min(d_r, D - 1);

      return right_down_h_[k][d_r + 1] - right_down_h_[k][d_l];
   } else {
      k = -k;

      if (k >= W_) {
         return 0;
      }

      int d_l = min(h1, h2);
      int d_r = max(h1, h2);
      int D = min(H_, W_ - k);

      d_l = max(d_l, 0);
      d_r = min(d_r, D - 1);

      return right_down_w_[k][d_r + 1] - right_down_h_[k][d_l];
   }
}

template <class T>
T CumSum2D<T>::query_left_down(const pair<int, int>& p1, const pair<int, int>& p2) const {
   auto [h1, w1] = p1;
   auto [h2, w2] = p2;

   int k = h1 + w1;

   if (k <= W_ - 1) {
      if (k < 0) {
         return 0;
      }

      int d_l = min(h1, h2);
      int d_r = max(h1, h2);
      int D = min(k + 1, H_);

      d_l = max(d_l, 0);
      d_r = min(d_r, D - 1);

      return left_down_w_[k][d_r + 1] - left_down_w_[k][d_l];
   } else {
      k -= W_ - 1;

      if (k >= H_) {
         return 0;
      }

      int d_l = min(W_ - 1 - w1, W_ - 1 - w2);
      int d_r = max(W_ - 1 - w1, W_ - 1 - w2);
      int D = min(W_, H_ - k);

      d_l = max(d_l, 0);
      d_r = min(d_r, D - 1);

      return left_down_h_[k][d_r + 1] - left_down_h_[k][d_l];
   }
}

int main() {
   // H: height, W: width
   int H, W;
   cin >> H >> W;
   vector<vector<int>> grid(H + 1, vector<int>(W + 1));

   for (int i = 1; i <= H; i++) {
      for (int j = 1; j <= W; j++) {
         int c;
         cin >> c;

         if ((i + j) % 2 == 0) {
            c = -c;
         }

         grid[i][j] = c;
      }
   }

   CumSum2D<int> cum_sum(grid);
   int ans = 0;

   for (int i1 = 1; i1 <= H; i1++) {
      for (int j1 = 1; j1 <= W; j1++) {
         for (int i2 = i1; i2 <= H; i2++) {
            for (int j2 = j1; j2 <= W; j2++) {
               int sum = cum_sum.query_area({i1, j1}, {i2, j2});

               if (sum == 0) {
                  int area = (i2 - i1 + 1) * (j2 - j1 + 1);
                  chmax(ans, area);
               }
            }
         }
      }
   }

   cout << ans << endl;

   return 0;
}