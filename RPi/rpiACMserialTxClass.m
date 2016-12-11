classdef rpiACMserialTxClass < matlab.System ...
        & coder.ExternalDependency ...
        & matlab.system.mixin.Propagates ...
        & matlab.system.mixin.CustomIcon
    % rpiACMserialTxClass  Transmit data via tty ACM? serial
    %#codegen

    % Public, tunable properties
    properties

    end

    % Public, non-tunable properties
    properties(Nontunable)
        % ttyACM
        portID = 0;
    end

    properties(DiscreteState)

    end

    % Pre-computed constants
    properties(Access = private)

    end

    methods
        % Constructor
        function obj = rpiACMserialTxClass(varargin)
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

        function stepImpl(obj, data)
            % Execute USB data transmission
            if coder.target('rtw')
                sz = length(data);
                coder.cinclude('rpiserial_wrapper.h');
                coder.ceval('rpi_ACMserial_Transmit', coder.ref(data), sz);
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
        
        function num = getNumInputsImpl(~)
            % Single input
            num = 1;
        end

        function num = getNumOutputsImpl(~)
            % No output
            num = 0;
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
