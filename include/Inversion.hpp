#pragma once

#include "SegmentTree.hpp"
#include "Misc.hpp"

// [Start] Inversion
// [Prefix] inversion-func
// [Verified] N<=2*10^5: ALDS1_5_D The Number of Inversions(https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_5_D&lang=ja)
// 転倒数を求める
// 計算量: O(N log N)
// 依存ライブラリ: 座標圧縮(pos-comp), セグメント木(seg-tree)
long long CalcInversion(const vector<long long>& al) {
   PosCompression<long long> pc(al);
   auto n = pc.size();

   // - 合計
   auto oper = [](long long x, long long y) -> long long { return x + y; };
   const long long unit_x = 0;  // Xの単位元
   vector<long long> seg_init(n, 0);

   SegmentTree<long long> seg(seg_init, oper, unit_x);

   long long inv = 0;

   for (auto a : al) {
      auto r_asc = pc.Encode(a);   // r_asc : [0, n), 昇順ソートした時の順位
      auto r_des = n - 1 - r_asc;  // r_des : [0, n), 降順ソートした時の順位

      long long val_inv = seg.query(0, r_des);  // 自分より前 かつ 自分より大きい数字の個数
      inv += val_inv;

      long long v = seg.get(r_des);

      seg.update(r_des, v + 1);
   }

   return inv;
}
// [End] Inversion
