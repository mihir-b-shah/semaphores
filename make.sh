
g++ -O3 -pthread -std=c++11 -I $(pwd)/utils -o $1/$1 $1/test.cc && $1/$1 && rm $1/$1
