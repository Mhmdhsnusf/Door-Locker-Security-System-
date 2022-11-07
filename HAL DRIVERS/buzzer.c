/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the buzzer driver
 *
 * Author: Mohamed Hassan
 *
 *******************************************************************************/
#include"gpio.h"
#include"buzzer.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for assigning buzzer pin as output pin
 */
void Buzzer_Init()
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW );
}
/*
 * Description :
 * Functional responsible for turning on the buzzer
 */
void Buzzer_on()
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH );
}
/*
 * Description :
 * Functional responsible for turning on the buzzer
 */
void Buzzer_off()
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW );
}


