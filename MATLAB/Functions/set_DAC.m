function set_DAC( obj, addr, ch, volt )
%SET_DAC Sets a voltage on the selected channel of the addressed DAC.
%   SET_DAC(OBJ, ADDR, CH, VOLT) writes asynchronously to the serial object
%   OBJ. OBJ must already exist and be open for writing. Make sure to
%   properly close OBJ when finished reading/writing.
%
%   The data precision used for ADDR_CH is 'int8'. ADDR is written first, 
%   followed by CH, and VOLT. The data precision used for VOLT is 'float'. 
%   Since the function uses asynchronous writing, it contains code to wait
%   until the write is finished after each write.
    
    % Constants
    VREF = 5;        % Volts
    RES = 14;        % DAC Resolution Bits
    
    if (addr > 3) || (addr < 0)
        fclose(obj);
        delete(obj);
        clear obj;
        error('Expected ADDR between 0 and 3.')
    elseif (ch > 39) || (ch < 0)
        fclose(obj);
        delete(obj);
        clear obj;
        error('Expected CH between 0 and 39.')
    elseif (volt > 5) || (volt < 0)
        fclose(obj);
        delete(obj);
        clear obj;
        error('Expected VOLT between 0 and 5.')
    else
        % Encode CH, and VOLT
        ch_b = dec2bin(ch, 6);   
        step_size = VREF / (2^RES - 1);
        volt_b = dec2bin( round( volt / step_size ), 14);

        % Generate 24-bit AD5380 Instruction
        % Format (binary): 00 A5:A0 11 DB13:DB0
        DAC_instr = bin2dec( ['00', ch_b, '11', volt_b] );

        % Write ADDR
        fwrite(obj, addr, 'async'); 

        % Wait until write is finished
        status = get(obj, 'TransferStatus');
        idle = strcmp(status, 'idle');
        while idle == 0        
            status = get(obj, 'TransferStatus');
            idle = strcmp(status, 'idle');
        end

        % Write DAC_instr
        fwrite(obj, DAC_instr, 'int32', 'async');

        % Wait until write is finished
        status = get(obj, 'TransferStatus');
        idle = strcmp(status, 'idle');
        while idle == 0        
            status = get(obj, 'TransferStatus');
            idle = strcmp(status, 'idle');
        end
    end
end

