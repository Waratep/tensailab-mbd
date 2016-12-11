#ifndef _RPISERIAL_WRAPPER_H_
#define _RPISERIAL_WRAPPER_H_
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void rpi_ACMserial_Init(uint8_T portIdx);
void rpi_ACMserial_Transmit(void *data, uint8_T sz);
uint8_T rpi_ACMserial_Receive(void *data, uint8_T sz);
void rpi_ACMserial_Terminate(void);

#ifdef __cplusplus
}
#endif

#endif //_RPISERIAL_WRAPPER_H_