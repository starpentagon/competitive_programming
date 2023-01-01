#include <iostream>
#include <cassert>

using namespace std;

// 与えられた整数Nが素数かどうかを判定する
// 計算量: O(sqrt{N})
bool IsPrime(const long long N) {
   assert(N >= 1);

   if (N == 1) {
      return false;
   }

   for (long long i = 2; i * i <= N; i++) {
      if (N % i == 0) {
         return false;
      }
   }

   return true;
}

int main() {
   int q;
   cin >> q;

   int ans = 0;

   while (q--) {
      int n;
      cin >> n;

      if (IsPrime(n)) ans++;
   }

   cout << ans << endl;
   return 0;
}