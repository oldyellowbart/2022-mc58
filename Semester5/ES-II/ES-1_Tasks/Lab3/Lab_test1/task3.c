//|_______________________________________________________________________________________________________________________________________________________|
//|Task 3 test																									  |
//|-------------------------------------------------------------------------------------------------------------------------------------------------------|
//|Name : Haseeb-ul-hassan																																																																|
//|Date : 21-FEB-2024																																																																			|
//|Hardware: Tiva C series Launchpad Evaluation kit by Texas instruments																																									|	
//|IDE and Compiler : Keil microvision 5.39																																																								|
//|Disclaimer : This program has been written as an open source develpment program so any usage must be credited and i am not responsible for any missuse |
//|Contact : 2022mc58@student.uet.edu.pk(feel free to ask me anything about this code	)																																		|
//|_______________________________________________________________________________________________________________________________________________________|


//_________________________________________________________________________________________________________________________________________________________
// Configuring Desired  Register Adresses... 
//_________________________________________________________________________________________________________________________________________________________
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608)
// portc on APB = 0x4000 6000 (Base Address)
#define GPIO_portc_AFSEL_R *(( volatile unsigned long *)0x40006420)
#define GPIO_portc_DEN_R *(( volatile unsigned long *)0x4000651C)
#define GPIO_portc_DIR_R *(( volatile unsigned long *)0x40006400)
#define GPIO_portc_DATA_R *(( volatile unsigned long *)0x40006040)
//_________________________________________________________________________________________________________________________________________________________
// Asigning Registers Values...
//_________________________________________________________________________________________________________________________________________________________
#define GPIO_portc_CLOCK_EN 0x04 //(0000 0100) PortC clock gating control
#define GPIO_portc_AFSEL_DS 0x00 // Port F alternate function disable
#define GPIO_portc_pin4_EN 0x10  //(0001 0000) Port F pin 3 digital enable
#define GPIO_portc_pin4_OP 0x10 // Port F pin 3 as digital output
#define GPIO_portc_pin4 0x10 // Mask for pin 3 write operation (0001 0000)
//_________________________________________________________________________________________________________________________________________________________
// Setting System Clock...
//_________________________________________________________________________________________________________________________________________________________
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE 334000
//_________________________________________________________________________________________________________________________________________________________
// Mian Program ...
//_________________________________________________________________________________________________________________________________________________________
unsigned volatile long j;

int main ()
{
	// User main program
	
	// Step 1: Clock enable on portc
	RCGC_GPIO_R |= GPIO_portc_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected for portc by selecting
	// 0x40025000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on portc
	GPIO_portc_AFSEL_R &= ~GPIO_portc_AFSEL_DS;
	
	// Step 4: Enable digital pin functionaliy on portc pin 3
	GPIO_portc_DEN_R |= GPIO_portc_pin4_EN; // Digital enable for PF3
	
	// Step 5: Set portc pin 3 as an output pin
	GPIO_portc_DIR_R |= GPIO_portc_pin4_OP; // PF3 as output
		
	while (1)
	{
		// Toggle the green LED
		GPIO_portc_DATA_R ^= GPIO_portc_pin4;
		
		// delay loop
		for (j =0; j< DELAY_VALUE ; j++);
	}
}//_________________________________________________________________________________________________________________________________________________________