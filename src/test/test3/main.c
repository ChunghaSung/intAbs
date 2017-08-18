// Author: Chungha Sung
// This is a simple example to check whether inteference query works or not using datalong rule.
// 
#include <assert.h>
#include <pthread.h>

int a=0;
int b=0;
//int cnt1, cnt2, cnt3, cnt4;
#define LIMIT 20

// f1 priority is 2
void *f1(void *unused)               // f1 -> #x00000000
{
    //while(cnt1<LIMIT) {
        a = 1;
        b = 1;
        // assert(a == 1);         // #x00000002
        // traditional: violated
        // our method: violated
        if (a != 1) {
            assert(0);
        }
        b = 0;
        //cnt1++;
    //}
    return NULL;
}

// f2 priority is 1
void *f2(void *unused)               // f2 ->  #x00000003
{
    //while(cnt2<LIMIT) {
        a = 0;
        // assert(a == 0);
        // traditional: violated
        // our method: violated
        if (a != 0) {
            assert(0);
        }
        //cnt2++;
    //}
    return NULL;
}

// f3 priority is 1
void *f3(void *unused)               // f2 ->  #x00000003
{
    //while(cnt3<LIMIT) {
        b = 0;
        // assert(b == 0);
        // traditional: violated
        // our method: Not violated
        if (b != 0) {
            assert(0);
        }
        //cnt3++;
    //}
    return NULL;
}

// f4 priority is 3
void *f4(void *unused)               // f2 ->  #x00000003
{
    //while(cnt4<LIMIT) {
        a = 0;
        // assert(a == 0);
        // traditional: violated
        // our method: Not violated
        if (a != 0) {
            assert(0);
        }
        //cnt4++;
    //}
    return NULL;
}

int main()
{
    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, 0, f1, 0);
    pthread_create(&t2, 0, f2, 0);
    pthread_create(&t3, 0, f3, 0);
    pthread_create(&t4, 0, f4, 0);
    return 0;
}
