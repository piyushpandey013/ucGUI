/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*           (C) 2000    SEGGER Microcontroller Systeme GmbH          *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

**** uC/GUI Grafical user interface for embedded applications ****
uC/GUI is protected by international copyright laws. Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : Branding.c
Purpose     : Brandings for Micrium
---------------------------END-OF-HEADER------------------------------
*/

#define MICRO "\xb5"
static const char _acCopyright[]    = "(c) 2002  Micrium, Inc.\n"
                                      "www.micrium.com\n\n"
                                      "(c) 1998-2002  Segger\n"
                                      "Microcontroller Systeme GmbH\n"
                                      "www.segger.com";
static const char _acAppNameLong[]  = MICRO "C/GUI-Simulation";
static const char _acAppNameShort[] = MICRO "C/GUI-Simulation";

const char * BRANDING_GetCopyright(void) { return _acCopyright; }
const char * BRANDING_GetAppNameShort(void)   { return _acAppNameShort;   }
const char * BRANDING_GetAppNameLong(void) { return _acAppNameLong; }
