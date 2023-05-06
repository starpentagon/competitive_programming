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

template <class T>
vector<pair<T, long long>> RunLengthEncoding(const vector<T>& vec) {
   vector<pair<T, long long>> encode;

   for (auto v : vec) {
      if (encode.empty() || encode.back().first != v) {
         encode.emplace_back(v, 1);
      } else {
         encode.back().second++;
      }
   }

   return encode;
}

int main() {
   int N;
   cin >> N;

   vector<char> cl;
   rep(i, N) {
      char c;
      cin >> c;
      cl.emplace_back(c);
   }

   auto run_len = RunLengthEncoding(cl);

   if (run_len.size() == 1) {
      cout << -1 << endl;
      return 0;
   }

   ll ans = 0;

   for (auto [c, cnt] : run_len) {
      if (c == 'o') {
         chmax(ans, cnt);
      }
   }

   cout << ans << endl;

   return 0;
}
