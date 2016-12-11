# Simulink Models for FRDM-KL25Z Boards
A collection of Simulink models that extend features of NXP FRDM-KL25Z Microcontroller Support from Simulink Coder. 

## Requirements
* MATLAB and Simulink 2016a
* [Simulink Coder Support Package for NXP FRDM-KL25Z Board](https://www.mathworks.com/hardware-support/frdm-kl25z.html)

## Preparation for USB Serial blocks
1. Apply for [mbed](https://developer.mbed.org/) account.
2. Select FRDM-KL25Z as development platform.
3. Import "USB Serial" example from [mbed Handbook](https://developer.mbed.org/handbook/USBSerial).
4. Export program for **Keil uVision5** compiler.
5. Copy source files under /USBDevice directory tree into /src directory.
   * USBDevice/USBCDC.cpp
   * USBDevice/USBDevice.cpp
   * USBDevice/USBHAL_KL25Z.cpp
   * USBSerial/USBSerial.cpp 
6. Copy header files under /USBDevice directory tree into /include directory.
   * USBDevice/USBCDC.h
   * USBDevice/USBDescriptor.h
   * USBDevice/USBDevice.h
   * USBDevice/USBDevice_Types.h
   * USBDevice/USBEndpoints.h 
   * USBDevice/USBEndpoints_KL25Z.h 
   * USBDevice/USBHAL.h 
   * USBSerial/CircBuffer.h
   * USBSerial/USBSerial.h
6. Use **MATLAB System** block. 
7. Add C source and header files for hardware configuration and operation.
8. Buffer size can be adjusted from USBSerial.h.
```Matlab
CircBuffer<uint8_t,128> buf;
```

## Blocks
1. **USB Serial Transmit** and **USB Serial Receive** blocks for USB serial communication.

## Test
1. Open **frdmKL25Z.slx** library.
2. Open **demoUSBSerial.slx** model.
3. Connect USB serial port of FRDM-KL25Z board with computer.
4. Use terminal software to send 4 bytes. 
5. Expected to see 4 echoed bytes with blinking color depends on the number of transmission.

## References 
1. [Structure of Device Driver System Object](https://www.mathworks.com/help/supportpkg/arduino/ug/introduction-to-device-drivers-and-system-objects.html)
2. [USBSerial](https://developer.mbed.org/handbook/USBSerial).
3. [mbed Stream API](https://developer.mbed.org/users/mbed_official/code/mbed/file/487b796308b0/Stream.h).
