#define PBIN (volatile unsigned char *) 0xFFFFFFF3
#define PBOUT (volatile unsigned char *) 0xFFFFFFF4
#define PBDIR (volatile unsigned char *) 0xFFFFFFF5
#define PSTAT (volatile unsigned char *) 0xFFFFFFF6
#define PCONT (volatile unsigned char *) 0xFFFFFFF7
#define CNTM (volatile unsigned int *) 0xFFFFFFD0
#define CTCON (volatile unsigned char *) 0xFFFFFFD8
#define CTSTAT (volatile unsigned char *) 0xFFFFFFD9
#define IVECT (volatile unsigned int *) (0x20)

interrupt void intserv();
unsigned char digit = 0; /* Digit to be displayed */
unsigned char enabled = 0; /*Whether the display incrementing is enabled (0/1 = disabled/enabled)
int main() {
  *PBDIR = 0xF3; /* Set Port B direction */
  *PSTAT = 0x00 /* Clear Status register just in case*/
  *PCONT = 0x20 /* Set ENBIN to 1 to enable interrupts on changes to PBIN /*
  *CTCON = 0x02; /* Stop Timer */
  *CTSTAT = 0x0; /* Clear “reached 0” flag */
  *CNTM = 100000000; /* Initialize Timer */
  *IVECT = (unsigned int *) &intserv; /* Set interrupt vector */
  asm(“MoveControl PSR,#0x40”); /* CPU responds to IRQ */
  *CTCON = 0x1; /* start counting without interrupt*/
  *PBOUT = 0x0; /* Display 0 */
  while (1) {
    while (*CTSTAT & 0x1 != 0); /* Wait for timer to end */
    digit = (digit + 1)%10; /* Increment digit */
    *PBOUT = ((digit << 4) | *PBOUT & 0x0F); /* Update Port B by making the left 4 bits store the digit and keeping whatever is still in Port B the same */
    *CTSTAT = 0x0; /* Clear “reached 0” flag for timer*/
  }
  exit(0);
}
interrupt void intserv() {
     /* Assume we don't know if it will be IBIN that caused interrupt */
     if(*PSTAT & 0x20 != 0){
      /*Check whether the display is incrementing*/
      if(enabled){
       /* Check if switch d has been pressed to disable incrementing */
       if()
      }
     }
     /*Switch E at PB[0] allows the digit to increment*/
    while ((*PBIN & switch_d) != 0); /* Wait until Switch E is pressed */
    while ((*PBIN & switch_d) == 0); /* Wait until Switch E is released */
    asm(“BitSet #6, PSR”); /*Set processor interrupt bit to enable interupts*/
    /*Switch D at PB[1] disables the digit incrementing*/
    while ((*PBIN & switch_e) != 0); /* Wait until Switch E is pressed */
    while ((*PBIN & switch_e) == 0); /* Wait until Switch E is released */
    asm(“BitClear #6, PSR”); /*Clear processor interrupt bit to disable interupts*/
}
