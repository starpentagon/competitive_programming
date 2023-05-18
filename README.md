# Libraries for Competitve Programming

# 数学系(Math)

## 素数列挙, 素因数分解, 区間篩
* Prefix: prime-table-class
* Description: エラトステネスの篩, 区間篩でN以下の素数列挙, 素因数分解を行う
* 計算量: $O(N \log(\log N))$

## 素数判定

* Prefix: is-prime-func
* Description: 与えられた整数Nが素数かどうかを判定する
* 計算量: $O(\sqrt{N})$

## 素因数分解
* Prefix: prime-factorization-func
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

## $\sqrt{n}$のfloor/ceil計算
* Prefix: sqrt-int-func
* Description: 平方根のfloor, ceilを求める。nの平方数判定も可能。
* 計算量: $O(\log \log n)$ 程度

## 組合せ数(DP版)
* Prefix: comb-dp-class
* Description: 組合せ数を求める。N=2000程度まで適用可能
* 計算量: O(N^2)

## 組合せ数(Mod版)
* Prefix: comb-mod-class
* Description: 組合せ数のMODを求める。Modは素数でMod > Nを仮定。N=10^7程度まで適用可能
* 計算量: O(N)

## 指数部の計算(N)
* Prefix: exponent-N-func
* Description: Nの素数pの指数部を求める
* 計算量: O(log N), Nの素因数の指数部の合計がlog Nで抑えられる

## 指数部の計算(N!)
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
* Prefix: g-shortest-dijkstra
* Description: ダイクストラ法で単一始点最短路を求める
* Note: 枝の重みがすべて非負の場合に適用可能
* 計算量: $O(E + N log N)$

### Bellman-Ford法
* Prefix: g-shortest-bellman
* Description: Bellman-Ford法で単一始点最短路を求める
* Note: 枝の重みが負の場合も適用可能
* 計算量: O(N * E)

### Warshall Floyd法
* Prefix: g-shortest-warshall
* Description: Warshall Floyd法で全点最短路を求める
* Note: 枝の重みが負の場合も適用可能
* 計算量: O(N^3)

### 最短経路の抽出
* Prefix: g-find-shortest-path-func
* Description: 最短距離からノード間の最短経路を算出する
* 計算量: O(E)

## トポロジカルソート
* Prefix: g-topological-sort-func
* Description: トポロジカルソートを求める。DAGの判定も行える。
* 計算量: O(N + E)

## 閉路検出(無向グラフ)
* Prefix: g-loop-undirected-detection
* Description: 連結成分ごとに閉路を検出し、存在すればその隣接関係を返す
* 計算量: O(N + E)

## 閉路検出(有向グラフ)
* Prefix: g-loop-directed-detection
* Description: 連結成分ごとに閉路を検出し、存在すればその隣接関係を返す
* 計算量: O(N + E)

## 最長路
* Prefix: g-longest-path-func
* Description: DAGにおける最長路を求める
* 計算量: O(N + E)

## 閉路上での移動
* Prefix: g-move-on-loop-func
* Description: 指定回数移動後にどのノードにいるかを返す。
* 計算量: 事前計算: O(N), startからMoveCount回の移動計算: O(1)

## Euler Tour
* Prefix: g-euler-tour-class
* Description: 木に対するEuler Tourを求める(辺, 頂点の重み変更可能)
* 依存ライブラリ: セグメント木(seg-tree-class), SparseTable(sp-table-class)

### 前処理
* 計算量: O(N log N)
* LCAが不要の場合はSparseTableの構築をskipすることでO(N)に高速化可能

### 部分木に対する操作
* サイズの取得: (O(1))  
* ノードが部分木に含まれるかの判定: (O(1))
* 部分木の辺, 頂点の重みの合計: (O(log N)), 重みの更新がなければセグメント木を累積和に変えることでO(1)に高速化

### rootからのパスに対する操作
* 深さの取得: O(1)
* パス上の辺, 頂点の重みの合計: (O(log N)), 重みの更新がなければセグメント木を累積和に変えることでO(1)に高速化

### LCA
* LCAの取得: O(1)

### 2頂点間のパスに対する操作
* パスの長さ: O(1)
* パス上の辺, 頂点の重みの合計: (O(log N)), 重みの更新がなければセグメント木を累積和に変えることでO(1)に高速化

## 強連結成分分解(Strongly Connected Components, SCC)
* Prefix: g-scc-class
* Description: 強連結成分を求める。強連結成分はトポロジカルソート済

# Grid操作

## Gridのロード
* Prefix: grid-load-inline
* Description: グリッドを読み込む
* 計算量: O(HW)

## Gridの方向定義
* Prefix: grid-direction-inline
* Description: 方向定義を読み込む
* 計算量: O(1)

## グリッド内判定
* Prefix: grid-in-check-lambda
* Description: グリッド内判定
* 計算量: O(1)

# 行列ライブラリ(Matrix.code-snippets)
## 行列クラス
* Prefix: mat-class
* Description: 行列を扱うクラス
* 計算量: -

# Sparse Table
## Sparse Tableクラス
* Prefix: sp-table-class
* Description: SparseTableを構築する
* 計算量/空間量: 前計算 O(N log N), クエリ: O(1)

## min/max演算子
* Prefix: sp-table-min-oper-inline, sp-table-max-oper-inline
* Description: 最小, 最大演算子の定義
* 計算量: -

# RangeQuery
## SegmentTree
* Prefix: seg-tree-class
* Description: セグメント木の構築、一点更新、最大／最小／XOR等のRange query
* 計算量: 構築 O(N), 一点更新 O(log N), Range Query O(log N)

## SegmentTree用の演算子
* Prefix: seg-oper-min/max/sum/xor-inline
* Description: セグメント木上の演算子定義
* 計算量: -

## LazySegmentTree
* Prefix: laz-seg-tree-class
* Description: 遅延評価セグメント木の構築、区間更新、最大／最小／XOR等のRange query
* 計算量: 構築 O(N), 区間更新 O(log N), Range Query O(log N)

## SegmentTree用の演算子
* Prefix: laz-seg-oper-min/max/sum/xor-inline
* Description: 遅延評価セグメント木上の演算子定義
* 計算量: -

## LazySegmentTree用の更新演算子
* Prefix: laz-seg-range-add/update-inline
* Description: 遅延評価セグメント木上の区間加算/区間更新
* 計算量: -

# 累積和
* Prefix: cum-sum-class
* Description: 累積和を管理するクラス
* 計算量: 前処理O(N), 区間和 O(1)

# 累積和(2次元配列)
* Prefix: cum-sum-2d-class
* Description: 二次元配列上の累積和/矩形和を管理するクラス
* 計算量: 前処理O(HW), 縦横斜めの区間和 O(1)

# いもす法(1次元)
* Prefix: imos-1d-class
* Description: 1次元のいもす法
* 計算量: O(N)

# いもす法(2次元)
* Prefix: imos-2d-class
* Description: 2次元のいもす法
* 計算量: O(HW)

# 探索ライブラリ(Search.code-snippets)

## 二分探索(整数)
* Prefix: bin-search-int
* Description: 条件を満たす境界(最大/最小)を二分探索で求める。
* 計算量: O(log(|kTrivialOK - kTrivialNG|))

## 二分探索(浮動小数点)
* Prefix: bin-search-dbl大阪府堺市北区長曽根町３０４３番地４
* Description: 条件を満たす境界(最大/最小)を二分探索で求める。
* 計算量: O(log(|kTrivialOK - kTrivialNG|))

## しゃくとり法
* Prefix: shakutori
* Description: しゃくとり法のベースコード
* 計算量: O(N)

## 指定した値以上で最小の要素の探索
* Prefix: find-min-geq-vec, find-min-geq-set
* Description: val以上で最小の要素を返す
* 計算量: O(log N)

## 指定した値より大きい最小の要素の探索
* Prefix: find-min-greater-vec, find-min-greater-set
* Description: valより大きい最小の要素を返す
* 計算量: O(log N)

## 指定した値以下で最大の要素の探索
* Prefix: find-max-leq-vec, find-max-leq-set
* Description: val以下で最大の要素を返す
* 計算量: O(log N)

## 指定した値より小さい最大の要素の探索
* Prefix: find-max-less-vec, find-max-less-set
* Description: valより小さい最大の要素を返す
* 計算量: O(log N)

# 転倒数
* Prefix: inversion
* Description: 転倒数を計算する
* 計算量: O(N log N)
* 依存ライブラリ: 座標圧縮(pos-comp), セグメント木(seg-tree)

# その他ライブラリ(Misc.code-snippets)
## 座標圧縮
* Prefix: pos-comp-class
* Description: 座標圧縮を行う
* 計算量: O(N log N)

## ランレングス圧縮
* Prefix: run-len-func
* Description: ランレングス圧縮を行う
* 計算量: O(N)

## std::pairのHash関数
* Prefix: hash-pair-struct
* Description: std::pair型のハッシュ関数
* 計算量: -

## 小数点出力の設定
* Prefix: cout-float-inline
* Description: 小数点以下15桁まで出力
* 計算量: -

## 出力
* Prefix: p-(ans/yes/no/neg)-inline
* Description: 回答出力
* 計算量: -

# 動的計画法

## 動的計画法のフレームワーク
* Prefix: dp-framework-inline
* Description: 動的計画法の実装FW
* 計算量: -

## 最長増加部分列
* Prefix: dp-lis-func
* Description: 最長増加部分列を求める
* 計算量: O(N log N)

# インタラクティブ問題
* Prefix: interactive-class
* Description: インタラクティブ問題用フレームワーク
* 計算量: -

# マクロ/テンプレート
* Prefix: macro-templates
* Description: rep, chmin/max, vector用cin/outなど
* 計算量: -

# 文字列

## Rolling Hash
* Prefix: rolling-hash-class
* Description: 文字列のRolling Hash生成(部分文字列の一致判定、最長共通部分列(LCP)の長さなど)
* 計算量: -
