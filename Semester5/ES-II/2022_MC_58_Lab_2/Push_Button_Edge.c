/* Name: Muhtashim Butt
   Reg No# 2022-MC-44
	 Folder Name: 2022_MC_44_Lab_2
	 File name: Lab_2_Task_2    */
	 
// Libraries
#include "TM4C123.h"   

// Global Variable
unsigned volatile long j;

// Function Declarations
void PF3_Digital_Output_Init();
void PF4_External_Interrupt_Input_Init();
void PB0_External_Interrupt_Input_Init();

// Main Function
int main(){
    PF3_Digital_Output_Init();            // Initialize PF3 (LED)
    PF4_External_Interrupt_Input_Init();  // Initialize PF4 (Switch 1)
    PB0_External_Interrupt_Input_Init();  // Initialize PB0 (External Switch 2)
    while(1)
		{}
}

void GPIOF_Handler()
{
    // Handle PF4 (Switch 1) interrupt - Turn ON LED (PF3)
    if((GPIOF->MIS & (0x10)) == (0x10)){  // PF4 interrupt occurred
        GPIOF->DATA |= (0x08);            // Turn ON PF3 (LED)
        GPIOF->ICR |= 0x10;               // Clear interrupt flag for PF4
    }
}

void GPIOB_Handler()
{
	  // Handle PB0 (External Switch 2) interrupt - Turn OFF LED (PF3)
	  if((GPIOB->MIS & (0x01)) == (0x01)){  // PB0 interrupt occurred
        GPIOF->DATA &= ~(0x08);            // Turn OFF PF3 (LED)
        GPIOB->ICR |= 0x01;               // Clear interrupt flag for PB0
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
    GPIOF->IS &= ~(0x10);          // Edge-sensitive
    GPIOF->IEV &= ~(0x10);         // Falling edge
    GPIOF->IM |= 0x10;             // Unmask interrupt for PF4
    GPIOF->ICR |= 0x10;            // Clear interrupt flag
    
    // Step 6: NVIC configuration for Port F interrupt (IRQ 30)
    NVIC->ISER[0] |= (1<<30);      // Enable IRQ for PortF
    NVIC->IP[7] = 0x00600000;      // Set priority 3 
}

void PB0_External_Interrupt_Input_Init(){
    // Step 1: Clock enable on PortB
    SYSCTL->RCGCGPIO |= 0x02;       // Enable clock for PortB
    for (j = 0; j < 3; j++)        // Wait for clock to stabilize
    
    // Step 2: Disable alternate function on PB0
    GPIOB->AFSEL &= ~(0x01);
    
    // Step 3: Enable digital function on PB0
    GPIOB->DEN |= 0x01;            // Digital enable for PB0
    
    // Step 4: Set PB0 as input pin
    GPIOB->DIR &= ~(0x01);         // PB0 as input
    GPIOB->PUR |= 0x01;            // Enable pull-up resistor on PB0
    
    // Step 5: Configure PB0 for falling edge interrupt
    GPIOB->IS &= ~(0x01);          // Edge-triggered
    GPIOB->IEV |= 0x01;            // Rising edge
    GPIOB->IM |= 0x01;             // Unmask interrupt for PB0
    GPIOB->ICR |= 0x01;            // Clear interrupt flag
    
    // Step 6: NVIC configuration for Port B interrupt (IRQ 1)
    NVIC->ISER[0] |= (1<<1);       // Enable IRQ for PortB
    NVIC->IP[0] = 0x00400000;      // Set priority 2 
}