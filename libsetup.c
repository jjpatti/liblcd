/****************************************************************
 * Author: John J. Patti
 *
 * Version: 0.1
 * Description: LCD Control Functions
 * File: libsetup.c
 *
 * Copyright (c) 2020, John J. Patti
 * All rights reserved.
 *
 * Documents:
 *  Technical Reference Manual for the MSP430FR6989
 *  (Document No. SLAU367O) ("TRM")
 *
 *  Datasheet for the MSP430FR6989 ("Datasheet")
 *
 *  MSP430FR6989 LaunchPad DevelopmentKit(MSP--EXP430FR6989)
 *  User's Guide (Document No. SLAU627A) ("User's Guide")
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the “Software”), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************/

#include "libsetup.h"

/***************************************************************
 * @brief   Initializes GPIO for P1.1 and P2.3 use
 * @param   ctxGpio_t struct to set pins
 * @return  None
 **************************************************************/
void gpio_init(ctxGpio_t *inval)
{
    //------------------------------------------------------------|
    //////////////////////////////////////////////////////////////|
    // NOTE: GPIO is defaulted to input                           |
    // NOTE: Default module is GPIO                               |
    // NOTE: GPIO pins are set to output to lower power           |
    //       consumption, except when input as needed.            |
    //                                                            |
    // Settings for the GPIO can be found in Chapter 12 of        |
    // of the TRM.                                                |
    //                                                            |
    // Note that the sensor is connected to P2.3 and the button   |
    // S1 (P1.1) is active and used to reset the sensing          |
    // mechanism.  Both P1.1 and P2.3 are set to input with       |
    // pull-up resistors.                                         |
    //                                                            |
    //////////////////////////////////////////////////////////////|
    //------------------------------------------------------------|
    P1DIR = 0xFF & ~inval->pdir[0];// Sets Port 1 pin directions  |
    P2DIR = 0xFF & ~inval->pdir[1];// Sets Port 2 pin directions  |
    P3DIR = 0xFF & ~inval->pdir[2];// Sets Port 3 pin directions  |
    P4DIR = 0xFF & ~inval->pdir[3];// Sets Port 4 pin directions  |
    P5DIR = 0xFF & ~inval->pdir[4];// Sets Port 5 pin directions  |
    P6DIR = 0xFF & ~inval->pdir[5];// Sets Port 6 pin directions  |
    P7DIR = 0xFF & ~inval->pdir[6];// Sets Port 7 pin directions  |
    P8DIR = 0xFF & ~inval->pdir[7];// Sets Port 8 pin directions  |
                                   //                             |
    P1OUT = 0 | inval->pout[0];    // Sets Port 1 outputs         |
    P2OUT = 0 | inval->pout[1];    // Sets Port 2 outputs         |
    P3OUT = 0 | inval->pout[2];    // Sets Port 3 outputs         |
    P4OUT = 0 | inval->pout[3];    // Sets Port 4 outputs         |
    P5OUT = 0 | inval->pout[4];    // Sets Port 5 outputs         |
    P6OUT = 0 | inval->pout[5];    // Sets Port 6 outputs         |
    P7OUT = 0 | inval->pout[6];    // Sets Port 7 outputs         |
    P8OUT = 0 | inval->pout[7];    // Sets Port 8 outputs         |
                                   //                             |
    P1REN = 0 | inval->pren[0];    // Sets Port 1 resistor setup  |
    P2REN = 0 | inval->pren[1];    // Sets Port 2 resistor setup  |
    P3REN = 0 | inval->pren[2];    // Sets Port 3 resistor setup  |
    P4REN = 0 | inval->pren[3];    // Sets Port 4 resistor setup  |
    P5REN = 0 | inval->pren[4];    // Sets Port 5 resistor setup  |
    P6REN = 0 | inval->pren[5];    // Sets Port 6 resistor setup  |
    P7REN = 0 | inval->pren[6];    // Sets Port 7 resistor setup  |
    P8REN = 0 | inval->pren[7];    // Sets Port 8 resistor setup  |
                                   //                             |
    P1SEL0 = 0 | inval->psel0[0];  // Set P1 Mode Select0         |
    P2SEL0 = 0 | inval->psel0[1];  // Set P2 Mode Select0         |
    P3SEL0 = 0 | inval->psel0[2];  // Set P3 Mode Select0         |
    P4SEL0 = 0 | inval->psel0[3];  // Set P4 Mode Select0         |
    P5SEL0 = 0 | inval->psel0[4];  // Set P5 Mode Select0         |
    P6SEL0 = 0 | inval->psel0[5];  // Set P6 Mode Select0         |
    P7SEL0 = 0 | inval->psel0[6];  // Set P7 Mode Select0         |
    P8SEL0 = 0 | inval->psel0[7];  // Set P8 Mode Select0         |
                                   //                             |
    P1SEL0 = 0 | inval->psel1[0];  // Set P1 Mode Select1         |
    P2SEL0 = 0 | inval->psel1[1];  // Set P2 Mode Select1         |
    P3SEL0 = 0 | inval->psel1[2];  // Set P3 Mode Select1         |
    P4SEL0 = 0 | inval->psel1[3];  // Set P4 Mode Select1         |
    P5SEL0 = 0 | inval->psel1[4];  // Set P5 Mode Select1         |
    P6SEL0 = 0 | inval->psel1[5];  // Set P6 Mode Select1         |
    P7SEL0 = 0 | inval->psel1[6];  // Set P7 Mode Select1         |
    P8SEL0 = 0 | inval->psel1[7];  // Set P8 Mode Select1         |
                                   //                             |
    P1IE = 0 | inval->pie[0];      // IRQ Enable for Port 1       |
    P2IE = 0 | inval->pie[1];      // IRQ Enable for Port 2       |
    P3IE = 0 | inval->pie[2];      // IRQ Enable for Port 3       |
    P4IE = 0 | inval->pie[3];      // IRQ Enable for Port 4       |
                                   //                             |
    P1IES = 0 | inval->pes[0];     // IRQ Transition for Port 1   |
    P2IES = 0 | inval->pes[1];     // IRQ Transition for Port 2   |
    P3IES = 0 | inval->pes[2];     // IRQ Transition for Port 3   |
    P4IES = 0 | inval->pes[3];     // IRQ Transition for Port 4   |
                                   //                             |
    PJSEL0 |= BIT4;                // Set PJ.4 and PJ.5 to XTO    |
    PJSEL0 |= BIT5;                //                             |
                                   //                             |
    PM5CTL0 &= ~LOCKLPM5;          // Lock-in configurations      |
    //------------------------------------------------------------|
}

/***************************************************************
 * @brief   Initializes clock to use XTO and sets DCO to 1MHz
 * @param   None
 * @return  None
 **************************************************************/
void clk_init(uint8_t clkset)
{

    //---------------------------------------------------------|
    ///////////////////////////////////////////////////////////|
    // Settings for the GPIO can be found in Chapter 3 of      |
    // of the TRM.                                             |
    ///////////////////////////////////////////////////////////|
    //---------------------------------------------------------|
    CSCTL0_H = CSKEY_H;         // Unlock CS Register          |
    switch(clkset){             // Set the DCO Freq            |
        case DCO_1MHZ:          //                             |
            CSCTL1 = DCOFSEL_0; //                             |
            CSCTL1 &= ~DCORSEL; //                             |
            break;              //                             |
        case DCO_2P67MHZ:       //                             |
            CSCTL1 = DCOFSEL_1; //                             |
            CSCTL1 &= ~DCORSEL; //                             |
            break;              //                             |
        case DCO_3P33MHZ:       //                             |
            CSCTL1 = DCOFSEL_2; //                             |
            CSCTL1 &= ~DCORSEL; //                             |
            break;              //                             |
        case DCO_4MHZ:          //                             |
            CSCTL1 = DCOFSEL_3; //                             |
            CSCTL1 &= ~DCORSEL; //                             |
            break;              //                             |
        case DCO_5P33MHZ:       //                             |
            CSCTL1 = DCOFSEL_4; //                             |
            CSCTL1 &= ~DCORSEL; //                             |
            break;              //                             |
        case DCO_6P67MHZ:       //                             |
            CSCTL1 = DCOFSEL_5; //                             |
            CSCTL1 &= ~DCORSEL; //                             |
            break;              //                             |
        case DCO_8MHZ:          //                             |
            CSCTL1 = DCOFSEL_6; //                             |
            CSCTL1 &= ~DCORSEL; //                             |
            break;              //                             |
        case DCO_16MHZ:         //                             |
            CSCTL1 = DCOFSEL_4; //                             |
            CSCTL1 |= DCORSEL;  //                             |
            break;              //                             |
        case DCO_21MHZ:         //                             |
            CSCTL1 = DCOFSEL_5; //                             |
            CSCTL1 |= DCORSEL;  //                             |
            break;              //                             |
        case DCO_24MHZ:         //                             |
            CSCTL1 = DCOFSEL_6; //                             |
            CSCTL1 |= DCORSEL;  //                             |
            break;              //                             |
        default:                //                             |
            CSCTL1 = DCOFSEL_0; //                             |
            CSCTL1 &= ~DCORSEL; //                             |
            break;              //                             |
    }                           //                             |
    CSCTL2 = SELA__LFXTCLK;     // Set ACLK to XTO             |
    CSCTL2 |= SELS__DCOCLK;     // Set SCLK to DCO             |
    CSCTL2 |= SELM__DCOCLK;     // Set MCLK to DCO             |
    CSCTL3 = DIVA__1;           // Divide ACLK by 1            |
    CSCTL3 |= DIVS__1;          // Divide SCLK by 1            |
    CSCTL3 |= DIVM__1;          // Divide MCLK by 1            |
    CSCTL4 &= ~LFXTOFF;         // Select XTO                  |
    do                          //                             |
    {                           //                             |
        CSCTL5 &= ~LFXTOFFG;    // Clear XT1 fault flag        |
        SFRIFG1 &= ~OFIFG;      //                             |
    } while (SFRIFG1 & OFIFG);  // Test oscillator fault flag  |
    CSCTL0_H = 0;               // Lock CS registers           |
    //---------------------------------------------------------|
}
