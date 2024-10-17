/* 
-----------------------------------------------------------------------
** Interfacing details---------
RS   -> PA2
R/W  -> PA3
EN   -> PA4
D0-7 -> PB0-PB7
PE2  -> ADC0_in 0-3.3v 
PE1  -> pwm generation
PE0  -> ADC1,SS0,250Ks,Current sensor
-----------------------------------------------------------------------*/
// Libraries
#include "TM4C123.h"
#include <string.h>
#include <stdio.h>
// Global Variables
unsigned volatile long i;
unsigned volatile long j;
unsigned volatile long k;
float current;
unsigned int adc_data = 48;
unsigned volatile long pwm =0;
unsigned int adc_data2 = 48;
unsigned volatile long pwm2 =0;
int n[10] = {48,49,50,51,52,53,53,54,55,56};//Array for numbers 
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
void set_cursor(int column,int row);
void display(int adc1 , int pwm1,int adc2);
int calculateLength(int number);
void msdelay(int time1);
void pwm_generate(int pwm_value);
// Main Function
int main()
	{
	PortB_as_Output_Init();
	//PortE_as_Output();
	PortA23_as_Output_Init();
	PortE_as_analog_Input_Init();
	LCD_Init();
			//write_LCD_Str("Pulse ");
		//set_cursor(1);
	while(1)
		{	
	  adc_data = ( ADC0->SSFIFO3 & 0xFFF);//ADC_SS3_FIFO_DATA_R & 0xFFF
		pwm = (((adc_data * 0.263)+1000));//122.07 for 1sec
		adc_data2 = ( ADC1->SSFIFO0 & 0xFFF);//ADC_SS0_FIFO_DATA_R & 0xFFF
			current = (((908-(4084-adc_data2))*1000)/79)-130;
		pwm_generate(pwm);
		display(adc_data,pwm,(current));
		}	
	}

// Function Defintions
void PortB_as_Output_Init(void){
	SYSCTL->RCGCGPIO |= 0x02;// Step 1: Clock enable on PortB 00 0010
	for (j =0;j< 3 ; j++)	   // Step 2: wait at least 3 clock cyle
	GPIOB->AFSEL &= (0x00);  // Step 3: Disable alternate functionality on PortB (0000 0000)
	GPIOB->DEN |= 0xFF;      // Step 4: Enable digital pin functionaliy on PortB pin 0-7(1111 1111)
	GPIOB->DIR |= 0xFF;			 // Step 5: Set PortB pin 0-7 as an output pin(1111 1111)
}
void PortA23_as_Output_Init(void){
	SYSCTL->RCGCGPIO |= 0x01;// Step 1: Clock enable on PortB 00 0010
	for (j =0;j< 3 ; j++)	   // Step 2: wait at least 3 clock cyle
	GPIOA->AFSEL &= (0x1C);  // Step 3: Disable alternate functionality on PortB (0000 1100)
	GPIOA->DEN |= 0x1C; 		 // Step 4: Enable digital pin functionaliy on PortB pin 0-7(1111 1111)
	GPIOA->DIR |= 0x1C;			 // Step 5: Set PortA pin 0-1 as an output pin(0000 1100)
}

void PortE_as_Output(void)
{
	//SYSCTL->RCGCGPIO |= 0x10;	// 01 0000
	for (j =0; j < 3 ; j++)	// wait at least 3 clock cyles
	GPIOE->AFSEL &= ~(0x02);//(0000 0010) Disable for PB0-7 (1111 1111)
	GPIOE->DEN |= 0x02; 	  // Digital enable for PB0-7 (1111 1111)
	GPIOE->DIR |= 0x02;     // Pe1 as output (0000 0001)
}
void PortE_as_analog_Input_Init(void)
{
	SYSCTL->RCGCGPIO |= 0x10;//Step1 : Enable the clock for Port  E
	usdelay(3);							 //Step2 : Delay to to enable clock 
	GPIOE->AFSEL |= (0x05);// alternate on pe0,pe2
	GPIOE->AMSEL &= ~(0x02); //Step3 : Disable_analog_mode_select_on PE1
	GPIOE->DEN |= 0x02;			 //Step4 : Enable digital pin functionaliy
	GPIOE->DIR |= 0x02;			 //Step5 : Enable digital pins as an output pe1
	GPIOE->DIR &= ~(0x05); //input
	GPIOE->DEN &= ~(0x05); 	 //Step6 : digital disable on pe2(potentiometer),pe0
	GPIOE->AMSEL |= 0x05;		 //Step7 : analog enable on pe2,pe0
	SYSCTL->RCGCADC |= 0x03; //Step8 : clock enable on adc 1//checkj
	usdelay(3);							 //Step9 : Delay to to enable clock
	ADC0->ACTSS &= (0x00);// sample sequensor are disabled
	ADC0->EMUX |= 0xF000;//slect the trigger event for ss3
	ADC0->SSMUX3 |= 0x01;//sample sequencor input selectiom
	ADC0->PP |= 0x03;				 //Step10: 250ksps rate select
	ADC0->SSPRI |= 0x2000;												 //Step12: No sequence priority
	ADC0->SAC |= 0x08 ;//Step13: 2^4 == 16 means sample average of 16x(go upto 6 for 64x)
	ADC0->CTL |= 0x01;       //Step17: ADC_SS3_CONTROL_R |= 0x02; // Sample is end of sequence
	ADC0->SSCTL3 |= 0X02;
	ADC0->ACTSS |= (0x08);	 //turn on
	
	ADC1->ACTSS &= (0x00);// sample sequensor are disabled
	ADC1->EMUX |= 0x000F;		 // set the trigering event here i select continous triger  mode for ss0
	ADC1->SSMUX0 |= 0x03;
	ADC1->PP |= 0x03;				 //Step10: 250ksps rate select
	ADC1->SSPRI |= 0x0003;		 //Step12:  sequence priority low
	ADC1->SAC |= 0x08 ;			 //Step13: 2^4 == 16 means sample average of 16x(go upto 6 for 64x)
	ADC1->CTL |= 0x01;
	ADC1->SSCTL0 |= 0X02;
	ADC1->ACTSS |= 0x01;//turn on ss0
}
void write_LCD_Char(int data)
{
	// Interfacing details---------------
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA &= (0x00); // Step 1. Erase previous data
	GPIOB->DATA |= data;	 // Step 2. Pass data to 8-bit lines of LCD (D0-7)
	GPIOA->DATA &= ~(0x08);// Step 3. Enable write operation on LCD (R/W bit = 0) PA3 = 0000 1000
	GPIOA->DATA |= 0x04;	 // Step 4. Select Data Register of LCD (RS bit = 1)	PA2 = 0000 1000
	GPIOA->DATA |= 0x10;	 // Step 5. Enable LCD operation by sending high to low pulse on Enable pin	PA4 = 0001 0000
	for (j =0;j<500;j++);// Step 6. some milisecond delay to match lcd speed of writing data
	GPIOA->DATA &= ~(0x10);// Step 7. Disabled the enable for next time PA4 = 0001 0000
}

void send_LCD_Cmd(unsigned char cmd){
	// Interfacing details---------------
	// RS -> A2		R/W -> PA3			EN -> PA4			D0-D7 -> PB0-PB7
	GPIOB->DATA = cmd;		 			// Step 1. Pass command to 8-bit lines of LCD (D0-7)
	GPIOA->DATA &= ~(0x08);			// Step 2. Enable write operation on LCD (R/W bit = 0) PA3 = 0000 1000
	GPIOA->DATA &= ~(0x04);			// Step 3. Select Instructioin Register of LCD (RS bit = 0)	PA2 = 0000 0100
	GPIOA->DATA |= 0x10;	 			// Step 4. Enable LCD operation by sending high to low pulse on Enable pin 	PA4 = 0001 0000
	for (j =0; j <500 ; j++);    // Step 5. some milisecond(10us) delay to match lcd speed of writing data
	GPIOA->DATA &= ~(0x10); 	  // Step 6. Disabled for next time PA4 = 0001 0000
}
void write_LCD_Str(unsigned char *str)//Function to print String
	{
	unsigned int i, len;
	len = strlen(str);
	for (i = 0; i < len; i++){write_LCD_Char(str[i]);}
	}

void LCD_Init(void)
	{
	send_LCD_Cmd(0x38);//Step1: Function Set: 8-bit, 2 Line, 5x7 Dots
	send_LCD_Cmd(0x06);//Step2: Auto increment cursor (shift cursor to right)
	send_LCD_Cmd(0x0C);//Step3: Display on, cursor off
	send_LCD_Cmd(0x01);//Step4: Clear display screen
	send_LCD_Cmd(0x80);//Step5: Force cursor to the beginning (1st line)
 // send_LCD_Cmd(0x1C);//Step6: Shift text to right	
}

int calculateLength(int number)// Function to calculate length of the given number
	{
    int length = 0;
    if (number == 0) return 1; // Special case for 0
    while (number != 0){number /= 10;length++;}
    return length;
	}
void write_LCD_number(int number)// Function to convert an integer to its individual digits and display on LCD
	{
    int length = calculateLength(number);
    char digits[length + 1]; // Array to hold the individual digits as characters
    for (int i = length - 1; i >= 0; i--)// Extract each digit from the end
		{
        digits[i] = (number % 10) + '0'; // Convert digit to character
        number /= 10;
    }
    digits[length] = '\0'; // Null terminator
    for (int i = 0; i < length; i++){write_LCD_Char(digits[i]);}// Write each digit to the LCD
	
}
void usdelay(int time){for (i = 0; i < time*4; i++);}
void msdelay(int time1){for (i = 0; i < time1*4000; i++);}

void set_cursor(int column,int row)//sets the cursor
{
	if(column == 1){send_LCD_Cmd(0x80);}
	if(column == 2){send_LCD_Cmd(0xC0);}
	if(column == 3){send_LCD_Cmd(0x94);}
	if(column == 4){send_LCD_Cmd(0xD4);}
for(k=0 ; k < row ; k++)
{send_LCD_Cmd(0x14);}
}

void display(int adc1 , int pwm1,int adc2)
{			send_LCD_Cmd(0x01);
			usdelay(1400);// delay for 1 charachter to write is 400ms
			//set_cursor(1,0);
			//write_LCD_Str("adc1 =");
			//set_cursor(1,12);
		  //write_LCD_number(adc1);
			//set_cursor(2,0);
			//write_LCD_Str("pwm_onn =");
			//set_cursor(2,12);
		 // write_LCD_number(pwm1);
			//set_cursor(3,0);
			//write_LCD_Str("pwm_off =");
			//set_cursor(3,12);
			//write_LCD_number(15500-pwm1);
			set_cursor(4,0);
			write_LCD_Str("adc2 =");
			set_cursor(4,12);
			write_LCD_number(adc2);	
}

void pwm_generate(int pwm_value)
{
		GPIOE->DATA |= 0x02;// Turn PE1 High
		usdelay(pwm_value); // Delay required to set servo at 0 degrees
		GPIOE->DATA &= ~(0x02);// Turn PE1 Low
		usdelay(15500-pwm_value);//
		
}