/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCDConf_1386_C16_VGAMON_EvaBoard.h
Purpose     : Sample configuration file
----------------------------------------------------------------------
*/

#ifndef LCDCONF_H
#define LCDCONF_H

/*********************************************************************
*
*       General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE      (640)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE      (480)   /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL (16)
#define LCD_SWAP_RB      (1)

#define LCD_CONTROLLER 1386

#define LCD_SWAP_BYTE_ORDER 1

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/

void PC_Init(void);
void PC_WriteMem16(unsigned int Off, int Data);
void PC_WriteReg16(unsigned int Off, int Data);
int  PC_ReadMem16(unsigned int Off);
int  PC_ReadReg16(unsigned int Off);

/*********************************************************************
*
*       Full bus configuration
*
**********************************************************************
*/

#define LCD_READ_MEM(Off)       PC_ReadMem16(Off)
#define LCD_WRITE_MEM(Off,Data) PC_WriteMem16(Off,Data)
#define LCD_READ_REG(Off)       PC_ReadReg16(Off)
#define LCD_WRITE_REG(Off,Data) PC_WriteReg16(Off,Data)

#define LCD_INIT_CONTROLLER() PC_Init()

#endif /* LCDCONF_H */

 
