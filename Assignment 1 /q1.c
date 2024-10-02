#define PAOUT (volatile unsigned char *) 0xFFFFFFF1
#define PADIR (volatile unsigned char *) 0xFFFFFFF2
#define PBIN (volatile unsigned char *) 0xFFFFFFF3
#define PBDIR (volatile unsigned char *) 0xFFFFFFF5
#define CNTM (volatile unsigned int *) 0xFFFFFFD0
#define CTCON (volatile unsigned char *) 0xFFFFFFD8
#define CTSTAT (volatile unsigned char *) 0xFFFFFFD9
#define IVECT (volatile unsigned int *) (0x20)

interrupt void intserv();
unsigned char digit = 0; /* Digit to be displayed */
unsigned char switch_d = 0x1 /* Switch E state: 1/0 = on/off */
unsigned char switch_e = 0x2 /* Switch D state: 1/0 = on/off */
int main() {
  *PADIR = 0x00; /* Set Port A direction */
  *PBDIR = 0xF3; /* Set Port B direction */
  *CTCON = 0x02; /* Stop Timer */
  *CTSTAT = 0x0; /* Clear “reached 0” flag */
  *CNTM = 100000000; /* Initialize Timer */
  *IVECT = (unsigned int *) &intserv; /* Set interrupt vector */
  asm(“MoveControl PSR,#0x40”); /* CPU responds to IRQ */
  asm(“BitClear #6, PSR”); /*Clear processor interrupt bit to disable interupts*/
  *CTCON = 0x11; /* start counting with interrupt*/
  *PBOUT = 0x0; /* Display 0 */
  while (1) {
    /*Switch E at PB[0] allows the digit to increment*/
    while ((*PBIN & switch_d) != 0); /* Wait until Switch E is pressed */
    while ((*PBIN & switch_d) == 0); /* Wait until Switch E is released */
    asm(“BitSet #6, PSR”); /*Set processor interrupt bit to enable interupts*/
    /*Switch D at PB[1] disables the digit incrementing*/
    while ((*PBIN & switch_e) != 0); /* Wait until Switch E is pressed */
    while ((*PBIN & switch_e) == 0); /* Wait until Switch E is released */
    asm(“BitClear #6, PSR”); /*Clear processor interrupt bit to disable interupts*/
  }
  exit(0);
}
interrupt void intserv() {
 /*Conditionally increment the displays digit every second*/
  *CTSTAT = 0x0; /* Clear “reached 0” flag for timer*/
  digit = (digit + 1)%10; /* Increment digit */
  *PBOUT = ((digit << 4) | *PBOUT & 0x0F); /* Update Port B by making the left 4 bits store the digit and keeping whatever is still in Port B the same */
}
