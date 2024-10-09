//|FINAL EVALUATION																																																																																													  |
//|-------------------------------------------------------------------------------------------------------------------------------------------------------|
//|Name : Haseeb-ul-hassan And Mohammad Abdullah Nisar																																																																|
//|Date : 8-MAY-2024																																																																			|
//|Hardware: Tiva C series Launchpad Evaluation kit by Texas instruments																																									|
//|IDE and Compiler : Keil microvision 5.39																																																								|
//|Disclaimer : This program has been written as an open source develpment program so any usage must be credited and i am not responsible for any missuse |
//|Contact : 2022mc58@student.uet.edu.pk,2022mc50@student.uet.edu.pk  																																		|
//_________________________________________________________________________________________________________________________________________________________
// Configuring Desired  Register Adresses...
//_________________________________________________________________________________________________________________________________________________________
// For Port B
#include <stdint.h>
#define RCGC_GPIO_R *((volatile unsigned long *)0x400FE608)
// PortB on APB = 0x40005000 (Base Address)
#define GPIO_PORTB_AFSEL_R *((volatile unsigned long *)0x40005420)
#define GPIO_PORTB_DEN_R *((volatile unsigned long *)0x4000551C)
#define GPIO_PORTB_DIR_R *((volatile unsigned long *)0x40005400)
#define GPIO_PORTB_DATA_R *((volatile unsigned long *)0x400053FC)
// For Port A
#define GPIO_PORTA_AFSEL_R *((volatile unsigned long *)0x40004420)
#define GPIO_PORTA_DEN_R *((volatile unsigned long *)0x4000451C)
#define GPIO_PORTA_DIR_R *((volatile unsigned long *)0x40004400)
#define GPIO_PORTA_DATA_R *((volatile unsigned long *)0x40004170)
// For Port E
// PortE on APB = 0x40024000 (Base Address)
#define GPIO_PORTE_AFSEL_R *(( volatile unsigned long *)0x40024420)
#define GPIO_PORTE_DEN_R *(( volatile unsigned long *)0x4002451C)
#define GPIO_PORTE_DIR_R *(( volatile unsigned long *)0x40024400)
#define GPIO_PORTE_DATA_R *(( volatile unsigned long *)0x40024008) // 0000 0000 1000
//
#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_RCGCADC_R (*((volatile uint32_t *)0x400FE638))
//#define GPIO_PORTE_DIR_R (*((volatile uint32_t *)0x40024400))
////define GPIO_PORTE_DEN_R (*((volatile uint32_t *)0x4002451C))
#define GPIO_PORTE_AMSEL_R (*((volatile uint32_t *)0x40024528))
#define ADC_ACTSS_R (*((volatile uint32_t *)0x40038000))
#define ADC_EMUX_R (*((volatile uint32_t *)0x40038014))
#define ADC_SAC_R (*((volatile uint32_t *)0x40038030))
#define ADC_SS3MUX_R (*((volatile uint32_t *)0x400380A0))
#define ADC_SS3CTL_R (*((volatile uint32_t *)0x400380A4))
#define ADC_SS3FIFO_R (*((volatile uint32_t *)0x400380A8))
	#define GPIO_PORTF_DIR_R (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_DEN_R (*((volatile uint32_t *)0x4002551C))
#define GPIO_PORTF_DATA_R (*((volatile uint32_t *)0x40025020))

//_________________________________________________________________________________________________________________________________________________________
// Assigning Registers Values...
//_________________________________________________________________________________________________________________________________________________________
unsigned char digits[10] ={0xC0,0xF9,0xA4,0xB0,0x99, 0x92, 0x82,0xF8, 0x80, 0x90}; // SEGMENT ARRAY FOR THE DIPLAY
unsigned char D[4] = {0x04, 0x08, 0x10, 0x40};
#define GPIO_PORT_A_B_C_E_CLOCK_EN 0x37 // (0011 0111) PortF clock gating control
										// PortA
#define GPIO_PortA_AFSEL_DS 0x00		// (0000 0000)Port A alternate function disable
#define GPIO_PortA_EN 0x5C				// (0011 1100) Port A pin 2_5 digital enable
#define GPIO_PortA_pins_OP 0x5C			// (0011 1100) Port A pin 2_5 as Output
										// PortB
#define GPIO_PortB_AFSEL_DS 0x00		// (0000 0000)Port B alternate function disable
#define GPIO_PortB_EN 0xFF				// (1111 1111) Port B pin 0_7 digital enable
#define GPIO_PortB_pins_OP 0xFF			// (1111 1111) Port B pin 0_7 as Output
// PortE
#define GPIO_PORTE_CLOCK_EN 0x10 // Port E clock gating control (0001 0011)
#define GPIO_PORTE_MASK 0x02 // Mask for pin E1 write operation (0000 0010)
#define Default_segment 0x3F
//_________________________________________________________________________________________________________________________________________________________
// Setting System Clock...
//_________________________________________________________________________________________________________________________________________________________
#define SYSTEM_CLOCK_FREQUENCY 16000000				// Default clock frequency
#define DELAY_VALUE SYSTEM_CLOCK_FREQUENCY / 16000	// Delay value for the program 1MS second
#define DEBOUCE_DELAY SYSTEM_CLOCK_FREQUENCY / 1000 // Debounce delay
#define Delay(time){for (int i = 0; i < (1000*time*2); i++){};};
void usDelay(unsigned long microseconds);
#define delay__value 1000000; // Adjust as needed

//void Delay (uint32_t count) {while (count > 0) {count--;};}
void angle_servo(float angle);
void PortE_Digital_Input_Init(void);
	void digits_value(int value);
int k = 0;
int x = 0;
	int n =180;
	int v =0;
	int q =0;
	int dig_1 = 0;
	int dig_2 = 0;
	int dig_3 = 0;
	int dig_4 = 0;

unsigned volatile long j;
unsigned short i = 0;
void usDelay(unsigned long microseconds){for (k =0; k < 2*microseconds; k++);}
void angle_servo(float angle)
{
	x = 400 +(angle * 9.8);
	// Turn PE1 High
		GPIO_PORTE_DATA_R |= GPIO_PORTE_MASK;
		usDelay(x);	// Delay required to set servo at 0 degrees
		// Turn PE1 Low
		GPIO_PORTE_DATA_R &= ~GPIO_PORTE_MASK;
		usDelay(20000-x);
}
void PortE_Digital_Input_Init(void){
	// PE1 interfaced to signal pin of Servo
	// Step 1: Clock enable
	//RCGC_GPIO_R |= GPIO_PORTE_CLOCK_EN;
	for (j =0; j < 3 ; j++)		// at least 3 clock cyles	
	// Step 2: APB is selected by selecting Base Address	
	// Step 3: Disable alternate functionality
	GPIO_PORTE_AFSEL_R &= ~GPIO_PORTE_MASK;
	// Step 4: Enable digital pin functionaliy
	GPIO_PORTE_DEN_R |= GPIO_PORTE_MASK; // Digital enable 
	// Step 5: Enable digital pins as an output
	GPIO_PORTE_DIR_R |= GPIO_PORTE_MASK;
}
void digits_value(int n)
{
			dig_1 = n%10;
			dig_2 = ((n - dig_1)%100);
			dig_3 = (n - dig_2)%1000;
			dig_4 = (n - dig_3)%10000;
			GPIO_PORTA_DATA_R &= 0x00;
			GPIO_PORTB_DATA_R &= 0x00;
			GPIO_PORTA_DATA_R |= D[0];
			GPIO_PORTB_DATA_R |= digits[dig_1];
			Delay(2);
			GPIO_PORTA_DATA_R &= 0x00;
			GPIO_PORTB_DATA_R &= 0x00;
			GPIO_PORTA_DATA_R |= D[1];
			GPIO_PORTB_DATA_R |= digits[dig_2/10];
			Delay(2);
			GPIO_PORTA_DATA_R &= 0x00;
			GPIO_PORTB_DATA_R &= 0x00;
			GPIO_PORTA_DATA_R |= D[2];
			GPIO_PORTB_DATA_R |= digits[dig_3/100];
			Delay(2);
			GPIO_PORTA_DATA_R &= 0x00;
			GPIO_PORTB_DATA_R &= 0x00;
			GPIO_PORTA_DATA_R |= D[3];
			GPIO_PORTB_DATA_R |= digits[dig_4/1000];
			Delay(2);
}
//___________________________________________________________________________________________________________________________________________________
// Main Program ...
//_________________________________________________________________________________________________________________________________________________________


		int main()
{
    volatile uint32_t adc_data;
    volatile unsigned long j;
    
    // Enable clock for Port E and ADC0
    SYSCTL_RCGCGPIO_R |= 0x10; // Enable clock for Port E
    SYSCTL_RCGCADC_R |= 0x01; // Enable ADC0 clock
    
    // Configure Port E Pin 2 (potentiometer) as analog input
    GPIO_PORTE_DIR_R &= ~0x04; // Set PE2 as input
    GPIO_PORTE_DEN_R |= 0x04; // Enable digital function for PE2
    GPIO_PORTE_AMSEL_R |= 0x04; // Enable analog mode for PE2
    
    // Configure ADC0
    ADC_ACTSS_R &= ~0x08; // Disable sample sequencer 3 for configuration
    ADC_EMUX_R |= 0xF000; // Continuous Sampling Mode
    ADC_SAC_R |= 0x04; // 16x oversampling and 64x averaging
    ADC_SS3MUX_R = 0x01; // ANI 1 (PE2) is assigned to SS3
    ADC_SS3CTL_R = 0x02; // Sample is end of sequence
    ADC_ACTSS_R |= 0x08; // Enable sample sequencer 3
    
    // Enable clock for Port F and configure Pin 3 as output
    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for Port F
    GPIO_PORTF_DIR_R |= 0x08; // Set PF3 as output
    GPIO_PORTF_DEN_R |= 0x08; // Enable digital function for PF3
    
	RCGC_GPIO_R |= GPIO_PORT_A_B_C_E_CLOCK_EN; // Step 1: Clock enable on PortA_B_C_E
	Delay(16);
	PortE_Digital_Input_Init();
	while ((RCGC_GPIO_R & GPIO_PORT_A_B_C_E_CLOCK_EN) == 0);
	// PORT A(D1,D2,D3,D4)
	GPIO_PORTA_AFSEL_R &= GPIO_PortA_AFSEL_DS; // Step 3: Disable alternate functionality on PortB
	GPIO_PORTA_DEN_R |= GPIO_PortA_EN;		   // Step 4: Enable digital pin functionaliy on PortB 0-7
	GPIO_PORTA_DIR_R |= GPIO_PortA_pins_OP;	   // Step 5: Set PortB pin 0-7 as an output pinS
											   // PORT B(7 SEGMENT)
	GPIO_PORTB_AFSEL_R &= GPIO_PortB_AFSEL_DS; // Step 3: Disable alternate functionality on PortF
	GPIO_PORTB_DEN_R |= GPIO_PortB_EN;		   // Step 4: Enable digital pin functionaliy on PortB_F pin 2,6
	GPIO_PORTB_DIR_R |= GPIO_PortB_pins_OP;	   // Step 5: Set PortB_F pin 2,4 as an output pin
	   // Step 5: Set PortB pin 0-7 as an output pinS
	GPIO_PORTB_DATA_R |=  Default_segment;
	// Initialize the GPIO ports
	while (1) // Loop to run infinite
	{
            ADC_ACTSS_R |= 0x08; // Initiate SS3 conversion
        while (!(ADC_ACTSS_R & 0x08)) {} // Wait for conversion to complete
        // Read the potentiometer value from ADC FIFO
    adc_data = (ADC_SS3FIFO_R & 0xFFF);
		digits_value(adc_data);
					if(adc_data==580){
					GPIO_PORTF_DATA_R ^= 0x08;}
					else{
					GPIO_PORTF_DATA_R &= 0x00;}
		angle_servo(adc_data/22.7555);			
	};
};
	