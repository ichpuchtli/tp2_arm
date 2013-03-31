#ifndef _TASKQ_H_
#define _TASKQ_H_

/* Must be a multiple of 2 */
#define TASKQSIZE 32

typedef void (*func_t)(void*);

void vTaskQAppend(func_t func, void* data); 

void vTaskQRun(void);

void vTaskQReset(void);

#endif
