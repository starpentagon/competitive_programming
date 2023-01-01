#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

// 正の整数Nのすべての約数を求める
// @param divisor_list : 約数の格納先
// 計算量 : O(sqrt{N})
vector<long long> EnumerateDivisors(long long N) {
   assert(N >= 1);

   vector<long long> smalls, bigs;

   for (long long i = 1; i * i <= N; i++) {
      if (N % i != 0) {
         continue;
      }

      smalls.emplace_back(i);

      if (N / i != i) {
         bigs.emplace_back(N / i);
      }
   }

   vector<long long> divisor_list;
   divisor_list.reserve(smalls.size() + bigs.size());

   reverse(bigs.begin(), bigs.end());

   for (auto v : smalls) {
      divisor_list.emplace_back(v);
   }

   for (auto v : bigs) {
      divisor_list.emplace_back(v);
   }

   return divisor_list;
}

int main() {
   int a, b, c;
   cin >> a >> b >> c;

   auto div_list = EnumerateDivisors(c);

   int ans = 0;

   for (auto d : div_list) {
      if (a <= d && d <= b) ans++;
   }

   cout << ans << endl;
   return 0;
}