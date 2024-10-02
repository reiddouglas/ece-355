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
unsigned char enabled = 0; /*Whether the display incrementing is enabled (0/1 = disabled/enabled)*/
unsigned char switch_d = 0x2; /* Enable Switch D */
unsigned char switch_e = 0x1; /* Enable Switch E */
int main() {
  *PBDIR = 0xF3; /* Set Port B direction */
  *PSTAT = 0x00; /* Clear Status register */
  *PCONT = 0x20; /* Set ENBIN to 1 to enable interrupts on changes to PBIN */
  enabled = 0; /* Timer is disabled (stopped) by default */
  *CTCON = 0x02; /* Stop Timer */
  *CTSTAT = 0x0; /* Clear “reached 0” flag */
  *CNTM = 100000000; /* Initialize Timer */
  *IVECT = (unsigned int *) &intserv; /* Set interrupt vector */
  asm(“MoveControl PSR,#0x40”); /* CPU responds to IRQ */
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
     /* Check IBIN for interrupt bit*/
     if(*PSTAT & 0x20 != 0){
      /*Check whether the display is incrementing*/
      if(enabled){
       /* Check if switch d has been pressed to disable incrementing */
       if(*PBIN & switch_d != 0){
        /*Pause countdown*/
        *CTCON = 0x02; /* Stop Timer */
        enabled = 0;
       }
      } else {
        /* Check if switch e has been pressed to enable incrementing */
       if(*PBIN & switch_e != 0){
        /* Start countdown */
        *CTCON = 0x01; /* Start Timer */
        enabled = 1;
      }
     }
}
