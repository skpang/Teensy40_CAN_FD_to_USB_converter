/*

  www.skpang.co.uk

  MIT License
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE. 

 */
 
#include <FlexCAN_T4.h>
#include "can_fd.h"
extern FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> FD;
void set_baudrate(CAN_BITTIME_SETUP baudrate)
{
 

  CANFD_timings_t config;
  config.clock = CLK_40MHz;
  config.baudrate =   1000000;
  config.baudrateFD = 2000000;
  
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 75;

  switch(baudrate){
      case CAN_125K_500K:             // S4
        config.baudrate =   125000;
        config.baudrateFD = 500000;
        break;
        
        case CAN_250K_500K:
        config.baudrate =   250000;   // S5
        config.baudrateFD = 500000;
        break;
      
      case CAN_500K_1M:
        config.baudrate =   500000;   // S6
        config.baudrateFD = 1000000;
        break;

      case CAN_500K_2M:
        config.baudrate =   500000;   // SH
        config.baudrateFD = 2000000;
        break;

      case CAN_500K_3M:
        config.baudrate =   500000;   // SI
        config.baudrateFD = 3000000;
        break;

      case CAN_500K_4M:
        config.baudrate =   500000;   // SJ
        config.baudrateFD = 4000000;
        break;

      case CAN_500K_5M:
        config.baudrate =   500000;   // SK 
        config.baudrateFD = 5000000;
        break;

      case CAN_500K_6M:
        config.baudrate =   500000;  // SL
        config.baudrateFD = 6000000;
        break;  

      case CAN_500K_8M:
        config.baudrate =   500000;  // SM
        config.baudrateFD = 8000000;
        break; 
                              

      case CAN_1000K_1M:
        config.baudrate =   1000000;  // S8
        config.baudrateFD = 1000000;
        break;  

      case CAN_1000K_2M:
        config.baudrate =   1000000;  // S9
        config.baudrateFD = 2000000;
        break;
      
      case CAN_1000K_4M:
        config.baudrate =   1000000;  // SB
        config.baudrateFD = 4000000;
        break; 

      case CAN_1000K_5M:
        config.baudrate =   1000000;  // SC
        config.baudrateFD = 5000000;
        break; 

      case CAN_1000K_6M:
        config.baudrate =   1000000;  // SD
        config.baudrateFD = 6000000;
        break;         

      case CAN_1000K_8M:
        config.baudrate =   1000000;  // SE
        config.baudrateFD = 8000000;
        break;          

      case CAN_1000K_10M:
        config.baudrate =   1000000;  // SF
        config.baudrateFD = 10000000;
        break; 
  }
  
  
  FD.setRegions(64);
  FD.setBaudRate(config);

  FD.setMBFilter(ACCEPT_ALL);
  FD.setMBFilter(MB13, 0x1);
  FD.setMBFilter(MB12, 0x1, 0x3);
  //  //  FD.setMBFilter(MB3, ACCEPT_ALL);
  //  //  FD.setMBFilter(MB0, 0x8);
  //  FD.setMBFilter(MB8, 0x1, 0x04);
  FD.setMBFilterRange(MB8, 0x1, 0x04);
  //  FD.enableMBInterrupt(MB0);
  FD.enableMBInterrupt(MB8);
  FD.enableMBInterrupt(MB12);
  FD.enableMBInterrupt(MB13);
  //  FD.enableMBInterrupt(MB9);
    FD.enhanceFilter(MB8);
  //  FD.enhanceFilter(MB9);
  //  FD.enhanceFilter(MB12);
  FD.enhanceFilter(MB10);
  //  //  FD.enhanceFilter(MB13);
  FD.distribute();
  FD.mailboxStatus();
  
} 
