/****************************************************************
 * Author: John J. Patti
 *
 * Version: 0.1
 * Description: LCD Control Functions
 * File: liblcd.h
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

#ifndef LIBLCD_H_
#define LIBLCD_H_

/****************************************************************
 * Header includes
 ***************************************************************/
#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/****************************************************************
 * Defines
 ***************************************************************/
//---------------------------------------------------------|
///////////////////////////////////////////////////////////|
// Character position memory locations                     |
//                                                         |
// Note that the LCD is 4-mux setting                      |
//                                                         |
// Positions for segments S0 to S54 are defined at pg. 948 |
// of the TRM.                                             |
//                                                         |
// Note that the positions begin with a zero.  In other    |
// words, LCM Memory 1 is position 0, and LCD Memory 10 is |
// position 9.  That is why the memory locations are off by|
// 1.                                                      |
///////////////////////////////////////////////////////////|
//---------------------------------------------------------|
#define LCD_A1          (9)   // A1 begins at Pin S18      |
#define LCD_A2          (5)   // A2 begins at Pin S10      |
#define LCD_A3          (3)   // A3 begins at Pin S6       |
#define LCD_A4          (18)  // A4 begins at Pin S36      |
#define LCD_A5          (14)  // A5 begins at Pin S28      |
#define LCD_A6          (7)   // A6 begins at Pin S14      |
#define LCD_AT1         (2)   // AT1 begins at Pin S4      |
#define LCD_AT2         (17)   // AT1 begins at Pin S34    |
#define LCD_AT3         (13)   // AT1 begins at Pin S26    |
//---------------------------------------------------------|

//---------------------------------------------------------|
///////////////////////////////////////////////////////////|
// Special Symbol list                                     |
///////////////////////////////////////////////////////////|
//---------------------------------------------------------|
#define NONE_SYM        (0x0)                              |
#define NEG_SYM         (0x1)  // Negative sign            |
#define COLON1_SYM      (0x2)  // First colon LCD_A2       |
#define COLON2_SYM      (0x3)  // Second colon LCD_A4      |
#define DP1_SYM         (0x4)  // First Dec Pt LCD_A1      |
#define DP2_SYM         (0x5)  // Second Dec Pt LCD_A2     |
#define DP3_SYM         (0x6)  // Third Dec Pt LCD_A3      |
#define DP4_SYM         (0x7)  // Fourth Dec Pt LCD_A4     |
#define DP5_SYM         (0x8)  // Fifth Dec Pt LCD_A5      |
#define ANT_SYM         (0x9)  // Antenna Symbol           |
#define DEG_SYM         (0xA)  // Degree Symbol            |
#define TX_SYM          (0xB)  // TX Symbol                |
#define RX_SYM          (0xC)  // RX Symbol                |
#define EXCL_SYM        (0xD)  // Exclamation Point        |
#define REC_SYM         (0xE)  // Record                   |
#define HRT_SYM         (0xF)  // Heart                    |
#define TMR_SYM         (0x10) // Timer                    |
#define BRKT_SYM        (0x11) // Brackets                 |
#define B1_SYM          (0x12) // Battery 1                |
#define B3_SYM          (0x13) // Battery 3                |
#define B5_SYM          (0x14) // Battery 5                |
#define BATT_SYM        (0x15) // BATT Symbol              |
#define B2_SYM          (0x16) // Battery 2                |
#define B4_SYM          (0x17) // Battery 4                |
#define B6_SYM          (0x18) // Battery 6                |
/****************************************************************
 * Constants
 ***************************************************************/
extern const uint16_t digits[10];
extern const uint16_t capletters[26];
extern const uint16_t dec_pt;
extern const uint16_t colon;
extern const uint16_t tx_sym;
extern const uint16_t rx_sym;
extern const uint16_t deg_sym;
extern const uint16_t antenna;
extern const uint16_t neg_sym;
extern const uint16_t excl_sym;
extern const uint16_t rec_sym;
extern const uint16_t hrt_sym;
extern const uint16_t tmr_sym;
extern const uint16_t brackets;
extern const uint16_t b1_sym;
extern const uint16_t b3_sym;
extern const uint16_t b5_sym;
extern const uint16_t batt_sym;
extern const uint16_t b2_sym;
extern const uint16_t b4_sym;
extern const uint16_t b6_sym;

/****************************************************************
 * Forward Declarations
 ***************************************************************/
void display_char(char, int);
void display_timer_sym(void);
void clear_lcd_mem(int position);
void clear_lcd(void);
void clear_timer_sym(void);
void display_decimal_pt(void);
void scroll_text(char*);
void init_lcd(void);
void display_msg(char*);
void lcd_off(void);
void lcd_on(void);
void display_num(int);
void display_symbol(uint8_t sym);
void clear_symbol(uint8_t sym);

#endif /* LIBLCD_H_ */
