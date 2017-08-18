#include <pthread.h>
//#include <assert.h>

int val = 0;

void *closer(void *unused) {
  assert(val == 1);
  return NULL;
}

// markus: driver code
int main(int argc, char *argv[]) {
  // initialize
  val = 1;
  pthread_t t1;
  pthread_create(&t1, NULL, closer, NULL);
  return 0;
}
