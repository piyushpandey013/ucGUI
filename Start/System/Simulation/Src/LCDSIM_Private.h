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
File        : LCDSIM_Private.h
Purpose     : Declares LCD Private interface
----------------------------------------------------------------------
*/

#ifndef LCDSIM_PRIVATE_H
#define LCDSIM_PRIVATE_H

extern int LCDSIM_aLCDColorWhite[5];
extern int LCDSIM_aLCDColorBlack[5];

/********************************************************
*
*              Memory offsets
*
*********************************************************
*/

#define LCDSIM_OFF_XSIZE          32
#define LCDSIM_OFF_YSIZE          36
#define LCDSIM_OFF_VXSIZE         40
#define LCDSIM_OFF_VYSIZE         44
#define LCDSIM_OFF_CONTROLLER     48

#define LCDSIM_OFF_BPP            52
#define LCDSIM_OFF_NUMCOLORS      56

#define LCDSIM_OFF_MODIFY_CNT     60
#define LCDSIM_OFF_LUT_MODIFY_CNT 64

#define LCDSIM_OFF_FIXEDPALETTE   68
#define LCDSIM_OFF_DELTA_MODE     72

#define LCDSIM_OFF_XPOS           80
#define LCDSIM_OFF_YPOS           84
#define LCDSIM_OFF_KEYSTAT        88


#endif /* Avoid multiple inclusion */




