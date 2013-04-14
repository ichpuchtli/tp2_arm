#ifndef USB_H_
#define USB_H_

#include "inc/hw_types.h"
#include "usblib/usblib.h"
void USB_Create(void);

extern void * USBDMSCStorageOpen(unsigned long ulDrive);
extern void USBDMSCStorageClose(void * pvDrive);
extern unsigned long USBDMSCStorageRead(void * pvDrive, unsigned char *pucData,
                                        unsigned long ulSector,
                                        unsigned long ulNumBlocks);
extern unsigned long USBDMSCStorageWrite(void * pvDrive, unsigned char *pucData,
                                         unsigned long ulSector,
                                         unsigned long ulNumBlocks);
unsigned long USBDMSCStorageNumBlocks(void * pvDrive);

//unsigned long massStorageEventCallback(void* callback, unsigned long event, unsigned long messageParameters, void* messageData);

unsigned long
USBDMSCEventCallback(void *pvCBData, unsigned long ulEvent,
                     unsigned long ulMsgParam, void *pvMsgData);

#endif /* USB_H_ */
