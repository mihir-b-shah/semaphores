
#include "util.h"
#include "impl.h"
#include "test.h"

#include <cstdio>
#include <algorithm>

static constexpr unsigned LOOP_ITERS = 10000;

static Queue<int>* q = new Queue<int>();
static Atomic<int> value = 0;

void test(){
  auto ping = new Thread<int>([](void* arg) -> void* {
    for(int i = 0; i<LOOP_ITERS; ++i){
      q->send(i);
    } 
    return nullptr;
  }, 0);
  ping->start();

  auto pong = new Thread<int>([](void* arg) -> void* {
    for(int i = 0; i<LOOP_ITERS; ++i){
      value.add_fetch(q->recv());
    } 
    return nullptr;
  }, 0);
  pong->start();
  
  ping->join();
  pong->join();
}

/* not allocating anything too big- so just don't free. */
int main(){
  run_tests({test}, 10);
  return 0;
}
