/* Lab_Evaluation_1_Task_2_Binar_Counter.uvprojx
-----------------------------------------------------------------------
This C language program implements an 8-bit binary counter using 8 
external LEDs and 2 pushbuttons (external Pull-down) interfaced at GPIO 
Port B (PB0-PB7) and Port E (PE0-PE1) respectively. LEDs are used to 
display the binary value of the number of pushbutton intefaced at PE0 
presses. Use pushbutton interfaced at PE1 to decrement the counter.Also
the counter value overflows to zero on reaching maximum value.
-----------------------------------------------------------------------
Board used: TIVA C Series LaunchPad Evaluation Kit by Texas Instruments
Written by Azmeer Ali (azmeerali2253@gmail.com) on 29-March-2024.
Compiler and IDE used: Keil uVision 5.39 */

// Configuration Registers
// PortB on APB = 0x40005000 (Base Address)
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608)
#define GPIO_PORTB_AFSEL_R *(( volatile unsigned long *)0x40005420)
#define GPIO_PORTB_DEN_R *(( volatile unsigned long *)0x4000551C)
#define GPIO_PORTB_DIR_R *(( volatile unsigned long *)0x40005400)
#define GPIO_PORTB_DATA_R *(( volatile unsigned long *)0x400053FC)// 0011 1111 1100
// PortE on APB = 0x40024000 (Base Address)
#define GPIO_PORTE_AFSEL_R *(( volatile unsigned long *)0x40024420)
#define GPIO_PORTE_DEN_R *(( volatile unsigned long *)0x4002451C)
#define GPIO_PORTE_DIR_R *(( volatile unsigned long *)0x40024400)
#define GPIO_PORTE_DATA_R *(( volatile unsigned long *)0x4002400C)// 0000 0000 1100

// Configuration Registers Values (PortB)
#define GPIO_PORTB_CLOCK_EN 0x02 // Port B clock gating control (0000 0010)
#define GPIO_PORTB_AFSEL_DS 0xFF // Port B alternate function disable
#define GPIO_PORTB_EN 0xFF  // Port B digital enable (1111 1111)
#define GPIO_PORTB_OP 0xFF // Port B as digital output (1111 1111)

// Configuration Registers Values (PortE)
#define GPIO_PORTE_CLOCK_EN 0x10 // Port E clock gating control (0001 0000)
#define GPIO_PORTE_AFSEL_DS 0x03 // Port E pin 0-1 alternate function disable
#define GPIO_PORTE_PIN01_EN 0x03  // Port E pin 0-1 digital enable (0000 0011)
#define GPIO_PORTE_PIN01_IP 0x03 // Port E pin 0-1 as digital input (0000 0011)
#define GPIO_PORTE_PIN0 0x01 // Mask for pin 0 write operation (0000 0001)
#define GPIO_PORTE_PIN1 0x02 // Mask for pin 0 write operation (0000 0010)

// Constant Values
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE SYSTEM_CLOCK_FREQUENCY/16 // Counter value to generate 0.5 sec delay

unsigned volatile long j; // variable required for delay loop
volatile long counter = 0; // to hold the value of pushbutton presses
volatile int flag1 = 1;
volatile int flag2 = 1;

// function declarations
void PortB_Digital_Output_Init(void);
void PE0_Digital_Input_Init(void);
	
int main()
{
	// User main program
	
	// GPIO Configuration (Step 1-5)
	PortB_Digital_Output_Init(); // Function for GPIO PortB Configurations
	PE0_Digital_Input_Init(); // Function for GPIO PortE Configurations
	
	while(1){
		
		if((GPIO_PORTE_DATA_R == GPIO_PORTE_PIN0) && flag1)
			{
				counter++;
				flag1 = 0; // clear the flag
			}
		// delay loop to generate 50 mSec delay (switch debouncing)
		// (i.e. 1 empty loop iteration = 0.5 usec)
		for (j =0; j< DELAY_VALUE/10 ; j++);
		
		if(GPIO_PORTE_DATA_R != GPIO_PORTE_PIN0)
			{	
				flag1 = 1; // set the flag
			}
		if((GPIO_PORTE_DATA_R == GPIO_PORTE_PIN1) && flag2)
			{
				counter--;
				flag2 = 0; // clear the flag
			}
		// delay loop to generate 50 mSec delay (switch debouncing)
		// (i.e. 1 empty loop iteration = 0.5 usec)
		for (j =0; j< DELAY_VALUE/10 ; j++);
		
		if(GPIO_PORTE_DATA_R != GPIO_PORTE_PIN1)
			{
				flag2 = 1; // set the flag
			}
		if(counter>255)
			{
				counter=0;
			}
		if(counter<0)
			{
				counter=255;
			}
			
		GPIO_PORTB_DATA_R = counter;
	}
}

void PortB_Digital_Output_Init(void)
	{
	// Step 1: Clock enable on PortB
	RCGC_GPIO_R |= GPIO_PORTB_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected by selecting
	// 0x40005000 as Base Address (PortC) in DATA section
	
	// Step 3: Disable alternate functionality on PortB
	GPIO_PORTB_AFSEL_R &= ~GPIO_PORTB_AFSEL_DS;
	
	// Step 4: Enable digital pin functionaliy on PortB
	GPIO_PORTB_DEN_R |= GPIO_PORTB_EN; // Digital enable for PB0-7
	
	// Step 5: Set PortB pins as an output pins
	GPIO_PORTB_DIR_R |= GPIO_PORTB_OP; // PB0-7 as output
}
void PE0_Digital_Input_Init(void)
	{
	// Step 1: Clock enable on PortE
	RCGC_GPIO_R |= GPIO_PORTE_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected by selecting
	// 0x40024000 as Base Address (PortE) in DATA section
	
	// Step 3: Disable alternate functionality on PortE
	GPIO_PORTE_AFSEL_R &= ~GPIO_PORTE_AFSEL_DS;
	
	// Step 4: Enable digital pin functionaliy on PortE
	GPIO_PORTE_DEN_R |= GPIO_PORTE_PIN01_EN; // Digital enable for PE0
	
	// Step 5: Set PortE pins as an input pins
	GPIO_PORTE_DIR_R &= ~GPIO_PORTE_PIN01_IP; // PE0 as input
}