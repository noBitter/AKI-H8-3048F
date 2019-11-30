#include <3048f.h>

/* DDR: Data Direction Registr
 * 0: Input, 1: Output 
 * 
 * Port4: Tact Switches, Input
 * Port5: Standard LEDs, Output
 */

/* DR: Data Ragister
 *
 * PORT4.DR *******************
 *  7   6   5   4  3  2  1  0 *
 * S4  S3  S2  S1  1  1  1  1 *
 ******************************
 * S1 - S4: Tact Switches
 * 0: ON, 1: OFF
 *
 * PORT5.DR ***************************
 * 7  6  5  4     3     2     1     0 *
 * 0  0  0  0  LED4  LED3  LED2  LED0 *
 **************************************
 * 0: OFF, 1: ON
 */

/* PCR: Pull-UP MOS Control Register
 * 1: ON, 0: OFF
 *
 * All PCR should turned on in Port4.
 * Due to set Port4 DR high if a switch is unpushed.
 */

int main(void){
	unsigned char sw;

	P4.DDR = 0x00;
	P4.PCR.BYTE = 0xFF;
	P5.DDR = 0xFF;
	P5.PCR.BYTE = 0x00;
	
	while(1){
		sw = ~P4.DR.BYTE; /* ~: Reverse binary number */
		
		if(sw == 0x10) P5.DR.BYTE = 0x01;	/* S1 -> LED1 */
		else if(sw == 0x20) P5.DR.BYTE = 0x02;	/* S2 -> LED2 */
		else if(sw == 0x40) P5.DR.BYTE = 0x04;	/* S3 -> LED3 */
		else if(sw == 0x80) P5.DR.BYTE = 0x08;	/* S4 -> LED4 */
		else if(sw == 0x30) P5.DR.BYTE = 0x03;	/* S1, 2 -> LED1, 2 */
		else P5.DR.BYTE = 0x00;			/* All LEDs are off */
	}
	
	wait(5000); /* Avoid chattering */
	
	return 0;
}
