
/******************************************************************************
 *
 * Module: Control_ECU
 *
 * File Name: Control_ECU.h
 *
 * Description: Source file for the Control_ECU Driver
 *
 * Author: Mohamed Hassan
 *
 *******************************************************************************/
#define MOTOR 0
#define BUZZER 1
#define RECEIVING_ENTERED_PASSWORDS 0
#define CHECK_PASSWORDS_MATCHING 1
#define SAVE_PASSWORDS 2
#define RECEIVE_AND_CHECK_WITH_EEPROM 3
#define OPEN_DOOR 4
#include "common_macros.h"
#include "uart.h"
#include"control_ecu.h"
#include "external_eeprom.h"
#include "buzzer.h"
#include"gpio.h"
#include"twi.h"
#include"Timer1_driver.h"
#include<util/delay.h>
#include"Motor.h"
#include <avr/interrupt.h> /* For enabling I-bit*/
#include<avr/io.h>

/*******************************************************************************
 *                      Global Variables									   *
 *******************************************************************************/
static uint8 *g_ptr1[PASSWORD_SIZE],*g_ptr2[PASSWORD_SIZE]; /* Storing password sent from HMI_ECU*/
static uint8 application_sequence_MC2=0; /*Controls Control_ECU MC application sequence*/
static volatile uint8 g_tick_motor; /* For counting the operation time of the motor*/
static volatile uint8 g_tick_buzzer; /* For counting the operation time of the buzzer*/

/*******************************************************************************
 *                      Main Function                         				   *
 *******************************************************************************/
int main(void)
{
	SREG|=(1<<7); /*Enabling Global interrupts enable*/
	UART_ConfigType UART_Config = {8,1,UART_STOP_1BIT_SELECT,9600}; /*UART configurations*/
	Timer1_ConfigType  Config_Ptr = {0,0,Prescaler_1024,Overflow_Mode};/*Timer1 Configurations*/
	UART_init(&UART_Config); /*Initializing UART*/
	DcMotor_Init(); /* Initializing DC Motor (I/O pins)*/
	Buzzer_Init(); /* Initializing Buzzer (I/O pins)*/
	EEPROM_init(); /*Initializing TWI for interacting with EEPROM*/
	while(1)
	{
		switch(application_sequence_MC2)
		{
		case RECEIVING_ENTERED_PASSWORDS:
		{
			receive_Password(&g_ptr1);/*Receiving the entered password from HMI_ECU*/
			receive_Password(&g_ptr2);/*Receiving the re-entered password from HMI_ECU*/
			application_sequence_MC2++; /* Jump to the next step (Checking Step)*/
			break;
		}
		case CHECK_PASSWORDS_MATCHING :
		{
			uint8 current_state=check_Password(&g_ptr1,&g_ptr2); /*Check if the two passwords sent are the same*/
			if(current_state == MATCHED) /*Two passwords are matched*/
			{
				sendByte(MATCHED); /*Send to HMI_ECU that they're matched*/
				application_sequence_MC2++; /*Jump to the next step*/
			}
			else
			{
				sendByte(UN_MATCHED);/*Send to HMI_ECU that they're not matched*/

			}
			break;
		}
		case SAVE_PASSWORDS:
		{
			save_Password(&g_ptr1); /*Save password entered by the user*/
			application_sequence_MC2++; /*Jumps to the next step*/
			break;
		}
		case RECEIVE_AND_CHECK_WITH_EEPROM:
		{
			receive_Password(&g_ptr1); /*Receive the password entered by the user (User's old password)*/
			uint8 current_state=Compare_with_saved_password(&g_ptr1, &g_ptr2); /*Compare it with the password saved in the EEPROM*/
			if(current_state == MATCHED)
			{
				sendByte(MATCHED);/*Send that the password matched with the one saved in the EEPROM*/
				if(recieveByte()=='+')
				{
					application_sequence_MC2++;/*Jump to next step to operate the motor*/
					sendByte(0x04);
					/*
					 * Let application sequence variable in the HMI go to the step
					 * where the LCD displays the required messages for motor operations
					 */

				}
				else
				{
					sendByte(0x00);
					/*
					 * Let application sequence variable in the HMI go to the step
					 * where the LCD displays the required messages for the user to change his password
					 */
					application_sequence_MC2=0;
				}

			}
			else
			{
				/*Send to HMI_ECU that the passwords is not matched with the one in the EEPROM*/
				sendByte(UN_MATCHED);
				uint8 i;
				/*
				 * Receive the password entered by the user two more times
				 * if it's the same with the one saved in the EEPROM then continue the program and execute
				 * the function which is responsible for the choice he chose (Open Door/Change password)
				 * else
				 * Activate the Buzzer and block the system for 1 minute
				 * After the 1 min ends return to case 3
				 */
				for(i=0;i<2;i++)
				{
					receive_Password(&g_ptr1);
					if(Compare_with_saved_password(&g_ptr1, &g_ptr2))
					{
						sendByte(MATCHED);
						application_sequence_MC2=3;
						break;

					}
					else
					{
						sendByte(UN_MATCHED);
					}
				}
				if(i==2)
				{
					timer1_SetRequiredCallBack(BUZZER); /*Set Timer 1 callback to buzzerTime function*/
					Timer1_init(& Config_Ptr); /*Initialize Timer 1*/
					while(g_tick_buzzer < 8)/*Operate the buzzer for 1 min*/
					{
						Buzzer_on();
					}
					if(g_tick_buzzer==8)/*Turn off the buzzer after the 1 min ends*/
					{
						Buzzer_off();
						Timer1_deInit();
						g_tick_buzzer=0;

					}
					/*Return to case main options control (Case 3)*/
					application_sequence_MC2=3;
				}
			}
			break;
		}
		case OPEN_DOOR:
		{
			timer1_SetRequiredCallBack(MOTOR);/*Set Timer 1 callback to motorTime function*/
			Timer1_init(& Config_Ptr);/*Initialize Timer 1*/
			while(g_tick_motor<2) /*Turn on the motor clockwise for 15 sec*/
			{
				DcMotor_Rotate(CW, 25);
			}
			if(g_tick_motor == 2)/*Turn off the motor for 3 sec*/
			{
				DcMotor_Rotate(STOP, 0);
				Timer1_deInit();
				_delay_ms(3000);
				Timer1_init(&Config_Ptr);
			}
			while(g_tick_motor>= 2 && g_tick_motor < 4)/*Turn on the motor anti-clockwise for 15 sec*/
			{
				DcMotor_Rotate(A_CW, 25);
			}
			if(g_tick_motor==4) /*Turn off the motor*/
			{
				DcMotor_Rotate(STOP, 0);
				Timer1_deInit();
				g_tick_motor=0;
			}
			application_sequence_MC2=3; /*Return to main options control*/
			break;

		}

		}

	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for :
 * Receiving Password entered by the user from HMI_ECU
 */
void receive_Password(uint8* password_holder)
{
	uint8 i=0;
	while(UART_recieveByte()!=READY);
	UART_sendByte(READY);
	while(i<PASSWORD_SIZE)
	{
		password_holder[i]=UART_recieveByte();
		i++;

	}
	UART_sendByte(DONE);
}
/*
 * Description :
 * Functional responsible for :
 * 1. Taking two passwords (entered/re-entered) entered by the user and sent through Receive_password function
 * 2. Check if both are the same
 * 3. return if they are matched or not
 */
uint8 check_Password(uint8 *Password_array1,uint8 *Password_array2)
{

	uint8 i;
	for(i=0;i<PASSWORD_SIZE;i++)
	{
		if(Password_array1[i]!=Password_array2[i])
		{
			return UN_MATCHED;
		}

	}
	return MATCHED;


}
/*
 * Description :
 * Functional responsible for :
 * Comparing the password entered by the user with the password saved in the EEPROM (Entered by the user before)
 */
uint8 Compare_with_saved_password(uint8*Password_array1,uint8*Password_array2)
{
	uint8 i;

	for(i=0;i<PASSWORD_SIZE;i++)
	{
		EEPROM_readByte(0x0000+i,Password_array2[i]);
		_delay_ms(10);
	}

	for(i=0;i<PASSWORD_SIZE;i++)
	{
		if(Password_array1[i]!=Password_array2[i])
		{
			return UN_MATCHED;
		}

	}
	return MATCHED;



}
/*
 * Description :
 * Functional responsible for :
 * Saving the password passed to it in the EEPROM
 */
void save_Password(uint8 *Password_array)
{
	uint8 i;
	for(i=0;i<PASSWORD_SIZE;i++)
	{
		EEPROM_writeByte((0x0000+i),Password_array[i]);
		_delay_ms(10);
	}
}
/*
 * Description :
 * Functional responsible for :
 * Syncing the UART in the Control_ECU and the UART in HMI_ECU
 * Receiving data from the UART in the HMI_ECU
 */
uint8 recieveByte(void)
{
	uint8 byte;
	while(UART_recieveByte() != READY){}
	UART_sendByte(READY);
	byte=UART_recieveByte();
	UART_sendByte(DONE);
	return byte;
}
/*
 * Description :
 * Functional responsible for :
 * Syncing the UART in the Control_ECU and the UART in HMI_ECU
 * Sending data to the UART in the HMI_ECU
 */
void sendByte(uint8 byte)
{
	UART_sendByte(READY);
	while(UART_recieveByte() != READY){}
	UART_sendByte(byte);
	while(UART_recieveByte() != DONE){}
}
/*
 * Description :
 * Function called when by the ISR when the overflow interrupt take place
 */
void motorTime(void)
{
	g_tick_motor++;
}
/*
 * Description :
 * Function called when by the ISR when the overflow interrupt take place
 */
void buzzerTime(void)
{
	g_tick_buzzer++;
}
/*
 * Description :
 * Functional responsible for :
 * Setting the required call back function
 */
void timer1_SetRequiredCallBack(uint8 device)
{
	switch(device)
	{
	case MOTOR : Timer1_setCallBack(&motorTime);
	break;
	case BUZZER : Timer1_setCallBack(&buzzerTime);
	}
}

