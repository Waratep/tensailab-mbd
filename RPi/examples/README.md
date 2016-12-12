# Examples for Raspberry Pi boards

## demoACMserial
1. Open **RPi.slx** library.
2. Connect an Arduino board with **demoACMserial_Arduino** code to send binary 8-bit values from 0 - 250 stepping by 10 and receive reply to ON/OFF the on-board LED. 
3. Run **demoACMserial.slx** model in External Mode.
4. Expected to see blinking LED with value on Display block on computer.

## demoUDPhost and demoUDPtarget
1. Open **demoUDPtarget.slx** and configure different UDP ports for Send/Receive blocks, i.e. 25001/25000.
2. Build and deploy **demoUDPtarget.slx**.
3. Open **demoUDPhost.slx** and configure corresponding UDP ports for Send/Receive blocks, i.e. 25000/25001.
4. Run **demoUDPhost.slx**.
5. Expected to see blinking LED and response with respect to sample time of **UDP Receive** block in **demoUDPtarget** model.
6. Remark: Timing in Simulink model on PC is not accurate **without Simulink real-time toolbox**. 

## demoProxyRPi and demoProxyFRDM
1. Open **demoProxyFRDM.slx**, then build and deploy.
2. Use **PuTTY** program to log into Raspberry Pi, then check serial port with **dmesg** command.
3. Open **demoProxyRPi.slx** and configure different UDP ports for Send/Receive blocks, i.e. 25001/25000.
4. Run **demoProxyRPi** model in External Mode.
5. Open **demoUDPhost.slx** and configure corresponding UDP ports for Send/Receive blocks, i.e. 25000/25001.
6. Run **demoUDPhost.slx**.
7. Expected to see blinking LEDs on both boards synchronized with data display.
8. Remark: Received data display will be delayed from transmited data display by 2-3 steps.
