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


S4 = 125 k
S5 = 250 k
S6 = 500 k
S8 = 1 M
S9 = 1000k/2000k 
SB = 1000k/4000k 
SC = 1000k/5000k 
SD = 1000k/6000k 
SE = 1000k/8000k 
SF = 1000k/10000k 
SH = 500k/2000k 
SI = 500k/3000k
SJ = 500k/4000k
SK = 500k/5000k 
SL = 500k/6000k 
SM = 500k/8000k 
    

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





