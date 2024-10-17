#include "TM4C123.h"                    // Device header

unsigned volatile long j;
uint32_t edge_count = 0;

void PB2_as_T3CCP0_Init(void);
void timer3A_RisingEdgeEvent_Init(void);
int timer3A_RisingEdgeEvent_Capture(void);


int main(){
	// Initialize PortB pin 2 as T3CCP0
	PB2_as_T3CCP0_Init();
	// Initialize Timer3A as Rising Edge Event Counter
	timer3A_RisingEdgeEvent_Init();
	
	while(1){
		edge_count = timer3A_RisingEdgeEvent_Capture();
	}
}

void PB2_as_T3CCP0_Init(void){
	// Step 1: Clock enable on PortB
	SYSCTL->RCGCGPIO |= 1<<1;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected for PortB by selecting
	// 0x40005000 as Base Address in DATA section
	
	// Step 3: Enable alternate functionality on PortB
	GPIOB->AFSEL |= 1<<2;
	
	// Step 4: Enable digital pin functionaliy on PortB pin 2
	GPIOB->DEN |= 1<<2; // Digital enable for PB2
	
	// Step 5: Set PortB pin 2 as an output pin
	GPIOB->DIR &= ~(1<<2); // PB2 as input
	
	// Step 6: Configure PortB pin 2 as T3CCP0 pin (Table 10-2 of Datasheet, page # 651)
	GPIOB->PCTL &= ~0x00000F00;		// clear the bit fields
	GPIOB->PCTL |= 0x00000700;
}

void timer3A_RisingEdgeEvent_Init(void){
	
	// Step 1: Enable Timer Clock on timer3
	SYSCTL->RCGCTIMER |= 1<<3;		// b 0000 1000
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: Ensure Timer is disabled before making any changes
	TIMER3->CTL = 0x00;					// TAEN = 0, i.e., timer3A is disablled
	
	// Step 3: Select Mode of Operation of timer3 (Split/cancatenated/RTC)
	TIMER3->CFG = 0x04;					// timer3 is used as a 16-bit (split) timer, i.e., timer3A
	TIMER3->TAMR = 0x13; 				// TAMR = 3 (capture), TACMR = 0 (edge-count) TACDIR = 1 (count-up)
	
	// Step 4: Set counter value limit, comparted to TAR to determine match event
	TIMER3->TAMATCHR = 0xFFFF;		// 
	TIMER3->TAPMR = 0xFF;		// used with TAMATCHR to expand to 0xFFFFFF with 8-bit prescaler (bit 16:23)

	// Step 5: Interrupt configurations
	TIMER3->ICR = 1<<0;					// Clear timer status flag (TATORIS, TATOMIS)
	
	// Step 6: Enable the Timer and start counting
	TIMER3->CTL |= 1<<0;					// TAEN = 1
}

int timer3A_RisingEdgeEvent_Capture(void){
	return TIMER3->TAR;	// Compared with TAMATCHR to determine match event
				// contains the number of edges that have occurred
}