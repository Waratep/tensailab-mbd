#include "mbed.h"
#include "USBSerial.h"
#include "frdmserial_wrapper.h"

// Shared variable declaration
USBSerial serial;


/*
    Initialize code
*/
extern "C" void kl25z_USBserial_Init(void)
{
}


/*
    Transmit byte array
*/
extern "C" void kl25z_USBserial_Transmit(void *data, uint8_T sz)
{
    serial.writeBlock((uint8_t *)data, sz);
}


/*
    Receive byte array 
*/
extern "C" uint8_T kl25z_USBserial_Receive(void *buffer, uint8_T sz)
{
    uint8_t status = 0;
    uint8_t *data = (uint8_t *)buffer;
    
    if (sz <= serial.available()) {
        status = 1;
        for (int i = 0; i < sz; i++) {
            data[i] = serial.getc();
        }
    }
   
    return status;
}


/*
    Reset hardware
*/
extern "C" void kl25z_USBserial_Terminate(void)
{
    
}
