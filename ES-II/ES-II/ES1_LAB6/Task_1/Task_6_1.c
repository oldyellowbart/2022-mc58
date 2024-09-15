/* Lab_6_Task_1.uvprojx
-------------------------------------------------------------------------------------------------
Interface a 4x4 Keypad (GPIO PortC 4-7, PortE 0-3) and a single-digit common anode 7-segment display
(GPIO Port B Pin 0-7) and program it to display the keypad key pressed on 7-Segement. Note for B and D
display b and d respectively and for * and # display 5. and H respectively.
-------------------------------------------------------------------------------------------------
Written by Alishba Kiyani on 23-March-2024.
Roll no: 2022-MC-75
Compiler and IDE used: Keil uVision 5.39 */

// Control the gating clock for Port B,C and E
#define SYSCTL_RCGC_R *(( volatile unsigned long *) 0x400FE608)
#define SYSCTL_RCGC_GPIOBCE 0x16 //Enable CLock for Port B,C and E

// Port B Configuration Registers
#define GPIO_PORTB_AFSEL_R *(( volatile unsigned long *) 0x40005420)
#define GPIO_PORTB_DIR_R *(( volatile unsigned long *) 0x40005400)
#define GPIO_PORTB_DEN_R *(( volatile unsigned long *) 0x4000551C)
#define GPIO_PORTB_DATA_R *(( volatile unsigned long *) 0x400053FC)
	
// Port C Configuration Registers for 4x4 KeyPad in Columns
#define GPIO_PORTC_AFSEL_R *(( volatile unsigned long *) 0x40006420)
#define GPIO_PORTC_DIR_R *(( volatile unsigned long *) 0x40006400)
#define GPIO_PORTC_DEN_R *(( volatile unsigned long *) 0x4000651C)
#define GPIO_PORTC_DATA_R *(( volatile unsigned long *) 0x400063C0) 
#define GPIO_PORTC_PD_R *(( volatile unsigned long *) 0x40006514) 
	
// Port E Configuration Registers for 4x4 KeyPad in Rows
#define GPIO_PORTE_AFSEL_R *(( volatile unsigned long *) 0x40024420)
#define GPIO_PORTE_DIR_R *(( volatile unsigned long *) 0x40024400)
#define GPIO_PORTE_DEN_R *(( volatile unsigned long *) 0x4002451C)
#define GPIO_PORTE_DATA_R *(( volatile unsigned long *) 0x4002403C) 
	
// Configuration Registers Values 
// for Port B
#define GPIO_PORTB_AFSEL_R_DS 0x00 //Disable functionality of Port B
#define GPIO_PORTB_PINS_0_7 0xFF //Mask for Port B Pin 0 to 7

// for Port C
#define GPIO_PORTC_AFSEL_R_DS 0x00 //Disable functionality of Port C
#define GPIO_PORTC_PINS_4_7 0xF0 //Mask for Port C Pin 4 to 7
#define Key_Pad_4x4_COLUMN_1 0x10

// for Port E
#define GPIO_PORTE_AFSEL_R_DS 0x00 //Disable functionality of Port E
#define GPIO_PORTE_PINS_0_3 0x0F //Mask for Port E Pin 0 to 3
#define Key_Pad_4x4_ROW_1 0x01

// Default Clock Frequency
#define SYSTEM_CLOCK_FREQUENCY 16000000
#define DELAY_DEBOUNCE SYSTEM_CLOCK_FREQUENCY/1000

// Function Prototype for Delay time
void Delay( volatile unsigned int delay );

// Global Variables Declarations
unsigned char CC_7SEG_DISPLAY_LOOKUP_TABLE[4][4] = { // col1  col2  col3  col 4
                                                      {0xF9, 0xA4, 0xB0, 0x88},  // Row 1
                                                      {0x99, 0x92, 0x82, 0x83},  // Row 2
                                                      {0xF8, 0x80, 0x90, 0xC6},  // Row 3
                                                      {0x12, 0xC0, 0x89, 0xA1}}; // Row 4
unsigned char Default_Value = 0x7F;																											

//User main Program
int main()
{
	SYSCTL_RCGC_R |= SYSCTL_RCGC_GPIOBCE;
	
	while((SYSCTL_RCGC_R & SYSCTL_RCGC_GPIOBCE) == 0);
	
	GPIO_PORTB_AFSEL_R &= ~GPIO_PORTB_AFSEL_R_DS;
	GPIO_PORTC_AFSEL_R &= ~GPIO_PORTC_AFSEL_R_DS;
	GPIO_PORTE_AFSEL_R &= ~GPIO_PORTE_AFSEL_R_DS;
	
	GPIO_PORTB_DEN_R |= GPIO_PORTB_PINS_0_7;
	GPIO_PORTB_DIR_R |= GPIO_PORTB_PINS_0_7; 
	
	GPIO_PORTC_DEN_R |= GPIO_PORTC_PINS_4_7;   
	GPIO_PORTC_DIR_R &= ~GPIO_PORTC_PINS_4_7;
	GPIO_PORTC_PD_R |= GPIO_PORTC_PINS_4_7;

  GPIO_PORTE_DEN_R |= GPIO_PORTE_PINS_0_3;   
	GPIO_PORTE_DIR_R |= GPIO_PORTE_PINS_0_3;    

  GPIO_PORTB_DATA_R = Default_Value;	
	
	while(1)
	{
		for(short j = 0; j < 4; j++)
		{
			GPIO_PORTE_DATA_R = Key_Pad_4x4_ROW_1 << j;
			for(short i = 0; i < 4; i++)
			{
				if((GPIO_PORTC_DATA_R & GPIO_PORTC_PINS_4_7) == Key_Pad_4x4_COLUMN_1 << i)
				{
					GPIO_PORTB_DATA_R = CC_7SEG_DISPLAY_LOOKUP_TABLE[j][i];
				}
			}
		}
		Delay(DELAY_DEBOUNCE);
	}
}

void Delay( volatile unsigned int delay )
{
	volatile unsigned int i, j;
	for(i = 0; i < delay; i++)
	{
		for(j = 0; j < 12; j++);
	}
}