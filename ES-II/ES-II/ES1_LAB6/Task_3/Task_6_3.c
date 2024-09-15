a/* Lab_6_Task_3.uvprojx
------------------------------------------------------------------------------------------
This C language program displays keywords pressed by the user (fron 4x4 keypad) on 4 
digit 7 segment display using TIVA C Series LaunchPad Evaluation Kit by Texas Instruments.
-------------------------------------------------------------------------------------------
Written by Alishba Kiyani on 23-March-2024.
Roll no: 2022-MC-75
Compiler and IDE used: Keil uVision 5.39 */

// Clock Gating Control for GPIO
#define SYSCTL_RCGC2_R *((volatile unsigned long *)0x400FE608)
#define SYSCTL_RCGC2_GPIOBCE 0x00000017 // Port A, B, C , E  clock enable 0001 0111
// Clock Gating Control for GPIO
#define SYSCTL_RCGC2_R *((volatile unsigned long *)0x400FE608)
#define SYSCTL_RCGC2_GPIOA 0x0000001 // Port A clock enable 0000 0001

// GPIO Port F Configuration Registers (for on-board LED)
#define GPIO_PORTA_DEN_R *((volatile unsigned long *)0x4000451C)
#define GPIO_PORTA_DIR_R *((volatile unsigned long *)0x40004400)
#define GPIO_PORTA_DATA_R *((volatile unsigned long *)0x400040F0) // 0000 1111 0000

// GPIO Port B Configuration Registers (7-Segment Display)
#define GPIO_PORTB_DATA_R *((volatile unsigned long *)0x400053FC) // 0011 1111 1100
#define GPIO_PORTB_DIR_R *((volatile unsigned long *)0x40005400)
#define GPIO_PORTB_DEN_R *((volatile unsigned long *)0x4000551C)
#define GPIO_PORTB_Pin0_7 0xFF

// GPIO Port C Configuration Registers (4x4 Keypad Columns)
#define GPIO_PORTC_DATA_R *((volatile unsigned long *)0x400063C0)
#define GPIO_PORTC_DIR_R *((volatile unsigned long *)0x40006400)
#define GPIO_PORTC_DEN_R *((volatile unsigned long *)0x4000651C)
#define GPIO_PORTC_PDR_R *((volatile unsigned long *)0x40006514)
#define GPIO_PORTC_Pin4_7 0xF0
#define Key_pad_Col_1 0x10

// GPIO Port E Configuration Registers (4x4 Keypad Rows)
#define GPIO_PORTE_DATA_R *((volatile unsigned long *)0x4002403C)
#define GPIO_PORTE_DIR_R *((volatile unsigned long *)0x40024400)
#define GPIO_PORTE_DEN_R *((volatile unsigned long *)0x4002451C)
#define GPIO_PORTE_Pin0_3 0x0F
#define Key_pad_Row_1 0x01

// Default clock frequency
#define SYSTEM_CLOCK_FREQUENCY 16000000
#define DELAY_DEBOUNCE SYSTEM_CLOCK_FREQUENCY / 10000

// Global variable declarations for common Cathode
unsigned char CC_7Seg_Disp_LookupTable[4][4] = { // col 1	 col2	 col3	 col4
	{0x79, 0x24, 0x30, 0x08},					 // row 1
	{0x19, 0x12, 0x02, 0x03},					 // row 2
	{0x78, 0x00, 0x10, 0x46},					 // row 3
	{0x92, 0x40, 0x09, 0x21}};					 // row 4

unsigned char defualt_7Seg_Val = 0x7F; // Only decimal point turn ON

int main()
{
	int press = 0;
	unsigned char D1, D2, D3, D4;
	// Enable the clock for Port B, C, and F
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	while ((SYSCTL_RCGC2_R & SYSCTL_RCGC2_GPIOBCE) == 0)
		; // Wait untill clock is enabled

	// Configure PortB as Digital Output (7-Segment Display)
	GPIO_PORTB_DEN_R |= GPIO_PORTB_Pin0_7;
	GPIO_PORTB_DIR_R |= GPIO_PORTB_Pin0_7;

	// Configure PortC as Digital Input (4x4 Keypad Columns)
	GPIO_PORTC_DEN_R |= GPIO_PORTC_Pin4_7;
	GPIO_PORTC_DIR_R &= ~GPIO_PORTC_Pin4_7; // for declaring the Port pins as input as Column
	GPIO_PORTC_PDR_R |= GPIO_PORTC_Pin4_7;

	// Configure PortE as Digital Output (4x4 Keypad Rows)
	GPIO_PORTE_DEN_R |= GPIO_PORTE_Pin0_3;
	GPIO_PORTE_DIR_R |= GPIO_PORTE_Pin0_3;

	// Configure PortA as Digital Output 7 SEGMEMT
	GPIO_PORTA_DEN_R |= 0x3C; // 0011 1100
	GPIO_PORTA_DIR_R |= 0x3C;

	// Display only decimal
	D1 = defualt_7Seg_Val;
	D2 = defualt_7Seg_Val;
	D3 = defualt_7Seg_Val;
	D4 = defualt_7Seg_Val;

	while (1)
	{

		for (short j = 0; j < 4; j++)
		{
			// Apply 3.3 V (Digital Output) on Keypad Columns one-by-one
			GPIO_PORTE_DATA_R = Key_pad_Row_1 << j;
			for (short i = 0; i < 4; i++)
			{
				// Digital read for any key pressed on Keypad between 0 to 9
				if ((GPIO_PORTC_DATA_R & GPIO_PORTC_Pin4_7) == Key_pad_Col_1 << i)
				{
					while ((GPIO_PORTC_DATA_R & GPIO_PORTC_Pin4_7) == Key_pad_Col_1 << i)
					{
					}
					press++;
					if (press == 5)
					{
						press = 0;
						D1 = defualt_7Seg_Val;
						D2 = defualt_7Seg_Val;
						D3 = defualt_7Seg_Val;
						D4 = defualt_7Seg_Val;
					}
					else if (press == 1)
					{
						D1 = CC_7Seg_Disp_LookupTable[j][i];
					}
					else if (press == 2)
					{
						D2 = D1;
						D1 = CC_7Seg_Disp_LookupTable[j][i];
					}
                    else if (press == 3)
					{
						D3 = D2;
						D2 = D1;
						D1 = CC_7Seg_Disp_LookupTable[j][i];
					}
                    else if (press == 4)
					{
						D4 = D3;
						D3 = D2;
						D2 = D1;
						D1 = CC_7Seg_Disp_LookupTable[j][i];
					}
				}
			}
			for (short iI = 0; iI < DELAY_DEBOUNCE; iI++); // to produce the delay
		}
		
		GPIO_PORTA_DATA_R = 0x20; // 0010 0000
		GPIO_PORTB_DATA_R = D1;
		for (short j1 = 0; j1 < DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE; j1++);

		GPIO_PORTA_DATA_R = 0x10; // 0001 0000
		GPIO_PORTB_DATA_R = D2;
		for (short j2 = 0; j2 < DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE; j2++);

		GPIO_PORTA_DATA_R = 0x08; // 0000 1000
		GPIO_PORTB_DATA_R = D3;
		for (short j3 = 0; j3 < DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE; j3++);

		GPIO_PORTA_DATA_R = 0x04; // 0000 0100
		GPIO_PORTB_DATA_R = D4;
		for (short j4 = 0; j4 < DELAY_DEBOUNCE / 2; j4++);

	}
}