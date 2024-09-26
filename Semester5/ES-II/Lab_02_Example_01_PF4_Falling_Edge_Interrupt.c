#include "TM4C123.h"                    // Device header

unsigned volatile long j;

void PF3_Digital_Output_Init();
void PF4_External_Interrupt_Input_Init();

int main()
{
	PF3_Digital_Output_Init();
	PF4_External_Interrupt_Input_Init();
	while(1){}
}

void GPIOF_Handler(){
	if((GPIOF->MIS&(1<<4))== (1<<4)){		// PF4
		GPIOF->DATA ^= 1<<3;
		GPIOF->ICR |= 1<<4;
	}
}

void PF3_Digital_Output_Init()
{
	SYSCTL->RCGCGPIO |= 1<<5;  // Step 1: Clock enable on PortF
	for (j =0; j < 3 ; j++)	  // at least 3 clock cyles
	GPIOF->AFSEL &= ~(1<<3); // Step 3: Disable alternate functionality on PortF
	GPIOF->DEN |= 1<<3; 	// Step 4: Enable digital pin functionaliy on PortF pin 3
	GPIOF->DIR |= 1<<3;    // Step 5: Set PortF pin 3 as an output pin
}

void PF4_External_Interrupt_Input_Init()
{
	SYSCTL->RCGCGPIO |= 1<<5; // Step 1: Clock enable on PortF
	for (j =0; j < 3 ; j++)	 // at least 3 clock cyles
	GPIOF->AFSEL &= ~(1<<4);// Step 3: Disable alternate functionality on PortF
	GPIOF->DEN |= 1<<4;    // Step 4: Enable digital pin functionaliy on PortF pin 4
	GPIOF->DIR &= ~(1<<4);// Step 5: Set PortF pin 4 as an input pin
	GPIOF->PUR |= 1<<4;	 // Use internal weak pull-up resistors
	// Step 6: Perform GPIO Port Interrupt Configurations
	GPIOF->IS &= ~(1<<4);		// Enable edge-triggered interrupt at PF4
	GPIOF->IEV &= ~(1<<4);		// Enable falling edge triggered interrupt at PF4
	GPIOF->IBE &= ~(1<<4);		// Disable both edge triggered interrupt
	GPIOF->IM |= 1<<4;			// PF0 interrupt is not masked
	GPIOF->ICR |= 1<<4; 		// Clearing interrupt flags (in RIS & MIS)
	
	// Step 7: NVIC Configurations
	// Set the NVIC Register for Enabling the EXTI for GPIOF
	NVIC->ISER[0] |= (1<<30);			// GPIO PortF IRQ # = 30
	// Set priority for the GPIOF Interrupt
	//NVIC->IPR
	NVIC->IPR[7] = 0x00600000;	// Priority 3
}