/*  
Copyright 1990 by John Wiley & Sons, Inc.
          All Rights Reserved.
*/

/****************************************/
/*    VGA Graphic Adapter Functions     */
/*       written in Turbo C 2.0         */
/*                by                    */
/*         Craig A. Lindley             */
/*                                      */
/*   Vers: 1.0  Last Update: 12/04/89   */
/****************************************/

#include <stdio.h>
#include <process.h>
#include <dos.h>
#include <graphics.h>
#include "misc.h"
#include "pcx.h"
#include "vga.h"

/* equally tempered 16 level gray scale */
ColorRegister Gray16ColorPalette[MAXPALETTECOLORS] =

{ 0, 0, 0, 5, 5, 5, 8, 8, 8,11,11,11,
 14,14,14,17,17,17,20,20,20,24,24,24,
 28,28,28,32,32,32,36,36,36,40,40,40,
 45,45,45,50,50,50,56,56,56,63,63,63 };



/* equally tempered 64 level gray scale */
ColorRegister Gray64ColorPalette[MAX256PALETTECOLORS] =

{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2,
  3, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5,
  6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9,
 10,10,10,10,10,10,11,11,11,12,12,12,
 13,13,13,14,14,14,15,15,15,16,16,16,
 18,18,18,19,19,19,20,20,20,21,21,21,
 22,22,22,23,23,23,24,24,24,25,25,25,
 26,26,26,27,27,27,28,28,28,29,29,29,
 31,31,31,32,32,32,33,33,33,34,34,34,
 35,35,35,36,36,36,37,37,37,39,39,39,
 40,40,40,41,41,41,42,42,42,43,43,43,
 44,44,44,46,46,46,47,47,47,48,48,48,
 49,49,49,50,50,50,52,52,52,53,53,53,
 54,54,54,55,55,55,56,56,56,58,58,58,
 59,59,59,60,60,60,61,61,61,63,63,63 };



/* Start of VGA functions */

/* Initialize the graphics subsystem */
void InitGraphics( void )
{
   int g_driver, g_mode, g_error;

   /*
   Make sure graphic system is not already open. Whether
   it is or not, close it and open it again. Otherwise
   memory for driver will be allocated each time this
   function is called.
   */

   closegraph();

   /* initialize graphics variables */
   g_driver = g_mode = g_error = 0;

   /*
   The call to registerbgidriver below links the display driver
   to the application program. It assumes the driver was converted from
   egavga.bgi to egavga.obj by the bgiobj converter program and
   linked into the application program. The line egavga.obj must
   be in the .prj file for the application program.
   */
   registerbgidriver(EGAVGA_driver);

   initgraph(&g_driver,&g_mode,"");
   g_error = graphresult();
   if (g_error < 0)
   {
      printf("Initgraph error: %s.\n",
      grapherrormsg(g_error));
      exit(EGraphics);
   }
   restorecrtmode();
}

/*
This function fetches the current video mode
from the active video controller.
*/

unsigned GetVideoMode( void )
{
   union REGS regs;

   regs.h.ah = 0x0F;		       /* request the current video mode */
   int86(VIDEO,&regs,&regs);
   return(regs.h.al);
}

/* Set the VGA into the 256 320 by 200 display mode. */
void Set256ColorMode( void )
{
   union REGS regs;

   setgraphmode(VGALO);			/* go into graphic mode */
   regs.h.ah = 0;			/* now into mode 13H for 256 colors */
   regs.h.al = 0x13;
   int86(VIDEO,&regs,&regs);
}

/* Set an individual VGA color register */
void SetAColorReg(unsigned RegNum, unsigned Red,
		  unsigned Green,  unsigned Blue)
{
   union REGS regs;

   /*
   With graphics mode set, we can load a color register
   in the DAC.
   */

   /* set a Color Register */
   regs.h.ah = 0x10;
   regs.h.al = 0x10;
   regs.x.bx = RegNum;
   regs.h.dh = Red;
   regs.h.ch = Green;
   regs.h.cl = Blue;
   int86(VIDEO,&regs,&regs);
}

/* Get the color components of a VGA color register */
void GetAColorReg(unsigned RegNum, unsigned *Red,
		  unsigned *Green, unsigned *Blue)
{
   union REGS regs;

   /*
   With graphics mode set, we can read a color register
   from the DAC.
   */

   /* get a Color Register's components */
   regs.h.ah = 0x10;
   regs.h.al = 0x15;
   regs.x.bx = RegNum;
   int86(VIDEO,&regs,&regs);
   /* store the returned values at the pointers */
   *Red   = regs.h.dh;
   *Green = regs.h.ch;
   *Blue  = regs.h.cl;
}


/* load the gray palette */
void LoadGray16Palette(void)
{
   struct palettetype palette;
   unsigned Index;
   union REGS regs;

   /*
   With a graphics mode set, we can proceed to load our palette and
   color registers in the DAC. The palette is set up in sequential
   order and the color register are set to gray scale values.
   */

   palette.size = 16;

   for (Index = 0; Index < MAXPALETTECOLORS; Index++)
      palette.colors[Index] = Index;

   /* set a block of Color Registers */
   regs.h.ah = 0x10;
   regs.h.al = 0x12;
   regs.x.bx = 0;
   regs.x.cx = MAXPALETTECOLORS;
   _ES = FP_SEG(Gray16ColorPalette);
   regs.x.dx =FP_OFF(Gray16ColorPalette);
   int86(VIDEO,&regs,&regs);

   /* install the newly created palette */
   setallpalette(&palette);
}


/* load the gray palette */
void LoadGray64Palette(void)
{
   union REGS regs;

   /*
   This 64 level gray scale can only be loaded when the VGA is in
   the 256 color mode 13h. The actual palette mechanism is bypassed
   in this mode. The color registers in the DAC are loaded and accessed
   directly.
   */

   /* set a block of Color Registers */
   regs.h.ah = 0x10;
   regs.h.al = 0x12;
   regs.x.bx = 0;
   regs.x.cx = 64;
   _ES = FP_SEG(Gray64ColorPalette);
   regs.x.dx =FP_OFF(Gray64ColorPalette);

   int86(VIDEO,&regs,&regs);
}
