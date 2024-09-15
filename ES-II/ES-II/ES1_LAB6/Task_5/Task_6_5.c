//aaaaa
// Clock Gating Control for GPIO
#define SYSCTL_RCGC2_R *((volatile unsigned long *)0x400FE608)
#define SYSCTL_RCGC2_GPIOBCE 0x00000017 // Port A, B, C , E  clock enable 0001 0111

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
#define DEALY_DEBOUNCE SYSTEM_CLOCK_FREQUENCY / 10000

// Global variable declarations for common Anode
unsigned char CC_7Seg_Disp_LookupTable[4][4] = { // col 1	 col2	 col3	 col4
	    {0x06, 0x5B, 0x4F, 0x77}, // row1
    {0x66, 0x6D, 0x7D, 0x7C}, // row2
    {0x07, 0x7F, 0x6F, 0x39}, // row3
    {0xED, 0x3F, 0x76, 0x5E}  // row4
};				 // row 4

	int NUMS[4][4] = { // col 1	 col2	 col3	 col4
	{1,2,3,0},					 // row 1
	{4,5,6,0},					 // row 2
	{7,8,9,0},					 // row 3
	{0,0,0,0}};					 // row 4

	unsigned char display[10]={0xC0,0xF9, 0xA4, 0xB0,0x99, 0x92, 0x82,0xF8, 0x80, 0x90};

unsigned char defualt_7Seg_Val = 0xFF; // Only decimal point turn ON

int main()
{
	int dec = 0;
	char hex = 0x00;
	int press = 0;
	unsigned char D1, D2, D3, D4;
	int d1,d2,d3,d4;
	// Enable the clock for Port B, C, and F
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOBCE;
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
					if (press == 3)
					{
						press = 0;
						D1 = defualt_7Seg_Val;
						D2 = defualt_7Seg_Val;
						D3 = defualt_7Seg_Val;
						D4 = defualt_7Seg_Val;
						d1=0;
						d2=0;
						d3=0;
						d4=0;
					}
					else if (press == 1)
					{
						D1 = CC_7Seg_Disp_LookupTable[j][i];
						d1=NUMS[j][i];
					}
					else if (press == 2)
					{
						D2 = D1;
						D1 = CC_7Seg_Disp_LookupTable[j][i];
						d2=NUMS[j][i];
					}
				}
			}
			for (short iI = 0; iI < DEALY_DEBOUNCE; iI++)
				; // to produce the delay
		}
		
		if (press==2){
		d3=(d1+d2)%10;
		d4=(d1+d2)/10;
		D3=display[d3];
		D4=display[d4];}
		GPIO_PORTA_DATA_R &= 0x20; // 0010 0000
		GPIO_PORTB_DATA_R = 0xff;
		for (short j1 = 0; j1 < DEALY_DEBOUNCE + DEALY_DEBOUNCE + DEALY_DEBOUNCE; j1++)
			;
		GPIO_PORTA_DATA_R = 0x10; // 0010 0000
		GPIO_PORTB_DATA_R = D2;
		for (short j2 = 0; j2 < DEALY_DEBOUNCE + DEALY_DEBOUNCE + DEALY_DEBOUNCE; j2++)
			;
		GPIO_PORTA_DATA_R = 0x08; // 0010 0000
		GPIO_PORTB_DATA_R = D3;

		for (short j3 = 0; j3 < DEALY_DEBOUNCE + DEALY_DEBOUNCE + DEALY_DEBOUNCE + DEALY_DEBOUNCE; j3++)
			;
		GPIO_PORTA_DATA_R = 0x04; // 0010 0000
		GPIO_PORTB_DATA_R = D4;
		for (short j4 = 0; j4 < DEALY_DEBOUNCE / 2; j4++)
			;
	}
}