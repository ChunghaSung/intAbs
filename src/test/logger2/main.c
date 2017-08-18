#include <pthread.h> 
#include <assert.h>
#include <stdlib.h>

//typedef unsigned int int;
//typedef unsigned short int;
//typedef unsigned char int;

#define MAX_RECORDS  64
int numberOfRecords;
//int records[MAX_RECORDS];
int records;

#define MAX_BUFFER  64
int msgBuffer[MAX_BUFFER];

int intervalCounter; 
int tickCounter;     
int interval;     
int startTime;
int decodingStatus;

int cmd;
int sendit;

#define IDLE 0
#define LOGGING 1
int systemState;
int rspStatus;

#define CMD_DEC_OK 1
#define CMD_DEC_ERROR 0
#define RSP_SUCCESS 1
#define RSP_ERROR 0

#define TASK_COMMUNICATE 1
#define TASK_MEASURE 2

#define CMD_START 1
#define CMD_STOP 2
#define CMD_READ_STATE 3
#define CMD_READ_DATA 4

const int __CPROVER_thread_priorities[] = {5, 2, 3};
const char* __CPROVER_threads[] = {"c::task_communicate", "c::task_measure1", "c::task_measure2"};

#define LIMIT 10
int cnt1, cnt2, cnt3;


void stop(void)
{
    //systemState = IDLE;
    systemState = 0;
}

void start(int _startTime, int _interval)
{
    if (numberOfRecords+1 >= 64) {//stop();
        //systemState = IDLE;
        systemState = 0;
    }
    intervalCounter = 0;
    tickCounter = 0;
    numberOfRecords = 0;
    startTime = _startTime;
    interval = _interval;
    systemState = 1;
    //assert(numberOfRecords==0);
    //if(numberOfRecords!=0) irq_err();
}

// Priority is 5
void *task_measure1(void *unused) {
    //while (cnt1 < LIMIT) {
        //cnt1++;
        tickCounter++;
        int tmp_tickCounter = tickCounter;
        int tmp_interval = interval;
        //int value = read_sensor_value();
        int value = 1;
        tickCounter = 0;
        // this assertion doesn't fail in both cases
        // assert(tickCounter == 0);
        if (tickCounter != 0) {
            assert(0);
        }
        intervalCounter++;
        //records[numberOfRecords] =  value;
        records = value;
        numberOfRecords++;
    //}
}

// Priority is 2
void *task_measure2(void *unused) {
    //while (cnt2 < LIMIT) {
        //cnt2++;
        tickCounter = 0;
        // this assertion fails in both cases
        // assert(tickCounter == 0);
        if (tickCounter != 0) {
            assert(0);
        }
        //records[numberOfRecords] =  value;
    //}
}

// Priority is 3
void *task_communicate(void *unused) {
    //while (cnt3 < LIMIT) {
        //cnt3++;
        //int _startTime = get_uint32();
        //int _interval = get_uint16();
        int _startTime = 1;
        int _interval = 1;
        //start(_startTime,_interval);
        cmd = rand() %4 +1;
        rspStatus = rand() %2 + 1;
        decodingStatus = 0;

        if(cmd==2 || cmd==3 || 
                cmd==4 || cmd==1) {
            decodingStatus = 1;
        }

        if (rspStatus == 1) {
            if (cmd == 2) {
                if (systemState != 1) {
                    rspStatus = 0;
                } else { //stop();
                    //systemState = IDLE;
                    systemState = 0;
                }
                //send_response();
                sendit = 1;
            }
            if (cmd == 3) {
                //send_response();
                sendit= 1;
            }
            if (cmd == 4) {
                //send_response();
                sendit= 1;
            }
            if (cmd == 0) {
                if (systemState != 0) {
                    rspStatus = 0;
                } else {
                    int tmp_num = numberOfRecords + 1;
                    if (tmp_num >= 64) { //stop();
                        systemState = 0;
                    }
                    intervalCounter = 0;
                    tickCounter = 0;
                    numberOfRecords = 0;
                    startTime = _startTime;
                    interval = _interval;
                    systemState = 1;
                    // This assertion doesn't fail on priority case
                    if (numberOfRecords != 0) {
                        assert(0);
                    }
                    //assert(numberOfRecords==0);
                    //if(numberOfRecords!=0) irq_err();
                    sendit = 1;
                    //send_response();
                }
            }
        }
    //}
}



int main() {
    systemState = 0;
    pthread_t t1, t2, t3;
    pthread_create(&t1, 0, task_communicate, 0);
    pthread_create(&t2, 0, task_measure1, 0);
    pthread_create(&t3, 0, task_measure2, 0);
    return 0;
}
