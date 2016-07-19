% set_DAC_test.m
% Author: CJ Tulowiecki
% Revision 1.0
% Last Modified: 6/16/16

format short; format compact;
clear;clc;

addr = 2;
ch = 10;
volt = 2.5;

PORT = 'COM6';  % Change as necessary
BAUD = 9600;
BITS = 8;        % Number of data bits

obj = serial(PORT, 'BaudRate', BAUD, 'DataBits', BITS);
fopen(obj);

pause(1);

set_DAC(obj, addr, ch, volt);

pause(1);
   
% while obj.ValuesReceived < 3
%     while obj.BytesAvailable == 0
%     end
%     
%     % Store AVR response in variable 'response'
%     response = fread(obj, obj.BytesAvailable, 'uint8');
% 
%     % Display received data on command line
%     disp('AVR:');
%     disp(dec2bin(response, 8));
% end

fclose(obj);
delete(obj);
clear obj;