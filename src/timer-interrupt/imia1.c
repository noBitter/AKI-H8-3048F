#include <3048f.h>

/* ITU1.TCR.BYTE 
 *     7     6     5     4     3     2     1     0
 * -(0)- CCLR1 CCLR0 CKEG1 CKEG0 TPSC2 TPSC1 TPSC0
 *
 *
 * TPSC: System clock
 * 000  001  010  011
 *  16    8    4    2 (MHz)
 *
 * CKEG: Count timing
 *     00      01   1-
 * Rising Falling Both (Edge)
 *
 * CCLR: Clear settings
 *           00             01  10     11
 * Forbid clear (Clear w/) GRA GRB Others 
 * */

/* ITU1.TIER.BYTE
 *     7     6     5     4     3     2     1     0
 * -(1)- -(1)- -(1)- -(1)- -(1)- OVIE  IMIEB IMIEA
 *  
 * Interrupt by flags
 * 0: Forbid, 1: Allow
 * */

/* ITU.TSTR.BYTE
 *     7     6     5    4     3     2     1     0
 * -(1)- -(1)- -(1)- STR4  STR3  STR2  STR1  STR0
 * 
 * Timer counter
 * STR0 - STR4
 * ITU0 - ITU4
 * 0: Stop, 1:Start
 * */

#pragma interrupt(int_imia1)
int cnt;

int main(void){
	unsigned char sw;
	
	P4.DDR = 0x00;		/* Set Port4 as input for Tact switches */
	P4.PCR.BYTE = 0xFF;	/* Turn on Pull-UP resistors of Port4 */
	P5.DDR = 0xFF;		/* Set Port5 as input for LEDs */
	P5.PCR.BYTE = 0x00;	/* Turn off Pull-UP resistors of port5 */
	
	ITU1.TCR.BYTE = 0x23;	/* Internai clock: 2-MHz, Counter: Rising Edge, Clear: GRA */	
	ITU1.GRA = 0x4E20;	/* Cycle: 10-ms (2-MHz * 20,000) */
	ITU1.TIER.BYTE = 0xF9;	/* Allow interrupt by IMIEA flag */
	ITU.TSTR.BIT.STR1 = 0;	/* Stop ITU1 Counter */
	ITU.TSTR.BIT.STR1 = 1;	/* Start ITU1 Counter */
	
	cnt = 0;

	while(1){
		sw = ~P4.DR.BYTE;			/* Reverse Port4 signal */
		if(sw == 0x10) P5.DR.BIT.B0 = 1;	/* Turn on LED1 when switch S1 is pressed */
		else P5.DR.BIT.B0 = 0;			/* Turn off LED1 when no switches are presed */
	}

	return 0;
}

/* Interrupt Processing Function **/
/* Called when IMIEA Flag is true */
void int_imia1(void){
	cnt++;

	if(cnt == 100){				/* Turn LED2 every 10-ms */
		cnt = 0;
		P5.DR.BIT.B1 = ~P5.DR.BIT.B1;	/* Blink LED2 by reversing signals */
	}
	ITU1.TSR.BIT.IMFA = 0;			/* Put IMIEA Flag back false and resume */
}
