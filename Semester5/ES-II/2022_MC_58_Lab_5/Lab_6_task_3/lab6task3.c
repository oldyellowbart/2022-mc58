#include "TM4C123.h"                    // Device header
unsigned volatile long j;
uint32_t edge_count = 0;
void PD0_as_WT2CCP0_Init(void);
void Wtimer2A_RisingEdgeEvent_Init(void);
int WTIMER2A_RisingEdgeEvent_Capture(void);
void PortB_as_Output_Init(void);
unsigned char CC_7seg_Table[10] ={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F} ;
int main()
{
	// Initialize PortD pin 2 as T3CCP0
	PD0_as_WT2CCP0_Init();
	PortB_as_Output_Init();
	// Initialize WTIMER2A as Rising Edge Event Counter
	Wtimer2A_RisingEdgeEvent_Init();
	while(1)
    {
	edge_count = WTIMER2A_RisingEdgeEvent_Capture();
	GPIOB->DATA = CC_7seg_Table[edge_count];
	}
}
void PD0_as_WT2CCP0_Init(void){
	SYSCTL->RCGCGPIO |= 0x08; // Step 1: Clock enable on PortD 00 1000
	for (j =0; j < 3 ; j++)	;// at least 3 clock cyles
	GPIOD->AFSEL |= 0x01;   // Step 3: Enable alternate functionality on PortB
	GPIOD->DEN |= 0x01;    // Step 4: Enable digital pin functionaliy on PortB pin 2 
	GPIOD->DIR &= ~(0x01);// Step 5: Set PortB pin 2 as an output pin
	GPIOD->PCTL &= ~0x0000000F;		// clear the bit fields
	GPIOD->PCTL |= 0x00000007;// Step 6: Configure PortB pin 2 as T3CCP0 pin (Table 10-2 of Datasheet, page # 651)
}
void Wtimer2A_RisingEdgeEvent_Init(void){
	SYSCTL->RCGCWTIMER |=0x04;// Step 1: Enable Timer Clock on WTIMER2 (0000 0100)
	for (j =0; j < 3 ; j++)	// wait at least 3 clock cyles
	WTIMER2->CTL = 0x00;// Step 2: Ensure Timer is disabled before making any changes TAEN = 0, i.e., WTIMER2A is disablled
	// Step 3: Select Mode of Operation of WTIMER2 (Split/cancatenated/RTC)
	WTIMER2->CFG = 0x04;// WTIMER2 is used as a 16-bit (split) timer, i.e., WTIMER2A
	WTIMER2->TAMR = 0x13;// TAMR = 3 (capture), TACMR = 0 (edge-count) TACDIR = 1 (count-up)
	// Step 4: Set counter value limit, comparted to TAR to determine match event
	WTIMER2->TAMATCHR = 0xFFFF;		// 
	WTIMER2->TAPMR = 0xFF;// used with TAMATCHR to expand to 0xFFFFFF with 8-bit prescaler (bit 16:23)
	// Step 5: Interrupt configurations
	WTIMER2->ICR = 1<<0;// Clear timer status flag (TATORIS, TATOMIS)
	// Step 6: Enable the Timer and start counting
	WTIMER2->CTL |= 1<<0;// TAEN = 1
}
void PortB_as_Output_Init(void){
	SYSCTL->RCGCGPIO |= 0x22;// Step 1: Clock enable on PortB(10 0010)
	for (j =0; j < 3 ; j++);// Step 2: wait at least 3 clock cyles
	GPIOB->AFSEL &= (0x00);// Step 3: Disable alternate functionality on PB0-7 (0000 0000)	
	GPIOB->DEN |= 0xFF;   // Step 4: Enable digital pin functionaliy on PortB pin 0-7 (1111 1111)
	GPIOB->DIR |= 0xFF;  // Step 5: Set PortB pin 0-7 as an output pin(1111 1111)
}
int WTIMER2A_RisingEdgeEvent_Capture(void){
	return WTIMER2->TAR;	// Compared with TAMATCHR to determine match event
				// contains the number of edges that have occurred
}