# Examples for FRDM-KL25Z Boards

## demoUSBserial
1. Open **frdmKL25Z.slx** library.
2. Open **demoUSBserial.slx** model.
3. Connect USB serial port of FRDM-KL25Z board with computer.
4. Use terminal software to send 4 bytes. Or use Python code to read/write serial port.

    ```Python
    import serial
    import time
    ser = serial.Serial('/dev/ttyACM0') # Serial port depends on OS
    for i in range(1,10):
        ser.write('idx' + str(i))
        print(ser.read(4))
        time.sleep(2)
    ser.close()
    ```

5. Expected to see 4 echo bytes with blinking color depends on the number of transmission.
