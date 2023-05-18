if [ -e a.out ]; then
   rm a.out
fi

g++ -fdiagnostics-color=always -g -O0 --std=c++17 -Wall -fsanitize=undefined,address StringTest.cpp -lgtest -lgtest_main
./a.out
