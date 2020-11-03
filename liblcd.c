/****************************************************************
 * Author: John J. Patti
 *
 * Version: 0.1
 * Description: LCD Control Functions
 * File: liblcd.c
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

#include "liblcd.h"
#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/***************************************************************
 * @brief   Constant Arrays for Digits and Capital Letters
 * @param   None
 * @return  None
 *
 * Note that decimal point is abbreviated as "dp."
 *
 * Note that the individual character spaces are 14-segment LCD
 * displays that have the following organization:
 *
 *                    A
 *              -------------         -----
 *             F| H\ J| K/ B|         |\|/|
 *             G------ ------M  ==>   -- --
 *             E| Q/ P| N\ C|         |/|\|
 *             --------------         -----
 *                    D
 *
 *  The layout for the characters is as follows:
 *
 *              -----------------------------------------
 *              |Position | Label | Additional Segments |
 *              -----------------------------------------
 *              |    1    | A1    | negative sign; dp   |
 *              -----------------------------------------
 *              |    2    | A2    | colon; dp           |
 *              -----------------------------------------
 *              |    3    | A3    | antenna; dp         |
 *              -----------------------------------------
 *              |    4    | A4    | colon; dp           |
 *              -----------------------------------------
 *              |    5    | A5    | degree; dp          |
 *              -----------------------------------------
 *              |    6    | A6    | tx; rx              |
 *              -----------------------------------------
 *
 *  (See User's Guide at pgs. 11-12)
 *
 *  The segments are controlled by a 16-bit word with each 8-bit
 *  portion being input in to adjacent segments of the LCD memory.
 *  The bit order for each position is as follows:
 *
 *  -----------------------------------------------------------------
 *  |Character|  15  |  14  |  13  |  12  |  11  |  10  |  9  |  8  |
 *  -----------------------------------------------------------------
 *  |    A1   |  A   |   B  |  C   |   D  |  E   |   F  |  G  |  M  |
 *  -----------------------------------------------------------------
 *  |    A2   |  A   |   B  |  C   |   D  |  E   |   F  |  G  |  M  |
 *  -----------------------------------------------------------------
 *  |    A3   |  A   |   B  |  C   |   D  |  E   |   F  |  G  |  M  |
 *  -----------------------------------------------------------------
 *  |    A4   |  A   |   B  |  C   |   D  |  E   |   F  |  G  |  M  |
 *  -----------------------------------------------------------------
 *  |    A5   |  A   |   B  |  C   |   D  |  E   |   F  |  G  |  M  |
 *  -----------------------------------------------------------------
 *  |    A6   |  A   |   B  |  C   |   D  |  E   |   F  |  G  |  M  |
 *  -----------------------------------------------------------------
 *
 *  -----------------------------------------------------------
 *  |Character|  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 *  -----------------------------------------------------------
 *  |    A1   |  H  |  J  |  K  |  P  |  Q  | Neg |  N  |  dp |
 *  -----------------------------------------------------------
 *  |    A2   |  H  |  J  |  K  |  P  |  Q  | col |  N  |  dp |
 *  -----------------------------------------------------------
 *  |    A3   |  H  |  J  |  K  |  P  |  Q  | ant |  N  |  dp |
 *  -----------------------------------------------------------
 *  |    A4   |  H  |  J  |  K  |  P  |  Q  | col |  N  |  dp |
 *  -----------------------------------------------------------
 *  |    A5   |  H  |  J  |  K  |  P  |  Q  | deg |  N  |  dp |
 *  -----------------------------------------------------------
 *  |    A6   |  H  |  J  |  K  |  P  |  Q  |  tx |  N  |  rx |
 *  -----------------------------------------------------------
 *
 *  -----------------------------------------------------------------
 *  |Character|  15  |  14  |  13  |  12  |  11  |  10  |  9  |  8  |
 *  -----------------------------------------------------------------
 *  |    AT2  |  N   |   N  |  N   |   N  |  B5  |  B3  |  B1 |  [] |
 *  -----------------------------------------------------------------
 *  |    AT3  |  N   |   N  |  N   |   D  |  B6  |  B4  |  B2 | BATT|
 *  -----------------------------------------------------------------
 *
 *  -----------------------------------------------------------
 *  |Character|  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 *  -----------------------------------------------------------
 *  |    AT1  |  N  |  N  |  N  |  N  | TMR | HRT | REC |  !  |
 *  -----------------------------------------------------------
 *  |    AT2  |  N  |  N  |  N  |  N  |  N  |  N  |  N  |  N  |
 *  -----------------------------------------------------------
 *  |    AT3  |  N  |  N  |  N  |  N  |  N  |  N  |  N  |  N  |
 *  -----------------------------------------------------------
 *
 *
 *  (See User's Guide at pgs. 12-13)
 *
 *  Example:
 *
 *  To generate the letter "A" in position A1, the following
 *  segments need to be activated: ABCEFGM.  This would result in:
 *
 *          -----
 *          |   |
 *          -- --
 *          |   |
 *
 *  To achieve this, 0b11101111 would need to be written to the high
 *  portion (LCD Position), while 0b00000000 would need to be written
 *  to the lower portion (LCD Position+1).  The respective hex values
 *  for each are 0xEF and 0x00, so the 16-bit word is 0xEF00.
 *
 *  NOTE: The symbols use only a single LCD position and one 8-bit
 *  word.
 *
 **************************************************************/

const uint16_t digits[10] = {
                             //---------------------------------------------------------|
                             ///////////////////////////////////////////////////////////|
                             // Basic digits                                            |
                             ///////////////////////////////////////////////////////////|
                             //---------------------------------------------------------|
                             0xFC28,  // "0"                                            |
                             0x6020,  // "1"                                            |
                             0xDB00,  // "2"                                            |
                             0xF300,  // "3"                                            |
                             0x6700,  // "4"                                            |
                             0xB700,  // "5"                                            |
                             0xBF00,  // "6"                                            |
                             0xE400,  // "7"                                            |
                             0xFF00,  // "8"                                            |
                             0xF700   // "9"                                            |
                             //---------------------------------------------------------|
};

const uint16_t capletters[26] = {
                                 //---------------------------------------------------------|
                                 ///////////////////////////////////////////////////////////|
                                 // Capital Letters                                         |
                                 ///////////////////////////////////////////////////////////|
                                 //---------------------------------------------------------|
                                 0xEF00,  // "A"                                            |
                                 0xF150,  // "B"                                            |
                                 0x9C00,  // "C"                                            |
                                 0xF050,  // "D"                                            |
                                 0x9F00,  // "E"                                            |
                                 0x8F00,  // "F"                                            |
                                 0xBD00,  // "G"                                            |
                                 0x6F00,  // "H"                                            |
                                 0x9050,  // "I"                                            |
                                 0x7800,  // "J"                                            |
                                 0x0E22,  // "K"                                            |
                                 0x1C00,  // "L"                                            |
                                 0x6CA0,  // "M"                                            |
                                 0x6C82,  // "N"                                            |
                                 0xFC00,  // "O"                                            |
                                 0xCF00,  // "P"                                            |
                                 0xFC02,  // "Q"                                            |
                                 0xCF02,  // "R"                                            |
                                 0xB700,  // "S"                                            |
                                 0x8050,  // "T"                                            |
                                 0x7C00,  // "U"                                            |
                                 0x0C28,  // "V"                                            |
                                 0x6C0A,  // "W"                                            |
                                 0x00AA,  // "X"                                            |
                                 0x00B0,  // "Y"                                            |
                                 0x9028   // "Z"                                            |
                                 //---------------------------------------------------------|
};

const uint16_t dec_pt = 0x01;   // For LCD_A1 to LCD_A5
const uint16_t colon = 0x04;    // For LCD_A2 and LCD_A4
const uint16_t tx_sym = 0x04;   // For LCD_A6
const uint16_t rx_sym = 0x01;   // For LCD_A6
const uint16_t deg_sym = 0x04;  // For LCD_A5
const uint16_t antenna = 0x04;  // For LCD_A3
const uint16_t neg_sym = 0x04;  // For LCD_A1
const uint16_t excl_sym = 0x01; // For LCD_AT1
const uint16_t rec_sym = 0x02;  // For LCD_AT1
const uint16_t hrt_sym = 0x04;  // For LCD_AT1
const uint16_t tmr_sym = 0x08;  // For LCD_AT1
const uint16_t brackets = 0x10; // For LCD_AT2
const uint16_t b1_sym = 0x20;   // For LCD_AT2
const uint16_t b3_sym = 0x40;   // For LCD_AT2
const uint16_t b5_sym = 0x80;   // For LCD_AT2
const uint16_t batt_sym = 0x10; // For LCD_AT3
const uint16_t b2_sym = 0x20;   // For LCD_AT3
const uint16_t b4_sym = 0x40;   // For LCD_AT3
const uint16_t b6_sym = 0x80;   // For LCD_AT3

/***************************************************************
 * @brief   Displays a Capital Letter or Digits on the display
 * @param   "symbol"  - input char; must be 0-9 or A-Z
 *
 *          "position" - memory position number; must be one of
 *          the following, which are defined in liblcd.h:
 *          LCD_A1, LCD_A2, LCD_A3, LCD_A4, LCD_A5, or LCD_A6
 *
 * @return  None
 **************************************************************/
void display_char(char symbol, int position)
{
    //---------------------------------------------------------------------------------|
    unsigned int i = (unsigned int) symbol;     // Stores input char as an int         |
    uint16_t symb_val;                          // 16-bit input word for the LCD Mem   |
                                                //                                     |
    if (symbol == ' ')                          // Input word is blank for a space     |
        symb_val = 0;                           //                                     |
    else if (symbol >= '0' && symbol <= '9')    // Determines if symbol is a digit     |
    {                                           //                                     |
        i -= 48;                                // Digits start 48 or "0" = 0x30 = 48  |
        symb_val = digits[i];                   // stores word from constant array     |
    }                                           //                                     |
    else if (symbol >= 'A' && symbol <= 'Z')    // Determines if symbol is a capital   |
    {                                           //                                     |
        i -= 65;                                // Caps start 65 or "A" = 0x41 = 65    |
        symb_val = capletters[i];               // stores word from constant array     |
    }                                           //                                     |
    else                                        //  Error trap                         |
        symb_val = 0xFFFF;                      //  All segments are activated         |
                                                //                                     |
    LCDMEM[position] = symb_val >> 8;           //  Write upper portion to memory 1    |
    LCDMEM[position+1] = symb_val & 0xFF;       //  Write lower portion to memory 2    |
    //---------------------------------------------------------------------------------|
}

/***************************************************************
 * @brief   Displays symbol
 * @param   symbol number ==> set in defines
 * @return  None
 **************************************************************/
void display_symbol(uint8_t sym)
{
    switch(sym){
        case NEG_SYM:
            LCDMEM[LCD_A1 + 1] |= neg_sym;
            break;
        case COLON1_SYM:
            LCDMEM[LCD_A2 + 1] |= colon;
            break;
        case COLON2_SYM:
            LCDMEM[LCD_A4 + 1] |= colon;
            break;
        case DP1_SYM:
            LCDMEM[LCD_A2 + 1] |= dec_pt;
            break;
        case DP2_SYM:
            LCDMEM[LCD_A3 + 1] |= dec_pt;
            break;
        case DP3_SYM:
            LCDMEM[LCD_A3 + 1] |= dec_pt;
            break;
        case DP4_SYM:
            LCDMEM[LCD_A4 + 1] |= dec_pt;
            break;
        case DP5_SYM:
            LCDMEM[LCD_A5 + 1] |= dec_pt;
            break;
        case ANT_SYM:
            LCDMEM[LCD_A3 + 1] |= antenna;
            break;
        case DEG_SYM:
            LCDMEM[LCD_A5 + 1] |= deg_sym;
            break;
        case TX_SYM:
            LCDMEM[LCD_A6 + 1] |= tx_sym;
            break;
        case RX_SYM:
            LCDMEM[LCD_A6 + 1] |= rx_sym;
            break;
        case EXCL_SYM:
            LCDMEM[LCD_AT1] |= excl_sym;
            break;
        case REC_SYM:
            LCDMEM[LCD_AT1] |= rec_sym;
            break;
        case HRT_SYM:
            LCDMEM[LCD_AT1] |= hrt_sym;
            break;
        case TMR_SYM:
            LCDMEM[LCD_AT1] |= tmr_sym;
            break;
        case BRKT_SYM:
            LCDMEM[LCD_AT2] |= brackets;
            break;
        case B1_SYM:
            LCDMEM[LCD_AT2] |= b1_sym;
            break;
        case B3_SYM:
            LCDMEM[LCD_AT2] |= b3_sym;
            break;
        case B5_SYM:
            LCDMEM[LCD_AT2] |= b5_sym;
            break;
        case BATT_SYM:
            LCDMEM[LCD_AT3] |= batt_sym;
            break;
        case B2_SYM:
            LCDMEM[LCD_AT3] |= b2_sym;
            break;
        case B4_SYM:
            LCDMEM[LCD_AT3] |= b4_sym;
            break;
        case B6_SYM:
            LCDMEM[LCD_AT3] |= b6_sym;
            break;
        default:
            break;
    }
}

/***************************************************************
 * @brief   Clears symbol
 * @param   symbol number ==> set in defines
 * @return  None
 **************************************************************/
void clear_symbol(uint8_t sym)
{
    switch(sym)
    {
        case NEG_SYM:
            LCDMEM[LCD_A1 + 1] &= ~neg_sym;
            break;
        case COLON1_SYM:
            LCDMEM[LCD_A2 + 1] &= ~colon;
            break;
        case COLON2_SYM:
            LCDMEM[LCD_A4 + 1] &= ~colon;
            break;
        case DP1_SYM:
            LCDMEM[LCD_A2 + 1] &= ~dec_pt;
            break;
        case DP2_SYM:
            LCDMEM[LCD_A3 + 1] &= ~dec_pt;
            break;
        case DP3_SYM:
            LCDMEM[LCD_A3 + 1] &= ~dec_pt;
            break;
        case DP4_SYM:
            LCDMEM[LCD_A4 + 1] &= ~dec_pt;
            break;
        case DP5_SYM:
            LCDMEM[LCD_A5 + 1] &= ~dec_pt;
            break;
        case ANT_SYM:
            LCDMEM[LCD_A3 + 1] &= ~antenna;
            break;
        case DEG_SYM:
            LCDMEM[LCD_A5 + 1] &= ~deg_sym;
            break;
        case TX_SYM:
            LCDMEM[LCD_A6 + 1] &= ~tx_sym;
            break;
        case RX_SYM:
            LCDMEM[LCD_A6 + 1] &= ~rx_sym;
            break;
        case EXCL_SYM:
            LCDMEM[LCD_AT1] &= ~excl_sym;
            break;
        case REC_SYM:
            LCDMEM[LCD_AT1] &= ~rec_sym;
            break;
        case HRT_SYM:
            LCDMEM[LCD_AT1] &= ~hrt_sym;
            break;
        case TMR_SYM:
            LCDMEM[LCD_AT1] &= ~tmr_sym;
            break;
        case BRKT_SYM:
            LCDMEM[LCD_AT2] &= ~brackets;
            break;
        case B1_SYM:
            LCDMEM[LCD_AT2] &= ~b1_sym;
            break;
        case B3_SYM:
            LCDMEM[LCD_AT2] &= ~b3_sym;
            break;
        case B5_SYM:
            LCDMEM[LCD_AT2] &= ~b5_sym;
            break;
        case BATT_SYM:
            LCDMEM[LCD_AT3] &= ~batt_sym;
            break;
        case B2_SYM:
            LCDMEM[LCD_AT3] &= ~b2_sym;
            break;
        case B4_SYM:
            LCDMEM[LCD_AT3] &= ~b4_sym;
            break;
        case B6_SYM:
            LCDMEM[LCD_AT3] &= ~b6_sym;
            break;
        default:
            break;
    }
}

/***************************************************************
 * @brief   Clears a memory segment
 * @param   "position" - memory position number; must be one of
 *          the following, which are defined in liblcd.h:
 *          LCD_A1, LCD_A2, LCD_A3, LCD_A4, LCD_A5 or LCD_A6
 *
 * @return  None
 **************************************************************/
void clear_lcd_mem(int position)
{
    LCDMEM[position] = 0x00;
    LCDMEM[position+1] = 0x00;
    LCDBMEM[position] = 0x00;
    LCDBMEM[position+1] = 0x00;
}

/***************************************************************
 * @brief   Clears all memory segments
 * @param   None
 * @return  None
 **************************************************************/
void clear_lcd(void)
{
    clear_lcd_mem(LCD_A1);
    clear_lcd_mem(LCD_A2);
    clear_lcd_mem(LCD_A3);
    clear_lcd_mem(LCD_A4);
    clear_lcd_mem(LCD_A5);
    clear_lcd_mem(LCD_A6);
    LCDMEM[LCD_AT1] = 0x00;
    LCDMEM[LCD_AT2] = 0x00;
    LCDMEM[LCD_AT3] = 0x00;
    LCDBMEM[LCD_AT1] = 0x00;
    LCDBMEM[LCD_AT2] = 0x00;
    LCDBMEM[LCD_AT3] = 0x00;
    LCDM14 = 0x00;
    LCDBM14 = 0x00;
    LCDM18 = 0x00;
    LCDBM18 = 0x00;
    LCDM3 = 0x00;
    LCDBM3 = 0x00;
}

/***************************************************************
 * @brief   Scrolls Message Across LCD
 * @param   string "msg"
 * @return  None
 **************************************************************/
void scroll_text(char *msg)
{
    //----------------------------------------------------------------------------------|
    ////////////////////////////////////////////////////////////////////////////////////|
    char *buf;                                  // Print buffer                         |
    int i, len;                                 // Loop variable and string length      |
                                                ////////////////////////////////////////|
    len = strlen(msg);                          // Writes string length to len          |
                                                ////////////////////////////////////////|
    buf = malloc(sizeof(char) * (len + 12));    // Allocates print buffer size. Note    |
                                                // the buffer includes six spaces before|
                                                // and after the string.                |
                                                ////////////////////////////////////////|
    for(i = 0; i < len + 12; i++)               // Fill loop                            |
    {                                           //                                      |
        if (i < 6)                              // Load print buffer with string with 6 |
            buf[i] = ' ';                       // blank spaces before and after.       |
        else if (i > len + 5)                   //                                      |
            buf[i] = ' ';                       //                                      |
        else                                    //                                      |
            buf[i] = msg[i - 6];                //                                      |
    }                                           //                                      |
                                                ////////////////////////////////////////|
    for(i = 0; i < len + 7; i++)                // Print loop                           |
    {                                           //                                      |
        display_char(buf[i], LCD_A1);           // Loads each position with a character |
        display_char(buf[i + 1], LCD_A2);       // from the print buffer.  The          |
        display_char(buf[i + 2], LCD_A3);       // characters are incremented over      |
        display_char(buf[i + 3], LCD_A4);       // position to allow for smooth, scorll-|
        display_char(buf[i + 4], LCD_A5);       // text.                                |
        display_char(buf[i + 5], LCD_A6);       //                                      |
                                                //                                      |
        __delay_cycles(2000000);                // Delay between loops of 250ms so that |
                                                // the text does not move too fast.     |
    }                                           ////////////////////////////////////////|
    free(buf);                                  // Free allocated memory                |
    clear_lcd();                                // clear the LCD                        |
                                                //                                      |
    __delay_cycles(2000000);                    // Delay of 250ms so that to allow      |
                                                // everything to clear.                 |
    //----------------------------------------------------------------------------------|
}

/***************************************************************
 * @brief   Displays static message on LCD (limited to 6
 *          characters)
 * @param   string "msg"
 * @return  None
 **************************************************************/
void display_msg(char *msg)
{
    //----------------------------------------------------------------------------------|
    ////////////////////////////////////////////////////////////////////////////////////|
    char buf[6] = "      ";                     // Print buffer                         |
    int i, len, lp;                             // Loop, string length, truncate        |
                                                ////////////////////////////////////////|
    len = strlen(msg);                          // Writes string length to len          |
                                                ////////////////////////////////////////|
    if(len > 6)                                 // Fill length loop.  Fills to 6 or to  |
        lp = 6;                                 // the truncation value                 |
    else                                        //                                      |
        lp = len;                               //                                      |
                                                ////////////////////////////////////////|
    for(i = 0; i < lp; i++)                     // Fill loop                            |
        buf[i] = msg[i];                        //                                      |
                                                ////////////////////////////////////////|
    display_char(buf[0], LCD_A1);               // Print buffer on LCD                  |
    display_char(buf[1], LCD_A2);               //                                      |
    display_char(buf[2], LCD_A3);               //                                      |
    display_char(buf[3], LCD_A4);               //                                      |
    display_char(buf[4], LCD_A5);               //                                      |
    display_char(buf[5], LCD_A6);               //                                      |
    //----------------------------------------------------------------------------------|
}

/***************************************************************
 * @brief   Initialize LCD
 * @param   None
 * @return  None
 **************************************************************/
void init_lcd(void)
{
    //---------------------------------------------------------|
    ///////////////////////////////////////////////////////////|
    //                                                         |
    // NOTE: The MSP-EXP4306989 uses Segments S4, S6-S21,      |
    // S27-S31 and S35-S39.  See SLAU627A at pg. 13            |
    //                                                         |
    // For LCD configuration see Chapter 36 of the TRM         |
    //                                                         |
    ///////////////////////////////////////////////////////////|
    //---------------------------------------------------------|
    uint16_t lcdctl0_ctx = 0;           // Sets divider to 1,  |
                                        // source to ACLK      |
                                        //                     |
    uint16_t lcdcpctl0_ctx = 0;         // Sets cxt for segment|
    uint16_t lcdcpctl1_ctx = 0;         // registers to 0      |
    uint16_t lcdcpctl2_ctx = 0;         //                     |
                                        //                     |
    lcdctl0_ctx |= LCDPRE__16;          // Prescaler to 16     |
    lcdctl0_ctx |= LCD4MUX;             // 4 mux               |
    lcdctl0_ctx |= LCDLP;               // LP wavefrorms       |
    lcdctl0_ctx &= ~LCDON;              // ensure LCD is off   |
                                        //                     |
    LCDCCTL0  = lcdctl0_ctx;            // load ctx to register|
                                        //                     |
    lcdcpctl0_ctx |= LCDS4;             // Enable segments     |
    lcdcpctl0_ctx |= LCDS6;             //                     |
    lcdcpctl0_ctx |= LCDS7;             //                     |
    lcdcpctl0_ctx |= LCDS8;             //                     |
    lcdcpctl0_ctx |= LCDS9;             //                     |
    lcdcpctl0_ctx |= LCDS10;            //                     |
    lcdcpctl0_ctx |= LCDS11;            //                     |
    lcdcpctl0_ctx |= LCDS12;            //                     |
    lcdcpctl0_ctx |= LCDS13;            //                     |
    lcdcpctl0_ctx |= LCDS14;            //                     |
    lcdcpctl0_ctx |= LCDS15;            //                     |
    lcdcpctl1_ctx |= LCDS16;            //                     |
    lcdcpctl1_ctx |= LCDS17;            //                     |
    lcdcpctl1_ctx |= LCDS18;            //                     |
    lcdcpctl1_ctx |= LCDS19;            //                     |
    lcdcpctl1_ctx |= LCDS20;            //                     |
    lcdcpctl1_ctx |= LCDS21;            //                     |
    lcdcpctl1_ctx |= LCDS27;            //                     |
    lcdcpctl1_ctx |= LCDS28;            //                     |
    lcdcpctl1_ctx |= LCDS29;            //                     |
    lcdcpctl1_ctx |= LCDS30;            //                     |
    lcdcpctl1_ctx |= LCDS31;            //                     |
    lcdcpctl2_ctx |= LCDS35;            //                     |
    lcdcpctl2_ctx |= LCDS36;            //                     |
    lcdcpctl2_ctx |= LCDS37;            //                     |
    lcdcpctl2_ctx |= LCDS38;            //                     |
    lcdcpctl2_ctx |= LCDS39;            //                     |
                                        //                     |
    LCDCPCTL0 = lcdcpctl0_ctx;          // load ctxs into      |
    LCDCPCTL1 = lcdcpctl1_ctx;          // register            |
    LCDCPCTL2 = lcdcpctl2_ctx;          //                     |
                                        //                     |
    LCDCVCTL = LCDCPEN | VLCD_8;        // CP config           |
    LCDCCPCTL = LCDCPCLKSYNC;           //                     |
                                        //                     |
    LCDCMEMCTL = LCDCLRM;               // clear memory        |
                                        //                     |
    LCDCCTL0 |= LCDON;                  // LCD on              |
    //---------------------------------------------------------|
}

/***************************************************************
 * @brief   Disables LCD
 * @param   None
 * @return  None
 **************************************************************/
void lcd_off(void)
{
    //---------------------------------------------------------|
    ///////////////////////////////////////////////////////////|
    //                                                         |
    // NOTE: The MSP-EXP4306989 uses Segments S4, S6-S21,      |
    // S27-S31 and S35-S39.  See SLAU627A at pg. 13            |
    //                                                         |
    // For LCD configuration see Chapter 36 of the TRM         |
    //                                                         |
    ///////////////////////////////////////////////////////////|
    //---------------------------------------------------------|
    LCDCCTL0 &= ~LCDON;             // Turns LCD off           |
    //---------------------------------------------------------|
}

/***************************************************************
 * @brief   Enables LCD
 * @param   None
 * @return  None
 **************************************************************/
void lcd_on(void)
{
    //---------------------------------------------------------|
    ///////////////////////////////////////////////////////////|
    //                                                         |
    // NOTE: The MSP-EXP4306989 uses Segments S4, S6-S21,      |
    // S27-S31 and S35-S39.  See SLAU627A at pg. 13            |
    //                                                         |
    // For LCD configuration see Chapter 36 of the TRM         |
    //                                                         |
    ///////////////////////////////////////////////////////////|
    //---------------------------------------------------------|
    LCDCCTL0 |= LCDON;            // Turns on LCD              |
    //---------------------------------------------------------|
}

/***************************************************************
 * @brief   Displays static number on LCD (limited to 6
 *          digits)
 * @param   input integer
 * @return  None
 **************************************************************/
void display_num(int in)
{
    //----------------------------------------------------------------------------------|
    ////////////////////////////////////////////////////////////////////////////////////|
    int i, temp;                                // Loop and dummy                       |
    char buf[6] = "      ";                     // Print buffer                         |
    clear_lcd();                                // Clear the LCD                        |
                                                ////////////////////////////////////////|
    for(i = 0; i < 6; i++)                      // int-to-string conversion loop        |
    {                                           //                                      |
        temp = in % 10;                         //                                      |
        if (temp == 0)                          //                                      |
            buf[i] = '0';                       //                                      |
        else if (temp == 1)                     //                                      |
            buf[i] = '1';                       //                                      |
        else if (temp == 2)                     //                                      |
            buf[i] = '2';                       //                                      |
        else if (temp == 3)                     //                                      |
            buf[i] = '3';                       //                                      |
        else if (temp == 4)                     //                                      |
            buf[i] = '4';                       //                                      |
        else if (temp == 5)                     //                                      |
            buf[i] = '5';                       //                                      |
        else if (temp == 6)                     //                                      |
            buf[i] = '6';                       //                                      |
        else if (temp == 7)                     //                                      |
            buf[i] = '7';                       //                                      |
        else if (temp == 8)                     //                                      |
            buf[i] = '8';                       //                                      |
        else                                    //                                      |
            buf[i] = '9';                       //                                      |
        in /= 10;                               //                                      |
        if (in == 0)                            //                                      |
            break;                              //                                      |
    }                                           ////////////////////////////////////////|
    display_char(buf[5], LCD_A1);               // Print buffer on LCD                  |
    display_char(buf[4], LCD_A2);               //                                      |
    display_char(buf[3], LCD_A3);               //                                      |
    display_char(buf[2], LCD_A4);               //                                      |
    display_char(buf[1], LCD_A5);               //                                      |
    display_char(buf[0], LCD_A6);               //                                      |
    //----------------------------------------------------------------------------------|
}
