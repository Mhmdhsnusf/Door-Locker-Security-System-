/******************************************************************************
 *
 * Module: Motor
 *
 * File Name: motor.h
 *
 * Description: Header file for the DC Motor driver
 *
 * Author: Mohamed Hassan
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_
#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define MOTOR_FIRST_PORT_ID PORTD_ID
#define MOTOR_FIRST_PIN_ID PIN6_ID
#define MOTOR_SECOND_PORT_ID PORTD_ID
#define MOTOR_SECOND_PIN_ID PIN7_ID
#define DcMotor_EN1_PORT_ID	PORTB_ID
#define DcMotor_EN1_Pin_ID	PIN3_ID
/*Motor Directions ENUM*/
typedef enum
{
	STOP,CW,A_CW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for assigning motor I/O pins
 */
void DcMotor_Init(void);

/*
 * Description :
 * Functional responsible for rotating the motor in the required direction and with the required speed
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* MOTOR_H_ */
