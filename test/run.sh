g++ -fdiagnostics-color=always -g -O0 --std=c++17 -Wall -fsanitize=undefined,address MathTest.cpp ModIntTest.cpp UnionFind.cpp GraphTest.cpp -lgtest -lgtest_main
./a.out
