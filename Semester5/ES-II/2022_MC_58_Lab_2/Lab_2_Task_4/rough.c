
#include "TM4C123.h"                	// Device header

unsigned volatile long j;

void PF3_Digital_Output_Init();
void PF4_External_Interrupt_Input_Init();

int main()
{
	PF3_Digital_Output_Init();
	PF4_External_Interrupt_Input_Init();
	while(1)
	{
	}
}

void GPIOF_Handler(void)
{
    // Check if interrupt was triggered by PF4 
    if((GPIOF->MIS & 0x10) == 0x10)
    {
        if((GPIOF->DATA & 0x10) == 0x10)  // PF4 is high, Rising Edge
        {
            GPIOF->DATA |= (1<<3);  	 // Turn ON Green LED
        }
        else  							 // PF4 is low, Falling Edge
        {
            GPIOF->DATA &= ~(1<<3);      // Turn OFF Green LED
        }

        GPIOF->ICR |= 0x10;
    }
}

void PF3_Digital_Output_Init()
{
	// Clock enable on PortF
	SYSCTL->RCGCGPIO |= 1<<5;
	for (j =0; j < 3 ; j++)	    // at least 3 clock cyles
	
	// Disable alternate functionality on PortF
	GPIOF->AFSEL &= ~(1<<3);
	
	// Enable digital pin functionaliy on PF3
	GPIOF->DEN |= 1<<3;
	
	// Set PF3 as an output pin
	GPIOF->DIR |= 1<<3;
}

void PF4_External_Interrupt_Input_Init()
{
	// Clock enable on PortF
	SYSCTL->RCGCGPIO |= 1<<5;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Disable alternate functionality on PortF
	GPIOF->AFSEL &= ~0x10;

	// Enable digital pin functionaliy on PF4
	GPIOF->DEN |= 0x10;
	
	// Set PF0 & PF4 as an input pin
	GPIOF->DIR &= ~0x10; 	    // PF4 as input
	GPIOF->PUR |= 0x10;	        // Use internal weak pull-up resistors
	
	// Perform GPIO Port Interrupt Configurations
	GPIOF->IS &= ~0x10;			// Enable edge-triggered interrupt at PF4
	GPIOF->IBE |= 0X10;		    // Enable both edge triggered interrupt at PF4
	GPIOF->IM |= 0x10;			// PF4 interrupt are masked
	GPIOF->ICR |= 0x10; 		// Clearing interrupt flags (in RIS & MIS)
	
	// NVIC Configurations

	// Set the NVIC Register for Enabling the EXTI for GPIOF
	NVIC->ISER[0] |= (1<<30);	// GPIO PortF IRQ = 30

	// Set priority for the GPIOF Interrupt
	NVIC->IPR[7] = 0x00200000;	// Priority 1
}