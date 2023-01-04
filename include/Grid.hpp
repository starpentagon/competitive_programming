#pragma once

#include <vector>
#include <iostream>

using namespace std;

void GridLoad() {
   int H, W;
   // [Start] Grid load
   // [Prefix] grid-load-inline
   // H: height, W: width
   vector<vector<char>> grid(H + 1, vector<char>(W + 1));

   for (int i = 1; i <= H; i++) {
      for (int j = 1; j <= W; j++) {
         char c;
         cin >> c;

         grid[i][j] = c;
      }
   }
   // [End] Grid load
}

void GridDirection() {
   // [Start] Grid direction
   // [Prefix] grid-direction-inline
   // Gridの移動方向の定義
   vector<pair<long long, long long>> dir_list = {
       make_pair(-1, 0),  // 上
       make_pair(1, 0),   // 下
       make_pair(0, -1),  // 左
       make_pair(0, 1),   // 右

       make_pair(-1, -1),  // 左上
       make_pair(-1, 1),   // 右上
       make_pair(1, -1),   // 左下
       make_pair(1, 1),    // 右下
   };
   // [End] Grid direction
}

void InGrid() {
   int H, W;

   // [Start] InGrid check
   // [Prefix] grid-in-check-lambda
   // 指定された位置(i, j)がグリッド内かチェックする
   auto InGrid = [&](const int i, const int j) {
      return (1 <= i && i <= H) && (1 <= j && j <= W);
   };
   // [End] InGrid check
}