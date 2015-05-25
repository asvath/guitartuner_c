#include "msp430.h"

// This is a program for an electric guitar tuner.
// The period of the input guitar signal was compared to the period of standard tuning. 
//The input period was calculated using zero crossing

volatile unsigned int last_tar=0;

volatile unsigned int last_diff=0;

volatile unsigned int current_diff;

volatile unsigned int diff;

void display_7seg(char address,char digit){ //

  P2OUT &= ~3;// sets A0,A1 to 0,0 
  P2OUT|= (address & 3);//sets A0,A1 to address
  P1OUT &= ~0xF0;//set P1.4-1.7 to 0
  P1OUT |=(digit & 0xF) <<4;// sets data lines
  P2OUT &= ~4;//turns off stobe P2.2
  P2OUT |=4;//turns strobe back on


}
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  DCOCTL = 0x00; //DCOCLK to 1MHz
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;


  P1DIR = 0xF9; //  P1.0,4,5,6,7 as output
  //P1DIR=0xFF;
  P2DIR =0x3F; //all pin 2 output
  display_7seg(0,0);
  display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  

P1SEL=2;  // P1.1 to TA0.0:. makes the pin to be the timer input bit TA0.0.
  //P2SEL=2 ; // P2.1 to TA0.0:. makes the pin to be the timer intput bit TA0.0
 TACTL = TASSEL_2 + MC_2; //SMLK, continuous mode.cont means: the Timer repeatedly counts from 0 to 0FFFFh.

 TACCTL0 = CM_2 + CCIS_0 + SCS + CAP + CCIE; //capture on falling edge + CC10A + synchonize capture + set capure mode + interrupt enabled 

 

 _BIS_SR(LPM0_bits + GIE); //Enter Low Power Mode 0

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  static int count=0;
  
  TACCTL0 &= ~ CCIFG;
  //last_tar=0;
  //last_diff=0;
  //P1OUT=0;
  //P2OUT=0;
  current_diff=TACCR0-last_tar; //Current timer value - last timer value = period
  diff=last_diff-current_diff;
  if (diff<0){
    diff= 0-diff;}
  
  last_diff=current_diff;
  last_tar=TACCR0;

 
  //E: 6th string
 
  //E: 6th string
  //Eb:71.85 hz< E< 81.85 hz

  if ((12288 <current_diff)&&(current_diff<14004)){ //the numbers are in microseconds
    P1OUT=0x01; //E string flat: turns on the yellow light to indicate a flat note
      P2OUT=0x30;
      display_7seg(0,6); //displays the number 6 on the first LED on the 7 seg display as E is the 6th string
display_7seg(1,0); //displays the number 0 for the other LEDs
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

  //E: 81.85hz <E<82.15hz in tune


   else if ((12243 <current_diff)&&(current_diff<12288)){ //E string in tune. 
       P1OUT=0x01; //turns on the green light to indicate a note that is in tune
      P2OUT=0x10;
      display_7seg(0,6);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }
   
  //E#: 82.15hz<E<95.95hz

   else if( (10476 <current_diff)&&(current_diff<12243)){
      P1OUT=0x01;//turns on the red light to indicate a sharp note
      P2OUT=0x18;
      display_7seg(0,6);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

  
  //A String 
  else  if ((9116 <current_diff)&&(current_diff<9149)){ //A string in tune. between 110.2hz and 109.8 hz; green led and 5 display
       P1OUT=0x01;
      P2OUT=0x10;
      display_7seg(0,5);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }
    
   else if ((9149 <current_diff)&&(current_diff<10476)){
    P1OUT=0x01; //A string flat, yellow light
      P2OUT=0x30;
      display_7seg(0,5);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }
  else if ((7812 <current_diff)&&(current_diff<9116)){//A string sharp, red light
      P1OUT=0x01;
      P2OUT=0x18;
      display_7seg(0,5);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }


  //D string:

  //D flat : 128.5<D<146.73, yellow light
 else if ((6837 <current_diff)&&(current_diff<7812)){ 
    P1OUT=0x01; 
      P2OUT=0x30;
      display_7seg(0,4);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

  //D in tune: 146.73<D<147.27, green light

 else  if ((6811 <current_diff)&&(current_diff<6837)){ 
       P1OUT=0x01;
      P2OUT=0x10;
      display_7seg(0,4);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }
  //D#: <147.27<D<171.47, red light

 else if ((5845 <current_diff)&&(current_diff<6811)){
      P1OUT=0x01;
      P2OUT=0x18;
      display_7seg(0,4);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

 //G string:

 //gflat : 171.47 <G<195.64, yellow light

 else if ((5117 <current_diff)&&(current_diff<5845)){
    P1OUT=0x01; 
      P2OUT=0x30;
      display_7seg(0,3);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

  //G in tune: 195.643<G<196.36, green light

 else  if ((5099 <current_diff)&&(current_diff<5117)){ 
       P1OUT=0x01;
      P2OUT=0x10;
      display_7seg(0,3);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }
  //G#: <196.36<G<221.46, red light

 else if ((4516 <current_diff)&&(current_diff<5099)){
      P1OUT=0x01;
      P2OUT=0x18;
      display_7seg(0,3);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

  //B string


 //bflat : 221.46 <G<246.55, yellow light

 else if ((4052 <current_diff)&&(current_diff<4516)){
    P1OUT=0x01; 
      P2OUT=0x30;
      display_7seg(0,2);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

  //B in tune: 246.55<B<247.45, green light

 else  if ((4037 <current_diff)&&(current_diff<4052)){ 
       P1OUT=0x01;
      P2OUT=0x10;
      display_7seg(0,2);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }
  //B#: 247.456<B<288.45, red light

 else if ((3458 <current_diff)&&(current_diff<4037)){
      P1OUT=0x01;
      P2OUT=0x18;
      display_7seg(0,2);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

  //e string:

//eflat : 288.45 <e<329.4, yellow light

 else if ((3023 <current_diff)&&(current_diff<3458)){
    P1OUT=0x01; 
      P2OUT=0x30;
      display_7seg(0,1);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

  //e in tune: 329.4<e<330.6, green light

 else  if ((3012 <current_diff)&&(current_diff<3023)){ 
       P1OUT=0x01;
      P2OUT=0x10;
      display_7seg(0,1);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }
  //e#: 330.6<e<340.6, red light

 else if ((2922 <current_diff)&&(current_diff<3012)){
      P1OUT=0x01;
      P2OUT=0x18;
      display_7seg(0,1);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);
  return;
    }

 
 else {P1OUT=0;
   P2OUT=0;
 display_7seg(0,0);
display_7seg(1,0);
  display_7seg(2,0);
  display_7seg(3,0);};
}
