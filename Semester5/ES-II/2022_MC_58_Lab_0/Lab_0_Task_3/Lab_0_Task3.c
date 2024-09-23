/* Lab_01_task_3-6.3 task recreate using library
-----------------------------------------------------------------------
Task 0.3: Repeat Task 6.3 of ES-I Lab (Spring-2024) by 
#including <TM4C123.h> library and without defining macros
for GPIO registers and use user-defined functions.
-----------------------------------------------------------------------
Written by Haseeb-ul-hassan (github-oldyellowbart) on 15-SEP-2024.
Compiler and IDE used: Keil uVision 5.31a */
//_______________________________________________________________________
// Libraries
#include<TM4C123.h>
#define Key_pad_Row_1 0x01;
#define DELAY_DEBOUNCE 16000000 / 10000
unsigned char CC_7Seg_Disp_LookupTable[4][4] = { // col 1	 col2	 col3	 col4
	{0x79, 0x24, 0x30, 0x08},					 // row 1
	{0x19, 0x12, 0x02, 0x03},					 // row 2
	{0x78, 0x00, 0x10, 0x46},					 // row 3
	{0x92, 0x40, 0x09, 0x21}};					 // row 4
// Global Variables
unsigned volatile long j;
// Function Declarations
void PE0123_as_Output_Init(void);//ok
void PC4567_as_Input_Init(void);
void PortB_as_Output_Init(void);//ok
void PortA2345_as_Output_Init(void);//ok
void msDelay(int dValue);
// Main Function
int main()
	{
		void PE0123_as_Output_Init();
		void PC4567_as_Input_Init();
		void PortB_as_Output_Init();
		void PortA2345_as_Output_Init();
		int press = 0;
	unsigned char D1, D2, D3, D4;

	while(1)
		{
					for (short j = 0; j < 4; j++)
					{GPIOE->DATA == 0x01<<j;
						for (short i = 0; i < 4; i++)
						{
						// Digital read for any key pressed on Keypad between 0 to 9
							if ((GPIOC->DATA & 0xF0) == 0x10 << i)
							{
								while ((GPIOC->DATA & 0xF0) == 0x01 << i)
									{
										}
										press++;
										if (press == 5)
										{
											press = 0;
											D1 = 0xFF;
											D2 = 0xFF;
											D3 = 0xFF;
											D4 = 0xFF;
										}
										else if (press == 1){D1 = CC_7Seg_Disp_LookupTable[j][i];}
										else if (press == 2){D2 = D1;D1 = CC_7Seg_Disp_LookupTable[j][i];}
										else if (press == 3){D3 = D2;D2 = D1;D1 = CC_7Seg_Disp_LookupTable[j][i];}
										else if (press == 4){D4 = D3;D3 = D2;D2 = D1;D1 = CC_7Seg_Disp_LookupTable[j][i];}
									}
								}
														
							
							GPIOA->DATA = 0x20; // 0010 0000
							GPIOB->DATA = D1;
							for (short j1 = 0; j1 < DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE; j1++);

							GPIOA->DATA = 0x10; // 0001 0000
							GPIOB->DATA = D2;
							for (short j2 = 0; j2 < DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE; j2++);

							GPIOA->DATA = 0x08; // 0000 1000
							GPIOB->DATA = D3;
							for (short j3 = 0; j3 < DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE; j3++);

							GPIOA->DATA = 0x04; // 0000 0100
							GPIOB->DATA = D4;
							for (short j4 = 0; j4 < DELAY_DEBOUNCE / 2; j4++);

						}
							}}
						

	void PE0123_as_Output_Init(void)
					{
					// Step 1: Clock enable on PortE
					SYSCTL->RCGCGPIO |= 0x10;	// 01 0000
					for (j =0; j < 3 ; j++);		// wait at least 3 clock cyles
					// Step 2: APB is selected for PortE by selecting
					// 0x40007000 as Base Address in DATA section
					// Step 3: Disable alternate functionality on PortE
					GPIOE->AFSEL &= ~(0x0F); // Disable for PE0-3 (000 1111)
					// Step 4: Enable digital pin functionaliy on PortE pin 0-4
					GPIOE->DEN |= 0x0F; // Digital enable for PE0-4 (0001 1111)
					// Step 5: Set PortD pin 0-4 as an output pin
					GPIOE->DIR &= ~(0x0F); // PE0-4 as output (0000 0111)
					}
void PortB_as_Output_Init(void)
					{
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
void PortA2345_as_Output_Init(void)
					{
					// Step 1: Clock enable on PortB
					SYSCTL->RCGCGPIO |= 0x01;	// 00 0001
					for (j =0; j < 3 ; j++)		// wait at least 3 clock cyles
					// Step 2: APB is selected for PortA by selecting
					// Step 3: Disable alternate functionality on PortB
					GPIOB->AFSEL &= ~(0x3C); // Disable for PA2345 (0011 1100)
					// Step 4: Enable digital pin functionaliy on PA2345 (0011 1100)
					GPIOB->DEN |= 0x3C; // Digital enable for PA2345 (0011 1100)
					// Step 5: Set PortB pin  as an output pin
					GPIOB->DIR |= 0x3C; // PA as output PA2345 (0011 1100)
					}
void PC4567_as_Input_Init(void)
					{
					// Step 1: Clock enable on PortC
					SYSCTL->RCGCGPIO |= 0x04;	// 00 0100
					for (j =0; j < 3 ; j++)		// wait at least 3 clock cyles
					// Step 2: APB is selected for PortA by selecting
					// Step 3: Disable alternate functionality on PortB
					GPIOB->AFSEL &= ~(0xF0); // Disable for PF4567 (1111 0000)
					// Step 4: Enable digital pin functionaliy on PA2345 (0011 1100)
					GPIOB->DEN |= 0xF0; // Digital enable for PA2345 (0011 1100)
					// Step 5: Set PortB pin  as an output pin
					GPIOB->DIR &= ~(0xF0); // PA as Input PA2345 (0011 1100)
					}
void msDelay(int dValue){
	for (j =0; j < 4000*dValue; j++);
}