/* Lab_04_Task_4.3_Simon Game.uvprojx
-----------------------------------------------------------------------
Implement a simplified version of the Simon game using
LEDs and pushbuttons. The microcontroller should display a sequence of LED
flashes, and the user must repeat the sequence by pressing the corresponding
buttons. You Google to get an idea about the Simon Game and it's rules.
-----------------------------------------------------------------------
Written by Alishba Kiyani (alishbasaniyakiyani@gmail.com) 
Compiler and IDE used: Keil uVision 5.34*/
// For Port B
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608)
// PortB on APB = 0x40005000 (Base Address)
#define GPIO_PORTB_AFSEL_R *(( volatile unsigned long *)0x40005420)
#define GPIO_PORTB_DEN_R *(( volatile unsigned long *)0x4000551C)
#define GPIO_PORTB_DIR_R *(( volatile unsigned long *)0x40005400)
#define GPIO_PORTB_DATA_0_R *(( volatile unsigned long *)0x40005004)
#define GPIO_PORTB_DATA_1_R *(( volatile unsigned long *)0x40005008)		
#define GPIO_PORTB_DATA_2_R *(( volatile unsigned long *)0x40005010)
#define GPIO_PORTB_DATA_3_R *(( volatile unsigned long *)0x40005020)
#define GPIO_PORTB_DATA_4_R *(( volatile unsigned long *)0x40005040)
#define GPIO_PORTB_DATA_5_R *(( volatile unsigned long *)0x40005080)
#define GPIO_PORTB_DATA_6_R *(( volatile unsigned long *)0x40005100)
#define GPIO_PORTB_DATA_7_R *(( volatile unsigned long *)0x40005200)
#define GPIO_PORTB_DATA_R *(( volatile unsigned long *)0x400053FC)

// For Port E pin 0
// PortE on APB = 0x40024000 (Base Address)
#define GPIO_PORTE_AFSEL_R *(( volatile unsigned long *)0x40024420)
#define GPIO_PORTE_DEN_R *(( volatile unsigned long *)0x4002451C)
#define GPIO_PORTE_DIR_R *(( volatile unsigned long *)0x40024400)
#define GPIO_PORTE_DATA_0_R *(( volatile unsigned long *)0x40024004)
	// for port E Pin 1
#define GPIO_PORTE_PIN1 0x02 // Mask for pin 1 
#define GPIO_PORTE_DATA_1_R *(( volatile unsigned long *)0x40024008
#define GPIO_PORTE_PD_R *(( volatile unsigned long *)0x40024514)
#define GPIO_PORTE_DATA_R	*(( volatile unsigned long *)0x4002400C)

// Configuration Registers Values 
#define GPIO_PORTBE_CLOCK_EN 0x12 // Port B and E clock gating control

// For Port B
#define GPIO_PORTB_AFSEL_DS 0xFF //  alternate function disable
#define GPIO_PORTB_PINS_EN 0xFF  //  all pins digital enable
#define GPIO_PORTB_PINS_OP 0xFF //  all pins as digital output
#define GPIO_PORTB_PIN0 0x01 // Mask for pin 0  (0000 0001)
#define GPIO_PORTB_PIN1 0x02 // Mask for pin 1  (0000 0010)
#define GPIO_PORTB_PIN2 0x04 // Mask for pin 2  (0000 0100)
#define GPIO_PORTB_PIN3 0x08 // Mask for pin 3  (0000 1000)
#define GPIO_PORTB_PIN4 0x10 // Mask for pin 4  (0001 0000)
#define GPIO_PORTB_PIN5 0x20 // Mask for pin 5  (0010 0000)
#define GPIO_PORTB_PIN6 0x40 // Mask for pin 6  (0100 0000)
#define GPIO_PORTB_PIN7 0x80 // Mask for pin 7  (1000 0000)
#define GPIO_PORTB_PINS 0xFF // Mask for all pins  (1111 1111)
// For Port E
#define GPIO_PORTE_AFSEL_DS 0x03 //  alternate function disable
#define GPIO_PORTE_PINS_EN 0x03 // pin 0_1 digital enable
#define GPIO_PORTE_PINS_OP 0x03 // pin 0_1 as digital input
#define GPIO_PORTE_PIN_PD 0x03 //  pin 0_1 Pull down resistor mask
#define GPIO_PORTE_PIN0 0x01 // Mask for pin 0  (0000 0001)
#define GPIO_PORTE_PIN1 0x02 // Mask for pin 1

// Constant Values
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE SYSTEM_CLOCK_FREQUENCY/16

unsigned volatile long j, count ;

int main ()
{
	// User main program
	
	// Step 1: Clock enable on PortB and PortE
	RCGC_GPIO_R |= GPIO_PORTBE_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected for PortB and PortE by selecting
	// 0x40005000 as Base Address in DATA section
	// 0x40024000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on PortB and PortE
	GPIO_PORTB_AFSEL_R &= ~GPIO_PORTB_AFSEL_DS;
	GPIO_PORTE_AFSEL_R &= ~GPIO_PORTE_AFSEL_DS;
	
	// Step 4: Enable digital pin functionaliy on PortB and PortE 
	GPIO_PORTB_DEN_R |= GPIO_PORTB_PINS_EN; // Digital enable for pin 1-4
	GPIO_PORTE_DEN_R |= GPIO_PORTE_PINS_EN; // Digital enable for PE
	
	// Step 5: Set PortB pins 1-4 as an output pin
	GPIO_PORTB_DIR_R |= GPIO_PORTB_PINS_OP; // PB0-PB7 as output
	// Set PortE pins 0 as an input pin
	GPIO_PORTE_DIR_R &= ~GPIO_PORTE_PINS_OP;
	// PE0as input
	
	// Step 6: Set internal pull down resistor enable  
	GPIO_PORTE_PD_R |= GPIO_PORTE_PIN_PD;
	
	while(1){
	for(count=0,count<=256,count++)
		{
		GPIO_PORTB_DATA_R = count;
	}}