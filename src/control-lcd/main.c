#include <3048f.h>

/* Port3: LCD
 * P3.DR.BYTE *******************
 * 7  6  5   4    3   2   1   0 *
 * -  -  E  RS  DB7 DB6 DB5 DB4 *
 * ******************************
 */

/* LCD pin state & Control instructions *******************************************************
 * Control instructions    E  RS  R/W  DB7  DB6  DB5  DB4  DB3  DB2  DB1  DB0  Execution time *
 * Clear display           -   0    0    0    0    0    0    0    0    0    1         1.64-ms *
 * Cursor home             -   0    0    0    0    0    0    0    0    1    -         1.64-ms *
 * Set Entry mode          -   0    0    0    0    0    0    0    1  I/D    S           40-us *
 * Control Display ON/OFF  -   0    0    0    0    0    0    1    D    C    B           40-us *
 * Cursor, Shift display   -   0    0    0    0    0    1  S/C  R/L    -    -           40-us *
 * Function set            -   0    0    0    0    1   DL    N    F    -    -           40-us *
 * Set CGRAM address       -   0    0    0    1  ----------- CGRAM address --           40-us *
 * Set DDRAM address       -   0    0    1  ---------------- DDRAM address --           40-us *
 * BF, Read address        -   0    1   BF  -------------- Address counter --           40-us *
 * Write to CGRAM/DDRAM    -   1    0  ------------------------ Write data --           40-us *
 * Read from CGRAM/DDRAM   -   1    1  ------------------------- Read data --           40-us *
 * ********************************************************************************************
 *
 * CGRAM: Character Generator RAM
 * DDRAM: Display Data RAM
 * BF: Busy Flag
 *
 * Clear display: Clear all display
 * Cursor home: Set the cursor to home position
 * Set Entry mode:
 *     I/D: Write or read CGRAM and DDRAM direction [1: Positive, 0: Negative]
 * Control Display ON/OFF:
 *     D: Display [1: ON, 0: OFF] with keeping DDRAM data
 *     C: Cursor [1: ON, 0: OFF]
 *     B: Cursor blinking [1: ON, 0:OFF]
 * Cursor, Shift display
 *     R/L: Shift display [1: Right, 0: Left]
 *     S/C: Shift cursor when shift display [1: ON, 0: OFF]
 * Function set:
 *     DL: Interface data length [1: 8-bit (Upper 4 -> Lower 4), 0: 4-bit]
 *     N: Line number [1: 1st, 0: 2nd]
 *     F: Set the text font, usually 0
 */

#define LCD P3.DR.BYTE
#define LCD_RS P3.DR.BIT.B4
#define LCD_E P3.DR.BIT.B5

void LCD_dsp(unsigned char *, int);
void LCD_out4(unsigned char);
void wait_time(unsigned int);
void ioinit(void);
void wait20u(void);
void LCD_out8(unsigned char);

int main(void){
	unsigned char str1[16] = "  Welocome to   ";
	unsigned char str2[16] = "   TEST Program.";

	ioinit();
	LCD_dsp(str1, 1);
	LCD_dsp(str2, 2);

	while(1);

	return 0;
}

/* Display strings on LCD */
void LCD_dsp(unsigned char *str, int line){
	unsigned char *ad;
	int i;

	LCD_RS = 0;						/* Set function */
	if(line == 1) LCD_out4(0x02);	/* Cursor home */
	else LCD_out4(0xC0);			/* Cursor 2nd line */
	wait_time(164);					/* Wait >= 1.64-ms */
	ad = str;
	while(*ad){
		LCD_RS = 1;					/* Set data mode */
		LCD_out4(*ad);
		LCD_RS = 0;
		ad++;
	}
}

/* 8-bit transfer */
void LCD_out8(unsigned char data){
	LCD_RS = 0;	/* Set function */
	LCD_E = 1;	/* E signal set */
	LCD = data;	/* Output */
	wait20u();
	LCD_E = 0;
	wait20u();
}

/* 4-bit transfer */
void LCD_out4(unsigned char data){
	unsigned char tmp1,tmp2;

	LCD_E = 1;					/* E signal set */
	tmp1 = LCD & 0xF0;			/* Store RS signail */
	tmp2 = (data >> 4) & 0x0F;	/* Upper 4-bit set */
	LCD = tmp1 | tmp2;			/* Output RS+data(up4) */
	wait20u();
	LCD_E = 0;
	wait20u();

	LCD_E = 1;				/* E signal set */
	tmp1 = LCD & 0xF0;		/* Store RS signal */
	tmp2 = data & 0x0F;		/* Lower 4-bit set */
	LCD = tmp1 | tmp2;		/* Output RS+data(low4) */
	wait20u();
	LCD_E = 0;
	wait20u();
}

void wait_time(unsigned int tm){ /* Time unit is 0.01-ms */
	unsigned int t;
	t = tm * 10;
	while(t--);
}

void wait20u(void){ /* Wait >= 20-us */
	unsigned int dmy;
	for(dmy = 50; dmy > 0; dmy--);
}

/* LCD initial settings */
void ioinit(void){
	P3.DDR = 0xFF;		/* Set Port3 as output for LCD */

	wait_time(1500);	/* Wait 15-ms (V_DD reaches 4.5-V) */
	LCD_out8(0x23);		/* Function set (1): Lower 4-bit, DB3-DB0 are all low */
	wait_time(410);
	LCD_out8(0x23);		/* Function set (2) */
	wait_time(10);
	LCD_out8(0x23);		/* Function set */
	wait_time(4);
	LCD_out8(0x22);		/* Function set */
	wait_time(4);
	LCD_RS = 0;			/* Set function */
	LCD_out4(0x28);		/* Function set */
	wait_time(4);
	LCD_RS = 0;
	LCD_out4(0x0E);		/* Display ON */
	wait_time(4);
	LCD_RS = 0;
	LCD_out4(0x06);		/* Entry mode */
	wait_time(4);
	LCD_RS = 0;
	LCD_out4(0x01);		/* Clear display */
	wait_time(164);		/* Wait >= 1.64-ms */
}