/****************************************************************
 * Author: John J. Patti
 *
 * Version: 0.1
 * Description: LCD Control Functions
 * File: main.c
 *
 * Copyright (c) 2020, John J. Patti
 * All rights reserved.
 *
 * Description: This file illustrates example uses of the
 * LCD libraries.
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

#include <msp430.h> 
#include "liblcd.h"
#include "libsetup.h"

void set_board(void);

int main(void)
{
    //----------------------------------------------------------------------------------|
    ////////////////////////////////////////////////////////////////////////////////////|
    uint8_t i;                                      // Loop variables                   |
    int j;                                          //                                  |
    WDTCTL = WDTPW | WDTHOLD;                       // stop watchdog timer              |
    set_board();                                    // Sets board                       |
                                                    ////////////////////////////////////|
    scroll_text("THIS IS A TEST");                  // Scroll test                      |
                                                    ////////////////////////////////////|
    for (i = 0x01; i <= 0x18; i++){                 // Iterate over symbols             |
        display_symbol(i);                          //                                  |
        __delay_cycles(2000000);                    //                                  |
        clear_symbol(i);                            //                                  |
    }                                               //                                  |
                                                    ////////////////////////////////////|
    for (j = 0; j <= 100; j ++){                    // Iterate over numbers             |
        display_num(j);                             //                                  |
        __delay_cycles(2000000);                    //                                  |
        clear_lcd();                                //                                  |
    }                                               //                                  |
                                                    ////////////////////////////////////|
    lcd_off();                                      // Shut off LCD                     |
    ////////////////////////////////////////////////////////////////////////////////////|
    //----------------------------------------------------------------------------------|
	return 0;
}

void set_board(void)
{
    //------------------------------------------------------------------------------------------|
    ////////////////////////////////////////////////////////////////////////////////////////////|
    ctxGpio_t setup = { 0 };                            // Instantiate struct                   |
                                                        ////////////////////////////////////////|
    gpio_init(&setup);                                  // Setup pins                           |
                                                        ////////////////////////////////////////|
    clk_init(DCO_8MHZ);                                 // Initialize clock with 8MHz DCO       |
                                                        ////////////////////////////////////////|
    init_lcd();                                         //  Initialize LCD                      |
    ////////////////////////////////////////////////////////////////////////////////////////////|
    //------------------------------------------------------------------------------------------|
}
