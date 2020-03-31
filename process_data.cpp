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
#include "process_data.h"
#include "can_fd.h"

extern FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> FD;

unsigned char timestamping = 0;
extern uint8_t prompt;
extern volatile uint8_t state;
extern CANFD_message_t msg;

extern int LED_R;
extern int LED_G;
extern int LED_B;

uint8_t dlc;
static const uint8_t skp_DLCtoBytes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

static uint8_t len_to_dlc(uint8_t len)
{
  uint8_t dlc;
  
  if(len <=8)
  {
    dlc = len;
  }else if (len <=12)
  { 
    dlc = 0x09; 
  }else if (len <=16)
  {
    dlc = 0x0A;  
  }else if (len <=20)
  {
    dlc = 0x0B;
  }else if (len <=24)
  {
    dlc = 0x0C;
  }else if (len <=32)
  {
    dlc = 0x0D;
  }else if (len <=48)
  {
    dlc = 0x0E;  
  }else if (len <=64)
  {
    dlc = 0x0F;  
  }
  
  return dlc;
  
}
uint8_t parseHex(char * line, uint8_t len, unsigned long * value) 
{
    *value = 0;
    while (len--) {
        if (*line == 0) return 0;
        *value <<= 4;
        if ((*line >= '0') && (*line <= '9')) {
           *value += *line - '0';
        } else if ((*line >= 'A') && (*line <= 'F')) {
           *value += *line - 'A' + 10;
        } else if ((*line >= 'a') && (*line <= 'f')) {
           *value += *line - 'a' + 10;
        } else return 0;
        line++;
    }
    return 1;
}


void sendByteHex(uint8_t value) 
{

    uint8_t ch = value >> 4;
    if (ch > 9) ch = ch - 10 + 'A';
    else ch = ch + '0';
  
    Serial.write(ch);
   
    ch = value & 0xF;
    if (ch > 9) ch = ch - 10 + 'A';
    else ch = ch + '0';
    
    Serial.write(ch);
}


uint8_t transmitStd(char *line) 
{

    uint32_t temp;
    uint8_t idlen;
    uint8_t i;
    uint8_t length;
    uint8_t canfd = 0;
    
    switch(line[0])
    {
      case 'D':
      case 'd':
        msg.brs = 1;
        canfd = 1;
        break;
      case 'B':
      case 'b':
         msg.brs = 0;
         canfd = 1;
        break;
      default:
        msg.brs = 0;
        break;

    }
   
    if (line[0] < 'Z') {
        msg.flags.extended = 1;
        idlen = 8;
    } else {
        msg.flags.extended = 0;
        idlen = 3;
    }

    if (!parseHex(&line[1], idlen, &temp)) return 0;

    if (line[0] < 'Z') {
        msg.id = temp;
     } else {
         msg.id = temp;
     }

    if (!parseHex(&line[1 + idlen], 1, &temp)) return 0;
     msg.len = skp_DLCtoBytes[temp];
    length = msg.len;

    if(canfd == 0)
    {               
      msg.edl = 0;   // Classic CAN
      msg.brs = 0;
      if (length > 8) length = 8;
        for (i = 0; i < length; i++) {
            if (!parseHex(&line[idlen + 2 + i*2], 2, &temp)) return 0;
            msg.buf[i] = temp;
        }
    }else
    { 
        msg.edl = 1;  // CAN FD frame
        if (length > 64) length = 64;
            for (i = 0; i < length; i++) 
            {
                if (!parseHex(&line[idlen + 2 + i*2], 2, &temp)) return 0;
                msg.buf[i] = temp;
            }
    }
   
    FD.write(msg);   // Sent data out on CAN
    return 1;

}


void parseLine(char * line) 
{

    uint8_t result = BELL;
   
    switch (line[0]) {
        case 'S': // Setup with standard CAN bitrates
            if (state == STATE_CONFIG)
            {
                switch (line[1]) {
               //     case '0': ;  result = CR; break;
               //     case '1': ;  result = CR; break;
               //     case '2': ;  result = CR; break;
               //     case '3': ;  result = CR; break;
                      case '4':  set_baudrate(CAN_125K_500K); result = CR; break;
                      case '5':  set_baudrate(CAN_250K_500K); result = CR; break;
                      case '6':  set_baudrate(CAN_500K_1M);   result = CR; break;
              //      case '7':  ; result = CR; break;
                      case '8':  set_baudrate(CAN_1000K_1M); result = CR; break;

                      
                      case '9':  set_baudrate(CAN_1000K_2M); result = CR; break;
                      case 'A':  set_baudrate(CAN_1000K_3M); result = CR; break;
                      case 'B':  set_baudrate(CAN_1000K_4M); result = CR; break;
                      case 'C':  set_baudrate(CAN_1000K_5M); result = CR; break;
                      case 'D':  set_baudrate(CAN_1000K_6M); result = CR; break;
                      case 'E':  set_baudrate(CAN_1000K_8M); result = CR; break;
                      case 'F':  set_baudrate(CAN_1000K_10M); result = CR; break;
                      case 'G':  set_baudrate(CAN_500K_1M); result = CR; break;
                      case 'H':  set_baudrate(CAN_500K_2M); result = CR; break;
                      case 'I':  set_baudrate(CAN_500K_3M); result = CR; break;
                      case 'J':  set_baudrate(CAN_500K_4M); result = CR; break;
                      case 'K':  set_baudrate(CAN_500K_5M); result = CR; break;
                      case 'L':  set_baudrate(CAN_500K_6M); result = CR; break;
                      case 'M':  set_baudrate(CAN_500K_8M); result = CR; break;
                      //case 'N':  set_baudrate(CAN_125K_500K); result = CR; break;

                }

            }
            break;
        case 's': 
            if (state == STATE_CONFIG)
            {
                unsigned long cnf1, cnf2, cnf3;
                if (parseHex(&line[1], 2, &cnf1) && parseHex(&line[3], 2, &cnf2) && parseHex(&line[5], 2, &cnf3)) {
                    result = CR;
                }
            }
            break;
        case 'G': 
            {
                unsigned long address;
                if (parseHex(&line[1], 2, &address)) {
                     result = CR;
                }
            }
            break;
        case 'W': 
            {
                unsigned long address, data;
                if (parseHex(&line[1], 2, &address) && parseHex(&line[3], 2, &data)) {
                  
                    result = CR;
                }

            }
            break;
        case 'V': // Get hardware version
            {

                Serial.print('V');
                sendByteHex(VERSION_HARDWARE_MAJOR);
                sendByteHex(VERSION_HARDWARE_MINOR);
                result = CR;
            }
            break;
        case 'v': // Get firmware version
            {

                Serial.print('v');;
                sendByteHex(VERSION_FIRMWARE_MAJOR);
                sendByteHex(VERSION_FIRMWARE_MINOR);
                result = CR;
            }
            break;
        case 'N': // Get serial number
            {
                Serial.print('N');
                Serial.print('f');
                Serial.print('f');
                Serial.print('f');
                Serial.print('f');
                
                result = CR;
            }
            break;
        case 'O': // Open CAN channel
            
            if (state == STATE_CONFIG)
            {
                digitalWriteFast(LED_B,ON);  
               
                state = STATE_OPEN;
                result = CR;
            }
            break;
        case 'l': // Loop-back mode
            if (state == STATE_CONFIG)
            {
                //TO DO loop back mode
                state = STATE_OPEN;
                result = CR;
            }
            break;
        case 'L': // Open CAN channel in listen-only mode
            if (state == STATE_CONFIG)
            {
                // TO DO listen-only mode
                state = STATE_LISTEN;
                result = CR;
            }
            break;
        case 'C': // Close CAN channel
            if (state != STATE_CONFIG)
            {
              digitalWriteFast(LED_B,OFF);  
              state = STATE_CONFIG;
              result = CR;
            }
            break;
        case 'r': // Transmit standard RTR (11 bit) frame
        case 'R': // Transmit extended RTR (29 bit) frame
        case 't': // Transmit standard (11 bit) frame
        case 'T': // Transmit extended (29 bit) frame
        case 'd': // Transmit FD standard (11 bit) frame with BRS
        case 'D': // Transmit FD extended (29 bit) frame with BRS
        case 'B': // Transmit FD standard (11 bit) frame no BRS
        case 'b': // Transmit FD extended (29 bit) frame no BRS
      
            if (state == STATE_OPEN)
            {
                if (transmitStd(line)) {
                Serial.print('z');
                result = CR;
                }

            }
            break;
        case 'F': // Read status flags
            {
                unsigned char status = 0;
                sendByteHex(status);
                result = CR;
                
            }
            break;
         case 'Z': // Set time stamping
            {
                unsigned long stamping;
                if (parseHex(&line[1], 1, &stamping)) {
                    timestamping = (stamping != 0);
                    result = CR;
                }
            }
            break;
         case 'm': // Set accpetance filter mask
            if (state == STATE_CONFIG)
            {
                unsigned long am0, am1, am2, am3;
          
                    result = CR;
              
            }
            break;
         case 'M': // Set accpetance filter code
            if (state == STATE_CONFIG)
            {
                unsigned long ac0, ac1, ac2, ac3;
                result = CR;
              
            }
            break;

    }
   
   Serial.write(result); 
}

uint8_t canmsg2ascii_getNextChar(uint32_t id, uint8_t dlc, uint8_t * step) {

    char ch = BELL;
    char newstep = *step;

    if (*step == RX_STEP_TYPE) {

        // type 1st char
        if ((msg.flags.extended  == 1) && (msg.edl == 1) && (msg.brs == 1))
        {
           newstep = RX_STEP_ID_EXT;
           ch = 'D';
        } //else if ((rxObj.bF.ctrl.IDE == 0) && (rxObj.bF.ctrl.FDF == 1)&& (rxObj.bF.ctrl.BRS == 1))
        else if ((msg.flags.extended  == 0) && (msg.edl == 1)&& (msg.brs== 1))
        {
            newstep = RX_STEP_ID_STD;
            ch = 'd';
        } //else if ((rxObj.bF.ctrl.IDE == 1) && (rxObj.bF.ctrl.FDF == 0))
        else if ((msg.flags.extended == 1) && (msg.edl == 0))
        {
            newstep = RX_STEP_ID_EXT;
            ch = 'T';
        }//else if ((rxObj.bF.ctrl.IDE == 0) && (rxObj.bF.ctrl.FDF == 0))
        else if ((msg.flags.extended == 0) && (msg.edl == 0))
        {
            newstep = RX_STEP_ID_STD;
            ch = 't';
        }//else if ((rxObj.bF.ctrl.IDE == 1) && (rxObj.bF.ctrl.FDF == 1)&& (rxObj.bF.ctrl.BRS == 0))
        else if ((msg.flags.extended == 1) && (msg.edl == 1)&& (msg.brs == 0))
        {
            newstep = RX_STEP_ID_EXT;
            ch = 'B';
        }//else if ((rxObj.bF.ctrl.IDE == 0) && (rxObj.bF.ctrl.FDF == 1)&& (rxObj.bF.ctrl.BRS == 0))
        else if ((msg.flags.extended == 0) && (msg.edl == 1)&& (msg.brs == 0))
        {
            newstep = RX_STEP_ID_STD;
            ch = 'b';
        }


    } else if (*step < RX_STEP_DLC) {

        uint8_t i = *step - 1;
        uint8_t * id_bp = (uint8_t*)&id; // rxObj.bF.id.SID;
        ch = id_bp[3 - (i / 2)];
        if ((i % 2) == 0) ch = ch >> 4;

        ch = ch & 0xF;
        if (ch > 9) ch = ch - 10 + 'A';
        else ch = ch + '0';

        newstep++;

    } else if (*step < RX_STEP_DATA) {

        ch = len_to_dlc(dlc);
        ch = ch & 0xF;
        if (ch > 9) ch = ch - 10 + 'A';
        else ch = ch + '0';

        if (dlc ==0) newstep = RX_STEP_TIMESTAMP;
        else newstep++;

    } else if (*step < RX_STEP_TIMESTAMP) {

        uint8_t i = *step - RX_STEP_DATA;

        ch = msg.buf[i/2];
        if ((i % 2) == 0) ch = ch >> 4;

        ch = ch & 0xF;
        if (ch > 9) ch = ch - 10 + 'A';
        else ch = ch + '0';

        newstep++;
        if (newstep - RX_STEP_DATA ==dlc*2) newstep = RX_STEP_TIMESTAMP;

    } else if (timestamping && (*step < RX_STEP_CR)) {

        uint8_t i = *step - RX_STEP_TIMESTAMP;

        if ((i % 2) == 0) ch = ch >> 4;

        ch = ch & 0xF;
        if (ch > 9) ch = ch - 10 + 'A';
        else ch = ch + '0';

        newstep++;

    } else {

        ch = CR;
        newstep = RX_STEP_FINISHED;
    }

    *step = newstep;
    return ch;
}

void out_usb(void)
{

  uint8_t i,dlc,rxstep;
  uint8_t out_buff[200];
  uint8_t outdata = 0;
  uint32_t can_id;
 
  i =0;
  rxstep = 0;
  
  can_id = msg.id;
  dlc = msg.len;

  while( rxstep != RX_STEP_FINISHED)
  {
    outdata = canmsg2ascii_getNextChar(can_id, dlc, &rxstep);
    out_buff[i++] =outdata;
  }
  
  out_buff[i] = 0;  //Add null
 
  Serial.print((char *)out_buff);   // Sent data out on USB
  
}
