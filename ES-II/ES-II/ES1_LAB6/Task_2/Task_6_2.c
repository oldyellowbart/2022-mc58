/* Lab_6_Task_2.uvprojx
-------------------------------------------------------------------------------
This C language program was written to Interfaces a 4x4 Keypad with TIVA C Series 
GPIO ports and program it to turn ON on-board green LED if anydigit (0-9) is pressed,
turn ON on-board blue LED if any alphabet (A-D) is pressed, and turn ON on-board
red LED if any other key is pressed
-------------------------------------------------------------------------------
Written by Alishba Kiyani on 23-March-2024.
Roll no: 2022-MC-75
Compiler and IDE used: Keil uVision 5.39 */

// Control the gating clock for Port B,C and E
#define SYSCTL_RCGC_R *(( volatile unsigned long *) 0x400FE608)
#define SYSCTL_RCGC_GPIOBCE 0x34 //Enable CLock for Port B,C and E

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

// Port F Configuration Registers for 4x4 KeyPad in Rows
#define GPIO_PORTF_AFSEL_R *(( volatile unsigned long *) 0x40025420)
#define GPIO_PORTF_DIR_R *(( volatile unsigned long *) 0x40025400)
#define GPIO_PORTF_DEN_R *(( volatile unsigned long *) 0x4002551C)
#define GPIO_PORTF_DATA_R *(( volatile unsigned long *) 0x40025038) 
	
// Configuration Registers Values 

// for Port C
#define GPIO_PORTC_AFSEL_R_DS 0x00 //Disable functionality of Port C
#define GPIO_PORTC_PINS_4_7 0xF0 //Mask for Port C Pin 4 to 7
#define Key_Pad_4x4_COLUMN_1 0x10

// for Port E
#define GPIO_PORTE_AFSEL_R_DS 0x00 //Disable functionality of Port E
#define GPIO_PORTE_PINS_0_3 0x0F //Mask for Port E Pin 0 to 3
#define Key_Pad_4x4_ROW_1 0x01

// for Port F
#define GPIO_PORTF_AFSEL_R_DS 0x00 //Disable functionality of Port F
#define GPIO_PORTF_PINS_1_3 0x0E //Mask for Port F
#define GPIO_PORTF_PIN_1 0x02 //Mask for Port F PF1
#define GPIO_PORTF_PIN_2 0x04 //Mask for Port F PF2
#define GPIO_PORTF_PIN_3 0x08 //Mask for Port F PF3


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
unsigned char Numbers[10]={0xC0,0xF9, 0xA4, 0xB0,0x99, 0x92, 0x82,0xF8, 0x80, 0x90};
unsigned char Characters[4]={0x88, 0x83, 0xC6, 0xA1};

//User main Program
int main()
{
    unsigned char Input = 0;

	SYSCTL_RCGC_R |= SYSCTL_RCGC_GPIOBCE;
	
	while((SYSCTL_RCGC_R & SYSCTL_RCGC_GPIOBCE) == 0);
	
	GPIO_PORTC_AFSEL_R &= ~GPIO_PORTC_AFSEL_R_DS;
	GPIO_PORTE_AFSEL_R &= ~GPIO_PORTE_AFSEL_R_DS;
	GPIO_PORTF_AFSEL_R &= ~GPIO_PORTF_AFSEL_R_DS;
	
	GPIO_PORTC_DEN_R |= GPIO_PORTC_PINS_4_7;   
	GPIO_PORTC_DIR_R &= ~GPIO_PORTC_PINS_4_7;
	GPIO_PORTC_PD_R |= GPIO_PORTC_PINS_4_7;

    GPIO_PORTE_DEN_R |= GPIO_PORTE_PINS_0_3;   
	GPIO_PORTE_DIR_R |= GPIO_PORTE_PINS_0_3;    

    GPIO_PORTF_DEN_R |= GPIO_PORTF_PINS_1_3;   
	GPIO_PORTF_DIR_R |= GPIO_PORTF_PINS_1_3; 

	
	while(1)
	{
		for(short j = 0; j < 4; j++)
		{
			GPIO_PORTE_DATA_R = Key_Pad_4x4_ROW_1 << j;
			for(short i = 0; i < 4; i++)
			{
				if((GPIO_PORTC_DATA_R & GPIO_PORTC_PINS_4_7) == Key_Pad_4x4_COLUMN_1 << i)
				{
					Input = CC_7SEG_DISPLAY_LOOKUP_TABLE[j][i];
                    if (Input == 0xC0 || Input == 0xF9 || Input == 0xA4 || Input == 0xB0 || Input == 0x99 || Input == 0x92 || Input == 0x82 || Input == 0xF8 || Input == 0x80 || Input == 0x90)
                    {
                        GPIO_PORTF_DATA_R &= ~GPIO_PORTF_PIN_1;
                        GPIO_PORTF_DATA_R &= ~GPIO_PORTF_PIN_2;
                        GPIO_PORTF_DATA_R |= GPIO_PORTF_PIN_3;

                    }

                    else if(Input == 0x88 || Input == 0x83 || Input == 0xC6 || Input == 0xA1)
                    {
                        GPIO_PORTF_DATA_R |= GPIO_PORTF_PIN_2;
                        GPIO_PORTF_DATA_R &= ~GPIO_PORTF_PIN_1;
                        GPIO_PORTF_DATA_R &= ~GPIO_PORTF_PIN_3;


                    }

                    else
                    {
                        GPIO_PORTF_DATA_R |= GPIO_PORTF_PIN_1;
                        GPIO_PORTF_DATA_R &= ~GPIO_PORTF_PIN_2;
                        GPIO_PORTF_DATA_R &= ~GPIO_PORTF_PIN_3;


                    }
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