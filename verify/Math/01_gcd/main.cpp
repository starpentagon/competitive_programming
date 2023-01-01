#include <iostream>
#include <cassert>

using namespace std;

// 最大公約数を求める
// 計算量: O(log max(m, n))
long long GCD(long long m, long long n) {
   assert(m >= 0 && n >= 0);

   if (m < n) return GCD(n, m);
   // m >= n
   if (n == 0) return m;

   long long r = 0;

   while ((r = m % n)) {
      m = n;
      n = r;
   }

   return n;
}

int main() {
   int x, y;
   cin >> x >> y;

   auto ans = GCD(x, y);

   cout << ans << endl;

   return 0;
}