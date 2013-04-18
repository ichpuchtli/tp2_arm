#include "taskq.h"

#include "inc/hw_types.h" // tBoolean type
#include "driverlib/interrupt.h"

#include "circ_buf.h"

struct task_t {

  func_t fFunc;
  void*  vpData;
};

struct tqueue_t {

  struct task_t start[TASKQSIZE];
  volatile unsigned int w_off; /* Write Offset e.g. start + w_off = back of queue */
  volatile unsigned int r_off; /* Read Offset e.g. start + r_off = front of queue */
};

/* Queue Decleration */
/* Static Initialization C99 only */
struct tqueue_t tTaskQ = {.w_off = 0xFFFFFFFF, .r_off = 0xFFFFFFFF};

/* Internal fFunctions */
/******************************************************************************/
static void vAddtoBack(struct tqueue_t* queue, func_t fFunc, void* vpData); 
static inline void vRemoveFront(struct tqueue_t* queue);
static inline void vInitQueue(struct tqueue_t* queue);
static inline struct task_t* tpGetFront(struct tqueue_t* queue);

static void vAddtoBack(struct tqueue_t* queue, func_t fFunc, void* vpData){ 

  struct task_t* task = (struct task_t*) 0x0;

  /* Ensure buffer is not full, if not add task to queue */
  if(CIRC_SPACE(queue->w_off, queue->r_off, TASKQSIZE)){

    task = (struct task_t*)(queue->start + queue->w_off);
    
    task->fFunc = fFunc;
    task->vpData = vpData;

    /* Advance write offset */
    queue->w_off = (queue->w_off + 1) & (TASKQSIZE - 1);
  }
}

static inline void vRemoveFront(struct tqueue_t* queue){

    if(CIRC_CNT(queue->w_off, queue->r_off, TASKQSIZE) > 0){

        /* Advance read offset */
        queue->r_off = (queue->r_off + 1) & (TASKQSIZE - 1);

    }
}

static inline struct task_t* tpGetFront(struct tqueue_t* queue){

    if(CIRC_CNT(queue->w_off, queue->r_off, TASKQSIZE) > 0){

        return (struct task_t*)(queue->start + queue->r_off);
    }

    return (struct task_t*) 0x0;
}

static inline void vInitQueue(struct tqueue_t* queue){

    queue->w_off = queue->r_off = 0;
}

/* Public fFunction's */
/******************************************************************************/
void vTaskQReset(void){

    vInitQueue(&tTaskQ);
}

void vTaskQAppend(func_t fFunc, void* vpData){

    if((tTaskQ.w_off & tTaskQ.r_off) == 0xFFFFFFFF) 
        vInitQueue(&tTaskQ);

    vAddtoBack(&tTaskQ, fFunc, vpData);

}

void vTaskQRun(void){

    struct task_t task = {0x0};
    struct task_t* front = tpGetFront(&tTaskQ);

    if(front){

        task.fFunc = front->fFunc;
        task.vpData = front->vpData;
        vRemoveFront(&tTaskQ);

        if(task.fFunc) (*task.fFunc)(task.vpData);
    }

}
