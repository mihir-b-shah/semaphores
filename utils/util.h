
#ifndef _UTIL_H_
#define _UTIL_H_

#include <pthread.h>
#include <semaphore.h>
#include <cassert>

template<typename T>
class Atomic {
private:
  volatile T val;

public:
  Atomic(T v) : val(v) {}

  T get(void){ return __atomic_load_n(&val, __ATOMIC_SEQ_CST); }
  void set(T v){ __atomic_store_n(&val, v, __ATOMIC_SEQ_CST); }
  T add_fetch(T inc){ return __atomic_add_fetch(&val, inc, __ATOMIC_SEQ_CST); }
  T fetch_add(T inc){ return __atomic_fetch_add(&val, inc, __ATOMIC_SEQ_CST); }
  T exchange(T v){ T ret; __atomic_exchange(&val, &v, &ret, __ATOMIC_SEQ_CST); return ret; }
};

class Semaphore {
private:
  sem_t sem;

public:
  Semaphore(int ct){ assert(!sem_init(&sem, 0, ct)); }
  void up(){ assert(!sem_post(&sem)); }
  void down(){ assert(!sem_wait(&sem)); }
};

template<typename Arg>
class Thread {
private:
  pthread_t thr;
  void* (*work)(void*);
  Arg arg;
  void* ret;

public:
  // enforces no-capture and signature rule.
  Thread(void* (*work)(void*), Arg arg) : work(work), arg(arg) {}
  void start(){ assert(!pthread_create(&thr, nullptr, work, (void*) &arg)); }
  void* join(){ assert(!pthread_join(thr, &ret)); return ret; }
};

#endif
