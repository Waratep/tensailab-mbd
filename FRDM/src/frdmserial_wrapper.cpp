#include "mbed.h"
#include "USBSerial.h"
#include "frdmserial_wrapper.h"

// Shared variable declaration
USBSerial serial;

// Private constants
#define MAX_SIZE        64

#define IDLE            0
#define BUFFERING       1

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
    serial.writeBlock((uint8_T *)data, sz);
}


/*
    Receive byte array 
*/
extern "C" uint8_T kl25z_USBserial_Receive(void *buffer, uint8_T sz)
{
    uint8_T status = 0;
    uint8_T *data = (uint8_T *)buffer;
    
    if (sz <= serial.available()) {
        status = 1;
        for (int i = 0; i < sz; i++) {
            data[i] = serial.getc();
        }
    }
   
    return status;
}

/*
    Transmit byte frame
*/
extern "C" void kl25z_USBserial_FrameTransmit(void *buffer, uint8_T sz, uint8_T sof, uint8_T eof)
{
    static uint8_T tmp_buf[MAX_SIZE];
    uint8_T *data = (uint8_T *)buffer;
    
    tmp_buf[0] = sof;
    for (int i = 0; i < sz; i++) {
        tmp_buf[i+1] = data[i];
    }
    tmp_buf[sz+1] = eof;
    serial.writeBlock(tmp_buf, sz+2);
}

/*
    Receive byte frame
*/
extern "C" uint8_T kl25z_USBserial_FrameReceive(void *buffer, uint8_T sz, uint8_T sof, uint8_T eof)
{
    static uint8_T buf[MAX_SIZE];
    static uint8_T index = 0;
    static int state = IDLE;
    uint8_T len = 0;
    uint8_T *data = (uint8_T *)buffer;
    
    while (serial.available()) {
        uint8_T c = serial.getc();
        switch (state) {
            case IDLE:
                if (c == sof) {
                    state = BUFFERING;
                }
                break;
            case BUFFERING:
                if (c == sof) {
                    index = 0; 
                    break;
                }
                if (c != eof) {
                    buf[index++] = c;
                } else {
                    for (int i = 0; i < index; i++) {
                        data[i] = buf[i];                        
                    }
                    state = IDLE;
                    len = index;
                    index = 0;
                }
                if (index > sz) {
                    state = IDLE;
                    index = 0;
                }
                break;
            default:
                state = IDLE;
                index = 0;
        }
        if (len > 0) {
            break;
        }
    }
    
    return len;
}

/*
    Reset hardware
*/
extern "C" void kl25z_USBserial_Terminate(void)
{
    
}
