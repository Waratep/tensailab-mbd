#ifndef _FRDMENCODER_WRAPPER_H_
#define _FRDMENCODER_WRAPPER_H_
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void kl25z_Encoder_Init(uint8_T channel);
void kl25z_Encoder_Read(void *pps, void *pulses);
void kl25z_Encoder_Terminate(void);

#ifdef __cplusplus
}
#endif

#endif //_FRDMENCODER_WRAPPER_H_