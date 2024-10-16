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

-----------------------------------------------------------------------
Written by Shujat Ali (engrshujatali@gmail.com) on 18-Aug-2024.
Compiler and IDE used: Keil uVision 5.38 */

// Libraries
#include "TM4C123.h"
#include <string.h>

// Global Variables
unsigned volatile long j;
unsigned volatile long ik;
// Function Declarations
void PORTF_OUTPUT_INIT(void);
void PD0_2_as_Output_Init(void);
void PortB_as_Output_Init(void);
void write_LCD_Char(unsigned char data);
void send_LCD_Cmd(unsigned char cmd);
void write_LCD_Str(unsigned char *str);
void LCD_Init(void);
void usdelay(unsigned long time);
int count = 0;
int count1 = 0;
// Main Function
int main(){
	PD0_2_as_Output_Init();
	PortB_as_Output_Init();
	PORTF_OUTPUT_INIT();
	LCD_Init();
	GPIOB->DATA = 0xFF;
	
	while(1){
	//write_LCD_Str("SW2 Pressed");
	if(GPIOF->DATA == 0x01)
{
count++;
write_LCD_Str("SW1 Pressed");
send_LCD_Cmd(0xC0);
write_LCD_Str(count);
}
		else if(GPIOF->DATA == 0x10)
{
count++;
write_LCD_Str("SW2 Pressed");
send_LCD_Cmd(0xC0);
write_LCD_Str(count1);
}
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
void PORTF_OUTPUT_INIT(void)
{
	SYSCTL->RCGCGPIO |= 0x20;	// 00 0010
	for (j =0; j < 3 ; j++)		// wait at least 3 clock cyles
	
	// Step 2: APB is selected for PortB by selecting
	// 0x40005000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on PortB
	GPIOF->AFSEL &= ~(0x11); // Disable for PB0-7 (1111 1111)
	
	// Step 4: Enable digital pin functionaliy on PortB pin 0-7
	GPIOF->DEN |= 0x11; // Digital enable for PB0-7 (1111 1111)
	
	// Step 5: Set PortB pin 0-7 as an output pin
	GPIOF->DIR &= (0x11); // PB0-7 as output (1111 1
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
	usdelay(10);
	// Step 2. Enable write operation on LCD (R/W bit = 0)
	GPIOD->DATA &= ~(0x02);		// PD1 = 0000 0010
	
	// Step 3. Select Instructioin Register of LCD (RS bit = 0)
	GPIOD->DATA &= ~(0x01);			// PD0 = 0000 0001
	
	// Step 4. Enable LCD operation by sending high to low pulse on Enable pin
	GPIOD->DATA |= 0x04; 			// PD2 = 0000 0100
	usdelay(500);		// some milisecond delay
	GPIOD->DATA &= ~(0x04); 	// PD2 = 0000 0100
}
void write_LCD_Str(unsigned char *str){
	unsigned int i, len;
	len = strlen(str);
	
	for (i = 0; i < len; i++){
		write_LCD_Char(str[i]);
		usdelay(10);
		
	}
}

void LCD_Init(void){
	send_LCD_Cmd(0x38);		// Function Set: 8-bit, 2 Line, 5x7 Dots
	send_LCD_Cmd(0x06);		// Auto increment cursor (shift cursor to right)
	send_LCD_Cmd(0x0C);		// Display on, cursor off
	send_LCD_Cmd(0x01);		// Clear display screen
	send_LCD_Cmd(0x80);		// Force cursor to the beginning (1st line)
	//send_LCD_Cmd(0x18);	
}
void usdelay(unsigned long time){for (ik = 0; ik*4 < time; ik++){}}