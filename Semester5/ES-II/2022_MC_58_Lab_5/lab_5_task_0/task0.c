

#include "TM4C123.h"

// Global Variables
unsigned volatile long j;
unsigned volatile long ik;
// Function Declarations
void PORTD_INPUT_INIT(void);
void PortB_as_Output_Init(void);
void usdelay(unsigned long time);
unsigned char CC_7seg_Table[10] ={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F} ;
int i = 0;
int count1 = 0;
// Main Function
int main(){
	PortB_as_Output_Init();
	PORTD_INPUT_INIT();
//	GPIOB->DATA |= 0xFF;
	while(1){
	//	GPIOB->DATA |=  CC_7seg_Table[7];
		//usdelay(100000);
		//GPIOB->DATA &= ~(0xFF);
	while(GPIOD->DATA == 0x01 )     // rising edge 
		{	
			for (j =0; j<3 ; j++);
			if(GPIOD->DATA != 0x01 )
			{
				i++;
				for (j =0; j< 3; j++);
			
				if(i>9)
				{
					i = 0;
				}
				GPIOB->DATA = CC_7seg_Table[i];
				for (j =0; j< 3 ; j++);
			}	
		}
		
		if(GPIOD->DATA != 0x00 )    // falling edge 
	 {
		 for (j =0; j< 3 ; j++);
		 if(GPIOD->DATA == 0x00 )
			{
		///		i--;
			for (j =0; j< 3 ; j++);
			
				if(i<0)
				{
					i = 9;
				}
		
			GPIOB->DATA = CC_7seg_Table[i];
			usdelay(500000);	
			}
		}
	}  
}
		


// Function Defintions

void PORTD_INPUT_INIT(void)
{
	SYSCTL->RCGCGPIO |= 0x08;	// 00 0010
	for (j =0; j < 3 ; j++)		// wait at least 3 clock cyles
	
	// Step 2: APB is selected for PortB by selecting
	// 0x40005000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on PortB
	GPIOF->AFSEL &= ~(0x01); // Disable for PB0-7 (1111 1111)
	
	// Step 4: Enable digital pin functionaliy on PortB pin 0-7
	GPIOF->DEN |= 0x01; // Digital enable for PB0-7 (1111 1111)
	
	// Step 5: Set PortB pin 0-7 as an output pin
	GPIOF->DIR &= (0x01); // PB0-7 as output (0001 0000)
}
void PortB_as_Output_Init(void){
	// Step 1: Clock enable on PortB
	SYSCTL->RCGCGPIO |= 0x22;	// 10 0010
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


void usdelay(unsigned long time){for (ik = 0; ik*4 < time; ik++){}}
