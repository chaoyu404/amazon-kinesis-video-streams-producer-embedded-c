
#include "pthread.h"

int pthread_mutex_init(pthread_mutex_t* __restrict mutex,
                       const void* __restrict attr) {
  return 0;
}

int pthread_mutex_destroy(pthread_mutex_t* mutex) { return 0; }

int pthread_mutex_lock(pthread_mutex_t* mutex) { return 0; }

int pthread_mutex_unlock(pthread_mutex_t* mutex) { return 0; }