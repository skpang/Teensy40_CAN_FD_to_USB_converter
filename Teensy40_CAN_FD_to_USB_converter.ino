/*
 * Teeny 4.0 CAN FD to USB Converter Demo
 * 
 * For use with
 * http://skpang.co.uk/catalog/teensy-40-can-fd-to-usb-converter-p-1588.html
 * 
 * v1.0 April 2020
 * 
 * www.skpang.co.uk
 * 
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
FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> FD;
#include "process_data.h"

#include "can_fd.h"

#define LINE_MAXLEN 150
#define BELL 7
#define CR 13
#define LR 10

int LED_R = 0;
int LED_G = 2;
int LED_B = 1;
int SILENT = 9;

IntervalTimer timer;

uint16_t i=0;
int led = 13;
CANFD_message_t msg;
uint8_t rx_byte = 0 ;
uint8_t linepos = 0;
volatile uint8_t state = STATE_CONFIG;

char line[LINE_MAXLEN];
char ch =0;

elapsedMillis can_rx_timer;
elapsedMillis can_tx_timer;


void setup(void) {

  pinMode(LED_R,OUTPUT);pinMode(LED_G,OUTPUT);pinMode(LED_B,OUTPUT);  
  pinMode(SILENT,OUTPUT); 
  pinMode(led,OUTPUT);

  digitalWrite(LED_G,OFF);digitalWrite(LED_B,OFF);
  digitalWrite(SILENT,LOW);
  
  digitalWrite(LED_R,ON);
  delay(200);
  digitalWrite(LED_R,OFF);
  digitalWrite(LED_G,ON);
  delay(200);
  digitalWrite(LED_G,OFF);
  digitalWrite(LED_B,ON);
  
  Serial.begin(115200); delay(600);
 //Serial.println("Teensy 4.0 CAN FD to USB Converter www.skpang.co.uk v1.0 March 2020 dd");
  digitalWrite(LED_B,OFF);

  FD.begin();
  set_baudrate(CAN_1000K_2M);
     
}


void loop() 
{
 
  rx_byte = Serial.available();   // Check USB incoming data
 
  if (rx_byte) 
  {
    for(i=0;i<rx_byte;i++)
    {
       ch=  Serial.read(); 
       if (ch == CR) {
           line[linepos] = 0;
           parseLine(line);             // Full line received, process it
           digitalWriteFast(LED_G,ON);
           can_tx_timer = 0;
           linepos = 0;
           
        }else if (ch != LR) {
            line[linepos] = ch;
            if (linepos < LINE_MAXLEN - 1) linepos++;
        }
    }
   
  }
  
  FD.events(); 

  if(FD.readMB(msg))                // Check any CAN frame received
  {
      out_usb();                    // Sent it out on USB
      digitalWriteFast(LED_R,ON);
      can_rx_timer = 0;
  }

  if(can_rx_timer > 10)             // 10ms red LED on
  {
    digitalWriteFast(LED_R,OFF);
  }

  if(can_tx_timer > 10)             // 10ms green LED on
  {
    digitalWriteFast(LED_G,OFF);
  }

}
