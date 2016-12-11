# tensailab-mbd 
Collection of MATLAB scripts and Simulink models related to model-based development. 

## Contents
### Arduino directory
A collection of Simulink models that extend features of Simulink Support Package for Arduino Hardware.

### FRDM directory
A collection of Simulink models that extend features of NXP FRDM-KL25Z Microcontroller Support from Simulink Coder. 

### RPi directory
A collection of Simulink models that extend features of Hardware Support Package for Raspberry Pi.

### STM32 directory
A collection of Simulink models that extends features of ST Discovery Board Support from Embedded Coder.

## Device driver development
### Custom library 
1. Create new **Library**.
2. Make model that enclosed operations within **Subsystem** block.
3. Unlock library by clicking at bottom-left corner.
4. Copy **Subsystem** block into library.
5. To make sublibrary, add **Subsystem** block and delete input/output ports.
 
### MATLAB System block
1. Place **MATLAB System** block and create a new Simulink-extension m-file.
2. Add **coder.ExternalDependency** as another template class.
3. Modify class name at the first line and constructor.
4. Modify constructor method.

    ```Matlab
    coder.allowpcode('plain');
    ```

5. Modify setupImpl(), stepImpl() and add releaseImpl() methods with C-code invocation.

    ```Matlab
    if coder.target('rtw')
        coder.cinclude('PUT C HEADER FILE HERE');
        coder.ceval('PUT C FUNCTION NAME HERE');
    elseif ( coder.target('Sfun') )
        % Place simulation termination code here
    end
    ```

6. Add methods for building information.

    ```Matlab
    methods(Static)
        function name = getDescriptiveName()
            name = 'ttyACM Serial Transmit';
        end
        
        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end
                
        function updateBuildInfo(buildInfo, context)
            if context.isCodeGenTarget('rtw')
                libroot = [mfilename('fullpath'), '/..'];
                %buildInfo.addIncludePaths({[libroot, '/include']});
                %buildInfo.addSourcePaths({[libroot, '/src']});
                buildInfo.addSourceFiles( {'PUT C SOURCE FILE HERE'});
            end
        end
    end    
    ```

### Code generation tips for embedded boards
1. Use **coder.ref(data)** to pass data by reference to C code as **void *data**.
2. Input type and complexity are defined by inport's Signal Attributes.
3. Data type declaration is in **rtwtypes.h** header file.
4. **MATLAB System** block's sample time is **inherited** only.
    
## References
1. [Create a Custom Library](https://www.mathworks.com/help/simulink/ug/creating-block-libraries.html).
2. [Structure of Device Driver System Object](https://www.mathworks.com/help/supportpkg/arduino/ug/introduction-to-device-drivers-and-system-objects.html).
3. [Add Libraries to the Library Browser](https://www.mathworks.com/help/simulink/ug/adding-libraries-to-the-library-browser.html).
