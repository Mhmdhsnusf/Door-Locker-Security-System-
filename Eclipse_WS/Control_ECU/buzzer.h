/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: header file for the buzzer driver
 *
 * Author: Mohamed Hassan
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT_ID PORTC_ID
#define BUZZER_PIN_ID PIN7_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for assigning buzzer pin as output pin
 */
void Buzzer_Init();
/*
 * Description :
 * Functional responsible for turning on the buzzer
 */
void Buzzer_on();
/*
 * Description :
 * Functional responsible for turning on the buzzer
 */
void Buzzer_off();

#endif
