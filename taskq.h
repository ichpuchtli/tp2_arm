#ifndef _TASKQ_H_
#define _TASKQ_H_

/* TODO Multiple Queue's Multiple Priorities */
/* Must be a multiple of 2 */
#define TASKQSIZE 32
#define TASKQ_DEFINE_TASK(NAME) void NAME(void* data)

typedef void (*func_t)(void*);

void vTaskQAppend(func_t func, void* data); 

void vTaskQRun(void);

void vTaskQReset(void);

#endif
