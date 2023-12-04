#include "nios2_control.h"

/* place additional #define macros here */
#include "chario.h"
#include "adc.h"
/* define global program variables here */

#ifndef _TIMER_H_
#define _TIMER_H_


/* define pointer macros for accessing the timer interface registers */

#define TIMER_STATUS	((volatile unsigned int *) 0x10004020)

#define TIMER_CONTROL	((volatile unsigned int *) 0x10004024)

#define TIMER_START_LO	((volatile unsigned int *) 0x10004028)

#define TIMER_START_HI	((volatile unsigned int *) 0x1000402C)

#define TIMER_SNAP_LO	((volatile unsigned int *) 0x10004030)

#define TIMER_SNAP_HI	((volatile unsigned int *) 0x10004034)


/* define a bit pattern reflecting the position of the timeout (TO) bit
   in the timer status register */

#define TIMER_TO_BIT 0x1


#endif /* _TIMER_H_ */

/*-----------------------------------------------------------------*/

#ifndef _LEDS_H_
#define _LEDS_H_


/* define pointer macro for accessing the LED interface data register */

#define LEDS	((volatile unsigned int *) 0x10000010)


#endif /* _LEDS_H_ */
/* place additional functions here */

/* place additional #define macros here */
#define TIMER1_STATUS	((volatile unsigned int *) 0x10004020)

#define TIMER1_CONTROL	((volatile unsigned int *) 0x10004024)

#define TIMER1_START_LO	((volatile unsigned int *) 0x10004028)

#define TIMER1_START_HI	((volatile unsigned int *) 0x1000402C)

#define TIMER1_SNAP_LO	((volatile unsigned int *) 0x10004030)

#define TIMER1_SNAP_HI	((volatile unsigned int *) 0x10004034)



#define TIMER3_STATUS	((volatile unsigned int *) 0x10004060)

#define TIMER3_CONTROL	((volatile unsigned int *) 0x10004064)

#define TIMER3_START_LO	((volatile unsigned int *) 0x10004068)

#define TIMER3_START_HI	((volatile unsigned int *) 0x1000406C)

#define TIMER3_SNAP_LO	((volatile unsigned int *) 0x10004070)

#define TIMER3_SNAP_HI	((volatile unsigned int *) 0x10004074)
//JTAG DATA

#define JTAG_UART_DATA	((volatile unsigned int *) 0x10001000)
#define JTAG_UART_STATUS	((volatile unsigned int *) 0x10001004)

/* define global program variables here */
#define HEX_DISP ((volatile unsigned int *) 0x10000020)

#define SWIT_X ((volatile unsigned int *) 0x10000040)

/*-----------------------------------------------------------------*/
/* this routine is called from the_exception() in exception_handler.c */

int counter = 0;
int counterEF = 0;

volatile int timer_3_flag = 0;
volatile int timer_1_flag = 0;


void interrupt_handler(void)
{
	unsigned int ipending;

   
   unsigned int swit = *SWIT_X;
   /* read current value in ipending register */
   ipending = NIOS2_READ_IPENDING();

	/* do one or more checks for different sources using ipending value */
	if ((ipending & 0x10000)) { 
		timer_3_flag = 1;
		*TIMER3_STATUS = 0;
	}
	if ((ipending & 0x4000)) { 
		timer_1_flag = 1;
		*TIMER1_STATUS = 0;
	}	
   /*if ((ipending & 0x8000) && (swit & 0x2)) { 
		*TIMER2_STATUS = 0;
		*JTAG_UART_DATA	= 0X21; // write '!' to jtag
		//*JTAG_UART_DATA	= 0X21; // write '!' to jtag
	}
	
	//
   if ((ipending & 0x10000) && (swit & 0x1)) { 
		*TIMER3_STATUS = 0;
		if (counterEF==0){
			*HEX_DISP = 0x7F000000; //toggle leftmost LED
			counterEF++;
		}
		else if (counterEF == 1){
			*HEX_DISP = 0x7F; //toggle leftmost LED
			counterEF=0;
		} */

	
}
   /* remember to clear interrupt sources */

	/* read current value in ipending register */

	/* do one or more checks for different sources using ipending value */

	/* remember to clear interrupt sources */


/*-----------------------------------------------------------------*/
//GetChar
unsigned int GetChar(void){
	unsigned int st, data;
	
	do {
		data = *JTAG_UART_DATA;
		st = data & 0x8000;
	} while (st == 0);
	
	data = data & 0xFF;
	
	return data;
}



void Init (void)
{
			/* initialize software variables */
   /*
   //50 MHz = 20ns, 0.5s/20ns=25 000 000 = 0x017D7840
   *TIMER_START_LO=0x7840;
   *TIMER_START_HI=0x017D;
   *TIMER_STATUS= 0x1; //assumption!!
   *TIMER_CONTROL=7;
   */
   
   //50 MHz = 20ns, 1s/20ns=50 000 000 = 0x02FAF080
   *TIMER1_START_LO=0xF080;
   *TIMER1_START_HI=0x02FA;
   *TIMER1_STATUS= 0x1; //assumption!!
   *TIMER1_CONTROL=0x7;
   

	//50 MHz = 20ns, 0.25s/20ns=100 000 000 = 0x05F5E100
   *TIMER3_START_LO=0xBC20;
   *TIMER3_START_HI=0x00BE;
   *TIMER3_STATUS= 0x1; //assumption!!
   *TIMER3_CONTROL=0x7;
	/* set up each hardware interface */
	*LEDS = 0x300; //toggle leftmost LED
   
	/* set up ienable */
   NIOS2_WRITE_IENABLE(0x1C000);
   NIOS2_WRITE_STATUS(0x1);

}

/*-----------------------------------------------------------------*/

	unsigned int x; 
	int swit;
int main (void)
{
	Init ();	/* perform software/hardware initialization */
	InitADC(2, 2);
	
	int rightmost = 0;
	
	if (GetChar()== 'r'|| GetChar()== 'R'){
		rightmost = 1;
	}
	else{
		rightmost = 0;
	}
	
	PrintString("\n");
	PrintString("ELEC 371 Lab 4 By Ahmed, Aria, Leo, and Michael.");
	PrintString("\n");
	PrintString("four lowest switches: 0x\?");
	
	while (1){
		
		if (timer_3_flag == 1){
			x = ADConvert();
			x= x/2;
		
			if (rightmost == 1){
				*HEX_DISP = x; 
			}
			else if (rightmost == 0){
				x= x<<24;
				*HEX_DISP = x; 
			}
			//LED cycle
			if (counter==0){
				*LEDS = 0x300; //toggle leftmost LED
				counter++;
			}
			else if (counter == 1){
				*LEDS = 0x030; 
				counter++;
			}
			else if (counter == 2){
				*LEDS = 0x003; //toggle rightmost LED
				counter=0;
			}
			
			
		timer_3_flag = 0;	
		}
		
		if (timer_1_flag){
			  timer_1_flag = 0;
			  swit = *SWIT_X & 0xF;
			  PrintChar('\b');
			  PrintHex(swit);
			  //PrintHex((swit & 0xF0) >> 4);
			  //PrintHex(swit & 0xF);
			}
			

		}

	
	return 0;	/* never reached, but main() must return a value */
}
