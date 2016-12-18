classdef frdmEncoderClass < matlab.System ...
        & coder.ExternalDependency ...
        & matlab.system.mixin.Propagates ...
        & matlab.system.mixin.CustomIcon
    % frdmEncoderClass    Encoder on TFC shield
    %#codegen
    
    % Public, tunable properties
    properties

    end

    % Public, non-tunable properties
    properties(Nontunable)
        % Encoder channel
        channel = 0;
    end

    properties(DiscreteState)

    end

    % Pre-computed constants
    properties(Access = private)

    end

    methods
        % Constructor
        function obj = frdmEncoderClass(varargin)
            % Support name-value pair arguments when constructing object
            coder.allowpcode('plain');
            setProperties(obj,nargin,varargin{:});
        end
    end

    methods(Access = protected)
        %% Common functions
        function setupImpl(obj)
            % Initialize external interrupt
            if coder.target('rtw')
                coder.cinclude('frdmencoder_wrapper.h');
                coder.ceval('kl25z_Encoder_Init', uint8(obj.channel));
            elseif ( coder.target('Sfun') )
                % Place simulation initialization code here
            end            
        end

        function [pps, pulses] = stepImpl(obj)
            % Implement algorithm. Calculate y as a function of input u and
            % discrete states.
            pps = uint8(0);
            pulses = uint32(0);
            if coder.target('rtw')
                coder.cinclude('frdmencoder_wrapper.h');
                coder.ceval('kl25z_Encoder_Read', coder.ref(pps), coder.ref(pulses));
            elseif ( coder.target('Sfun') )
                % Place simulation initialization code here
            end              
        end

        function resetImpl(obj)
            % Initialize / reset discrete-state properties
        end
        
        function releaseImpl(obj)
            % Invoke hareware deconfiguration
            if coder.target('rtw')
                coder.cinclude('frdmserial_wrapper.h');
                coder.ceval('kl25z_Encoder_Terminate');
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

        function [pps_flag,pulses_flag]  = isOutputFixedSizeImpl(obj)
            pps_flag = true;
            pulses_flag = true;
        end
        
        function [pps_size,pulses_size] = getOutputSizeImpl(obj)
            % Return size for each output port
            pps_size = 1;
            pulses_size = 1;
        end

        function [pps_type,pulses_type] = getOutputDataTypeImpl(obj)
            % Return type for each output port
            pps_type = 'uint8';
            pulses_type = 'uint32';
        end
        
        function [pps_flag,pulses_flag] = isOutputComplexImpl(obj)
            % Return complexity for each output port
            pps_flag = false;
            pulses_flag = false;
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
            name = 'FRDM Encoder';
        end
        
        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end
                
        function updateBuildInfo(buildInfo, context)
            if context.isCodeGenTarget('rtw')
                libroot = [mfilename('fullpath'), '/..'];
                buildInfo.addIncludePaths({[libroot, '/include']});
                buildInfo.addSourcePaths({[libroot, '/src']});
                buildInfo.addSourceFiles( {'frdmencoder_wrapper.cpp'});
            end
        end
    end     
end
