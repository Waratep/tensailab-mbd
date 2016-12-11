classdef rpiACMserialRxClass < matlab.System ...
        & coder.ExternalDependency ...        
        & matlab.system.mixin.Propagates ...
        & matlab.system.mixin.CustomIcon
    % rpiACMserialRxClass   Receive data from /dev/ttyACM?
    %#codegen
    
    % Public, tunable properties
    properties

    end

    % Public, non-tunable properties
    properties(Nontunable)
        % ttyACM
        portID = 0;
        % Size of message
        msgLen = 4;
    end

    properties(DiscreteState)

    end

    % Pre-computed constants
    properties(Access = private)

    end

    methods
        % Constructor
        function obj = rpiACMserialRxClass(varargin)
            % Support name-value pair arguments when constructing object
            coder.allowpcode('plain');
            setProperties(obj,nargin,varargin{:});
        end
    end

    methods(Access = protected)
        %% Common functions
        function setupImpl(obj)
            % Invoke tty ACM? serial configuration
            if coder.target('rtw')
                coder.cinclude('rpiserial_wrapper.h');
                coder.ceval('rpi_ACMserial_Init', obj.portID);
            elseif ( coder.target('Sfun') )
                % Place simulation initialization code here
            end
        end

        function [buffer, status] = stepImpl(obj)
            % Execute USB data transmission
            buffer = uint8(zeros(obj.msgLen,1));
            status = uint8(0);
            if coder.target('rtw')
                coder.cinclude('rpiserial_wrapper.h');
                status = coder.ceval('rpi_ACMserial_Receive', coder.ref(buffer), uint8(obj.msgLen));
            elseif ( coder.target('Sfun') )
                % Place simulation execution code here
            end
        end

        function resetImpl(obj)
            % Initialize / reset discrete-state properties
        end
        
        function releaseImpl(obj)
            % Invoke hareware deconfiguration
            if coder.target('rtw')
                coder.cinclude('rpiserial_wrapper.h');
                coder.ceval('rpi_ACMserial_Terminate');
            elseif ( coder.target('Sfun') )
                % Place simulation termination code here
            end
        end

        %% Backup/restore functions
        function s = saveObjectImpl(obj)
            % Set properties in structure s to values in object obj

            % Set public properties and states
            s = saveObjectImpl@matlab.System(obj);

            % Set private and protected properties
            %s.myproperty = obj.myproperty;
        end

        function loadObjectImpl(obj,s,wasLocked)
            % Set properties in object obj to values in structure s

            % Set private and protected properties
            % obj.myproperty = s.myproperty; 

            % Set public properties and states
            loadObjectImpl@matlab.System(obj,s,wasLocked);
        end

        %% Simulink functions
        function z = getDiscreteStateImpl(obj)
            % Return structure of properties with DiscreteState attribute
            z = struct([]);
        end

        function flag = isInputSizeLockedImpl(obj,index)
            % Return true if input size is not allowed to change while
            % system is running
            flag = true;
        end

        function [buffer_flag,status_flag]  = isOutputFixedSizeImpl(obj)
            buffer_flag = true;
            status_flag = true;
        end
        
        function [buffer_size,status_size] = getOutputSizeImpl(obj)
            % Return size for each output port
            buffer_size = obj.msgLen;
            status_size = 1;
        end

        function [buffer_type,status_type] = getOutputDataTypeImpl(obj)
            % Return type for each output port
            buffer_type = 'uint8';
            status_type = 'uint8';
        end
        
        function [buffer_flag,status_flag] = isOutputComplexImpl(obj)
            % Return complexity for each output port
            buffer_flag = false;
            status_flag = false;
        end

        function icon = getIconImpl(obj)
            % Return text as string or cell array of strings for the System
            % block icon
            icon = mfilename('class'); % Use class name
        end
    end

    methods(Static, Access = protected)
        %% Simulink customization functions
        function header = getHeaderImpl
            % Define header panel for System block dialog
            header = matlab.system.display.Header(mfilename('class'));
        end

        function group = getPropertyGroupsImpl
            % Define property section(s) for System block dialog
            group = matlab.system.display.Section(mfilename('class'));
        end
    end
    
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
                buildInfo.addSourceFiles( {'rpiserial_wrapper.cpp'});
            end
        end
    end
end
