
#include <msp430.h>
#include "buzzer.h"	

/**Initalizes the buzzer so that the buzzer can be used
After this is called, it will start buzzing*/
void buzzer_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR |= BIT6;		/* enable output to speaker (P2.6) */
    buzzer_set_period(600);	/* start buzzing!!! */
}

/**Starts the buzzer, you need to make sure buzzer is playing at frequency*/
void buzzer_stop(){
  P2DIR &= ~BIT6;
}

/**Stops the buzzer from being able to tbe played*/
void buzzer_start(){
  P2DIR |= BIT6;
}

/**Changes speed of the buzzer*/
void buzzer_set_period(short cycles){
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}


    
    
  

