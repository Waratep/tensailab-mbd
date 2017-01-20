#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "rpiserial_wrapper.h"

// Shared variables
volatile int fd = 0;
struct termios orig_tty;

// Constants
#define IDLE        0
#define BUFFERING   1


/*
    Initialize code
*/
extern "C" void rpi_ACMserial_Init(uint8_T portIdx)
{
    struct termios tty;
    char port_name[20];

    if (fd == 0) {
        // Open serial port
        sprintf(port_name, "/dev/ttyACM%d", portIdx);
        fd = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1) {
            // Error opening port
        }
        tcgetattr(fd, &orig_tty);
        tty = orig_tty;

        cfsetospeed(&tty, (speed_t)B9600);    // Set baud rate
        cfsetispeed(&tty, (speed_t)B9600);
        cfmakeraw(&tty);                        // Raw transmission
        tty.c_cflag |= (CLOCAL | CREAD);        // Enable the receiver and set local mode
        tty.c_cflag &= ~CSTOPB;                 // 1 stop bit
        tty.c_cflag &= ~CRTSCTS;                // Disable hardware flow control
        tty.c_cc[VMIN]  = 0;                    // Non blocking
        tty.c_cc[VTIME] = 0;
        tcsetattr(fd, TCSANOW, &tty);
        tcflush(fd, TCIOFLUSH);       
    } else {
        // already opened file
    }
}


/*
    Transmit byte array
*/
extern "C" void rpi_ACMserial_Transmit(void *data, uint8_T sz)
{
    if (fd > 0) {
        write(fd, (uint8_T *)data, sz);
    } else {
        // Error using unopened port
    }    
}


/*
    Receive byte array 
*/
extern "C" uint8_T rpi_ACMserial_Receive(void *buffer, uint8_T sz)
{
    uint8_T tmp_buf[128]; // similar size as mbed USBSerial buffer
    uint8_T *data = (uint8_T *)buffer;
    uint8_T len = 0;
    int i, rdsz;
   
    if (fd > 0) {
        rdsz = read(fd, tmp_buf, sz);
        len = (rdsz < sz)?(rdsz):(sz);
        for (i = 0; i < len; i++) {
            data[i] = tmp_buf[i];
        }        
    } else {
        // Error using unopened port
    }

    return len;
}


/*
    Transmit byte frame 
*/
extern "C" void rpi_ACMserial_FrameTransmit(void *data, uint8_T sz, uint8_T sof, uint8_T eof)
{
    if (fd > 0) {
        write(fd, &sof, 1);
        write(fd, (uint8_T *)data, sz);
        write(fd, &eof, 1);
    } else {
        // Error using unopened port
    }    
}


/*
    Receive byte frame 
*/
extern "C" uint8_T rpi_ACMserial_FrameReceive(void *buffer, uint8_T sz, uint8_T sof, uint8_T eof)
{
    static uint8_T buf[64]; 
    static int index = 0;
    static int state = IDLE;
    uint8_T tmp_buf[64]; 
    uint8_T *data = (uint8_T *)buffer;
    uint8_T len = 0;
    int rdsz;
    
    if (fd > 0) {
        rdsz = read(fd, tmp_buf, sz+2);
        for (int i = 0; i < rdsz; i++) {            
            uint8_T c = tmp_buf[i];
            switch (state) {
                case IDLE:
                    if (c == sof) {
                        state = BUFFERING;
                    }
                    break;
                case BUFFERING:
                    if (c == sof) {
                        index = 0;
                    }
                    if (c != eof) {
                        buf[index++] = c;
                    } else {
                        for (int j = 0; j < index; j++) {
                            data[j] = buf[j];
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
        }
    } else {
        // Error using unopened port
    }
    
    return len;
}



/*
    Terminate hardware
*/
extern "C" void rpi_ACMserial_Terminate(void)
{
    if (fd > 0) {
        tcsetattr(fd, TCSANOW, &orig_tty);
        close(fd);
        fd = 0;
    }
}
