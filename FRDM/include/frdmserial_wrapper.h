#ifndef _FRDMSERIAL_WRAPPER_H_
#define _FRDMSERIAL_WRAPPER_H_
#include "rtwtypes.h"

void kl25z_USBserial_Init(void);
void kl25z_USBserial_Transmit(void *data, unsigned char sz);
unsigned char kl25z_USBserial_Receive(void *data, unsigned char sz);
void kl25z_USBserial_Terminate(void);

#endif //_FRDMSERIAL_WRAPPER_H_
