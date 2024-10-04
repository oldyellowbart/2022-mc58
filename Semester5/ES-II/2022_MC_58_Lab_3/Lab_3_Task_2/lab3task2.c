//|_______________________________________________________________________________________________________________________________________________________|
//|Task 3.2: Modify Example_01 program such that LED blinks with two times slower than the current value. You are not allowed to change the value of      |
//|RELOAD Register. Hint: You may need to count High STATUS flag two times before toggling the LED.                                                       |
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
int count = 0;
void PF3_Digital_Output_Init();
void SysTick_Init();
int main()// User main program
		{
				PF3_Digital_Output_Init();
				SysTick_Init();
				while (1)
				{
					if(SysTick->CTRL & (1<<16)){count ++;}
					if(count == 2){GPIOF->DATA ^= 1<<3;}}
		        }
void PF3_Digital_Output_Init()
{
	SYSCTL->RCGCGPIO |= 1<<5; // Step 1: Clock enable on PortF
	for (j =0; j < 3 ; j++)	 // at least 3 clock cyles
	GPIOF->AFSEL &= ~(1<<3);// Step 3: Disable alternate functionality on PortF
	GPIOF->DEN |= 1<<3;    // Step 4: Enable digital pin functionaliy on PortF pin 3
	GPIOF->DIR |= 1<<3;   // Step 5: Set PortF pin 3 as an output pin
}
void SysTick_Init(void)
{
	
	SysTick->LOAD = 16000000-1;      // 1. Program the value in the STRELOAD register
	SysTick->VAL = 0x00;	        // 2. Clear the STCURRENT register by writing to it with any value
	SysTick->CTRL |= (1<<2)|(1<<0);// SysTick enable, Main Oscillator as Clock Source

}
