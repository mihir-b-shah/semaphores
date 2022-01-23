
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "util.h"

template<typename T>
class Queue {
private:
  T hold;
  Semaphore send_sem;
  Semaphore recv_sem;

public:
  Queue() : send_sem(0), recv_sem(0) {}

  T recv(){
    recv_sem.up();
    send_sem.down();
    return hold;
  }

  /*
  I am the one who supplies the value. Therefore, recv must wait for sender
  to place the value in holder.

  A sender should be blocked ONLY if there are no receivers.
  */
  void send(T v){
    recv_sem.down();
    hold = v;
    send_sem.up();
  }
};

#endif
