/* Lab_06_Task_05_Single_Digit_Adder.uvprojx
-----------------------------------------------------------------------

Note: Also no external buffer circuit is used for driving individual 
digits of the 7-segment display. If external buffer circuit is used 
then digit turning on logic will invert.
-----------------------------------------------------------------------
Board used: TIVA C Series LaunchPad Evaluation Kit by Texas Instruments
Written by Shujat Ali (engrshujatali@gmail.com) on 03-March-2024.
Compiler and IDE used: Keil uVision 5.34 */

// Configuration Registers Addresses
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608)
// PortA on APB = 0x40004000 (Base Address)---------------------------------
#define GPIO_PORTA_AFSEL_R *(( volatile unsigned long *)0x40004420)
#define GPIO_PORTA_DEN_R *(( volatile unsigned long *)0x4000451C)
#define GPIO_PORTA_DIR_R *(( volatile unsigned long *)0x40004400)
#define GPIO_PORTA_DATA_R *(( volatile unsigned long *)0x400040F0)// 0000 1111 0000
// PortB on APB = 0x40005000 (Base Address)---------------------------------
#define GPIO_PORTB_AFSEL_R *(( volatile unsigned long *)0x40005420)
#define GPIO_PORTB_DEN_R *(( volatile unsigned long *)0x4000551C)
#define GPIO_PORTB_DIR_R *(( volatile unsigned long *)0x40005400)
#define GPIO_PORTB_DATA_R *(( volatile unsigned long *)0x400053FC)// 0011 1111 1100
// PortC on APB = 0x40006000 (Base Address)---------------------------------
#define GPIO_PORTC_AFSEL_R *(( volatile unsigned long *)0x40006420)
#define GPIO_PORTC_DEN_R *(( volatile unsigned long *)0x4000651C)
#define GPIO_PORTC_DIR_R *(( volatile unsigned long *)0x40006400)
#define GPIO_PORTC_DATA_R *(( volatile unsigned long *)0x400063C0)// 0011 1100 0000
// PortE on APB = 0x40024000 (Base Address)---------------------------------
#define GPIO_PORTE_AFSEL_R *(( volatile unsigned long *)0x40024420)
#define GPIO_PORTE_DEN_R *(( volatile unsigned long *)0x4002451C)
#define GPIO_PORTE_DIR_R *(( volatile unsigned long *)0x40024400)
#define GPIO_PORTE_PDR_R *(( volatile unsigned long *)0x40024514)
#define GPIO_PORTE_DATA_R *(( volatile unsigned long *)0x4002403C)// 0000 0011 1100

// Configuration Registers Values
// Configuration Registers Values (PortA)---------------------------------
#define GPIO_PORTA_CLOCK_EN 0x01 // Port A clock gating control (0000 0001)
#define GPIO_PORTA_MASK 0x3C // Port A mask (0011 1100)
// Configuration Registers Values (PortB)---------------------------------
#define GPIO_PORTB_CLOCK_EN 0x02 // Port B clock gating control (0000 0010)
#define GPIO_PORTB_MASK 0xFF // Port B mask (1111 1111)
// Configuration Registers Values (PortC)---------------------------------
#define GPIO_PORTC_CLOCK_EN 0x04 // Port C clock gating control (0000 0100)
#define GPIO_PORTC_MASK 0xF0 // Port C mask (1111 0000)
// Configuration Registers Values (PortE)---------------------------------
#define GPIO_PORTE_CLOCK_EN 0x10 // Port E clock gating control (0001 0000)
#define GPIO_PORTE_MASK 0x0F // Port E mask (0000 1111)

// Constant Values
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE SYSTEM_CLOCK_FREQUENCY/16 // Counter value to generate 0.5 sec delay

volatile unsigned long i, j = 0; // variable required for delay loop
volatile unsigned char display_values[4] = {0xFF, 0xFF, 0xFF, 0xFF};
volatile unsigned char Lookup_Table_CA[4][4] = {// col1  col2  col3  col4
																	{0xF9, 0xA4, 0xB0, 0x88},// row1
																	{0x99, 0x92, 0x82, 0x83},// row2
																	{0xF8, 0x80, 0x90, 0xC6},// row3
																	{0x06, 0xC0, 0x0E, 0xA1},// row4
																	};
volatile unsigned short Lookup_Table_val[4][4] = {// col1  col2  col3  col4
																	{1, 2, 3, 10},// row1
																	{4, 5, 6, 11},// row2
																	{7, 8, 9, 12},// row3
																	{15, 0, 14, 13},// row4
																	};
unsigned char CA_Lookup_Table[16] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,0x48,0x03,0x27,0x21,0x06,0x0E};

void delay(void);
void PortA_Digital_Output_Init(void);
void PortB_Digital_Output_Init(void);
void PortC_Digital_Output_Init(void);
void PortE_Digital_Input_Init(void);
void DisplayCounter(volatile unsigned char value[4]);
void keypadScan(volatile short *row, volatile short *col);

int main()
{	// User main program
	volatile unsigned short op1 = 0, op2 = 0, sum = 0;
	volatile short row=0, col=0;
	volatile unsigned short digitNum = 0;
	
	// GPIO Configuration (Step 1-5)
	PortA_Digital_Output_Init(); // Function for GPIO PortA Configurations
	PortB_Digital_Output_Init(); // Function for GPIO PortB Configurations
	PortC_Digital_Output_Init(); // Function for GPIO PortC Configurations
	PortE_Digital_Input_Init(); // Function for GPIO PortE Configurations
	
	while(1){
		for(i=0; i<15; i++){
			DisplayCounter(display_values);
		}
		keypadScan(&row,&col);
		if(row!=-1){
			display_values[digitNum] = Lookup_Table_CA[row][col];
			if(digitNum==0)
				op1 = Lookup_Table_val[row][col];
			else
				op2 = Lookup_Table_val[row][col];
			
			if(digitNum++>1){ // Reset
				digitNum=0;
				display_values[0] = 0xFF;
				display_values[1] = 0xFF;
				display_values[2] = 0xFF;
				display_values[3] = 0xFF;
				op1=0; op2=0; sum=0;
			}
		}
		if(digitNum>1){
			sum = op1 + op2;
			display_values[2]=CA_Lookup_Table[sum/16];
			display_values[3]=CA_Lookup_Table[sum%16];
		}4
	}
}

void delay(void){
	for (j =0; j < DELAY_VALUE/100; j++); // 5 mSec delay
}
void PortA_Digital_Output_Init(void){
	// PA2-5 interfaced to DIG 1-4 of 7-segment display
	
	// Step 1: Clock enable
	RCGC_GPIO_R |= GPIO_PORTA_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected by selecting Base Address
		
	// Step 3: Disable alternate functionality
	GPIO_PORTA_AFSEL_R &= ~GPIO_PORTA_MASK;
	
	// Step 4: Enable digital pin functionaliy
	GPIO_PORTA_DEN_R |= GPIO_PORTA_MASK; // Digital enable 
	
	// Step 5: Enable digital pins as an output
	GPIO_PORTA_DIR_R |= GPIO_PORTA_MASK;
}
void PortB_Digital_Output_Init(void){
	// PB0-7 interfaced to A-G, and DP of 7-segment display
	
	// Step 1: Clock enable
	RCGC_GPIO_R |= GPIO_PORTB_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected by selecting Base Address
	
	// Step 3: Disable alternate functionality
	GPIO_PORTB_AFSEL_R &= ~GPIO_PORTB_MASK;
	
	// Step 4: Enable digital pin functionaliy
	GPIO_PORTB_DEN_R |= GPIO_PORTB_MASK; // Digital enable 
	
	// Step 5: Enable digital pins as an output
	GPIO_PORTB_DIR_R |= GPIO_PORTB_MASK;
}
void PortC_Digital_Output_Init(void){
	// PC4-7 interfaced to C1-C4 of 4x4 Keypad
	
	// Step 1: Clock enable
	RCGC_GPIO_R |= GPIO_PORTC_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected by selecting Base Address
	
	// Step 3: Disable alternate functionality
	GPIO_PORTC_AFSEL_R &= ~GPIO_PORTC_MASK;
	
	// Step 4: Enable digital pin functionaliy
	GPIO_PORTC_DEN_R |= GPIO_PORTC_MASK; // Digital enable 
	
	// Step 5: Enable digital pins as an output
	GPIO_PORTC_DIR_R |= GPIO_PORTC_MASK;
}
void PortE_Digital_Input_Init(void){
	// PE0-3 interfaced to R1-R4 of 4x4 Keypad
	
	// Step 1: Clock enable
	RCGC_GPIO_R |= GPIO_PORTE_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected by selecting Base Address
	
	// Step 3: Disable alternate functionality
	GPIO_PORTE_AFSEL_R &= ~GPIO_PORTE_MASK;
	
	// Step 4: Enable digital pin functionaliy
	GPIO_PORTE_DEN_R |= GPIO_PORTE_MASK; // Digital enable 
	
	// Step 5: Enable digital pins as an input
	GPIO_PORTE_DIR_R &= ~GPIO_PORTE_MASK;
	
	// Step 6: Enable weak internal pull-down resistor
	GPIO_PORTE_PDR_R |= GPIO_PORTE_MASK;
}
void DisplayCounter(volatile unsigned char display_values[4]){
	volatile short k;
	for(k=5; k>=2; k--){
		GPIO_PORTA_DATA_R |= (1<<k);	// Activate the corresponding digit of 7-segment display
		GPIO_PORTB_DATA_R = display_values[k-2];
		delay();
		GPIO_PORTA_DATA_R &= ~(1<<k);	// Deactivate the corresponding digit of 7-segment display
	}
}

void keypadScan(volatile short *row, volatile short *col){
	*row=-1; // Default value
	*col=-1;
	short c, e;
	for(c = 0; c < 4; c++){
		// Apply 3.3 V (digital output) on keypad columns one-by-one
		GPIO_PORTC_DATA_R |= 1<<(c+4);
		
		for(e = 0; e < 4; e++){
			// Digital read on keypad rows one-by-one
			if(GPIO_PORTE_DATA_R&(1<<e)){
				*row=e;
				*col=c;
				break;
			}
		}
		GPIO_PORTC_DATA_R &= ~(1<<(c+4));
	}
}
