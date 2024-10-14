#include<TM4C123.h>

#define PIN3 0x3
#define PIN0 0x1

void GPIO_PORTF_Init(int pin);
void timer0_Periodic();
unsigned volatile long i;
unsigned volatile long j;
void PLL_Init(void);
int main(){
	// Initialize PortF pin 3 as digital out
	GPIO_PORTF_Init(PIN3);
	GPIO_PORTF_Init(PIN0);
	GPIO_PORTF_Init(0x2);
	timer0_Periodic();
	PLL_Init();
	while(1){
		//1
		GPIOF->DATA |= 1<<3;//on green
		GPIOF->DATA |= 1<<2;//on blue
		GPIOF->DATA |= 1<<1;//on red
		//2
		while((TIMER0->RIS & 1<<0)==0);
		TIMER0->ICR = 1<<0;
		GPIOF->DATA &= ~(1<<1);//of red
		//3
		while((TIMER0->RIS & 1<<0)==0);
		TIMER0->ICR = 1<<0;
		GPIOF->DATA |= 1<<3;//on green
		GPIOF->DATA &= ~(1<<2);//on blue
		GPIOF->DATA |= 1<<1;//on red
		//4
		while((TIMER0->RIS & 1<<0)==0);
		TIMER0->ICR = 1<<0;
		GPIOF->DATA |= 1<<3;//on green
		GPIOF->DATA &= ~(1<<2);//on blue
		GPIOF->DATA &= ~(1<<1);//on red
		//5
		while((TIMER0->RIS & 1<<0)==0);
		TIMER0->ICR = 1<<0;
		GPIOF->DATA &= ~(1<<3);//on green
		GPIOF->DATA |= (1<<2);//on blue
		GPIOF->DATA |= (1<<1);//on red
		//6
		while((TIMER0->RIS & 1<<0)==0);
		TIMER0->ICR = 1<<0;
		GPIOF->DATA &= ~(1<<3);//on green
		GPIOF->DATA |= (1<<2);//on blue
		GPIOF->DATA &= ~(1<<1);//on red
		//7
		while((TIMER0->RIS & 1<<0)==0);
		TIMER0->ICR = 1<<0;
		GPIOF->DATA &= ~(1<<3);//on green
		GPIOF->DATA &= ~(1<<2);//on blue
		GPIOF->DATA |= (1<<1);//on red
		//8
		while((TIMER0->RIS & 1<<0)==0);
		TIMER0->ICR = 1<<0;
		GPIOF->DATA &= ~(1<<3);//on green
		GPIOF->DATA &= ~(1<<2);//on blue
		GPIOF->DATA &= ~(1<<1);//on red
		
	}
}

void GPIO_PORTF_Init(int pin){
	/* function to initialize the GPIO PortF pin 3
	by manipulating the configuration registers */
	
	// Step 1: Clock enable on PortF
	SYSCTL->RCGC2 |= 1<<5;
	
	//RCGC_GPIO_R |= GPIO_PORTF_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: APB is selected for PortF by selecting
	// 0x40025000 as Base Address in DATA section
	
	// Step 3: Disable alternate functionality on PortF
	GPIOF->AFSEL &= ~(1<<pin);
	
	// Step 4: Enable digital pin functionaliy on PortF pin 3
	GPIOF->DEN |= 1<<pin; // Digital enable for PF3
	
	// Step 5: Set PortF pin 3 as an output pin
	GPIOF->DIR |= 1<<pin; // PF3 as output
}

void timer0_Periodic(){
	
	// This function generations mDelay msecond delay
	// Note: This function can generate 4 msec delay (maximum)
	
	// Step 1: Enable Timer Clock on timer0
	SYSCTL->RCGCTIMER |= 1<<0;		// b 0000 0001
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles
	
	// Step 2: Ensure Timer is disabled before making any changes
	TIMER0->CTL = 0x00;					// TAEN = 0, i.e., timer0A is disablled
	
	// Step 3: Select Mode of Operation of timer0 (Split/cancatenated/RTC)
	TIMER0->CFG |= 0<<0;					// timer0 is used as a 32-bit (split) timer, i.e., timer0A
	TIMER0->TAMR = 2<<0; 				// TAMR = 1 (1-shot), TACDIR = 0 (count-down)
	
	// Step 4: Load counter start value in Interval Load Register
	TIMER0->TAILR = 8000000 - 1;		// for 32 MHz, 16000 will generate 1 mSec delay

	// Step 5: Interrupt configurations
	//	TIMER0->IMR = 0x00;					// No interrupt enabled (RO Register)
	TIMER0->ICR = 1<<0;					// Clear timer status flag (TATORIS, TATOMIS)
	
	// Step 6: Enable the Timer and start counting
	TIMER0->CTL |= 1<<0;					// TAEN = 1
	
	// Step 7: Poll TATORIS (Time-Out Raw Interrupt Status) bit to check timer0A timeout
	//while((TIMER0->RIS & 1<<0)==0); // Wait for timeout flag to set
	
	// After time-out, timer stops automatically in one-shot mode
	//TIMER0->ICR = 1<<0;					// Clear timer status flag (TATORIS, TATOMIS)
}
// Configure the system clock to 16 MHz using PLL
void PLL_Init(void){

  // 0) Use RCC2
  SYSCTL->RCC2 |=  0x80000000;  // USERCC2

  // 1) bypass PLL while initializing
  SYSCTL->RCC2 |=  0x00000800;  // BYPASS2, PLL bypass

  // 2) select the crystal value and oscillator source
  SYSCTL->RCC = (SYSCTL->RCC &~ 0x000007C0) + 0x00000540;  // clear XTAL field, bits 10-6  // 10101, configure for 16 MHz crystal
  SYSCTL->RCC2 &= ~0x00000070;  // configure for main oscillator source

  // 3) activate PLL by clearing PWRDN
  SYSCTL->RCC2 &= ~0x00002000;

  // 4) set the desired system divider
  SYSCTL->RCC2 |= 0x40000000;   // use 400 MHz PLL
  SYSCTL->RCC2 = (SYSCTL->RCC2 &~ 0x1FC00000) + (24<<22);      // configure for 16 MHz clock / / clear system clock divider +

  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL->RIS&0x00000040)==0){}  // wait for PLLRIS bit

  // 6) enable use of PLL by clearing BYPASS
  SYSCTL->RCC2 &= ~0x00000800;
}