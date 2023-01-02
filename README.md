# Libraries for Competitve Programming

# 数学系(Math)

## 素数判定

* Prefix: is-prime-func
* Description: 与えられた整数Nが素数かどうかを判定する
* 計算量: $O(\sqrt{N})$

## 素因数分解
* Prefix: prime-fact
* Description: $N$の素因数分解を行う
* 計算量: $O(\sqrt{N})$

## 約数列挙
* Prefix: enum-divisors-func
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

## 組合せ数(DP版)
* Prefix: comb-dp-class
* Description: 組合せ数を求める。N=2000程度まで適用可能
* 計算量: O(N^2)

## 組合せ数(Mod版)
* Prefix: comb-mod-class
* Description: 組合せ数のMODを求める。Modは素数でMod > Nを仮定。N=10^7程度まで適用可能
* 計算量: O(N)

## 指数部の計算
* Prefix: exponent-N-func
* Description: Nの素数pの指数部を求める
* 計算量: O(log N), Nの素因数の指数部の合計がlog Nで抑えられる

## 指数部の計算
* Prefix: exponent-factorial-func
* Description: N!の素数pの指数部を求める(ルジャンドルの定理に基づく)
* 計算量: O(log N)

## 有理数/分数
* Prefix: frac-class
* Description: 有理数を管理する
* 計算量: O(1), 通分をする場合は O(log min(a, b))

# ModIntライブラリ(ModInt.code-snippets)
## MODをコンパイル時に指定
* Prefix: mod-int-static-class
* Description: ModIntのMODをコンパイル時に指定するバージョン
* 計算量: -

## MODを実行時に指定
* Prefix: mod-int-dynamic-class
* Description: ModIntのMODを実行時に指定するバージョン
* 計算量: -

# UnionFindライブラリ

## UnionFindクラス
* Prefix: uf-class
* Description: UnionFindクラス
* 計算量: -

# グラフライブラリ(Graph.code-snippets)

## グラフの読み込み
* Prefix: g-load-directed-edge-inline, g-load-undirected-edge-inline
* Description: 重みなしグラフの読み込み
* 計算量: O(E)

* Prefix: g-load-directed-weight-edge-inline, g-load-undirected-weight-edge-inline
* Description: 重みありグラフの読み込み
* 計算量: O(E)

## 最短路
### 幅優先探索
* Prefix: g-shortest-bfs
* Description: 幅優先探索で単一始点最短路を求める
* 計算量: $O(N + E)$

### ダイクストラ法
* Prefix: g-h-dijkstra, g-i-dijkstra
* Description: ダイクストラ法で単一始点最短路を求める
* Note: 枝の重みがすべて非負の場合に適用可能
* 計算量: $O(E + N log N)$

