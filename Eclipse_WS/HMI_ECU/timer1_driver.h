 /******************************************************************************
 *
 * Module: Timer 1 Driver
 *
 * File Name: timer1_driver.h
 *
 * Description: Header file for Timer 1 driver
 *
 * Author: Mohamed Hassan
 *
 *******************************************************************************/
#ifndef TIMERS_H_
#define TIMERS_H_

#include "std_types.h"

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
/*
 * Description : Function to initialize the Timer1 driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);
/* Function to de-initialize timer1*/
void Timer1_deInit(void);
/*Function to set ISR call back function*/
void Timer1_setCallBack(void(*a_ptr)(void));
#endif /* TIMERS_H_ */

