
#ifndef CAN_FD_H_
#define CAN_FD_H_


#include <Arduino.h>
#include <string.h>

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" { 
#endif

typedef enum {
    CAN_125K_500K,
    CAN_250K_500K,
    CAN_500K_1M,    
    CAN_500K_2M,   
    CAN_500K_3M,
    CAN_500K_4M,
    CAN_500K_5M,    
    CAN_500K_6M,
    CAN_500K_8M,    
    CAN_500K_10M,
    CAN_1000K_1M,
    CAN_1000K_2M,
    CAN_1000K_3M,
    CAN_1000K_4M,
    CAN_1000K_5M,
    CAN_1000K_6M,  
    CAN_1000K_8M,
    CAN_1000K_10M, 
} CAN_BITTIME_SETUP;


void set_baudrate(CAN_BITTIME_SETUP baudrate);


#ifdef  __cplusplus
}
#endif

#endif /* CAN_FD_H_ */
