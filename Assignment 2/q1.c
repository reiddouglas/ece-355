#define PAOUT (volatile unsigned char *) 0xFFFFFFF1
#define PADIR (volatile unsigned char *) 0xFFFFFFF2
#define PBIN (volatile unsigned char *) 0xFFFFFFF3
#define PBOUT (volatile unsigned char *) 0xFFFFFFF4
#define PBDIR (volatile unsigned char *) 0xFFFFFFF5
#define CNTM (volatile unsigned int *) 0xFFFFFFD0
#define CTCON (volatile unsigned char *) 0xFFFFFFD8
#define CTSTAT (volatile unsigned char *) 0xFFFFFFD9
#define IVECT (volatile unsigned int *) (0x20)

interrupt void intserv();
unsigned char digit_1 = 0; /* Digit to be displayed for led 1 display*/
unsigned char digit_2 = 0; /* Digit to be displayed for led 2 display*/
unsigned char led_1 = 0x0; /* LED state: 0/1 = on/off */
unsigned char led_2 = 0x1; /* LED state: 0/1 = on/off */
unsigned char LED_1_ON_MASK = 0x20;
unsigned char LED_2_ON_MASK = 0x40;
int main() {
	*CTCON = 0x02; /* Stop Counter */
	*CTSTAT = 0x0; /* Clear “reached 0” flag */
	
	*PADIR = 0x6F; /* Set Port A direction */
	*PAOUT = LED_1_ON_MASK; /* Set LED 1 to on, zero digit 1 and turn LED 2 off */
	
	*PBDIR = 0xF0; /* Set Port B direction */
	*PBOUT = 0x00; /* Zero digit 2*/
	
	*CNTM = 100000000; /* Initialize Timer to 100 000 000 ticks (1 second timer for 100 MHz timer)*/
	*IVECT = (unsigned int *) &intserv; /* Set interrupt vector */
	asm(“MoveControl PSR,#0x40”); /* CPU responds to IRQ */
	*CTCON = 0x11; /* Start Counter with interrupts*/
	
	while (1) {
		
		while ((*PBIN & 0x1) != 0); /* Wait until SW is pressed */
		while ((*PBIN & 0x1) == 0); /* Wait until SW is released */
		
		// If LED 1 is on...
		if (led_1 == 0x0){
			*PAOUT = (digit_1 | LED_2_ON_MASK);
			led_1 = 0x1 //turn off
			led_2 = 0x0 //turn on
		} else {
			*PAOUT = (digit_1 | LED_1_ON_MASK);
			led_1 = 0x0 //turn on
			led_2 = 0x1 //turn off
		}

	}
	exit(0);
}
interrupt void intserv() {
	*CTSTAT = 0x00; // Reset CTSTAT counter reached zero flag
	// If LED 1 is on...
	if (led_1 == 0x0){
		digit_1 = (digit_1 + 1)%10;
		*PAOUT = (digit_1 | LED_1_ON_MASK); /* Update Port A */
	} else {
		digit_2 = (digit_2 + 1)%10;
		*PBOUT = ((digit_2 << 4) | (*PAOUT & 0x0F)); /* Update Port B */
	}
}
