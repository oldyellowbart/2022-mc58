//|_______________________________________________________________________________________________________________________________________________________|
//|Task 4.1: LED Light Chaser: Interface 8 external LEDs and 5 external pushbuttons with Port B pin 0-7 and Port E pin 0-4 respectively and program to implement 5 different LED light chaser patterns																																																																																															  |
//|-------------------------------------------------------------------------------------------------------------------------------------------------------|
//|Name : Haseeb-ul-hassan																																																																|
//|Date : 03-MARCH-2024																																																																			|
//|Hardware: Tiva C series Launchpad Evaluation kit by Texas instruments																																									|	
//|IDE and Compiler : Keil microvision 5.39																																																								|
//|Disclaimer : This program has been written as an open source develpment program so any usage must be credited and i am not responsible for any missuse |
//|Contact : 2022mc58@student.uet.edu.pk(feel free to ask me anything about this code	)																																		|
//|_______________________________________________________________________________________________________________________________________________________|

//_________________________________________________________________________________________________________________________________________________________
// Configuring Desired  Register Adresses... 
//_________________________________________________________________________________________________________________________________________________________
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608)         //Configuration Registers for Registor Clock Gatting Control(RCGC)
													 // PortB_F on ADvance Periferal Bus(APB) = 0x40005000,0x40024000 (Base Address)   
																																		 // Port E Base Register Adress(Main Adres																			                             //PortB
#define GPIO_PortB_AFSEL_R *(( volatile unsigned long *)0x40005420)  // Alternative Function Select Register Adress(Offset 0x420)
#define GPIO_PortB_DEN_R *(( volatile unsigned long *)0x4000551C)    // Digital Enable Register Adress(Offset 0x51C)
#define GPIO_PortB_DIR_R *(( volatile unsigned long *)0x40005400)    // Direction Control Register Adress(Offset 0x400)
#define GPIO_PortB_DATA_Pin0 *(( volatile unsigned long *)0x40005004)//Digital Data Rigister Adress(Determined using IDK
#define GPIO_PortB_DATA_Pin1 *(( volatile unsigned long *)0x40005008)//Digital Data Rigister Adress(Determined using IDK
#define GPIO_PortB_DATA_Pin2 *(( volatile unsigned long *)0x40005010)//Digital Data Rigister Adress(Determined using IDK
#define GPIO_PortB_DATA_Pin3 *(( volatile unsigned long *)0x40005020)//Digital Data Rigister Adress(Determined using IDK
#define GPIO_PortB_DATA_Pin4 *(( volatile unsigned long *)0x40005040)//Digital Data Rigister Adress(Determined using IDK
#define GPIO_PortB_DATA_Pin5 *(( volatile unsigned long *)0x40005080)//Digital Data Rigister Adress(Determined using IDK
#define GPIO_PortB_DATA_Pin6 *(( volatile unsigned long *)0x40005100)//Digital Data Rigister Adress(Determined using IDK
#define GPIO_PortB_DATA_Pin7 *(( volatile unsigned long *)0x40005200)//Digital Data Rigister Adress(Determined using IDK
																																		 //PortE
#define GPIO_PortE_AFSEL_R *(( volatile unsigned long *)0x40024420)  // Alternative Function Select Register Adress(Offset 0x420)
#define GPIO_PortE_DEN_R *(( volatile unsigned long *)0x4002451C)    // Digital Enable Register Adress(Offset 0x51C)
#define GPIO_PortE_DIR_R *(( volatile unsigned long *)0x40024400)    // Direction Control Register Adress(Offset 0x400)
#define GPIO_PortE_DATA_pin0 *(( volatile unsigned long *)0x40024004)//Digital Data Rigister Adress(Determined using IDK)
#define GPIO_PortE_DATA_pin1 *(( volatile unsigned long *)0x40024008)//Digital Data Rigister Adress(Determined using IDK)
#define GPIO_PortE_DATA_pin2 *(( volatile unsigned long *)0x40024010)//Digital Data Rigister Adress(Determined using IDK)
#define GPIO_PortE_DATA_pin3 *(( volatile unsigned long *)0x40024020)//Digital Data Rigister Adress(Determined using IDK)
#define GPIO_PortE_DATA_pin4 *(( volatile unsigned long *)0x40024040)//Digital Data Rigister Adress(Determined using IDK)
#define GPIO_PortE_resistor_R *(( volatile unsigned long *)0x40024514)
//_________________________________________________________________________________________________________________________________________________________
// Assigning Registers Values...
//_________________________________________________________________________________________________________________________________________________________
#define GPIO_PortB_F_CLOCK_EN 0x12// (0000 0010) PortB_E clock gating control
																	//PortB
#define GPIO_PortB_AFSEL_DS 0xFF  // (0000 0000)Port B alternate function disable
#define GPIO_PortB_EN 0xFF    		// (0010 0010) Port B pin 0_7 digital enable
#define GPIO_PortB_pins_OP 0xFF   // (0010 0010) Port B pin 0_7 as Output
#define GPIO_PortB_pin0 0x01      // (0000 0001) Mask for pin 0 write operation 
#define GPIO_PortB_pin1 0x02      // (0000 0010) Mask for pin 1 write operation
#define GPIO_PortB_pin2 0x04      // (0000 0100) Mask for pin 2 write operation
#define GPIO_PortB_pin3 0x08      // (0000 1000) Mask for pin 3 write operation
#define GPIO_PortB_pin4 0x10      // (0001 0000) Mask for pin 4 write operation
#define GPIO_PortB_pin5 0x20      // (0010 0000) Mask for pin 5 write operation
#define GPIO_PortB_pin6 0x40      // (0100 0000) Mask for pin 6 write operation
#define GPIO_PortB_pin7 0x80      // (1000 0000) Mask for pin 7 write operation
																  //PortE
#define GPIO_PortE_AFSEL_DS 0x1F// (0000 0000)Port B alternate function disable
#define GPIO_PortE_EN 0x1F		    // (0010 0010) Port B pin 0_4 as digital enable
#define GPIO_PortE_pins_IN 0x1F   // (0010 0010) Port B pin 0_4 as Input
#define GPIO_PortE_pin0 0x01      // (0000 0001)Mask for pin 0 read operation
#define GPIO_PortE_pin1 0x02      // (0000 0010)Mask for pin 1 read operation
#define GPIO_PortE_pin2 0x04      // (0000 0100)Mask for pin 2 read operation
#define GPIO_PortE_pin3 0x08      // (0000 1000)Mask for pin 3 read operation
#define GPIO_PortE_pin4 0x10      // (0001 0000)Mask for pin 4 read operation
#define GPIO_PortE_resistor_value 0x1F

//_________________________________________________________________________________________________________________________________________________________
//Setting System Clock...
//_________________________________________________________________________________________________________________________________________________________
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE  SYSTEM_CLOCK_FREQUENCY/240					   //Delay value for the program 1 second
#define Delay(time){for (int i = 0; i < (DELAY_VALUE/time); i++){};}
//_________________________________________________________________________________________________________________________________________________________
//Mian Program ...
//_________________________________________________________________________________________________________________________________________________________
unsigned volatile long j;
int main ()
{
	RCGC_GPIO_R |= GPIO_PortB_F_CLOCK_EN;// Step 1: Clock enable on PortB_F
	for (j =0; j < 3 ; j++)						// Wait 3 clock cyles to update the clock Register
	GPIO_PortE_resistor_R &= ~GPIO_PortE_resistor_value ; 				 // Step 2: APB is selected for PortB_F by selecting 0x40005000 as Base Address in DATA section
	GPIO_PortB_AFSEL_R &= ~GPIO_PortB_AFSEL_DS;   // Step 3: Disable alternate functionality on PortB_F
	GPIO_PortB_DEN_R |= GPIO_PortB_EN; 		  		// Step 4: Enable digital pin functionaliy on PortB_F pin 2,6
	GPIO_PortB_DIR_R |= GPIO_PortB_pins_OP; // Step 5: Set PortB_F pin 2,4 as an output pin
	
	GPIO_PortE_AFSEL_R &= ~GPIO_PortE_AFSEL_DS;   // Step 3: Disable alternate functionality on PortB_F
	GPIO_PortE_DEN_R |= GPIO_PortE_pins_IN;// Step 4: Enable digital pin functionaliy on PortB_F pin 2,6
	GPIO_PortE_DIR_R &= ~GPIO_PortE_pins_IN;// Step 5: Set PortB_F pin 2,4 as an output pin
	while (1)								        	    // Loop to run infinite
	{
  if(GPIO_PortE_DATA_pin0 == GPIO_PortE_pin0)
{
	for (int i = 0; i < 5; i++)
	{
	
	GPIO_PortB_DATA_Pin0 |= GPIO_PortB_pin0;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin1 |= GPIO_PortB_pin1;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin2 |= GPIO_PortB_pin2;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin3 |= GPIO_PortB_pin3;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin4 |= GPIO_PortB_pin4;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin5 |= GPIO_PortB_pin5;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin6 |= GPIO_PortB_pin6;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin7 |= GPIO_PortB_pin7;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin0 &= ~GPIO_PortB_pin0;
	GPIO_PortB_DATA_Pin1 &= ~GPIO_PortB_pin1;
	GPIO_PortB_DATA_Pin2 &= ~GPIO_PortB_pin2;
	GPIO_PortB_DATA_Pin3 &= ~GPIO_PortB_pin3;
	GPIO_PortB_DATA_Pin4 &= ~GPIO_PortB_pin4;
	GPIO_PortB_DATA_Pin5 &= ~GPIO_PortB_pin5;
	GPIO_PortB_DATA_Pin6 &= ~GPIO_PortB_pin6;
	GPIO_PortB_DATA_Pin7 &= ~GPIO_PortB_pin7;
}}
 if (GPIO_PortE_DATA_pin1 == GPIO_PortE_pin1)
{
	for (int i = 0; i < 5; i++)
	{
	GPIO_PortB_DATA_Pin0 |= GPIO_PortB_pin0;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin0 &= ~GPIO_PortB_pin0;
	GPIO_PortB_DATA_Pin1 |= GPIO_PortB_pin1;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin1 &= ~GPIO_PortB_pin1;
	GPIO_PortB_DATA_Pin2 |= GPIO_PortB_pin2;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin2 &= ~GPIO_PortB_pin2;
	GPIO_PortB_DATA_Pin3 |= GPIO_PortB_pin3;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin3 &= ~GPIO_PortB_pin3;
	GPIO_PortB_DATA_Pin4 |= GPIO_PortB_pin4;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin4 &= ~GPIO_PortB_pin4;
	GPIO_PortB_DATA_Pin5 |= GPIO_PortB_pin5;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin5 &= ~GPIO_PortB_pin5;
	GPIO_PortB_DATA_Pin6 |= GPIO_PortB_pin6;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin6 &= ~GPIO_PortB_pin6;
	GPIO_PortB_DATA_Pin7 |= GPIO_PortB_pin7;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin7 &= ~GPIO_PortB_pin7;
}}
 if (GPIO_PortE_DATA_pin2 == GPIO_PortE_pin2)
{
	for (int i = 0; i < 5; i++)
	{
	GPIO_PortB_DATA_Pin0 |= GPIO_PortB_pin0;
	GPIO_PortB_DATA_Pin7 |= GPIO_PortB_pin7;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin1 |= GPIO_PortB_pin1;
	GPIO_PortB_DATA_Pin6 |= GPIO_PortB_pin6;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin2 |= GPIO_PortB_pin2;
	GPIO_PortB_DATA_Pin5 |= GPIO_PortB_pin5;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin3 |= GPIO_PortB_pin3;
	GPIO_PortB_DATA_Pin4 |= GPIO_PortB_pin4;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin0 &= ~GPIO_PortB_pin0;
	GPIO_PortB_DATA_Pin1 &= ~GPIO_PortB_pin1;
	GPIO_PortB_DATA_Pin2 &= ~GPIO_PortB_pin2;
	GPIO_PortB_DATA_Pin3 &= ~GPIO_PortB_pin3;
	GPIO_PortB_DATA_Pin4 &= ~GPIO_PortB_pin4;
	GPIO_PortB_DATA_Pin5 &= ~GPIO_PortB_pin5;
	GPIO_PortB_DATA_Pin6 &= ~GPIO_PortB_pin6;
	GPIO_PortB_DATA_Pin7 &= ~GPIO_PortB_pin7;
	
}}
 if (GPIO_PortE_DATA_pin3 == GPIO_PortE_pin3)
{
	for (int i = 0; i < 5; i++)
	{	
	GPIO_PortB_DATA_Pin0 |= GPIO_PortB_pin0;
	GPIO_PortB_DATA_Pin1 |= GPIO_PortB_pin1;
	GPIO_PortB_DATA_Pin2 |= GPIO_PortB_pin2;
	GPIO_PortB_DATA_Pin3 |= GPIO_PortB_pin3;
	for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin0 &= ~GPIO_PortB_pin0;
	GPIO_PortB_DATA_Pin1 &= ~GPIO_PortB_pin1;
	GPIO_PortB_DATA_Pin2 &= ~GPIO_PortB_pin2;
	GPIO_PortB_DATA_Pin3 &= ~GPIO_PortB_pin3;
		GPIO_PortB_DATA_Pin4 |= GPIO_PortB_pin4;
		GPIO_PortB_DATA_Pin5 |= GPIO_PortB_pin5;
		GPIO_PortB_DATA_Pin6 |= GPIO_PortB_pin6;
		GPIO_PortB_DATA_Pin7 |= GPIO_PortB_pin7;
	for(j=0; j<DELAY_VALUE; j++);
		GPIO_PortB_DATA_Pin4 &= ~GPIO_PortB_pin4;
		GPIO_PortB_DATA_Pin5 &= ~GPIO_PortB_pin5;
		GPIO_PortB_DATA_Pin6 &= ~GPIO_PortB_pin6;
		GPIO_PortB_DATA_Pin7 &= ~GPIO_PortB_pin7;
}}
if (GPIO_PortE_DATA_pin4 == GPIO_PortE_pin4)
{
	for (int i = 0; i < 5; i++)
	{	
	GPIO_PortB_DATA_Pin0 |= GPIO_PortB_pin0;
	GPIO_PortB_DATA_Pin1 |= GPIO_PortB_pin1;
for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin0 &= ~GPIO_PortB_pin0;
	GPIO_PortB_DATA_Pin1 &= ~GPIO_PortB_pin1;
	GPIO_PortB_DATA_Pin2 |= GPIO_PortB_pin2;
	GPIO_PortB_DATA_Pin3 |= GPIO_PortB_pin3;
for(j=0; j<DELAY_VALUE; j++);
	GPIO_PortB_DATA_Pin2 &= ~GPIO_PortB_pin2;
	GPIO_PortB_DATA_Pin3 &= ~GPIO_PortB_pin3;
	GPIO_PortB_DATA_Pin4 |= GPIO_PortB_pin4;
	GPIO_PortB_DATA_Pin5 |= GPIO_PortB_pin5;
for(j=0; j<DELAY_VALUE; j++);
		GPIO_PortB_DATA_Pin4 &= ~GPIO_PortB_pin4;
		GPIO_PortB_DATA_Pin5 &= ~GPIO_PortB_pin5;
		GPIO_PortB_DATA_Pin6 |= GPIO_PortB_pin6;
		GPIO_PortB_DATA_Pin7 |= GPIO_PortB_pin7;
for(j=0; j<DELAY_VALUE; j++);
		GPIO_PortB_DATA_Pin6 &= ~GPIO_PortB_pin6;
		GPIO_PortB_DATA_Pin7 &= ~GPIO_PortB_pin7;
}
}

	}}
//_________________________________________________________________________________________________________________________________________________________
//If You find a gun in the  last comments row then shot the Tiva with this Code
//_______________________________________________________(HAPPY-CODDING)___________________________________________________________________________________
