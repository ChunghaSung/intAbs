// Author: Chungha Sung
// This is a simple example to check whether inteference query works or not using datalong rule.
// 

#include <assert.h>
#include <pthread.h>
int a=0, a_temp;
int r1, r2, r3;
int cnt1, cnt2;
#define LIMIT 10

// f1 priority is 5
void *f1(void *unused)               // f1 -> #x00000000
{
    //while(cnt1<LIMIT) {
        a++;
        cnt1++;
        a =a ;
    //}
    //a_temp = a;
    return NULL;
}

// f2 priority is 1
void *f2(void *unused)               // f2 ->  #x00000003
{
    //while(cnt2<LIMIT) {
        // assert(a==1);
        // traditional: violated
        // our method: violated
        if (a != 2 || a != 0) {
            assert(0);
        }
        cnt2++;
    //}
    return NULL;
}


int main()
{
    pthread_t t1, t2, t3;
    pthread_create(&t1, 0, f1, 0);
    pthread_create(&t2, 0, f2, 0);
    //pthread_create(&t3, 0, f3, 0);
    return 0;
}
