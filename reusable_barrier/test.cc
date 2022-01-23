
#include <cstdio>
#include <algorithm>

#include "test.h"
#include "util.h"
#include "impl.h"

static constexpr unsigned NUM_THREADS = 5;
static constexpr unsigned LOOP_ITERS = 10000;

static int times[LOOP_ITERS][2][NUM_THREADS];

static ReusableBarrier* rv = new ReusableBarrier(NUM_THREADS);

static int time(){ 
  static Atomic<int> logical_ctr(0);
  return logical_ctr.fetch_add(1);
}

void test(){
  Thread<int>* thrs[NUM_THREADS];

  for(int i = 0; i<NUM_THREADS; ++i){
    thrs[i] = new Thread<int>([](void* arg) -> void* {
      int index = *((int*) arg);
      for(int j = 0; j<LOOP_ITERS; ++j){
        rv->sync();
        times[j][0][index] = time();
        rv->sync();
        times[j][1][index] = time();
      }
      return nullptr;
    }, i);
    thrs[i]->start();
  }

  for(int i = 0; i<NUM_THREADS; ++i){
    thrs[i]->join();
  }

  for(int i = 0; i<LOOP_ITERS; ++i){
    int min_after = (1 << 30);
    int max_before = 0;
    for(int j = 0; j<NUM_THREADS; ++j){
      max_before = std::max(max_before, times[i][0][j]);
      min_after = std::min(min_after, times[i][1][j]);
    }
    assert(max_before <= min_after);
  }
}

/* not allocating anything too big- so just don't free. */
int main(){
  run_tests({test}, 1);
  return 0;
}
