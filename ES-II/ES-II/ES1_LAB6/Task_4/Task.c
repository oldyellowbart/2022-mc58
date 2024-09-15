/* Lab_6_Task_4.uvprojx
-----------------------------------------------------------------------
This C language program implements Digital Password Verification entered by the user 
(from 4x4 Keypad) using TIVA C Series LaunchPad Evaluation Kit by Texas Instrument.
-----------------------------------------------------------------------
Written by Alishba Kiyani on 23-March-2024.
Roll no: 2022-MC-75
Compiler and IDE used: Keil uVision 5.39 */

// Clock Gating Control for GPIO
#define SYSCTL_RCGC2_R *((volatile unsigned long *)0x400FE608)
#define SYSCTL_RCGC2_GPIOBCE 0x37 // Port A, B, C, E & F clock enable 0001 0111

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

// GPIO Port F Configuration Registers (4x4 Keypad Rows)
#define GPIO_PORTF_DATA_R *((volatile unsigned long *)0x40025028)
#define GPIO_PORTF_DIR_R *((volatile unsigned long *)0x40025400)
#define GPIO_PORTF_DEN_R *((volatile unsigned long *)0x4002551C)
#define GPIO_PORTF_Pins 0x0A    // Mask for PF1 & PF3
#define GPIO_PORTF_Pin1 0x02    // Mask for pin 1 (Red LED)
#define GPIO_PORTF_Pin3 0x08   //Mask for pin 3 (Green LED)

// Default clock frequency
#define SYSTEM_CLOCK_FREQUENCY 16000000
#define DELAY_DEBOUNCE SYSTEM_CLOCK_FREQUENCY / 10000

// Global variable declarations for common Anode
unsigned char CC_7Seg_Disp_LookupTable[4][4] = 
{ // col 1	 col2	 col3	 col4
		{0xF9, 0xA4, 0xB0, 0xBF},					 // row 1
		{0x99, 0x92, 0x82, 0xBF},					 // row 2
		{0xF8, 0x80, 0x90, 0xBF},					 // row 3
		{0xBF, 0xC0, 0xBF, 0xBF}};					 // row 4

int display[4][4] = 
{
    {1,2,3,0},
    {4,5,6,0},
    {7,8,9,0},
    {0,0,0,0}
};

unsigned char defualt_7Seg_Val = 0x7F; // Only decimal point turn ON
unsigned int Password = 9211;	
unsigned int User_Password;

int main()
{
	int press = 0;
    int d1,d2,d3,d4;
	unsigned char D1, D2, D3, D4;

	// Enable the clock for Port B, C, and F
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOBCE;

	while ((SYSCTL_RCGC2_R & SYSCTL_RCGC2_GPIOBCE) == 0);  // Wait untill clock is enabled

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

    // Configure Port F as Digital Output
	GPIO_PORTF_DEN_R |= GPIO_PORTF_Pins;
	GPIO_PORTF_DIR_R |= GPIO_PORTF_Pins;

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
                    d1 = display[j][i];
                }
                else if (press == 2)
                {
                    D2 = CC_7Seg_Disp_LookupTable[j][i];
                    d2 = display[j][i];
                }
                else if (press == 3)
                {
                    D3 = CC_7Seg_Disp_LookupTable[j][i];
                    d3 = display[j][i];
                }
                else if (press == 4)
                {
                    D4 = CC_7Seg_Disp_LookupTable[j][i];
                    d4 = display[j][i];

                    d1 *= 1000;
                    d2 *= 100;
                    d3 *= 10;
                    User_Password = (d1 + d2 + d3 + d4);

                    if (Password == User_Password)
                    {
                        GPIO_PORTF_DATA_R |= GPIO_PORTF_Pin3;
                    }
                    else
                    {
                        GPIO_PORTF_DATA_R |= GPIO_PORTF_Pin1;
                    }
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

		GPIO_PORTA_DATA_R = 0x04; // 0000 0100
		GPIO_PORTB_DATA_R = D3;
		for (short j3 = 0; j3 < DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE + DELAY_DEBOUNCE; j3++);

		GPIO_PORTA_DATA_R = 0x08; // 0000 1000
		GPIO_PORTB_DATA_R = D4;
		for (short j4 = 0; j4 < DELAY_DEBOUNCE / 2; j4++);

	}
}
