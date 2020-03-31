# Teensy40_CAN_FD_to_USB_converter
 
For use with Teensy4.0 CAN FD to USB board
http://skpang.co.uk/catalog/teensy-40-can-fd-to-usb-converter-p-1588.html

ASCII Commands

Example: Open the CAN port at the default rate of 500k/2M. 
O[CR]<br>
t001411223344[CR]

d7DF81122334455667788[CR]

d0A2f11223345AFED9879aabbccdd54678765543212de6545dea21234567865 4343237678886545dead451290897876543678334455667788990099887677 ddeeaa32[CR]


Command list

O Open CAN channel

C Close CAN channel


Sx Set baudrate

x: Bitrate id (0-8)

S0 = 10 k
S1 = 20 k
S2 = 50 k
S3 = 100 k
S4 = 125 k
S5 = 250 k
S6 = 500 k
S7 = 800 k
S8 = 1 M
S9 = 500k/1M 
SA = 500k/2M 
SB = 500k/3M 
SC = 500k/4M 
SD = 500k/5M 
SE = 500k/8M 
SF = 500k/10M 
SG = 250k/500k 
SH = 250k/1M 
SI = 250k/2M
SJ = 250k/3M
SK = 250k/4M 
SL = 1000k/4M 
SM = 1000k/8M 
SN = 125k/500k     

tiiildd

Transmit standard (11 bit) frame.

iii: Identifier in hexadecimal format (000-7FF)

l: Data length (0-8)

dd: Data byte value in hexadecimal format (00-FF)


Tiiiiiiiildd..

Transmit extended (29 bit) frame.

iiiiiii:Identifier in hexadecimal format (0000000-1FFFFFFF) l: Data length (0-8)

dd: Data byte value in hexadecimal format (00-FF)


diiildd..

Transmit CANFD standard (11 bit) frame.


iii: Identifier in hexadecimal format (000-7FF)
l: Data length (0-F)

dd: Data byte value in hexadecimal format (00-FF)


Diiiiiiiildd..

Transmit CANFD extended (29 bit) frame.

iiiiiiii:Identifier in hexadecimal format (0000000-1FFFFFFF) l: Data length (0-F)

dd: Data byte value in hexadecimal format (00-FF)



Data length for CAN FD

0 to 8   As in CAN 2.0

9  12 bytes
A  16 bytes
B  20 bytes
C  24 bytes
D  32 bytes
E  48 bytes
F  64 bytes


V Get hardware version

v Get firmware version.





