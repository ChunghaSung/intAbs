// Author: Chungha Sung
// This is a simple example to check whether inteference query works or not using datalong rule.
// 
#include <assert.h>
#include <pthread.h>
int a;
int b;
void f1();
void f2();
int cnt1, cnt2;
#define LIMIT 20

const int __CPROVER_thread_priorities[] = {2, 1};
const char* __CPROVER_threads[] = {"c::f1", "c::f2"};

// f1 priority is 2
void f1(void)               // f1 -> #x00000000
{
    // a -> #x00000006
    //while(cnt1<LIMIT) {
        a = 1;                  // #x00000001 
        if (a != 1) {
            assert(0);
        }
        cnt1++;
    //}
    return NULL;
}

// f2 priority is 1
void f2(void)               // f2 ->  #x00000003
{
    __CPROVER_ASYNC_1:
    f1();
    // b -> #x00000007
    //while(cnt2<LIMIT) {
        b = 0;              // #x00000004
        a = 2;                  // #x00000005
        if (b != 0) {
            assert(0);
        }
        cnt2++;
    //}
    return NULL;
}

int main()
{
    cnt1= 0;
    cnt2 = 0;
    __CPROVER_ASYNC_1:
    f1();
    /*
    pthread_t t1, t2;
    pthread_create(&t1, 0, f1, 0);
    pthread_create(&t2, 0, f2, 0);
    */
    return 0;
}
