/* Lab_05_Task_5.1_Interfacing 7 segment with TIVA C Series LaunchPad.uvprojx
--------------------------------------------------------------------------------------------
This C language program interface 7 segment with Port B pin 0--7 to increment or decrement the
count using 2 switches
--------------------------------------------------------------------------------------------
Written by Alishba Kiyani (alishbasaniyakiyani@gmail.com) .
Compiler and IDE used: Keil uVision 5.60 */
// Configuration Registers
// For Port B
#define RCGC_GPIO_R *(( volatile unsigned long *)0x400FE608)
// PortB on APB = 0x40005000 (Base Address)
#define GPIO_PORTB_AFSEL_R *(( volatile unsigned long *)0x40005420)
#define GPIO_PORTB_DEN_R *(( volatile unsigned long *)0x4000551C)
#define GPIO_PORTB_DIR_R *(( volatile unsigned long *)0x40005400)
#define GPIO_PORTB_DATA_R *(( volatile unsigned long *)0x400053FC)		

// For Port F
// PortE on APB = 0x40024000 (Base Address)
#define GPIO_PORTF_AFSEL_R *(( volatile unsigned long *)0x40025420)
#define GPIO_PORTF_DEN_R *(( volatile unsigned long *)0x4002551C)
#define GPIO_PORTF_DIR_R *(( volatile unsigned long *)0x40025400)
#define GPIO_PORTF_DATA_0_R *(( volatile unsigned long *)0x40025004)
#define GPIO_PORTF_DATA_4_R *(( volatile unsigned long *)0x40025040)
#define GPIO_PORTF_PU_R *(( volatile unsigned long *)0x40025510)
#define GPIO_LOCK_PORTF_R   *((volatile unsigned long *)0x40025520) // GPIO Lock
#define GPIO_CR_PORTF_R     *((volatile unsigned long *)0x40025524) // GPIO Commit

// Configuration Registers Values 
#define GPIO_PORTBE_CLOCK_EN 0x22  // Port B and E clock gating control

// For Port B
#define GPIO_PORTB_AFSEL_DS 0xFF // Port B alternate function disable
#define GPIO_PORTB_PINS_EN 0xFF  // Port B pin 0-7 digital enable
#define GPIO_PORTB_PINS_OP 0xFF // Port B pin 0-7 as digital output


// For Port F
#define GPIO_PORTF_AFSEL_DS 0x11 // Port F alternate function disable
#define GPIO_PORTF_PINS_EN 0x11 // Port F pin 0 & 4 digital enable
#define GPIO_PORTF_PINS_IP 0x11 // Port F pin 0 & 4 as digital input
#define GPIO_PORTF_PIN_PU 0x11 // Port F pin 0 & 4 Pull down resistor mask
#define GPIO_PORTF_PIN4 0x10 // Mask for pin 4 write operation (0001 0000)
#define GPIO_PORTF_PIN0 0x01 // Mask for pin 0 write operation (0000 0001)
#define GPIO_LOCK_KEY 0x4C4F434B  // Unlocks the GPIO_CR register

// Constant Values
#define SYSTEM_CLOCK_FREQUENCY 16000000 // Default clock frequency
#define DELAY_VALUE SYSTEM_CLOCK_FREQUENCY/16
#define DELAY_DEBOUNCE SYSTEM_CLOCK_FREQUENCY/1000


int main ()
{
	unsigned char CC_7seg_Table[10] ={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F} ; 
  unsigned volatile long j ;
	signed volatile long i =0;
	
	// User main program
	
	// Step 1: Clock enable on PortB and PortF
	RCGC_GPIO_R |= GPIO_PORTBE_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected for PortB and PortF by selecting
	// 0x40005000 as Base Address in DATA section
	// 0x40025000 as Base Address in DATA section

	// Unlock PF0
  GPIO_LOCK_PORTF_R = GPIO_LOCK_KEY;
  GPIO_CR_PORTF_R |= GPIO_PORTF_PIN0;
		
	// Step 3: Disable alternate functionality on PortB and PortF
	GPIO_PORTB_AFSEL_R &= ~GPIO_PORTB_AFSEL_DS;
	GPIO_PORTF_AFSEL_R &= ~GPIO_PORTF_AFSEL_DS;
	
	// Step 4: Enable digital pin functionaliy on PortB and PortF 
	GPIO_PORTB_DEN_R |= GPIO_PORTB_PINS_EN; // Digital enable 
	GPIO_PORTF_DEN_R |= GPIO_PORTF_PINS_EN; // Digital enable 
	
	// Step 5: Set PortB pins 0-7 as an output pin
	GPIO_PORTB_DIR_R |= GPIO_PORTB_PINS_OP; // PB0-PB7 as output
	// Set PortF pins 0 and 4 as an input pin
	GPIO_PORTF_DIR_R &= ~GPIO_PORTF_PINS_IP; // PF0 and PF4 as input
	
	// Step 6: Set internal pull up resistor enable  
	GPIO_PORTF_PU_R |= GPIO_PORTF_PIN_PU;
	
	GPIO_PORTB_DATA_R = 0x3F; 
	
	while(1)
	{
		while(GPIO_PORTF_DATA_4_R == 0x00 )     // rising edge 
		{	
			for (j =0; j< DELAY_DEBOUNCE ; j++);
			if(GPIO_PORTF_DATA_4_R != 0x00 )
			{
				i++;
				for (j =0; j< DELAY_DEBOUNCE ; j++);
			
				if(i>9)
				{
					i = 0;
				}
				GPIO_PORTB_DATA_R = CC_7seg_Table[i];
				for (j =0; j< DELAY_VALUE ; j++);
			}	
		}
		
		if(GPIO_PORTF_DATA_0_R != 0x00 )    // falling edge 
	 {
		 for (j =0; j< DELAY_DEBOUNCE ; j++);
		 if(GPIO_PORTF_DATA_0_R == 0x00 )
			{
				i--;
				for (j =0; j< DELAY_DEBOUNCE ; j++);
			
				if(i<0)
				{
					i = 9;
				}
		
			GPIO_PORTB_DATA_R = CC_7seg_Table[i];
			for (j =0; j< DELAY_VALUE ; j++);	
			}
		}
	}  
}