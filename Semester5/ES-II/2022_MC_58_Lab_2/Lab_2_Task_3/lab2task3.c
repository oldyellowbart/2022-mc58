/*Task 2.3: Write a C language program that toggles on-board green
LED at 1 Hz. Configure PF4 and PF0 as falling edge interrupts (priority = 7)
and program it to change it the blink rate of LED. Frequency of blinking LED
gets double when PF4 interrupt is active and it gets half when PF0 interrupt is active.*/
#include "TM4C123.h"          
unsigned volatile long j;
void PF3_Digital_Output_Init();
void PF4_External_Interrupt_Input_Init();
void PF0_External_Interrupt_Input_Init();
void msDelay(int dValue);
	void msDelay(int dValue){
	for (j =0; j < 4000*dValue; j++);
}
int main()// Main Function
	{
    PF3_Digital_Output_Init();             // Initialize PF3 (LED)
    PF4_External_Interrupt_Input_Init();  // Initialize PF4 (Switch 1)
    PF0_External_Interrupt_Input_Init(); // Initialize PF0 (Switch 2)
    while(1)
{
	GPIOF->DATA ^= (0x08);
	msDelay(500);
}
	}
void GPIOF_Handler()
	{
    if((GPIOF->MIS & (0x10)) == (0x10))// Handle PF4 (Switch 1) interrupt - Turn ON LED (PF3)
			{  // PF4 interrupt occurred
        GPIOF->DATA ^= (0x08);
				msDelay(1000);
       // GPIOF->ICR |= 0x10;  // Clear interrupt flag for PF4
			}
    // Handle PF0 (Switch 2) interrupt - Turn OFF LED (PF3)
    if((GPIOF->MIS & (0x01)) == (0x01))
			{  // PF0 interrupt occurred
        GPIOF->DATA ^= (0x08);
				msDelay(250);
        //GPIOF->ICR |= 0x01;   // Clear interrupt flag for PF0
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

void PF4_External_Interrupt_Input_Init()
{
    SYSCTL->RCGCGPIO |= 0x20; // Step 1: Clock enable on PortF
    for (j = 0; j < 3; j++)  // at least 3 clock cycles delay
    GPIOF->AFSEL &= ~(0x10);// Step 2: Disable alternate function on PF4
    GPIOF->DEN |= 0x10;    // Step 3: Enable digital function on PF4
    GPIOF->DIR &= ~(0x10);// Step 4: Set PF4 as input pin
    GPIOF->PUR |= 0x10;  // Enable pull-up resistor on PF4
    // Step 5: Configure PF4 for falling edge interrupt
    GPIOF->IS &= ~(0x10);  // Edge-sensitive  
    GPIOF->IEV &= ~(0x10);// Falling edge
    GPIOF->IM |= 0x10;   // mask interrupt for PF4 intrept management port
    GPIOF->ICR |= 0x10; // Clear interrupt flag
    // Step 6: NVIC configuration for Port F interrupt (IRQ 30)
    NVIC->ISER[0] |= (1<<30);  // Enable IRQ for PortF esception number
    NVIC->IPR[7] = 0x00E00000;// Set priority 7 for PortF interrupt 0000 0000 22,23,24
}
void PF0_External_Interrupt_Input_Init() 
	{
    SYSCTL->RCGCGPIO |= 0x20;   // Step 1: Clock enable on PortF
    for (j = 0; j < 3; j++)    // at least 3 clock cycles delay
    GPIOF->LOCK = 0x4C4F434B; // Step 2: Unlock PF0 (it is locked by default)
    GPIOF->CR |= 0x01;       // Enable commit for PF0
    GPIOF->AFSEL &= ~(0x01);// Step 3: Disable alternate function on PF0
    GPIOF->DEN |= 0x01;	   // Step 4: Enable digital function on PF0
    GPIOF->DIR &= ~(0x01);// Step 5: Set PF0 as input pin
    GPIOF->PUR |= 0x01;  // Enable pull-up resistor on PF0
    // Step 6: Configure PF0 for falling edge interrupt
    GPIOF->IS &= ~(0x01); // Edge-triggered
    GPIOF->IEV &= ~(0x10);  // faling edge
    GPIOF->IM |= 0x01;  // Unmask interrupt for PF0
    GPIOF->ICR |= 0x01;// Clear interrupt flag
	}