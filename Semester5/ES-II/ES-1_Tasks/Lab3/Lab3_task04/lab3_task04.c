//|_______________________________________________________________________________________________________________________________________________________|
//|Task 3.4: Interface and program an LED connected to Port A pin 5 to flash 5 times at 2 Hz after 3 seconds															  							|
//|-------------------------------------------------------------------------------------------------------------------------------------------------------|
//|Name : Haseeb-ul-hassan																																																																|
//|Date : 21-FEB-2024																																																																			|
//|Hardware: Tiva C series Launchpad Evaluation kit by Texas instruments																																									|	
//|IDE and Compiler : Keil microvision 5.39																																																								|
//|Disclaimer : This program has been written as an open source develpment program so any usage must be credited and i am not responsible for any missuse |
//|Contact : 2022mc58@student.uet.edu.pk(feel free to ask me anything about this code	)																																		|
//|_______________________________________________________________________________________________________________________________________________________|
// Configuration Registers
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608)
// PortA on APB = 0x4000 4000 (Base Address)
#define GPIO_PortA_AFSEL_R *(( volatile unsigned long *)0x40004420)
#define GPIO_PortA_DEN_R *(( volatile unsigned long *)0x4000451C)
#define GPIO_PortA_DIR_R *(( volatile unsigned long *)0x40004400)
#define GPIO_PortA_DATA_R *(( volatile unsigned long *)0x40004080)

// Configuration Registers Values
#define GPIO_PortA_CLOCK_EN 0x01 	 //(0000 0001) PortA clock gating control
#define GPIO_PortA_AFSEL_DS 0x00 	//(0000 0000)Port A alternate function disable
#define GPIO_PortA_Pin5_EN 0x20  //(0010 0000) Port A pin 5 digital enable
#define GPIO_PortA_Pin5_OP 0x20 //(0010 0000)Port A pin 5 as digital output
#define GPIO_PortA_Pin5 0x20 	 //(0010 0000) Mask for pin 5 write operation 

// Constant Values
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE 33400

unsigned volatile long j;

int main ()
{
	// User main program
	
	// Step 1: Clock enable on PortA
	RCGC_GPIO_R |= GPIO_PortA_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected for PortA by selecting
	// 0x40004000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on PortA
	GPIO_PortA_AFSEL_R &= GPIO_PortA_AFSEL_DS;
	
	// Step 4: Enable digital pin functionaliy on PortA pin 5
	GPIO_PortA_DEN_R |= GPIO_PortA_Pin5_EN; // Digital enable for PA5
	
	// Step 5: Set PortA pin 5 as an output pin
	GPIO_PortA_DIR_R |= GPIO_PortA_Pin5_OP; // PF3 as output
		
	while (1)
	{
		
		for (j =0; j< 167000 ; j++){
		for (j =0; j< DELAY_VALUE ; j++){GPIO_PortA_DATA_R ^= GPIO_PortA_Pin5;// Toggle the green LED
												for (j =0; j< DELAY_VALUE ; j++);}
		for (j =0; j< 1002000 ; j++);
		} // delay loop
		
		
	}
}