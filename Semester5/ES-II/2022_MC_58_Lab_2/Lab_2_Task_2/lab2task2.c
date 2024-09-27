/*Task 2.2: Redo the Task 2.1 and use PB0 instead of PF0 
pin for turning OFF the on-board green LED. You must
configure interrupt priority 2 and 3 for PortB and
PortF respectively.*/
#include "TM4C123.h"        
unsigned volatile long j;
void PF3_Digital_Output_Init();
void PF4_External_Interrupt_Input_Init();
void PB0_External_Interrupt_Input_Init();

// Main Function
int main()
	{
    PF3_Digital_Output_Init();        // Initialize PF3 (LED)
    PF4_External_Interrupt_Input_Init();  // Initialize PF4 (Switch 1)
    PB0_External_Interrupt_Input_Init();  // Initialize PF0 (Switch 2)
    while(1){}
	}
void GPIOB_Handler()
{
	if((GPIOB->MIS & (0x10)) == (0x10))// Handle PF4 (Switch 1) interrupt - Turn ON LED (PF3)
			{  // PF4 interrupt occurred
        GPIOB->DATA |= (0x08);// Turn ON PF3 (LED)
        GPIOB->ICR |= 0x10;  // Clear interrupt flag for PF4
			}
}

void GPIOF_Handler()
	{
    if((GPIOF->MIS & (0x10)) == (0x10))// Handle PF4 (Switch 1) interrupt - Turn ON LED (PF3)
			{  // PF4 interrupt occurred
        GPIOF->DATA |= (0x08);// Turn ON PF3 (LED)
        GPIOF->ICR |= 0x10;  // Clear interrupt flag for PF4
			}
	}
void PF3_Digital_Output_Init(){
    // Step 1: Clock enable on PortF
    SYSCTL->RCGCGPIO |= 0x20;
    for (j = 0; j < 3; j++)       // at least 3 clock cycles delay
    // Step 2: Disable alternate function on PF3
    GPIOF->AFSEL &= ~(0x08);
    // Step 3: Enable digital function on PF3
    GPIOF->DEN |= 0x08;            // Digital enable for PF3
    // Step 4: Set PF3 as an output pin
    GPIOF->DIR |= 0x08;            // PF3 as output
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
    
    // Step 6: NVIC configuration for Port F interrupt (IRQ 30)
    NVIC->ISER[0] |= (1<<30);      // Enable IRQ for PortF esception number
    NVIC->IPR[7] = 0x00600000;      // Set priority 3 for PortF interrupt 0000 0000 22,23,24
}

void PB0_External_Interrupt_Input_Init() 
	{
    SYSCTL->RCGCGPIO |= 0x02;		// Step 1: Clock enable on PortB 
    for (j = 0; j < 3; j++)    // at least 3 clock cycles delay
    GPIOB->AFSEL &= ~(0x01);// Step 3: Disable alternate function on PF0
    GPIOB->DEN |= 0x01;		 // Step 4: Enable digital function on PF0
    GPIOB->DIR &= ~(0x01);// Step 5: Set PF0 as input pin
    GPIOB->PUR |= 0x01;  // Enable pull-up resistor on PF0
    // Step 6: Configure PF0 for falling edge interrupt
    GPIOB->IS &= ~(0x01); // Edge-triggered
    GPIOB->IEV |= 0x01;  // Rising edge
    GPIOB->IM |= 0x01;  // Unmask interrupt for PF0
    GPIOB->ICR |= 0x01;// Clear interrupt flag
		 // Step 6: NVIC configuration for Port B interrupt (IRQ 1)
    NVIC->ISER[0] |= (1<<1);       // Enable IRQ for PortB
    NVIC->IPR[0] = 0x00400000;      // Set priority 2 
	}