// Author: Chungha Sung
// This is a simple example to check whether inteference query works or not using datalong rule.
// 

#include <assert.h>
#include <pthread.h>
int a=0;
int t=0;
int cnt1, cnt2;
#define LIMIT 20
void f1();
void f2();


const int __CPROVER_thread_priorities[] = {5, 1};
const char* __CPROVER_threads[] = {"c::f1", 
                                   "c::f2"};

// f1 priority is 5
void f1(void)               // f1 -> #x00000000
{
    //while(cnt1<LIMIT) {
        a = 1;
        // assert(a==1);
        // traditional: violated
        // our method: Not violated
        if (a != 1) {
            assert(0);
        }
        a = 0;
        cnt1++;
    //}
    return NULL;
}

// f2 priority is 1
void f2(void)               // f2 ->  #x00000003
{
    __CPROVER_ASYNC_1:
    f1();
    //while(cnt2<LIMIT) {
        a = 0;
        a = 1;
        // assert(a==1);
        // traditional: violated
        // our method: Not violated
        if (a != 1) {
            //assert(0);
        }
        cnt2++;
    //}
    return NULL;
}


int main()
{
    __CPROVER_ASYNC_1:
    f2();
    /*
    pthread_t t1, t2, t3;
    pthread_create(&t1, 0, f1, 0);
    pthread_create(&t2, 0, f2, 0);
    */
    //pthread_create(&t3, 0, f3, 0);
    return 0;
}
