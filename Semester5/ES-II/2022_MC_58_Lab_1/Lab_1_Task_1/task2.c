/* 
-----------------------------------------------------------------------
** Interfacing details---------
RS -> PD0,pa2
R/W -> PD1,pa3
EN -> PD2
D0-D7 -> PB0-PB7
PE2 - >adc_in 0-3.3v 
PE1 -> pwm generation
pa0 concadenate
-----------------------------------------------------------------------*/
// Libraries
#include "TM4C123.h"
#include <string.h>
#include <stdio.h>
// Global Variables
unsigned volatile long j;
unsigned volatile long i;
unsigned volatile long loop1;
unsigned int adc_data = 48;
unsigned volatile long pwm =0;
int test = 0;
int c0,c1,c2,c3,c4;
int n[10] = {48,49,50,51,52,53,53,54,55,56};
char pwm_char;
// Function Declarations
void PortA23_as_Output_Init(void);
void PortB_as_Output_Init(void);
void PortE_as_Output(void);
void PortE_as_analog_Input_Init(void);
void write_LCD_Char(int data);
void write_LCD_number(int number);
void send_LCD_Cmd(unsigned char cmd);
void write_LCD_Str(unsigned char *str);
void LCD_Init(void);
void usdelay(int time);
int calculateLength(int number);
void msdelay(int time1);
// Main Function
int main(){
	PortB_as_Output_Init();
	//PortE_as_Output();
	PortA23_as_Output_Init();
	PortE_as_analog_Input_Init();
	LCD_Init();
	//write_LCD_Str("Hell");
 // write_LCD_Char(65);	
	while(1){
		write_LCD_number(pwm);
	  //write_LCD_Char(n[test]);
		test = test+1;
		//write_LCD_Char(test);
		//GPIOE->DATA &= ~(0x02);
	  adc_data = ( ADC0->SSFIFO3 & 0xFFF);//ADC_SS3_FIFO_DATA_R & 0xFFF
		pwm = ((adc_data * 0.263)+1000);//122.07 for 1sec		
		GPIOE->DATA |= 0x02;// Turn PE1 High
		usdelay(pwm); // Delay required to set servo at 0 degrees
		GPIOE->DATA &= ~(0x02);// Turn PE1 Low
		usdelay(20850-pwm);
		send_LCD_Cmd(0x01);	
		}	
}

// Function Defintions
void PortB_as_Output_Init(void){
	SYSCTL->RCGCGPIO |= 0x02;// Step 1: Clock enable on PortB 00 0010
	for (j =0;j< 3 ; j++)	// wait at least 3 clock cyle
	GPIOB->AFSEL &= (0x00);// Step 3: Disable alternate functionality on PortB (0000 0000)
	GPIOB->DEN |= 0xFF; // Step 4: Enable digital pin functionaliy on PortB pin 0-7(1111 1111)
	GPIOB->DIR |= 0xFF;// Step 5: Set PortB pin 0-7 as an output pin(1111 1111)
}
void PortA23_as_Output_Init(void){
	SYSCTL->RCGCGPIO |= 0x01;// Step 1: Clock enable on PortB 00 0010
	for (j =0;j< 3 ; j++)	// wait at least 3 clock cyle
	GPIOA->AFSEL &= (0x1C);// Step 3: Disable alternate functionality on PortB (0000 1100)
	GPIOA->DEN |= 0x1C; // Step 4: Enable digital pin functionaliy on PortB pin 0-7(1111 1111)
	GPIOA->DIR |= 0x1C;// Step 5: Set PortA pin 0-1 as an output pin(0000 1100)
}
void usdelay(int time){for (i = 0; i < time*4; i++);}
void msdelay(int time1){for (i = 0; i < time1*4000; i++);}

void PortE_as_Output(void)
{
	//SYSCTL->RCGCGPIO |= 0x10;	// 01 0000
	for (j =0; j < 3 ; j++)	 // wait at least 3 clock cyles
	GPIOE->AFSEL &= ~(0x02);//(0000 0010) Disable for PB0-7 (1111 1111)
	GPIOE->DEN |= 0x02; 	 // Digital enable for PB0-7 (1111 1111)
	GPIOE->DIR |= 0x02;   // Pe1 as output (0000 0001)
}
void PortE_as_analog_Input_Init(void)
{

	// Enable the clock for Port A, B and E
	SYSCTL->RCGCGPIO |= 0x10;
	usdelay(3);
	GPIOE->AMSEL &= ~(0x02);//Disable_analog_mode_select_on PE2 1st time for digital write on pe1
	// Step 4: Enable digital pin functionaliy
	GPIOE->DEN |= 0x02;
	// Step 5: Enable digital pins as an output
	GPIOE->DIR |= 0x02;
	// potentiometer connected to GPIO pin PE2 (ANI_1)
	GPIOE->DEN &= ~(0x04); //digital disable on pe2
	GPIOE->AMSEL |= 0x04;//analog enable on pe2
	SYSCTL->RCGCADC |= 0x01; //clock enable on adc 1
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
void write_LCD_Char(int data)
{
	// Interfacing details---------------
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA &= (0x00);			 // Erase previous data
	GPIOB->DATA |= data;			  // Step 1. Pass data to 8-bit lines of LCD (D0-7)
	GPIOA->DATA &= ~(0x08);		 // Step 2. Enable write operation on LCD (R/W bit = 0) PD1 = 0000 0010
	GPIOA->DATA |= 0x04;	    // Step 3. Select Data Register of LCD (RS bit = 1)	PD0 = 0000 0001
	GPIOA->DATA |= 0x10;	   // Step 4. Enable LCD operation by sending high to low pulse on Enable pin	PD2 = 0000 0100
	for (j =0;j<10000;j++); // some milisecond delay
	GPIOA->DATA &= ~(0x10);// disabled the enable for next time PD2 = 0000 0100
}

void send_LCD_Cmd(unsigned char cmd){
	// Interfacing details---------------
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA = cmd;// Step 1. Pass command to 8-bit lines of LCD (D0-7)
	GPIOA->DATA &= ~(0x08); // Step 2. Enable write operation on LCD (R/W bit = 0) PD1 = 0000 0010
	GPIOA->DATA &= ~(0x04);// Step 3. Select Instructioin Register of LCD (RS bit = 0)	PD0 = 0000 001
	GPIOA->DATA |= 0x10;	// Step 4. Enable LCD operation by sending high to low pulse on Enable pin 	PD2 = 0000 0100
	for (j =0; j < 10000 ; j++);// some milisecond delay
	GPIOA->DATA &= ~(0x10); 	// disabled for next timePD2 = 0000 0100
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

int calculateLength(int number) {
    int length = 0;
    if (number == 0) return 1; // Special case for 0
    while (number != 0) {
        number /= 10;
        length++;
    }
    return length;
}

// Function to convert an integer to its individual digits and display on LCD
void write_LCD_number(int number) {
    int length = calculateLength(number);
    char digits[length + 1]; // Array to hold the individual digits as characters

    // Extract each digit from the end
    for (int i = length - 1; i >= 0; i--) {
        digits[i] = (number % 10) + '0'; // Convert digit to character
        number /= 10;
    }
    digits[length] = '\0'; // Null terminator

    // Write each digit to the LCD
    for (int i = 0; i < length; i++) {
        write_LCD_Char(digits[i]);
    }
}
