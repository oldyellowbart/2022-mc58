/* Name: Muhtashim Butt
   Reg No# 2022-MC-44
	 Folder Name: 2022_MC_44_Lab_2
	 File name: Lab_2_Task_4   */
	 
// Libraries
#include "TM4C123.h"

// GLobal Variable
unsigned volatile long j;

// Function Declaration
void PF3_Digital_Output_Init();
void PF4_External_Interrupt_Input_Init();

// Main Function
int main()
{
	// Function Calling
	PF3_Digital_Output_Init();
	PF4_External_Interrupt_Input_Init();
	while(1)
	{
	}
}

// Function Initialization
void GPIOF_Handler(void)
{
	  // Handle PF4 (Switch 1) interrupt
    if((GPIOF->MIS & 0x10) == 0x10)
    {
        if((GPIOF->DATA & 0x10) == 0x10)  
        {
            GPIOF->DATA |= (0x08);  	    
        }
        else  							              
        {
            GPIOF->DATA &= ~(0x08);       
        }

        GPIOF->ICR |= 0x10;
    }
}

// Function Initialization
void PF3_Digital_Output_Init()
{
	// Clock enable on PortF
	SYSCTL->RCGCGPIO |= 0x20;
	for (j =0; j < 3 ; j++)	    // at least 3 clock cyles
	
	// Disable alternate functionality on PortF
	GPIOF->AFSEL &= ~(0x08);
	
	// Enable digital pin functionaliy on PF3
	GPIOF->DEN |= 0x08;
	
	// Set PF3 as an output pin
	GPIOF->DIR |= 0x08;
}

void PF4_External_Interrupt_Input_Init()
{
	// Clock enable on PortF
	SYSCTL->RCGCGPIO |= 0x20;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Disable alternate functionality on PortF
	GPIOF->AFSEL &= ~(0x10);

	// Enable digital pin functionaliy on PF4
	GPIOF->DEN |= 0x10;
	
	// Set PF0 & PF4 as an input pin
	GPIOF->DIR &= ~(0x10); 	    // PF4 as input
	GPIOF->PUR |= 0x10;	        // Use internal weak pull-up resistors
	
	// Perform GPIO Port Interrupt Configurations
	GPIOF->IS &= ~0x10;			// Edge-sensitive
	GPIOF->IBE |= 0X10;		  // Enable both edge triggered interrupt at PF4
	GPIOF->IM |= 0x10;			// PF4 interrupt is not masked
	GPIOF->ICR |= 0x10; 		// Clearing interrupt flags 
	
	// Step 7: NVIC Configurations
	// Set the NVIC Register for Enabling the EXTI for GPIOF
	NVIC->ISER[0] |= (1<<30);	// GPIO PortF IRQ# = 30

	// Set priority for the GPIOF Interrupt
	NVIC->IP[7] = 0x00200000;	// Priority 1
}