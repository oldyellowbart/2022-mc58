/* Lab_01_Example_01_Static_String_at_16x2_LCD.uvprojx
-----------------------------------------------------------------------
This C language program displays static string ("Hello World") on 16x2 
LCD interfaced with TIVA C Series LaunchPad Evaluation Kit by Texas 
Instrument as per table given below.

** Interfacing details---------
RS -> PD0
R/W -> PD1
EN -> PD2
D0-D7 -> PB0-PB7
PE2 - >adc_in 0-3.3v 
PE1 -> pwm generation
-----------------------------------------------------------------------
Written by Shujat Ali (engrshujatali@gmail.com) on 18-Aug-2024.
Compiler and IDE used: Keil uVision 5.38 */

// Libraries
#include "TM4C123.h"
#include <string.h>

// Global Variables
unsigned volatile long j;
unsigned volatile long i;
	unsigned int adc_data = 0;
	unsigned volatile long pwm ;
// Function Declarations
void PD0_2_as_Output_Init(void);
void PortB_as_Output_Init(void);
void PortE_as_Output(void);
void PortE_as_analog_Input_Init(void);
void write_LCD_Char(unsigned char data);
void send_LCD_Cmd(unsigned char cmd);
void write_LCD_Str(unsigned char *str);
void LCD_Init(void);
void usdelay(int time);

// Main Function
int main(){
	PD0_2_as_Output_Init();
	PortB_as_Output_Init();
	PortE_as_Output();
	PortE_as_analog_Input_Init();
	LCD_Init();
	PortE_as_Output();	
	while(1){
		//write_LCD_Str("Haseeb");
		//GPIOE->DATA |= 0x02;
		//usdelay(1000000);
		//send_LCD_Cmd(0x01);
		//GPIOE->DATA &= ~(0x02);
		
		adc_data = ( ADC0->SSFIFO3 & 0xFFF);//ADC_SS3_FIFO_DATA_R & 0xFFF
			pwm = ((adc_data * 100));

			// Turn PE1 High
			//GPIO_PORTE_DATA_R |= GPIO_PORTE_MASK;
			GPIOE->DATA ^= 0x02;//on the pin1
		usdelay(pwm); // Delay required to set servo at 0 degrees

			// Turn PE1 Low
			//GPIO_PORTE_DATA_R &= ~GPIO_PORTE_MASK;
			//GPIOE->DATA &= ~(0x02);//of the pin1
		//usdelay(pwm);
	}	
}

// Function Defintions
void PD0_2_as_Output_Init(void){
	// Step 1: Clock enable on PortD
	SYSCTL->RCGCGPIO |= 0x08;	// 00 1000
	for (j =0; j < 3 ; j++);		// wait at least 3 clock cyles
	
	// Step 2: APB is selected for PortD by selecting
	// 0x40007000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on PortD
	GPIOD->AFSEL &= ~(0x07); // Disable for PD0-2 (0000 0111)
	
	// Step 4: Enable digital pin functionaliy on PortD pin 0-2
	GPIOD->DEN |= 0x07; // Digital enable for PD0-2 (0000 0111)
	
	// Step 5: Set PortD pin 0-2 as an output pin
	GPIOD->DIR |= 0x07; // PD0-2 as output (0000 0111)
}
void PortB_as_Output_Init(void){
	// Step 1: Clock enable on PortB
	SYSCTL->RCGCGPIO |= 0x02;	// 00 0010
	for (j =0; j < 3 ; j++)		// wait at least 3 clock cyles
	
	// Step 2: APB is selected for PortB by selecting
	// 0x40005000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on PortB
	GPIOB->AFSEL &= ~(0xFF); // Disable for PB0-7 (1111 1111)
	
	// Step 4: Enable digital pin functionaliy on PortB pin 0-7
	GPIOB->DEN |= 0xFF; // Digital enable for PB0-7 (1111 1111)
	
	// Step 5: Set PortB pin 0-7 as an output pin
	GPIOB->DIR |= 0xFF; // PB0-7 as output (1111 1111)
}

void write_LCD_Char(unsigned char data){
	// Interfacing details---------------
	// RS -> PD0		R/W -> PD1			EN -> PD2			D0-D7 -> PB0-PB7
	
	// Step 1. Pass data to 8-bit lines of LCD (D0-7)
	GPIOB->DATA = data;
	
	// Step 2. Enable write operation on LCD (R/W bit = 0)
	GPIOD->DATA &= ~(0x02);		// PD1 = 0000 0010
	
	// Step 3. Select Data Register of LCD (RS bit = 1)
	GPIOD->DATA |= 0x01;			// PD0 = 0000 0001
	
	// Step 4. Enable LCD operation by sending high to low pulse on Enable pin
	GPIOD->DATA |= 0x04; 			// PD2 = 0000 0100
	for (j =0; j < 10000 ; j++);		// some milisecond delay
	GPIOD->DATA &= ~(0x04); 	// PD2 = 0000 0100
}

void send_LCD_Cmd(unsigned char cmd){
	// Interfacing details---------------
	// RS -> PD0		R/W -> PD1			EN -> PD2			D0-D7 -> PB0-PB7
	
	// Step 1. Pass command to 8-bit lines of LCD (D0-7)
	GPIOB->DATA = cmd;
	// Step 2. Enable write operation on LCD (R/W bit = 0)
	GPIOD->DATA &= ~(0x02);		// PD1 = 0000 0010
	// Step 3. Select Instructioin Register of LCD (RS bit = 0)
	GPIOD->DATA &= ~(0x01);			// PD0 = 0000 001
	// Step 4. Enable LCD operation by sending high to low pulse on Enable pin
	GPIOD->DATA |= 0x04; 			// PD2 = 0000 0100
	for (j =0; j < 10000 ; j++);		// some milisecond delay
	GPIOD->DATA &= ~(0x04); 	// PD2 = 0000 0100
}
void write_LCD_Str(unsigned char *str){
	unsigned int i, len;
	len = strlen(str);
	
	for (i = 0; i < len; i++){
		write_LCD_Char(str[i]);
	}
}

void LCD_Init(void){
	send_LCD_Cmd(0x38);		// Function Set: 8-bit, 2 Line, 5x7 Dots
	send_LCD_Cmd(0x06);		// Auto increment cursor (shift cursor to right)
	send_LCD_Cmd(0x0C);		// Display on, cursor off
	send_LCD_Cmd(0x01);		// Clear display screen
	send_LCD_Cmd(0x80);		// Force cursor to the beginning (1st line)
//	send_LCD_Cmd(0x18);	
}
void usdelay(int time){for (i = 0; i < time*4; i++);}
void PortE_as_Output(void)
{
	SYSCTL->RCGCGPIO |= 0x10;	// 01 0000
	for (j =0; j < 3 ; j++)	 // wait at least 3 clock cyles
	GPIOE->AFSEL &= ~(0x02);//(0000 0010) Disable for PB0-7 (1111 1111)
	GPIOE->DEN |= 0x02; 	 // Digital enable for PB0-7 (1111 1111)
	GPIOE->DIR |= 0x02;   // Pe1 as output (0000 0001)
}
void PortE_as_analog_Input_Init(void)
{

	// Enable the clock for Port A, B and E
	//SYSTCL_RCGC_R |= SYSTCL_RCGC_GPIOPABE;
	//DELAY(3);
	SYSCTL->RCGCGPIO |= 0x10;
	usdelay(3);
	//GPIO_PORTE_AMSEL_R &= ~GPIO_PORTE_MASK;
	GPIOE->AMSEL &= ~(0x02);//Disable_analog_mode_select_on PE2 1st time for digital write on pe1
	// Step 4: Enable digital pin functionaliy
//	GPIO_PORTE_DEN_R |= GPIO_PORTE_MASK; // Digital enable disable on pe2
	GPIOE->DEN |= 0x02;

	// Step 5: Enable digital pins as an output
//	GPIO_PORTE_DIR_R |= GPIO_PORTE_MASK;
	GPIOE->DIR |= 0x02;

 

	// potentiometer connected to GPIO pin PE2 (ANI_1)
//	GPIO_PORTE_DEN_R &= ~GPIO_PORTE_PIN2;//disable digital for analog
	GPIOE->DEN &= ~(0x04); //digital disable on pe2
	//GPIO_PORTE_AMSEL_R |= GPIO_PORTE_PIN2;
	GPIOE->AMSEL |= 0x04;//analog enable on pe2
	// Enable the clock for ADC0
	//SYSTCL_RCGC_ADC_R |= ADC0_CLOCK_ENABLE;
	SYSCTL->RCGCADC |= 0x01; //clock enable on adc 1
//	DELAY(3);
usdelay(3);
	//ADC_PERI_CONFIG_R |= 0x03;	// 250 Ksps
	ADC0->PP |= 0x03;// 500ksps
	//ADC_TRIGGER_MUX_R = 0xF000; // Continuous Sampling Mode on SS3
ADC0->EMUX |= 0xF000;// as there are 4 sample sequencer ss0,ss1,ss2,ss3 each having 4 bit to set the trigering mode here i select continous triger  mode for ss3
	// No sequence priority
//	ADC_SAMPLE_AVG_R |= 0x04; // 16x oversampling and then averaged
ADC0->SAC |= 0x04 ;// 2^4 == 16 means sample average of 16x(go upto 6 for 64x)
	// No voltage selection
	
//ADC_ACTIVE_SS_R = 0x08;	   // Configure ADC0 module for sequencer 3 it takes single input//error
ADC0->ACTSS |= 0x08;//adc sample sequencer input  chanel upto 4 i selected 1 input from ss3
//ADC_SS3_IN_MUX_R = 0x01;   // ANI_1 (PE2) is assigned to Input 0 of SS3
	ADC0->SSMUX3 |= 0x01;// ANI_1 (PE2) is assigned to Input 0 of SS3
	//ADC_SS3_CONTROL_R |= 0x02; // Sample is end of sequence
	ADC0->CTL |= 0x02;
}