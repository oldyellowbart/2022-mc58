/*Task 2.4: Configure PF4 as an interrupt (both edges)
with interrupt priority of 1 and write its ISR to turn
on-board green LED on rising edge and turn it OFF when
falling edge is detected.*/
#include "TM4C123.h"          
unsigned volatile long j;
void PF3_Digital_Output_Init();
void PF4_External_Interrupt_Input_Init();
void msDelay(int dValue);
	void msDelay(int dValue){
	for (j =0; j < 4000*dValue; j++);
}
int main()// Main Function
	{
    PF3_Digital_Output_Init();             // Initialize PF3 (LED)
    PF4_External_Interrupt_Input_Init(); // Initialize PF0 (Switch 2)
    while(1)
{
	
}
	}
void GPIOF_Handler()
	{
    if((GPIOF->MIS & (0x10)) == (0x10))// Handle PF4 (Switch 1) interrupt - Turn ON LED (PF3)
			{  // PF4 interrupt occurred
				if(GPIOF->DATA & 0x10) {  // Check if rising edge
            GPIOF->DATA &= ~(0x08);  // Turn on green LED (PF3)
        GPIOF->ICR |= 0x10;
				} else {  // Falling edge
            GPIOF->DATA |= 0x08;  // Turn off green LED (PF3)
        }
				GPIOF->ICR |= 0x10;  // Clear interrupt flag for PF4
			}
   
	}
void PF3_Digital_Output_Init()
{
    SYSCTL->RCGCGPIO |= 0x20; // Step 1: Clock enable on PortF
    for (j = 0; j < 3; j++)  // at least 3 clock cycles delay
    GPIOF->AFSEL &= ~(0x08);// Step 2: Disable alternate function on PF3
    GPIOF->DEN |= 0x08;    // Step 3: Enable digital function on PF3
    GPIOF->DIR |= 0x08;   // Step 4: Set PF3 as an output pin
}

void PF4_External_Interrupt_Input_Init(){
    // Step 1: Clock enable on PortF
    SYSCTL->RCGCGPIO |= 0x20;
    for (j = 0; j < 3; j++)       // at least 3 clock cycles delay
    // Step 2: Disable alternate function on PF4
    GPIOF->AFSEL &= ~(0x10);
    // Step 3: Enable digital function on PF4
    GPIOF->DEN |= 0x10;            // Digital enable for PF4
    // Step 4: Set PF4 as input pin
    GPIOF->DIR &= ~(0x10);         // PF4 as input
    GPIOF->PUR |= 0x10;            // Enable pull-up resistor on PF4
    // Step 5: Configure PF4 for falling edge interrupt
    GPIOF->IS &= ~(0x10);          // Edge-sensitive // 
    GPIOF->IEV &= ~(0x10);         // Falling edge
    GPIOF->IM |= 0x10;             // mask interrupt for PF4 intrept management port
    GPIOF->ICR |= 0x10;            // Clear interrupt flag   
    GPIOF->IBE |= 0x10;
	// Step 6: NVIC configuration for Port F interrupt (IRQ 30)
    NVIC->ISER[0] |= (1<<30);      // Enable IRQ for PortF esception number
    NVIC->IPR[7] = 0x00600000;      // Set priority 3 for PortF interrupt 0000 0000 22,23,24
}