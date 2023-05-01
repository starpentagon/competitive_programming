#include <iostream>

using namespace std;

// [Start] Interactive framework
// [Prefix] interactive-class
// [Verified] ABC 244「C - Yamanote Line Game」(https://atcoder.jp/contests/abc244/tasks/abc244_c)
// インタラクティブ問題用のジャッジクラス
class Judge {
  public:
   Judge() {
   }

   // ジャッジから情報を受け取る
   void ReceiveInfo();

   // ジャッジに情報を送る
   void SendInfo();

  protected:
};

void Judge::ReceiveInfo() {
#ifdef LOCAL
   // ジャッジをシミュレート

#else
   // 提出時の動作
   int dummy;
   cin >> dummy;
#endif
}

void Judge::SendInfo() {
#ifdef LOCAL
   // ジャッジをシミュレート

#else
   // 提出時の動作
   int dummy;
   cout << dummy;
#endif
}
// [End] Interactive framework
