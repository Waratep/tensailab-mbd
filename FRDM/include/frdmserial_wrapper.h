#ifndef _FRDMSERIAL_WRAPPER_H_
#define _FRDMSERIAL_WRAPPER_H_
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void kl25z_USBserial_Init(void);
void kl25z_USBserial_Transmit(void *data, uint8_T sz);
uint8_T kl25z_USBserial_Receive(void *data, uint8_T sz);
void kl25z_USBserial_Terminate(void);

#ifdef __cplusplus
}
#endif

#endif //_FRDMSERIAL_WRAPPER_H_
