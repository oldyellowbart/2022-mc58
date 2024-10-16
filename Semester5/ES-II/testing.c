// Clock Gating Control COnfigurations for GPIO Ports
#define SYSTCL_RCGC_R *((volatile unsigned long *)0x400FE608)
#define SYSTCL_RCGC_GPIOPABE 0x10


// Port E Configuration Registers (LM35 Temperature Sensor)
#define GPIO_PORTE_AMSEL_R *((volatile unsigned long *)0x40024528)
#define GPIO_PORTE_DEN_R *((volatile unsigned long *)0x4002451C)
#define GPIO_PORTE_DIR_R *((volatile unsigned long *)0x40024400)
#define GPIO_PORTE_PIN2 0x04
#define GPIO_PORTE_MASK 0x02									  // Mask for pin 1 write operation (0000 0010)
#define GPIO_PORTE_DATA_R *((volatile unsigned long *)0x40024008) // 0000 0000 1000

// Clock Gating Control Configurations for ADC0 Module
#define SYSTCL_RCGC_ADC_R *((volatile unsigned long *)0x400FE638)
#define ADC0_CLOCK_ENABLE 0x01


// ADC module configuration registers
#define ADC_ACTIVE_SS_R *(volatile long *)(0x40038000)
#define ADC_INT_MASK_R *(volatile long *)(0x40038008)
#define ADC_TRIGGER_MUX_R *(volatile long *)(0x40038014)
#define ADC_PERI_CONFIG_R *(volatile long *)(0x40038FC4)
#define ADC_SAMPLE_AVG_R *(volatile long *)(0x40038030)

// ADC sequencer 3 configuration registers
#define ADC_SS3_IN_MUX_R *(volatile long *)(0x400380A0)
#define ADC_SS3_CONTROL_R *(volatile long *)(0x400380A4)
#define ADC_SS3_FIFO_DATA_R *(volatile long *)(0x400380A8)

// Default clock frequency
#define SYSTEM_CLOCK_FREQUENCY 16000000
#define DELAY_DIGIT_REFRESH SYSTEM_CLOCK_FREQUENCY / 1000


// Function Prototype
void DELAY(volatile unsigned int delay);
unsigned volatile long j;


void PortE_Digital_Input_Init(void);
void usDelay(unsigned long microseconds);

// Main Program User
int main()
{

	unsigned int adc_data = 0;

	// Enable the clock for Port A, B and E
	SYSTCL_RCGC_R |= SYSTCL_RCGC_GPIOPABE;
	DELAY(3);

	PortE_Digital_Input_Init(); // Function for GPIO PortE 

	// potentiometer connected to GPIO pin PE2 (ANI_1)
	GPIO_PORTE_DEN_R &= ~GPIO_PORTE_PIN2;
	GPIO_PORTE_AMSEL_R |= GPIO_PORTE_PIN2;

	// Enable the clock for ADC0
	SYSTCL_RCGC_ADC_R |= ADC0_CLOCK_ENABLE;
	DELAY(3);

	ADC_PERI_CONFIG_R |= 0x03;	// 250 Ksps
	ADC_TRIGGER_MUX_R = 0xF000; // Continuous Sampling Mode on SS3

	// No sequence priority
	ADC_SAMPLE_AVG_R |= 0x04; // 16x oversampling and then averaged

	// No voltage selection
	ADC_ACTIVE_SS_R = 0x08;	   // Configure ADC0 module for sequencer 3 it takes single input
	ADC_SS3_IN_MUX_R = 0x01;   // ANI_1 (PE2) is assigned to Input 0 of SS3
	ADC_SS3_CONTROL_R |= 0x02; // Sample is end of sequence
	while (1)
		{
			DELAY(100);
			// Read the potentiometer raw value from ADC FIFO
			adc_data = (ADC_SS3_FIFO_DATA_R & 0xFFF);

			j = ((adc_data * 0.22)+ 850);

			// Turn PE1 High
			GPIO_PORTE_DATA_R |= GPIO_PORTE_MASK;
			usDelay(j); // Delay required to set servo at 0 degrees

			// Turn PE1 Low
			GPIO_PORTE_DATA_R &= ~GPIO_PORTE_MASK;
			usDelay(18500-j);
		
		}
	}

void usDelay(unsigned long microseconds){for (j = 0; j < 2 * microseconds; j++);} // 1 iteration = 0.5 uSec
void PortE_Digital_Input_Init(void)
{
	// PE1 interfaced to signal pin of Servo

	// Step 3: Disable alternate functionality
	GPIO_PORTE_AMSEL_R &= ~GPIO_PORTE_MASK;

	// Step 4: Enable digital pin functionaliy
	GPIO_PORTE_DEN_R |= GPIO_PORTE_MASK; // Digital enable
	

	// Step 5: Enable digital pins as an output
	GPIO_PORTE_DIR_R |= GPIO_PORTE_MASK;
	
}