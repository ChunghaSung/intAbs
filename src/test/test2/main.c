// Author: Chungha Sung
// This is a simple example to check whether inteference query works or not using datalong rule.
// 
#include <assert.h>
#include <pthread.h>

#define LIMIT 20
//int cnt1, cnt2, cnt3;

int a;
int b;

// f1 priority is 2
void *f1(void *unused)               // f1 -> #x00000000
{
    //while (cnt1 <LIMIT) {
        // a -> #x00000006
        a = 1;                  // #x00000001 
        // assert(a == 1);
        if (a != 1) {
            assert(0);
        }
        //cnt1++;
    //}
    return NULL;
}

// f2 priority is 1
void *f2(void *unused)               // f2 ->  #x00000003
{
    //while (cnt2 < LIMIT) {
        // b -> #x00000007
        b = 0;              // #x00000004
        a = 2;                  // #x00000005
        // assert(b == 0);
        if (b != 0) {
            assert(0);
        }
        //cnt2++;
    //}
    return NULL;
}

// f3 priority is 3
void *f3(void *unused)               // f2 ->  #x00000003
{
    //while(cnt3 < LIMIT) {
        b++;
        // assert(b > 0);
        if (b <= 0) {
            assert(0);
        }
        //cnt3++;
    //}
    return NULL;
}

int main()
{
    pthread_t t1, t2, t3;
    pthread_create(&t1, 0, f1, 0);
    pthread_create(&t2, 0, f2, 0);
    pthread_create(&t3, 0, f3, 0);
    return 0;
}
