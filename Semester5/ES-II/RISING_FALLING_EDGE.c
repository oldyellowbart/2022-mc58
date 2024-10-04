/* Name: Muhtashim Butt
   Reg No# 2022-MC-44
	 Folder Name: 2022_MC_44_Lab_2
	 File name: Lab_2_Task_1    */
	 
// Libraries
#include "TM4C123.h"          

// Global Variable
unsigned volatile long j;

// Function Declarations
void PF3_Digital_Output_Init();
void PF4_External_Interrupt_Input_Init();
void PF0_External_Interrupt_Input_Init();

// Main Function
int main(){
    PF3_Digital_Output_Init();        // Initialize PF3 (LED)
    PF4_External_Interrupt_Input_Init();  // Initialize PF4 (Switch 1)
    PF0_External_Interrupt_Input_Init();  // Initialize PF0 (Switch 2)
    while(1)
		{

    }
}

void GPIOF_Handler(){
    // Handle PF4 (Switch 1) interrupt - Turn ON LED (PF3)
    if((GPIOF->MIS & (0x10)) == (0x10)){  // PF4 interrupt occurred
        GPIOF->DATA |= (0x08);            // Turn ON PF3 (LED)
        GPIOF->ICR |= 0x10;               // Clear interrupt flag for PF4
    }
    // Handle PF0 (Switch 2) interrupt - Turn OFF LED (PF3)
    if((GPIOF->MIS & (0x01)) == (0x01)){  // PF0 interrupt occurred
        GPIOF->DATA &= ~(0x08);           // Turn OFF PF3 (LED)
        GPIOF->ICR |= 0x01;               // Clear interrupt flag for PF0
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
    GPIOF->IM |= 0x10;             // mask interrupt for PF4
    GPIOF->ICR |= 0x10;            // Clear interrupt flag
    
    // Step 6: NVIC configuration for Port F interrupt (IRQ 30)
    NVIC->ISER[0] |= (1<<30);      // Enable IRQ for PortF
    NVIC->IP[7] = 0x00600000;      // Set priority 3 for PortF interrupt
}

void PF0_External_Interrupt_Input_Init(){
    // Step 1: Clock enable on PortF (already enabled in PF4 init)
    SYSCTL->RCGCGPIO |= 0x20;
    for (j = 0; j < 3; j++)       // at least 3 clock cycles delay
    
    // Step 2: Unlock PF0 (it is locked by default)
    GPIOF->LOCK = 0x4C4F434B;      // Unlock GPIOCR register
    GPIOF->CR |= 0x01;             // Enable commit for PF0
    
    // Step 3: Disable alternate function on PF0
    GPIOF->AFSEL &= ~(0x01);
    
    // Step 4: Enable digital function on PF0
    GPIOF->DEN |= 0x01;            // Digital enable for PF0
    
    // Step 5: Set PF0 as input pin
    GPIOF->DIR &= ~(0x01);         // PF0 as input
    GPIOF->PUR |= 0x01;            // Enable pull-up resistor on PF0
    
    // Step 6: Configure PF0 for falling edge interrupt
    GPIOF->IS &= ~(0x01);          // Edge-triggered
    GPIOF->IEV |= 0x01;            // Rising edge
    GPIOF->IM |= 0x01;             // Unmask interrupt for PF0
    GPIOF->ICR |= 0x01;            // Clear interrupt flag
}