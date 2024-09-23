/* Lab_01_task_1-4.1 task recreate using library
-----------------------------------------------------------------------
Task 0.1: Repeat Task 4.12of ES-I Lab (Spring-2024) by 
#including <TM4C123.h> library and without defining 
macros for GPIO registers and use user-defined functions.
-----------------------------------------------------------------------
Written by Haseeb-ul-hassan (github-oldyellowbart) on 15-SEP-2024.
Compiler and IDE used: Keil uVision 5.31a */
//_______________________________________________________________________
// Libraries
#include<TM4C123.h>
// Global Variables
unsigned volatile long j;
// Function Declarations
void PE0_4_as_Input_Init(void);
void PortB_as_Output_Init(void);
void msDelay(int dValue);
// Main Function


// Function Defintions

int main(){
	PE0_4_as_Input_Init();
	PortB_as_Output_Init();
	while(1){
        if (GPIOE->DATA == 0x01)//0000 0001
        {
            GPIOB->DATA |= 0x01;//0000 0001
						msDelay(500);
						GPIOB->DATA |= 0x02;//0000 0010
						msDelay(500);
						GPIOB ->DATA |= 0x04;//0000 0100
						msDelay(500);
						GPIOB ->DATA |= 0x08;//0000 1000
						msDelay(500);
						GPIOB ->DATA |= 0x10;//0001 0000
						msDelay(500);
						GPIOB ->DATA |= 0x20;//0010 0000
						msDelay(500);
						GPIOB ->DATA |= 0x40;//0100 0000
						msDelay(500);
						GPIOB ->DATA |= 0x80;//1000 0000
						msDelay(500);
					GPIOB->DATA &= 0x00;//0000 0001
						msDelay(1000);
						
        }
				else if (GPIOE->DATA == 0x02)//0000 0001
        {
            GPIOB->DATA |= 0x01;//0000 0001
						GPIOB->DATA |= 0x02;//0000 0010
						GPIOB ->DATA |= 0x04;//0000 0100
						GPIOB ->DATA |= 0x08;//0000 1000
						msDelay(500);
						GPIOB ->DATA |= 0x10;//0001 0000
						GPIOB ->DATA |= 0x20;//0010 0000
						GPIOB ->DATA |= 0x40;//0100 0000
						GPIOB ->DATA |= 0x80;//1000 0000
						msDelay(500);
						GPIOB->DATA &= 0x00;//0000 0001
					msDelay(1000);
				}	
				else if (GPIOE->DATA == 0x04)//0000 0001
        {
            GPIOB->DATA |= 0x03;//0000 0001
						msDelay(500);
						GPIOB->DATA |= 0x0C;//0000 0010
						msDelay(500);
						GPIOB ->DATA |= 0x30;//0001 0000
						msDelay(500);
						GPIOB ->DATA |= 0xC0;//0010 0000
						msDelay(500);
						GPIOB->DATA &= 0x00;//0000 0001
					msDelay(1000);
				}	
				else if (GPIOE->DATA == 0x08)//0000 0001
        {
            GPIOB->DATA |= 0xFF;//0000 0001
						msDelay(500);
						GPIOB->DATA &= 0x00;//0000 0001
						msDelay(500);
						GPIOB->DATA |= 0xFF;//0000 0001
						msDelay(500);
						GPIOB->DATA &= 0x00;//0000 0001
					msDelay(1000);
				}	
				else if (GPIOE->DATA == 0x10)//0000 0001
        {
            GPIOB->DATA |= 0xF1;//0000 0001
						msDelay(500);
						GPIOB->DATA &= 0x00;//0000 0001
						msDelay(500);
						GPIOB->DATA |= 0x1F;//0000 0001
						msDelay(500);
						GPIOB->DATA &= 0x00;//0000 0001
					msDelay(1000);
				}	
		}}

	void PE0_4_as_Input_Init(void){
	// Step 1: Clock enable on PortE
	SYSCTL->RCGCGPIO |= 0x10;	// 01 0000
	for (j =0; j < 3 ; j++);		// wait at least 3 clock cyles
	// Step 2: APB is selected for PortE by selecting
	// 0x40007000 as Base Address in DATA section
	// Step 3: Disable alternate functionality on PortE
	GPIOE->AFSEL &= ~(0x1F); // Disable for PE0-4 (0001 1111)
	// Step 4: Enable digital pin functionaliy on PortE pin 0-4
	GPIOE->DEN |= 0x1F; // Digital enable for PE0-4 (0001 1111)
	// Step 5: Set PortD pin 0-4 as an output pin
	GPIOE->DIR &= ~(0x1F); // PE0-4 as output (0000 0111)
							}
void PortB_as_Output_Init(void){
	// Step 1: Clock enable on PortB
	SYSCTL->RCGCGPIO |= 0x02;	// 00 0010
	for (j =0; j < 3 ; j++)		// wait at least 3 clock cyles
	// Step 2: APB is selected for PortB by selecting
	// 0x40005000 as Base Address in DATA section
	// Step 3: Disable alternate functionality on PortB
	GPIOB->AFSEL &= ~(0xFF); // Disable for PB0-7 (1111 1111)
	// Step 4: Enable digital pin functionaliy on PortB pin 0-7
	GPIOB->DEN |= 0xFF; // Digital enable for PB0-7 (1111 1111)
	// Step 5: Set PortB pin 0-7 as an output pin
	GPIOB->DIR |= 0xFF; // PB0-7 as output (1111 1111)
}
void msDelay(int dValue){
	for (j =0; j < 4000*dValue; j++);
}