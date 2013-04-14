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
static void vEventQInitTimerPeripheral(void){

    //One Shot Timer/Interrupt Configuration

    // Configure Timer Peripherials
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);

    // Disable Timer!
    TimerDisable(TIMER1_BASE, TIMER_A);

    // Configure Interrupt
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

}

static void vEventQStartOneShot(uint64_t ulMicroSeconds){

    // Calculate timer period from ulMicroSeconds
    uint32_t ulPeriod = ulMicroSeconds / SysCtlClockGet();

    TimerLoadSet(TIMER1_BASE, TIMER_A, ulPeriod);

    // Reset function?
}

static void vEventQIntHandler(void){

    struct event_t event;
    struct event_t* epEvent;

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    epEvent = epEventQGetFront();

    event.fFunc = epEvent->fFunc;
    event.vpData = epEvent->vpData;
    event.ulTime = epEvent->ulTime;

    vEventQRemoveFront();

    if(bEventQPendingEvents())
        vEventQInitTimer(ulEventQNextEvent());
    
    (void) (*event.fFunc)(event.vpData);
}
