#include <assert.h>

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

#define MAX_RECORDS  64
uint16_t numberOfRecords;
uint16_t records[MAX_RECORDS];

#define MAX_BUFFER  64
uint8_t msgBuffer[MAX_BUFFER];

uint32_t intervalCounter; 
uint16_t tickCounter;     
uint16_t interval;     
uint32_t startTime;

#define IDLE 0
#define LOGGING 1
uint8_t systemState;

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

// Lihao
//const int __CPROVER_thread_priorities[] = {5, 2};
//const char* __CPROVER_threads[] = {"c::task_communicate", "c::task_measure"};
void task_communicate(void);
void task_measure(void);


void stop(void)
{
  systemState = IDLE;
}

inline void start(uint32_t _startTime,uint16_t _interval)
{
  if (numberOfRecords+1 >= MAX_RECORDS) stop();
  intervalCounter = 0;
  tickCounter = 0;
  numberOfRecords = 0;
  startTime = _startTime;
  interval = _interval;
  systemState = LOGGING;
  assert(numberOfRecords==0);
}

extern uint16_t read_sensor_value();
void task_measure(void) 
{
  // Lihao
  __CPROVER_ASYNC_1: 
  task_communicate();

  tickCounter++;

  if (tickCounter == interval) {
    uint16_t value = read_sensor_value();
 
    tickCounter = 0;
    intervalCounter++;
    
    records[numberOfRecords] =  value;
    numberOfRecords++;
  }
}

extern uint8_t read_cmd();
uint8_t get_cmd(uint8_t* decodingStatus) {
  uint8_t cmd = read_cmd();
  *decodingStatus = CMD_DEC_ERROR;
  if(cmd==CMD_STOP || cmd==CMD_READ_STATE || cmd==CMD_READ_DATA ||
     cmd==CMD_START) *decodingStatus = CMD_DEC_OK;
  return cmd;
}

extern uint16_t get_uint16();
extern uint32_t get_uint32();
extern void send_response();
void task_communicate(void)
{
  uint8_t decodingStatus;
  uint8_t cmd = get_cmd(&decodingStatus);
  uint8_t rspStatus = RSP_SUCCESS;

  if (decodingStatus != CMD_DEC_OK) rspStatus = RSP_ERROR;

  if (rspStatus == RSP_SUCCESS) {
    switch(cmd) {
      case CMD_STOP:   
        if (systemState != LOGGING) rspStatus = RSP_ERROR;
        else stop();
        send_response();
        break;
      case CMD_READ_STATE:
        send_response();
	break;
      case CMD_READ_DATA:
        send_response();
	break;
      case CMD_START:
        if (systemState != IDLE) rspStatus = RSP_ERROR;  
        else {
          uint32_t _startTime = get_uint32();
          uint16_t _interval = get_uint16();
	  start(_startTime,_interval);
	}
        send_response();
        break;
      default: break;
    }
  }
}


void initialize(void) {
  systemState = IDLE;
}

void run(void)
{
  // Lihao
  //__CPROVER_ASYNC_1: 
  //task_communicate();

  __CPROVER_ASYNC_1: 
  task_measure();

/*
  while(1) {
    uint8_t currentTask;
    switch(currentTask) {
      case TASK_COMMUNICATE :
        task_communicate();
	break;
      case TASK_MEASURE:
        task_measure();
        break;
      default: break;
    }
  }
*/
}

void main() {
    initialize();
    run();
}
