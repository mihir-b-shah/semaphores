
#ifndef _IMPL_H_
#define _IMPL_H_

#include "util.h"

class ReusableBarrier {
private:
  const int lim;
  Atomic<int> sleeping;
  Semaphore sem;
  Semaphore sem_let;

public:
  ReusableBarrier(int lim) : lim(lim), sleeping(0), sem(0), sem_let(1) {}

  void sync(){
    sem_let.down();
    
    if(sleeping.add_fetch(1) != lim){
      sem_let.up();
      sem.down();
    }

    if(sleeping.add_fetch(-1) != 0){
      sem.up();
    } else {
      sem_let.up();
    }
  }
};

#endif
