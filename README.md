# Libraries for Competitve Programming

# 数学系(Math)

## 素数判定

* Prefix: is-prime
* Description: 与えられた整数Nが素数かどうかを判定する
* 計算量: $O(\sqrt{N})$

## 約数列挙
* Prefix: enum-divisors
* Description: 与えられた整数Nの約数を列挙する
* 計算量: $O(\sqrt{N})$

## $x^n$の計算
* Prefix: pow
* Description: 繰り返し二乗法でx^nを求める 
* 計算量: $O(\log n)$

## 最大公約数
* Prefix: gcm-func
* Description: 整数m, nの最大公約数を求める
* 計算量: $O(\log(\max\{m, n\}))$

## 最小公倍数
* Prefix: lcm-func
* Description: 整数m, nの最小公倍数を求める
* 計算量: $O(\log(\max\{m, n\}))$

## 組合せ数(Mod版)
* Prefix: comb-mod
* Description: 組合せ数のMODを求める。Modは素数でMod > Nを仮定。N=10^7程度まで適用可能
* 計算量: O(N)

## 組合せ数(DP版)
* Prefix: comb-dp
* Description: 組合せ数を求める。N=2000程度まで適用可能
* 計算量: O(N^2)

# ModIntライブラリ(ModInt.code-snippets)
## MODをコンパイル時に指定
* Prefix: mod-int-static-class
* Description: ModIntのMODをコンパイル時に指定するバージョン
* 計算量: -

## MODを実行時に指定
* Prefix: mod-int-dynamic-class
* Description: ModIntのMODを実行時に指定するバージョン
* 計算量: -

