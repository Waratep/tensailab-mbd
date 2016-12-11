#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "rpiserial_wrapper.h"

// Shared variables
int fd = 0;
struct termios orig_tty;


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

        cfsetospeed(&tty, (speed_t)B115200);    // Set baud rate
        cfsetispeed(&tty, (speed_t)B115200);
        cfmakeraw(&tty);                        // Raw transmission
        tty.c_cflag |= (CLOCAL | CREAD);        // Enable the receiver and set local mode
        tty.c_cflag &= ~CSTOPB;                 // 1 stop bit
        tty.c_cflag &= ~CRTSCTS;                // Disable hardware flow control
        tty.c_cc[VMIN]  = 0;                    // Non blocking
        tty.c_cc[VTIME] = 0;
        tcsetattr(fd, TCSANOW, &tty);        
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
    uint8_T tmpbuf[128]; // similar size as mbed USBSerial buffer
    uint8_T *data = (uint8_T *)buffer;
    uint8_T len = 0;
    int i, rdsz;
   
    if (fd > 0) {
        rdsz = read(fd, tmpbuf, sz);
        len = (rdsz < sz)?(rdsz):(sz);
        for (i = 0; i < len; i++) {
            data[i] = tmpbuf[i];
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
