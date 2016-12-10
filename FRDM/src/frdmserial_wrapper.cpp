#include "mbed.h"
#include "USBSerial.h"

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
extern "C" void kl25z_USBserial_Transmit(void *data, unsigned char sz)
{
    serial.writeBlock((uint8_t *)data, sz);
}


/*
    Receive byte array 
*/
extern "C" unsigned char kl25z_USBserial_Receive(void *buffer, unsigned char sz)
{
    uint8_t status = 0;
    uint8_t *data = (unsigned char *)buffer;
    
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
