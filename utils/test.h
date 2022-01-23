
#include <stdio.h>
#include <initializer_list>

typedef void(*test_func_t)(void);

void run_tests(std::initializer_list<test_func_t> funcs, size_t iters){
  printf("Starting tests.\n");
  for(test_func_t tfunc : funcs){
    for(size_t i = 0; i<iters; ++i){
      tfunc();
    }
  }
  printf("All tests passed.\n");
}
