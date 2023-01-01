/*
 * LoRa Wind Version 2
 * 9th January 2022
 * Version 2 attempts to get the standby current down really low to save
 * battery life.
 * File:   main.c
 * Author: andym
 *
 * Created on 09 January 2022, 11:18
 * Trace reduction in current:
 * Just SLEEP() with standard config.h, 2s watchdog, 64MHz crystal osc.  1.67mA
 * 337µA after turning off all peripherals and putting LoRa module into sleep mode with 16MHz crystal osc.
 * 29µA in wind tacho pull up 100k - change to 1M - need to check if tacho will still work after this.  Now 6uA if tacho in "ON" position
 * 333µA now.  What's left in the wrong state or turned on?
 * 48µA after setting RB1 high on PCB000040 (10k pullup to permanent 3V supply) Wanders around between 38uA and 52uA
 * 
 * 11th February 2022, 19:23 - Now on PCB000044 REV 1
 * 3µA steady current achieved without LoRa module fitted - just sleeping with everything shutdown.
 * 20:54: Now achieved 0 to 1µA (almost unmeasurable) by making sure programming pins are set as outputs and set low.
 * Otherwise these gradually float up after the programmer is removed, drawing 15µA or more after 30 minutes.
 * 22:00: Still <=1µA with LoRa module installed and set to sleep (1.5mA if you don't talk to it and put it to sleep).
 * 
 */


#include <xc.h>
#include "config.h"
#include "LoRa.h"

#define TX_FREQ 866.5
#define SYNC_WORD 0x55

void shutdown(void); //Shuts everything non-essential down to minimise power consumption.

void main(void) {
    OSCCONbits.IRCF=0b111; //Set internal clock to 16MHz
    OSCCONbits.OSTS=0; //Device is running from internal oscillator
    OSCCON2bits.PLLRDY=1; //System clock comes from 4xPLL
    OSCTUNEbits.PLLEN=1;//Enable PLL
    LoRaReset();
    LoRaStart(TX_FREQ, SYNC_WORD);
    __delay_ms(10);
    LoRaSleepMode();
    __delay_ms(10);
    shutdown();
    while(1){
        SLEEP();
        LATEbits.LE2=1; //Turn LED on
        __delay_ms(50);
        LATEbits.LE1=1;
        __delay_ms(50);
        LATEbits.LE0=1;
        __delay_ms(50);
        LATEbits.LE0=0;
        LATEbits.LE1=0;
        LATEbits.LE2=0; //Turn LED off again
    }
    
}

void shutdown(){
    //PORT A
    TRISAbits.RA0=0; //A0 is an output
    LATAbits.LA0=0; //Set to 0
    ANSELAbits.ANSA0=0; //Turn off analog input
    TRISAbits.RA1=0;
    LATAbits.LA1=0;
    ANSELAbits.ANSA1=0; //Turn off analog input
    TRISAbits.RA2=0; //A2 is an output
    LATAbits.LA2=1; //Set high to turn off PNP transistor
    ANSELAbits.ANSA2=0; //Turn off analog input
    TRISAbits.RA3=0; //A3 is an output
    LATAbits.LA3=0; //Set to 0
    ANSELAbits.ANSA3=0; //Turn off analog input
    TRISAbits.RA4=0; //A4 is an output
    LATAbits.LA4=0; //Set to 0
    //No analog on this port pin
    TRISAbits.RA5=0; //A5 is an output
    LATAbits.LA5=0; //Set to 0
    ANSELAbits.ANSA5=0; //Turn off analog input
    TRISAbits.RA6=0; //A6 is an output
    LATAbits.LA6=0; //Set to 0

    TRISAbits.RA7=0; //A5 is an output
    LATAbits.LA7=0; //Set to 0
    
    
    //PORT B
    LATBbits.LB0=0;
    TRISBbits.RB0=0;
    ANSELBbits.ANSB0=0;
    LATBbits.LB1=1; //Set high as 10k pullup to 3V on PCB000040
    TRISBbits.RB1=0;
    ANSELBbits.ANSB1=0;
    LATBbits.LB2=0;
    TRISBbits.RB2=0;
    ANSELBbits.ANSB2=0;
    LATBbits.LB3=0;
    TRISBbits.RB3=0;
    ANSELBbits.ANSB3=0;
    LATBbits.LB4=0;
    TRISBbits.RB4=0;
    ANSELBbits.ANSB4=0;
    LATBbits.LB5=0;
    TRISBbits.RB5=0;
    ANSELBbits.ANSB5=0;
    LATBbits.LB6=0;
    TRISBbits.RB6=0;
    LATBbits.LB7=0;
    TRISBbits.RB7=0;
    
    //PORT C
    LATCbits.LC0=0; //Input for wind tacho signal
    
    LATCbits.LC1=0;
    //No analogue on C0 or C1
    TRISCbits.RC1=0;
    
    LATCbits.LC2=0;
    TRISCbits.RC2=0;
    ANSELCbits.ANSC2=0;
    
    LATCbits.LC3=0;
    TRISCbits.RC3=0;
    ANSELCbits.ANSC3=0;
    
    LATCbits.LC4=0;
    TRISCbits.RC4=0;
    ANSELCbits.ANSC4=0;
    
    LATCbits.LC5=0;
    TRISCbits.RC5=0;
    ANSELCbits.ANSC5=0;
    
    LATCbits.LC6=0;
    TRISCbits.RC6=0;
    ANSELCbits.ANSC6=0;
    
    LATCbits.LC7=0;
    TRISCbits.RC7=0;
    ANSELCbits.ANSC7=0;
    
    //PORT D
    TRISDbits.RD0=0;
    LATDbits.LD0=0; //Leave high (SPI SCK)
    ANSELDbits.ANSD0=0; //Turn off analog
    
    TRISDbits.RD1=1; //Leave as an input (SPI MISO)
    //LATDbits.LD1=0;
    ANSELDbits.ANSD1=0; //Turn off analog
    
    TRISDbits.RD2=0;
    LATDbits.LD2=0;
    ANSELDbits.ANSD2=0; //Turn off analog
    
    TRISDbits.RD3=0; //Output
    LATDbits.LD3=1; //CS for SPI set to 1 to deselect LoRa device
    ANSELDbits.ANSD3=0; //Turn off analog
    
    TRISDbits.RD4=0; //Output
    LATDbits.LD4=0; //Leave high (SPI MOSI)
    ANSELDbits.ANSD4=0; //Turn off analog
    
    TRISDbits.RD5=0; //Output
    LATDbits.LD5=0;
    ANSELDbits.ANSD5=0; //Turn off analog
    
    TRISDbits.RD6=0; //Output
    LATDbits.LD6=0;
    ANSELDbits.ANSD6=0; //Turn off analog
    
    TRISDbits.RD7=0; //Output
    LATDbits.LD7=0;
    ANSELDbits.ANSD7=0; //Turn off analog
    
    //PORT E
    TRISEbits.RE0=0; //E0 is an output
    LATEbits.LE0=0; //Set to 0
    ANSELEbits.ANSE0=0; //Turn off analogue
    TRISEbits.RE1=0;
    LATEbits.LE1=0;
    ANSELEbits.ANSE1=0; //Turn off analogue
    TRISEbits.RE2=0; //Red LED is on this pin connected to ground via 150R resistor.
    LATEbits.LE2=0;
    ANSELEbits.ANSE2=0; //Turn off analogue
    
    //Peripherals
    ADCON0bits.ADON=0; //Turn off A to D module
    VREFCON0bits.FVREN=0; //Disable internal reference
    PMD0bits.UART2MD=1; //Turn off UART2
    PMD0bits.UART1MD=1; //Turn off UART1
    PMD0bits.TMR6MD=1; //Turn off timer 6
    PMD0bits.TMR5MD=1; //Turn off timer 5
    PMD0bits.TMR4MD=1; //Turn off timer 4
    PMD0bits.TMR3MD=1; //Turn off timer 3
    PMD0bits.TMR2MD=1; //Turn off timer 2
    PMD0bits.TMR1MD=1; //Turn off timer 1
    
    PMD0bits.SPI2MD=1; //Turn off SPI2
    PMD0bits.SPI1MD=1; //Turn off SPI1
    PMD0=0xFF; //Turn off all peripherals in PMD0 (UARTS, SPI)
    PMD1=0xFF; //Turn off all peripherals in PMD1 (including MSSP2 for SPI2)
    PMD2=0xFF; //Turn off all peripherals in PMD2 (ADC, comparators, CTMU)
}
