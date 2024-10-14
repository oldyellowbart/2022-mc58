/* 
-----------------------------------------------------------------------
** Interfacing details---------
RS -> PD0
R/W -> PD1
EN -> PD2
D0-D7 -> PB0-PB7
PE2 - >adc_in 0-3.3v 
PE1 -> pwm generation
pa0 concadenate
-----------------------------------------------------------------------*/
// Libraries
#include "TM4C123.h"
#include <string.h>

// Global Variables
unsigned volatile long j;
unsigned volatile long i;
unsigned int adc_data = 48;
unsigned volatile long pwm =0;
int test = 38;
// Function Declarations
void PD0_2_as_Output_Init(void);
void PortB_as_Output_Init(void);
void PortE_as_Output(void);
void PA7_as_Output_Init(void);
void PortE_as_analog_Input_Init(void);
void write_LCD_Char(int data);
void send_LCD_Cmd(unsigned char cmd);
void write_LCD_Str(unsigned char *str);
void LCD_Init(void);
void usdelay(int time);

// Main Function
int main(){
	PD0_2_as_Output_Init();
	PortB_as_Output_Init();
	PortE_as_Output();
	PA7_as_Output_Init();
	PortE_as_analog_Input_Init();
	LCD_Init();
	PortE_as_Output();
	//write_LCD_Str("Hello_world123");
//write_LCD_Char(02);	
	while(1){

		test = test+1;
		write_LCD_Str("Hello_world12345678");
		//write_LCD_Char(test);
		//GPIOE->DATA |= 0x02;
		//GPIOB->DATA |= (0x41);
		usdelay(1000000);
		
		//GPIOE->DATA &= ~(0x02);
		
		//adc_data = ( ADC0->SSFIFO3 & 0xFFF);//ADC_SS3_FIFO_DATA_R & 0xFFF
		//pwm = ((adc_data * 0.24)+1000);//122.07 for 1sec

			// Turn PE1 High
			//GPIO_PORTE_DATA_R |= GPIO_PORTE_MASK;
			//GPIOE->DATA |= 0x02;//on the pin1
		//usdelay(pwm); // Delay required to set servo at 0 degrees

			// Turn PE1 Low
			////GPIO_PORTE_DATA_R &= ~GPIO_PORTE_MASK;
			//GPIOE->DATA &= ~(0x02);//of the pin1
		//usdelay(20000-pwm);
		send_LCD_Cmd(0x01);
		//GPIOB->DATA &= (0x00);
	}	
}

// Function Defintions
void PD0_2_as_Output_Init(void){
	SYSCTL->RCGCGPIO|=0x08;// Step 1: Clock enable on PortD 00 1000
	for (j =0; j < 3 ;j++)// wait at least 3 clock cyles
	GPIOD->AFSEL&=(0x07);// Step 3: Disable alternate functionality on PortD(0000 0111)
	GPIOD->DEN |= 0x07; // Step 4: Enable digital pin functionaliy on PortD pin 0-2 (0000 0111)
	GPIOD->DIR |= 0x07;// Step 5: Set PortD pin 0-2 as an output pin(0000 0111)
}
void PortB_as_Output_Init(void){
	SYSCTL->RCGCGPIO|=0x02;// Step 1: Clock enable on PortB 00 0010
	for (j =0;j< 3 ; j++)	// wait at least 3 clock cyle
	GPIOB->AFSEL&=(0x00);// Step 3: Disable alternate functionality on PortB (0000 0000)
	GPIOB->DEN |= 0xFF; // Step 4: Enable digital pin functionaliy on PortB pin 0-7(1111 1111)
	GPIOB->DIR |= 0xFF;// Step 5: Set PortB pin 0-7 as an output pin(1111 1111)
}
void PA7_as_Output_Init(void){
	SYSCTL->RCGCGPIO|=0x01;// Step 1: Clock enable on PortD 00 1000
	for (j =0; j < 3 ;j++)// wait at least 3 clock cyles
	GPIOA->AFSEL&= ~(0x80);// Step 3: Disable alternate functionality on PortD(0000 0111)
	GPIOA->DEN |= 0x80; // Step 4: Enable digital pin functionaliy on PortD pin 0-2 (0000 0111)
	GPIOA->DIR |= 0x80;// Step 5: Set PortD pin 0-2 as an output pin(0000 0111)
}
void write_LCD_Char(int data)
{
	// Interfacing details---------------
	// RS -> PD0		R/W -> PD1			EN -> PD2			D0-D7 -> PB0-PB7
	GPIOA->DATA &= (0x00);
	GPIOB->DATA &= (0x00);			 // Erase previous data
	GPIOB->DATA |= data;			  // Step 1. Pass data to 8-bit lines of LCD (D0-7)
	GPIOA->DATA |= (data & 0x80);
	GPIOD->DATA &= ~(0x02);		 // Step 2. Enable write operation on LCD (R/W bit = 0) PD1 = 0000 0010
	GPIOD->DATA |= 0x01;	    // Step 3. Select Data Register of LCD (RS bit = 1)	PD0 = 0000 0001
	GPIOD->DATA |= 0x04;	   // Step 4. Enable LCD operation by sending high to low pulse on Enable pin	PD2 = 0000 0100
	for (j =0;j<10000;j++); // some milisecond delay
	GPIOD->DATA &= ~(0x04);// disabled the enable for next time PD2 = 0000 0100
}

void send_LCD_Cmd(unsigned char cmd){
	// Interfacing details---------------
	// RS -> PD0		R/W -> PD1			EN -> PD2			D0-D7 -> PB0-PB7
	
	GPIOB->DATA = cmd;
  GPIOA->DATA |= (cmd & 0x80);	// Step 1. Pass command to 8-bit lines of LCD (D0-7)
	GPIOD->DATA &= ~(0x02); // Step 2. Enable write operation on LCD (R/W bit = 0) PD1 = 0000 0010
	GPIOD->DATA &= ~(0x01);// Step 3. Select Instructioin Register of LCD (RS bit = 0)	PD0 = 0000 001
	GPIOD->DATA |= 0x04;	// Step 4. Enable LCD operation by sending high to low pulse on Enable pin 	PD2 = 0000 0100
	for (j =0; j < 10000 ; j++);// some milisecond delay
	GPIOD->DATA &= ~(0x04); 	// disabled for next timePD2 = 0000 0100
}
void write_LCD_Str(unsigned char *str){
	unsigned int i, len;
	len = strlen(str);
	for (i = 0; i < len; i++){write_LCD_Char(str[i]);}
	//write_LCD_Char(str);
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
	ADC0->PP |= 0x03;// 250ksps
	ADC0->EMUX |= 0xF000;// as there are 4 sample sequencer ss0,ss1,ss2,ss3 each having 4 bit to set the trigering mode here i select continous triger  mode for ss3
	// No sequence priority
	ADC0->SAC |= 0x04 ;// 2^4 == 16 means sample average of 16x(go upto 6 for 64x)
	// No voltage selection
	ADC0->ACTSS |= 0x08;//adc sample sequencer input  chanel upto 4 i selected 1 input from ss3 
	ADC0->SSMUX3 |= 0x01;// ANI_1 (PE2) is assigned to Input 0 of SS3
	ADC0->CTL |= 0x02;//ADC_SS3_CONTROL_R |= 0x02; // Sample is end of sequence
}