//|_______________________________________________________________________________________________________________________________________________________|
//|Task 3.1: Modify the Example_01 program such that LED blinks at double the current frequency. You are not allowed to change the value ofRELOAD Register|
//|Hint: you need to read CURRENT register for value equal to half the RELOAD value and toggle LED (in addition to LED toggle when CURRENT turns to zero).|
//|-------------------------------------------------------------------------------------------------------------------------------------------------------|
//|Name : Haseeb-ul-hassan																												   																																		  |
//|Date : 29-SEP-2024																																																																		  |
//|Hardware: Tiva C series Launchpad Evaluation kit by Texas instruments																				  																				|	
//|IDE and Compiler : Keil microvision 5.38a																											  																											|
//|Disclaimer : This program has been written as an open source develpment program so any usage must be credited and i am not responsible for any missuse |
//|Contact : 2022mc58@student.uet.edu.pk(feel free to ask me anything about this code),Github :oldyellowbart											  											|
//|_______________________________________________________________________________________________________________________________________________________|
#include <TM4C123.h>
unsigned volatile long j;
void P_Digital_Output_Init();
void PF3_Digital_Output_Init();
void PF3_Digital_Output_Init();
void PF1_Digital_Output_Init();
void SysTick_Init();
int main()// User main program
			{
				PF3_Digital_Output_Init();
				PF1_Digital_Output_Init();
				SysTick_Init();
				while (1)
				{
					if(SysTick->CTRL & (1<<16))// Read SysTick Status Bit
					{		
						//GPIOF->DATA ^= 1<<3;// Toggle the green LED
						}
					if(SysTick->VAL == 0x7A1200)
						{
							GPIOF->DATA ^= 1<<3;// Toggle the green LED
					
						}
				}
			}
void PF3_Digital_Output_Init()
{
	SYSCTL->RCGCGPIO |= 1<<5; // Step 1: Clock enable on PortF
	for (j =0; j < 3 ; j++)	 // at least 3 clock cyles
	GPIOF->AFSEL &= ~(1<<3);// Step 3: Disable alternate functionality on PortF
	GPIOF->DEN |= 1<<3;    // Step 4: Enable digital pin functionaliy on PortF pin 3
	GPIOF->DIR |= 1<<3;   // Step 5: Set PortF pin 3 as an output pin
}
void PF1_Digital_Output_Init()
{
	SYSCTL->RCGCGPIO |= 1<<5; // Step 1: Clock enable on PortF
	for (j =0; j < 3 ; j++)	 // at least 3 clock cyles
	GPIOF->AFSEL &= ~(1<<1);// Step 3: Disable alternate functionality on PortF
	GPIOF->DEN |= 1<<1;    // Step 4: Enable digital pin functionaliy on PortF pin 3
	GPIOF->DIR |= 1<<1;   // Step 5: Set PortF pin 3 as an output pin
}

void SysTick_Init(void)
{
	// 1. Program the value in the STRELOAD register
	SysTick->LOAD = 16000000-1; 	// Default clock frequency
	// 2. Clear the STCURRENT register by writing to it with any value.
	SysTick->VAL = 0x00;
	// 3. Configure the STCTRL register for the required operation.
	SysTick->CTRL |= (1<<2)|(1<<0);		// SysTick enable, Main Oscillator as Clock Source

}




/* Lab_01_Example_01_Static_String_at_16x2_LCD.uvprojx
-----------------------------------------------------------------------
This C language program displays static string ("Hello World") on 16x2 
LCD interfaced with TIVA C Series LaunchPad Evaluation Kit by Texas 
Instrument as per table given below.

** Interfacing details---------
RS -> PD0
R/W -> PD1
EN -> PD2
D0-D7 -> PB0-PB7

-----------------------------------------------------------------------
Written by Shujat Ali (engrshujatali@gmail.com) on 18-Aug-2024.
Compiler and IDE used: Keil uVision 5.38 */

// Libraries
#include "TM4C123.h"
#include <string.h>

// Global Variables
unsigned volatile long j;

// Function Declarations
void PD0_2_as_Output_Init(void);
void PortB_as_Output_Init(void);
void write_LCD_Char(unsigned char data);
void send_LCD_Cmd(unsigned char cmd);
void write_LCD_Str(unsigned char *str);
void LCD_Init(void);

// Main Function
int main(){
	PD0_2_as_Output_Init();
	PortB_as_Output_Init();
	LCD_Init();
	
	write_LCD_Str("Hello_World");
	
	while(1){
	}	
}

// Function Defintions
void PD0_2_as_Output_Init(void)
	{
	SYSCTL->RCGCGPIO |= 0x08; // 00 1000// Step 1: Clock enable on PortD
	for (j =0; j < 3 ; j++); // wait at least 3 clock cyles
	GPIOD->AFSEL &= ~(0x07);// Step 3: Disable alternate functionality on PortD
	GPIOD->DEN |= 0x07; 	 //Step 4: Enable digital pin functionaliy on PortD pin 0-2
	GPIOD->DIR |= 0x07; 	// Step 5: Set PortD pin 0-2 as an output pin
	}
void PortB_as_Output_Init(void)
	{
	SYSCTL->RCGCGPIO |= 0x02; // Step 1: Clock enable on PortB	 00 0010
	for (j =0; j < 3 ; j++)	 // wait at least 3 clock cyles
	GPIOB->AFSEL &= ~(0xFF);// Step 3: Disable alternate functionality on PortB
	GPIOB->DEN |= 0xFF;    // Step 4: Enable digital pin functionaliy on PortB pin 0-7
	GPIOB->DIR |= 0xFF;	  // Step 5: Set PortB pin 0-7 as an output pin
	}
void write_LCD_Char(unsigned char data)
	{
	// Interfacing details---------------
	// RS -> PD0		R/W -> PD1			EN -> PD2			D0-D7 -> PB0-PB7
	GPIOB->DATA = data;			// Step 1. Pass data to 8-bit lines of LCD (D0-7)
	GPIOD->DATA &= ~(0x02);//Step 2. Enable write operation on LCD (R/W bit = 0)
	GPIOD->DATA |= 0x01;  // Step 3. Select Data Register of LCD (RS bit = 1) PD0 = 0000 0001
	GPIOD->DATA |= 0x04; // Step 4. Enable LCD operation by sending high to low pulse on Enable pin 			// PD2 = 0000 0100
	for (j =0; j < 10000 ; j++);// some milisecond delay
	GPIOD->DATA &= ~(0x04); 	 // PD2 = 0000 0100
	}

void send_LCD_Cmd(unsigned char cmd)
	{
	// Interfacing details---------------
	// RS -> PD0		R/W -> PD1			EN -> PD2			D0-D7 -> PB0-PB7
	GPIOB->DATA = cmd;	     // Step 1. Pass command to 8-bit lines of LCD (D0-7)
	GPIOD->DATA &= ~(0x02); // Step 2. Enable write operation on LCD (R/W bit = 0) PD1 = 0000 0010
	GPIOD->DATA &= ~(0x01);// Step 3. Select Instructioin Register of LCD (RS bit = 0) PD0 = 0000 0001
	GPIOD->DATA |= 0x04;  // Step 4. Enable LCD operation by sending high to low pulse on Enable pin PD2 = 0000 0100
	for (j =0; j < 10000 ; j++);// some milisecond delay
	GPIOD->DATA &= ~(0x04); 	 // PD2 = 0000 0100
	}
void write_LCD_Str(unsigned char *str){
	unsigned int i, len;
	len = strlen(str);
	
	for (i = 0; i < len; i++){
		write_LCD_Char(str[i]);
	}
}

void LCD_Init(void){
	send_LCD_Cmd(0x38);		// Function Set: 8-bit, 2 Line, 5x7 Dots
	send_LCD_Cmd(0x06);		// Auto increment cursor (shift cursor to right)
	send_LCD_Cmd(0x0C);		// Display on, cursor off
	send_LCD_Cmd(0x01);		// Clear display screen
	send_LCD_Cmd(0x80);		// Force cursor to the beginning (1st line)
	send_LCD_Cmd(0x18);	
}
