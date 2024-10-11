#define PAIN (volatile unsigned char *) 0xFFFFFFF0
#define PAOUT (volatile unsigned char *) 0xFFFFFFF1
#define PADIR (volatile unsigned char *) 0xFFFFFFF2
#define PBOUT (volatile unsigned char *) 0xFFFFFFF4
#define PBDIR (volatile unsigned char *) 0xFFFFFFF5
#define PSTAT (volatile unsigned char *) 0xFFFFFFF6
#define PCONT (volatile unsigned char *) 0xFFFFFFF7
#define CNTM (volatile unsigned int *) 0xFFFFFFD0
#define CTCON (volatile unsigned char *) 0xFFFFFFD8
#define CTSTAT (volatile unsigned char *) 0xFFFFFFD9
#define IVECT (volatile unsigned int *) (0x20)

interrupt void intserv();
unsigned char digit_1 = 0; /* Digit to be displayed for led 1 display*/
unsigned char digit_2 = 0; /* Digit to be displayed for led 2 display*/
unsigned char led_1 = 0; /* LED state: 0/1 = on/off */
unsigned char led_2 = 1; /* LED state: 0/1 = on/off */
unsigned char button_pressed = 1; // Button State 0/1 = pressed/released
unsigned char LED_1_MASK = 0x04;
unsigned char LED_2_MASK = 0x01;
unsigned char BUTTON_PRESSED_MASK = 0x80;
int main() {
	
	*CTCON = 0x02; /* Stop Counter */
	*CTSTAT = 0x0; /* Clear “reached 0” flag */
	
	*PADIR = 0x78; /* Set Port A direction */
	*PAOUT = 0x00; /* Zero digit 1 */
	
	*PBDIR = 0xF5; /* Set Port B direction */
	*PBOUT = LED_1_ON_MASK; /* Zero digit 2 and enable LED 1*/

	*PCONT = 0x10; /* Set ENBIN bit to allow interrupts on change of PA input */
	
	*CNTM = 100000000; /* Initialize Timer to 100 000 000 ticks (1 second timer for 100 MHz timer)*/
	*IVECT = (unsigned int *) &intserv; /* Set interrupt vector */
	asm(“MoveControl PSR,#0x40”); /* CPU responds to IRQ */
	*CTCON = 0x01; /* Start Counter without interrupts*/
	
	while (1) {
		while((*CTSTAT & 0x1) == 0);
		// If LED 1 is on...
		if (led_1 == 0){
			digit_1 = (digit_1 + 1)%10;
			*PAOUT = (digit_1 << 3); /* Update Port A */
		} else {
			digit_2 = (digit_2 + 1)%10;
			*PBOUT = ((digit_2 << 4) | (*PAOUT & 0x0F)); /* Update Port B */
		}
		*CTSTAT = 0x00; // Reset CTSTAT counter reached zero flag
	}
	exit(0);
}
interrupt void intserv() {
	//if button is released and it previously wasn't...
	if(((*PAIN & BUTTON_PRESSED_MASK) != 0) && button_pressed = 0){
		//button was just released...
		button_pressed = 1;
		
		//flip LEDs
		led_1 = !led_1;
		led_2 = !led_2;

		//if LED 1 is on...
		if(led_1 == 0){
			//turn LED 1 off and LED 2 on
			*PBOUT = ((*PBOUT & 0xF0) | LED_1_MASK);
		}else{
			//turn LED 2 off and LED 1 on
			*PBOUT = ((*PBOUT & 0xF0) | LED_2_MASK);
		}
		
	}else{
		//button was just pressed...
		button_pressed = 0;
	}
}
