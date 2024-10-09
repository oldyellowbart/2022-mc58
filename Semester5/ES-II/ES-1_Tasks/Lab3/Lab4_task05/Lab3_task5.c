//|_______________________________________________________________________________________________________________________________________________________|
//|Task 3.5: Interface two LEDs with Port E pin 2 & 3 and program them to flash at 1 Hz and 1/2 Hz respectively.          															  |
//|-------------------------------------------------------------------------------------------------------------------------------------------------------|
//|Name : Haseeb-ul-hassan																																																																|
//|Date : 25-FEB-2024																																																																			|
//|Hardware: Tiva C series Launchpad Evaluation kit by Texas instruments																																									|	
//|IDE and Compiler : Keil microvision 5.39																																																								|
//|Disclaimer : This program has been written as an open source develpment program so any usage must be credited and i am not responsible for any missuse |
//|Contact : 2022mc58@student.uet.edu.pk(feel free to ask me anything about this code	)																																		|
//|_______________________________________________________________________________________________________________________________________________________|

//_________________________________________________________________________________________________________________________________________________________
// Configuring Desired  Register Adresses... 
//_________________________________________________________________________________________________________________________________________________________
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608) 					 // Ports Clock Gating Control Register Adress(General)
// PortE on ADvance Periferal Bus(APB) = 0x4002 4000 (Base Address)		// Port E Base Register Adress(Main Adress)
#define GPIO_PortE_AFSEL_R *(( volatile unsigned long *)0x40024420)	 // Alternative Function Select Register Adress(Offset 0x420)
#define GPIO_PortE_DEN_R *(( volatile unsigned long *)0x4002451C)		// Digital Enable Register Adress(Offset 0x51C)
#define GPIO_PortE_DIR_R *(( volatile unsigned long *)0x40024400)	 // Direction Control Register Adress(Offset 0x400)
#define GPIO_PortE_DATA_R *(( volatile unsigned long *)0x40024040)// Digital Data Rigister Adress(Determined using IDK)
//_________________________________________________________________________________________________________________________________________________________
// Assigning Registers Values...
//_________________________________________________________________________________________________________________________________________________________
#define GPIO_PortE_CLOCK_EN 0x10		 //(0001 0000) PortE clock gating control
#define GPIO_PortE_AFSEL_DS 0x00 		//(0000 0000)PortE alternate function disable
#define GPIO_PortE_pin2_3_EN 0x4C  //(0000 1100) Port E pin 2,3 digital enable
#define GPIO_PortE_pin2_3_OP 0x4C //(0000 1100) Port E pin 2,3 as digital output
#define GPIO_PortE_pin2 0x04 		 //(0000 0100) Mask for pin 2 write operation 
#define GPIO_PortE_pin3 0x08	  //(0000 1000) Mask for pin 3 write operation 
//_________________________________________________________________________________________________________________________________________________________
//Setting System Clock...
//_________________________________________________________________________________________________________________________________________________________
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE 334000						 //Delay value for the program
//_________________________________________________________________________________________________________________________________________________________
//Mian Program ...
//_________________________________________________________________________________________________________________________________________________________
unsigned volatile long j;
int main ()
{
	RCGC_GPIO_R |= GPIO_PortE_CLOCK_EN;// Step 1: Clock enable on PortE
	for (j =0; j < 3 ; j++)						// Wait 3 clock cyles to update the clock Register
																	 // Step 2: APB is selected for PortE by selecting Register Adress 0x4002 4000 (Base Address)																
	GPIO_PortE_AFSEL_R &= GPIO_PortE_AFSEL_DS;  // Step 3: Disable alternate functionality on PortE
	GPIO_PortE_DEN_R |= GPIO_PortE_pin2_3_EN;  // Step 4: Enable digital pin functionaliy on PortE pin 2,3
	GPIO_PortE_DIR_R |= GPIO_PortE_pin2_3_OP; // Step 5: Set PortE pin 2,3 as an output pin
	while (1)																 // Loop to run infinite
	{GPIO_PortE_DATA_R ^= GPIO_PortE_pin2;  // Toggle the Pin2
	for (j =0; j< DELAY_VALUE ; j++);		   // delay loop(1Hz) 
	GPIO_PortE_DATA_R ^= GPIO_PortE_pin3; // Toggle the Pin6
	for (j =0; j< (DELAY_VALUE/2) ; j++);// delay loop(0.5Hz)
	}
}
//_________________________________________________________________________________________________________________________________________________________
//If You find a gun in the last comments row then shot the Tiva with this Code
//_______________________________________________________(HAPPY-CODDING)___________________________________________________________________________________