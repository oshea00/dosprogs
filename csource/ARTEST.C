/*
Copyright 1990 by John Wiley & Sons, Inc.
          All Rights Reserved.
*/
/****************************************/
/*      Area Process Demo Program       */
/*       written in Turbo C 2.0         */
/*                by                    */
/*         Craig A. Lindley             */
/*                                      */
/*   Vers: 1.0  Last Update: 12/26/89   */
/****************************************/

#include <stdio.h>
/* #include <stdlib.h> */
#include <conio.h>
#include <dos.h>
#include <alloc.h>
#include <process.h>
#include <graphics.h>
#include "misc.h"
#include "pcx.h"
#include "vga.h"
#include "imagesup.h"
#include "arprocess.h"

static double LP1[]=
{ 0.11111111, 0.11111111, 0.11111111,
  0.11111111, 0.11111111, 0.11111111,
  0.11111111, 0.11111111, 0.11111111 };

static short HP1[]=
{  -1, -1, -1,
   -1,  9, -1,
   -1, -1, -1 };

static short LAP2[]=
{  -1, -1, -1,
   -1,  8, -1,
   -1, -1, -1 };


static short GN[]=
{   1,  1,  1,
    1, -2,  1,
   -1, -1, -1 };

static short GE[]=
{  -1,  1,  1,
   -1, -2,  1,
   -1,  1,  1 };

static short BLUR[]=
{   1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,
    1,  1,  1,  1,  1 };


/* main area process demo program */

void main(void)
{
   char *InFileName1 = "babyface";
   char *InFileName2 = "boyish";
   char *InFileName3 = "boyish";
   BYTE huge *TheInImage;
   BYTE huge *TheOutImage;
   unsigned GenPCXFiles = FALSE; /* controls the generation of output files */


   InitGraphics();

   printf("Area Process Demonstration Program\n\n");
   printf("Reading the Image PCX File into memory\n");

   /* generate image sequence p2ch10i1 - Various Convolutions */
   if (ReadPCXFileToBuf (InFileName1,&TheInImage) != NoError)
      exit(1);

   DisplayImageInBuf(TheInImage,INITVGALOADPALETTE,NOWAITFORKEY);

   if (RealConvolution(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		   LP1,3,3,0,FALSE,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i1b",8,320,200,1,320);
      farfree((BYTE far *)TheOutImage);
   }

   DisplayImageInBuf(TheInImage,NOVGAINIT,NOWAITFORKEY);
   if (Convolution(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		   HP1,3,3,0,FALSE,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i1c",8,320,200,1,320);
      farfree((BYTE far *)TheOutImage);
   }

   DisplayImageInBuf(TheInImage,NOVGAINIT,NOWAITFORKEY);
   if (Convolution(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		   LAP2,3,3,0,FALSE,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i1d",8,320,200,1,320);
      farfree((BYTE far *)TheOutImage);
   }

   DisplayImageInBuf(TheInImage,NOVGAINIT,NOWAITFORKEY);
   if (Convolution(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		   GN,3,3,0,FALSE,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i1e",8,320,200,1,320);
      farfree((BYTE far *)TheOutImage);
   }

   DisplayImageInBuf(TheInImage,NOVGAINIT,NOWAITFORKEY);
   if (Convolution(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		   GE,3,3,0,FALSE,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i1f",8,320,200,1,320);
      farfree((BYTE far *)TheOutImage);
   }

   DisplayImageInBuf(TheInImage,NOVGAINIT,NOWAITFORKEY);
   if (Convolution(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		   BLUR,5,5,4,FALSE,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i1g",8,320,200,1,320);
      farfree((BYTE far *)TheOutImage);
   }

   /* generate image sequence p2ch10i2 - Sobel Edge Detection */
   if (ReadPCXFileToBuf (InFileName2,&TheInImage) != NoError)
      exit(1);

   DisplayImageInBuf(TheInImage,NOVGAINIT,NOWAITFORKEY);
   if (SobelEdgeDet(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		    10,FALSE,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i2b",8,320,200,1,320);
      farfree((BYTE far *)TheOutImage);
   }
   /* Do detection again this time with overlaying */
   DisplayImageInBuf(TheInImage,NOVGAINIT,NOWAITFORKEY);
   if (SobelEdgeDet(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		    10,TRUE,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i2c",8,320,200,1,320);
      farfree((BYTE far *)TheOutImage);
   }

   /* generate image sequence p2ch10i3 - Median Filtering */
   if (ReadPCXFileToBuf (InFileName3,&TheInImage) != NoError)
      exit(1);

   DisplayImageInBuf(TheInImage,NOVGAINIT,NOWAITFORKEY);
   if (MedianFilter(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		    3,3,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i3b",8,320,200,1,320);
      CopyImage(TheOutImage,TheInImage);
      farfree((BYTE far *)TheOutImage);
   }

   /* apply filtering twice for effect */
   if (MedianFilter(TheInImage,MINCOLNUM,MINROWNUM,MAXCOLS,MAXROWS,
		    3,3,&TheOutImage) == NoError)
   {
      DisplayImageInBuf(TheOutImage,NOVGAINIT,WAITFORKEY);
      if (GenPCXFiles)
	 WritePCXFile("p2c10i3c",8,320,200,1,320);
      CopyImage(TheOutImage,TheInImage);
      farfree((BYTE far *)TheOutImage);
   }

   farfree((BYTE far *)TheInImage);
   restorecrtmode();
   closegraph();
}
