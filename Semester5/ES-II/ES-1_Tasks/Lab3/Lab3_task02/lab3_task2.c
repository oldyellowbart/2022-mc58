//|_______________________________________________________________________________________________________________________________________________________|
//|Task 3.2: Re-do Task 2.3 and instead of using on-board LEDs, interface two LEDs with Port B pin 3 and Port A pin 4.																	  |
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
																																			 // PortA on ADvance Periferal Bus(APB) = 0x4000 4000 (Base Address)
																																			// PortB on ADvance Periferal Bus(APB) = 0x4000 5000 (Base Address)
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608)				 //Configuration Registers for Registor Clock Gatting Control(RCGC)
#define GPIO_PortA_AFSEL_R *(( volatile unsigned long *)0x40004420) // Alternative Function Select Register Adress(Offset 0x420)
#define GPIO_PortB_AFSEL_R *(( volatile unsigned long *)0x40005420)// Alternative Function Select Register Adress(Offset 0x420)
#define GPIO_PortA_DEN_R *(( volatile unsigned long *)0x4000451C) // Digital Enable Register Adress(Offset 0x51C)
#define GPIO_PortB_DEN_R *(( volatile unsigned long *)0x4000551C)     // Digital Enable Register Adress(Offset 0x51C)
#define GPIO_PortA_DIR_R *(( volatile unsigned long *)0x40004400)    // Direction Control Register Adress(Offset 0x400)
#define GPIO_PortB_DIR_R *(( volatile unsigned long *)0x40005400)   // Direction Control Register Adress(Offset 0x400)
#define GPIO_PortA_DATA_R *(( volatile unsigned long *)0x40004040) //Digital Data Rigister Adress(Determined using IDK)
#define GPIO_PortB_DATA_R *(( volatile unsigned long *)0x40005020)//Digital Data Rigister Adress(Determined using IDK)
//_______________________________________________________________________________________________________________________________
// Assigning Registers Values...
//_______________________________________________________________________________________________________________________________
#define GPIO_PortA_CLOCK_EN 0x01 			 // (0000 0001) PortA clock gating control
#define GPIO_PortB_CLOCK_EN 0x02 			// (0000 0010) PortB clock gating control
#define GPIO_PortA_AFSEL_DS 0x00 		 // (0000 0000) Port A alternate function disable
#define GPIO_PortB_AFSEL_DS 0x00 		// (0000 0000)  Port B alternate function disable
#define GPIO_PortA_pin4_EN 0x10  	 // (0001 0000) Port A pin4 digital enable
#define GPIO_PortB_pin3_EN 0x08   // (0000 1000) Port B pin3  digital enable
#define GPIO_PortA_pin4_OP 0x10  // (0001 0000) Port A pin 4 as digital output
#define GPIO_PortB_pin3_OP 0x08 // (0000 1000) Port B pin 3 as digital output
#define GPIO_PortA_pin4 0x10 	 // (0001 0000) Mask for pin 4 write operation 
#define GPIO_PortB_pin3 0x08 	// (0000 1000) Mask for pin 3 write operation 
//_______________________________________________________________________________________________________________________________
// Clock Setting...
//_______________________________________________________________________________________________________________________________
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE 334000
//_______________________________________________________________________________________________________________________________
//Main Code(Core Body <Sensitive>)
unsigned volatile long j;
int main ()
{  
	RCGC_GPIO_R |= GPIO_PortA_CLOCK_EN; //Step 1: Clock enable on Port A 
	RCGC_GPIO_R |= GPIO_PortB_CLOCK_EN;//Step 1: Clock enable on Port B
	for (j =0; j < 3 ; j++)	 					// Wait 3 clock cyles to update the clock Register
																	 // Step 2: APB is selected for Port A,B by selecting  0x40005000,0x40005000 as Base Address in DATA section
	GPIO_PortA_AFSEL_R &= GPIO_PortA_AFSEL_DS;  // Step 3: Disable alternate functionality on Port A
	GPIO_PortB_AFSEL_R &= GPIO_PortB_AFSEL_DS; // Step 3: Disable alternate functionality on Port B
	GPIO_PortA_DEN_R |= GPIO_PortA_pin4_EN;   // Step 4: Digital enable for PA4
	GPIO_PortB_DEN_R |= GPIO_PortB_pin3_EN;  // Step 4: Digital enable for PB3
	GPIO_PortA_DIR_R |= GPIO_PortA_pin4_OP; // Step 5: PA4 as output
	GPIO_PortB_DIR_R |= GPIO_PortB_pin3_OP;// Step 5: PB3 as output
	while (1)															// Loop to run infinite
 {GPIO_PortA_DATA_R ^= GPIO_PortA_pin4;// Toggle the PortA Pin4
	for (j =0; j< DELAY_VALUE ; j++);   // delay loop(1Hz)
GPIO_PortB_DATA_R ^= GPIO_PortB_pin3;// Toggle the PortB Pin3
	for (j =0; j< DELAY_VALUE ; j++);// delay loop(1hz)
	}
}