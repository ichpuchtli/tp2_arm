#include "eventq.h"

#include "inc/hw_types.h" // tBoolean type
#include "driverlib/interrupt.h"

#include "circ_buf.h"

struct event_t {

  Func_t fFunc;
  void*  vpData;
  uint64_t ulTime;
};

struct tqueue_t {

  struct event_t start[EVENTQSIZE];
  volatile unsigned int w_off; /* Write Offset e.g. start + w_off = back of queue */
  volatile unsigned int r_off; /* Read Offset e.g. start + r_off = front of queue */
};

/* Queue Decleration */
/* Static Initialization C99 only */
struct equeue_t eEventQ = {.w_off = 0xFFFFFFFF, .r_off = 0xFFFFFFFF};

/* Internal fFunctions */
/******************************************************************************/
static void vEventQInitStructures(void){

  //TODO Init Timer allocation table
  //TODO Init Event Queue array
  //TODO Init Function Pointer Table
  //TODO Create Wide timer for eventq

}

static void vEventQIntHandler(void){

    struct event_t event;
    struct event_t* epEvent;

    //TODO Clear Wide Timer

    epEvent = epEventQGetFront();

    event.fFunc = epEvent->fFunc;
    event.vpData = epEvent->vpData;
    event.ulTime = epEvent->ulTime;

    vEventQRemoveFront();

    epEvent = vEventQGetFront();

    if(epEvent)
      vEventQStartOneShot(event->fFunc, event->vpData, event->ulTime - vEventQGetTickCount());
    
    (void) (*event.fFunc)(event.vpData);
}
