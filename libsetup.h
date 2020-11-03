/****************************************************************
 * Author: John J. Patti
 *
 * Project Name: Swing Robot Controller
 * File Name: libsetup.h
 *
 * Version: 0.1
 * Description: Setup Library
 *
 * Copyright (c) 2020, Godwud, LLC
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
 ***************************************************************/

#ifndef LIBSETUP_H_
#define LIBSETUP_H_

/****************************************************************
 * Includes
 ***************************************************************/
#include <msp430.h>
#include <stdint.h>

/****************************************************************
 * Defines
 ***************************************************************/
#define DCO_1MHZ        (0x00)
#define DCO_2P67MHZ     (0x01)
#define DCO_3P33MHZ     (0x02)
#define DCO_4MHZ        (0x03)
#define DCO_5P33MHZ     (0x04)
#define DCO_6P67MHZ     (0x05)
#define DCO_8MHZ        (0x06)
#define DCO_16MHZ       (0x07)
#define DCO_21MHZ       (0x08)
#define DCO_24MHZ       (0x09)

/****************************************************************
 * Typedefs and Structs
 ***************************************************************/
typedef struct{
    uint8_t pdir[8];
    uint8_t pout[8];
    uint8_t pren[8];
    uint8_t psel0[8];
    uint8_t psel1[8];
    uint8_t pie[8];
    uint8_t pes[8];

} ctxGpio_t;

/****************************************************************
 * Forward Declarations
 ***************************************************************/
void gpio_init(ctxGpio_t*);
void clk_init(uint8_t);

#endif /* LIBSETUP_H_ */
