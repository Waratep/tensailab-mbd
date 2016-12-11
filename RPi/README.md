# Simulink Models for Raspberry Pi Boards
A collection of Simulink models that extend features of Hardware Support Package for Raspberry Pi.

## Requirements
* MATLAB and Simulink 2016a
* [Simulink Support Package for Raspberry Pi Hardware](https://www.mathworks.com/hardware-support/raspberry-pi-simulink.html)

## Preparation procedure
1. Download and install [Raspbian image](https://www.raspberrypi.org/downloads/).
2. Use **apt-get** command to update and upgrade to the latest software version.
3. Target Language Compiler of Raspberry Pi hardware does not support sub-directory. Therefore put every source/header files on the same directory as .slx/.m files. 

### Preparation for USB serial blocks
1. Plug device into USB port.
2. Verify whether device driver is properly detected and initiated with **dmesg** and **ls /dev/ttyACM**. 

## Blocks
1. **ttyACM Serial Receive** and **ttyACM Serial Transmit** blocks for serial communication with USB devices.

## Test
1. Open **RPi.slx** library.
2. Connect Arduino or mbed boards with code to send binary 8-bit values from 0 - 250 stepping by 10 to connect. 
3. Run **demoACMserial.slx** model in External Mode.
4. Expected to see blinking LED with value on Display block on computer.

## References
1. [APT Raspberry Pi Documentation](https://www.raspberrypi.org/documentation/linux/software/apt.md).
2. [Serial Programming/termios](https://en.wikibooks.org/wiki/Serial_Programming/termios).
