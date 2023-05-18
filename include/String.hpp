#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <random>

using namespace std;

// [Start] Rolling Hash
// [Prefix] rolling-hash-class
// [Verified] ABC287「E - Karuta」, \sum |S_i| <= 5 x 10^5(https://atcoder.jp/contests/abc287/tasks/abc287_e?lang=ja)
// 参考
// * 「安全で爆速なRollingHashの話」(https://qiita.com/keymoon/items/11fac5627672a6d6a9f6)
// * 「Rolling Hashについて」(https://maspypy.com/rolling-hash%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6%EF%BC%88survey-%E7%A0%94%E7%A9%B6%EF%BC%89)
class RollingHash {
  public:
   explicit RollingHash(uint64_t base = generate_base())
       : base(base), power{1} {
   }

   // 文字列Sの部分文字列S[0, i), i=1,2,...,|S|のハッシュ値を生成する
   // 計算量: O(|S|)
   vector<uint64_t> build(const string S) {
      vector<uint64_t> hash(S.size() + 1);
      for (int i = 0; i < (int)S.size(); i++) {
         hash.at(i + 1) = add(mul(hash.at(i), base), S.at(i));
      }
      return hash;
   }

   // 文字列SのRolling hash listから部分文字列S[l,r)のハッシュ値を返す
   // 計算量: O(1)
   uint64_t query(const vector<uint64_t> &hash, int l, int r) {
      expand(r - l);
      return add(hash.at(r), mod - mul(hash.at(l), power.at(r - l)));
   }

   // 文字列S(hash: h1),T(hash: h2)を連結した文字列S+Tのハッシュ値を返す
   // 計算量: O(1)
   uint64_t combine(uint64_t h1, uint64_t h2, size_t h2len) {
      expand(h2len);
      return add(mul(h1, power.at(h2len)), h2);
   }

   // 文字列S[l1,r1)と文字列T[l2,r2)のLCP（最長共通接頭辞）の長さを返す
   // 計算量: O(log min{|S|, |T|})
   int lcp(const vector<uint64_t> &hash1, int l1, int r1, const vector<uint64_t> &hash2, int l2, int r2) {
      int len = min(r1 - l1, r2 - l2);
      int ok = 0;
      int ng = len + 1;
      int mid = -1;

      while (ng - ok > 1) {
         mid = (ok + ng) / 2;
         if (query(hash1, l1, l1 + mid) == query(hash2, l2, l2 + mid))
            ok = mid;
         else
            ng = mid;
      }
      return ok;
   }

  protected:
   static constexpr uint64_t mod = (1ull << 61ull) - 1;  // 2^61 - 1: 素数, およそ2.3 x 10^18
   const uint64_t base;
   vector<uint64_t> power;

   // 1以上mod - 1以下のランダムなbaseを生成
   static inline uint64_t generate_base() {
      mt19937_64 engine(1234);
      uniform_int_distribution<uint64_t> rand((uint64_t)1, (uint64_t)mod - 1);
      return rand(engine);
   }

   // 足し算
   static inline uint64_t add(uint64_t a, uint64_t b) {
      if ((a += b) >= mod) a -= mod;
      return a;
   }

   // 掛け算（__uint128_tを使用）
   static inline uint64_t mul(uint64_t a, uint64_t b) {
      __uint128_t c = (__uint128_t)a * b;
      return add(c >> 61, c & mod);
   }

   // base^nのテーブルを計算
   inline void expand(size_t sz) {
      if (power.size() < sz + 1) {
         int pre_sz = (int)power.size();
         power.resize(sz + 1);
         for (int i = pre_sz - 1; i < (int)sz; i++) {
            power.at(i + 1) = mul(power.at(i), base);
         }
      }
   }
};
// [End] Rolling Hash