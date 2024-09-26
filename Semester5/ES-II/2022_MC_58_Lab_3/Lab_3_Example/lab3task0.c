/* Lab_3_Example_1_Toggle_Onboard_LED_using_SysTick.uvprojx
-----------------------------------------------------------------------
This C language program toggles on-board user green LED of TIVA C Series 
LaunchPad Evaluation Kit by Texas Instrument at 1 Hz by using SysTick Timer.
-----------------------------------------------------------------------
Written by Haseeb-ul-Hassan().
Compiler and IDE used: Keil uVision 5.34 */
#include <TM4C123.h>
unsigned volatile long j;
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
						GPIOF->DATA ^= 1<<3;// Toggle the green LED
						if(GPIOF->DATA != 0x08){GPIOF->DATA ^= 1<<1;}
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
