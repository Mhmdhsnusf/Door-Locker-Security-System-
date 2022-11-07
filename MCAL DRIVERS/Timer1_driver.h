 /******************************************************************************
 *
 * Module: AVR timers driver
 *
 * File Name: avr_timers.h
 *
 * Description: Header file for the timers AVR driver
 *
 * Author: Mohamed Hassan
 *
 *******************************************************************************/
#ifndef TIMERS_H_
#define TIMERS_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/




/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
Overflow_Mode,Compare_Mode
}Timer1_Mode;
typedef enum
{
	No_clk,No_Prescaler,Prescaler_8,Prescaler_64,Prescaler_256,Prescaler_1024,External_clkSource_FallingEdge,External_clkSource_RisingEdge
}Timer1_Prescaler;

typedef struct {
uint16 initial_value;
uint16 compare_value; // it will be used in compare mode only.
Timer1_Prescaler prescaler;
Timer1_Mode mode;
}Timer1_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void Timer1_init(const Timer1_ConfigType * Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void(*a_ptr)(void));
#endif /* TIMERS_H_ */

