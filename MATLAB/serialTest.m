% serialTest.m
% Author: CJ Tulowiecki
% Description: This *.m file sends a value with float precision to the AVR.
% Since a float is 32-bits in MATLAB, the call to fwrite() on line 16 sends
% the IEEE 754 floating point value 2.5 with 4 transmissions, one for each
% of the 4 bytes. This program also waits for an 8-bit response from the
% AVR and prints it in the Command Window.

format short; format compact;
clear;clc;

PORT = 'COM4';  % Change as necessary
BAUD = 9600;
BITS = 8;        % Number of data bits

AVR = serial(PORT, 'BaudRate', BAUD, 'DataBits', BITS);
fopen(AVR);

pause(0.07);

fwrite(AVR, 3, 'async');

while AVR.BytesAvailable == 0
end

% Store AVR response in variable 'response'
response = fread(AVR, AVR.BytesAvailable, 'uint8');

% Display received data on command line
disp('AVR:');
disp(response);

fclose(AVR);
delete(AVR);
clear AVR;