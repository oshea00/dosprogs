/*  
Copyright 1990 by John Wiley & Sons, Inc.
          All Rights Reserved.
*/
/****************************************/
/*       Image Processing Code          */
/*      Area Processing Functions       */
/*       written in Turbo C 2.0         */
/*                 by                   */
/*          Craig A. Lindley            */
/*                                      */
/*   Vers: 1.0  Last Update: 11/13/89   */
/****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <alloc.h>
#include <process.h>
#include <math.h>
#include <graphics.h>
#include "misc.h"
#include "pcx.h"
#include "vga.h"
#include "imagesup.h"
#include "arprocess.h"


/*
Integer Convolution Function
*/

CompletionCode Convolution(BYTE huge *InImage, unsigned Col, unsigned Row,
			   unsigned Width, unsigned Height,
			   short *Kernel, unsigned KernelCols,
			   unsigned KernelRows, unsigned Scale,
			   unsigned Absolute, BYTE huge * *OutImageBufPtr)
{
   register unsigned ColExtent, RowExtent;
   register unsigned ImageCol, ImageRow, KernCol, KernRow;
   unsigned ColOffset, RowOffset, TempCol, TempRow;
   BYTE huge *OutputImageBuffer;
   long Sum;
   short *KernelPtr;


   if (ParameterCheckOK(Col,Row,Col+Width,Row+Height,"Convolution"))
   {
      /* Image must be at least the same size as the kernel */
      if (Width >= KernelCols && Height >= KernelRows)
      {
	 /* allocate far memory buffer for output image */
	 OutputImageBuffer = (BYTE huge *)
			     farcalloc(RASTERSIZE,(unsigned long)sizeof(BYTE));

	 if (OutputImageBuffer == NULL)
	 {
	    restorecrtmode();
	    printf("Error Not enough memory for convolution output buffer\n");
	    return (ENoMemory);
	 }

	 /* Store address of output image buffer */
	 *OutImageBufPtr = OutputImageBuffer;

	 /*
	 Clearing the output buffer to white will show the
	 boarder areas not touched by the convolution. It also
	 provides a nice white frame for the output image.
	 */

	 ClearImageArea(OutputImageBuffer,MINCOLNUM,MINROWNUM,
			MAXCOLS,MAXROWS,WHITE);

	 ColOffset = KernelCols/2;
	 RowOffset = KernelRows/2;
	 /* Compensate for edge effects */
	 Col += ColOffset;
	 Row += RowOffset;
	 Width  -= (KernelCols - 1);
	 Height -= (KernelRows - 1);

	 /* Calculate new range of pixels to act upon */
	 ColExtent = Col + Width;
	 RowExtent = Row + Height;

	 for (ImageRow = Row; ImageRow < RowExtent; ImageRow++)
	 {
	    TempRow = ImageRow - RowOffset;
	    for (ImageCol = Col; ImageCol < ColExtent; ImageCol++)
	    {
	       TempCol = ImageCol - ColOffset;
	       Sum = 0L;
	       KernelPtr = Kernel;
	       for (KernCol = 0; KernCol < KernelCols; KernCol++)
		  for (KernRow = 0; KernRow < KernelRows; KernRow++)
		     Sum += (GetPixelFromImage(InImage,
			     TempCol+KernCol, TempRow+KernRow) *
			     (*KernelPtr++));

	       /* If absolute value is requested */
	       if (Absolute)
		  Sum = labs(Sum);

	       /* Summation performed. Scale and range Sum*/
	       Sum >>= (long) Scale;

	       Sum = (Sum < MINSAMPLEVAL) ? MINSAMPLEVAL:Sum;
	       Sum = (Sum > MAXSAMPLEVAL) ? MAXSAMPLEVAL:Sum;
	       PutPixelInImage(OutputImageBuffer,ImageCol,ImageRow,(BYTE)Sum);
	    }
	 }
      }
      else
	 return(EKernelSize);
   }
   return(NoError);
}

/*
Real Number Convolution Function. This convolution function is
only used when the kernel entries are floating point numbers
instead of integers. Because of the floating point operations
envolved, this function is substantially slower than the already
slow integer version above.
*/

CompletionCode RealConvolution(BYTE huge *InImage,
			       unsigned Col, unsigned Row,
			       unsigned Width, unsigned Height,
			       double *Kernel, unsigned KernelCols,
			       unsigned KernelRows, unsigned Scale,
			       unsigned Absolute, BYTE huge * *OutImageBufPtr)
{
   register unsigned ColExtent, RowExtent;
   register unsigned ImageCol, ImageRow, KernCol, KernRow;
   unsigned ColOffset, RowOffset, TempCol, TempRow;
   BYTE huge *OutputImageBuffer;
   double Sum;
   double *KernelPtr;


   if (ParameterCheckOK(Col,Row,Col+Width,Row+Height,"Convolution"))
   {
      /* Image must be at least the same size as the kernel */
      if (Width >= KernelCols && Height >= KernelRows)
      {
	 /* allocate far memory buffer for output image */
	 OutputImageBuffer = (BYTE huge *)
			     farcalloc(RASTERSIZE,(unsigned long)sizeof(BYTE));

	 if (OutputImageBuffer == NULL)
	 {
	    restorecrtmode();
	    printf("Error Not enough memory for convolution output buffer\n");
	    return (ENoMemory);
	 }

	 /* Store address of output image buffer */
	 *OutImageBufPtr = OutputImageBuffer;

	 /*
	 Clearing the output buffer to white will show the
	 boarder areas not touched by the convolution. It also
	 provides a nice white frame for the output image.
	 */

	 ClearImageArea(OutputImageBuffer,MINCOLNUM,MINROWNUM,
			MAXCOLS,MAXROWS,WHITE);

	 ColOffset = KernelCols/2;
	 RowOffset = KernelRows/2;
	 /* Compensate for edge effects */
	 Col += ColOffset;
	 Row += RowOffset;
	 Width  -= (KernelCols - 1);
	 Height -= (KernelRows - 1);

	 /* Calculate new range of pixels to act upon */
	 ColExtent = Col + Width;
	 RowExtent = Row + Height;

	 for (ImageRow = Row; ImageRow < RowExtent; ImageRow++)
	 {
	    TempRow = ImageRow - RowOffset;
	    for (ImageCol = Col; ImageCol < ColExtent; ImageCol++)
	    {
	       TempCol = ImageCol - ColOffset;
	       Sum = 0.0;
	       KernelPtr = Kernel;
	       for (KernCol = 0; KernCol < KernelCols; KernCol++)
		  for (KernRow = 0; KernRow < KernelRows; KernRow++)
		     Sum += (GetPixelFromImage(InImage,
			     TempCol+KernCol, TempRow+KernRow) *
			     (*KernelPtr++));

	       /* If absolute value is requested */
	       if (Absolute)
		  Sum = fabs(Sum);

	       /* Summation performed. Scale and range Sum */
	       Sum /= (double)(1<<Scale);

	       Sum = (Sum < MINSAMPLEVAL) ? MINSAMPLEVAL:Sum;
	       Sum = (Sum > MAXSAMPLEVAL) ? MAXSAMPLEVAL:Sum;
	       PutPixelInImage(OutputImageBuffer,ImageCol,ImageRow,(BYTE)Sum);
	    }
	 }
      }
      else
	 return(EKernelSize);
   }
   return(NoError);
}



/*
Byte compare for use with the qsort library function call
in the Median filter function.
*/

int ByteCompare(BYTE *Entry1, BYTE *Entry2)
{
   if (*Entry1 < *Entry2)
      return(-1);
   else if (*Entry1 > *Entry2)
      return(1);
   else
      return(0);
}


CompletionCode MedianFilter(BYTE huge *InImage, unsigned Col, unsigned Row,
			   unsigned Width, unsigned Height,
			   unsigned NeighborhoodCols, unsigned NeighborhoodRows,
			   BYTE huge * *OutImageBufPtr)
{
   register unsigned ColExtent, RowExtent;
   register unsigned ImageCol, ImageRow, NeighborCol, NeighborRow;
   unsigned ColOffset, RowOffset, TempCol, TempRow, PixelIndex;
   unsigned TotalPixels, MedianIndex;
   BYTE huge *OutputImageBuffer;
   BYTE *PixelValues;


   if (ParameterCheckOK(Col,Row,Col+Width,Row+Height,"Median Filter"))
   {
      /* Image must be at least the same size as the neighborhood */
      if (Width >= NeighborhoodCols && Height >= NeighborhoodRows)
      {
	 /* allocate far memory buffer for output image */
	 OutputImageBuffer = (BYTE huge *)
			     farcalloc(RASTERSIZE,(unsigned long)sizeof(BYTE));

	 if (OutputImageBuffer == NULL)
	 {
	    restorecrtmode();
	    printf("Error Not enough memory for median filter output buffer\n");
	    return (ENoMemory);
	 }

	 /* Store address of output image buffer */
	 *OutImageBufPtr = OutputImageBuffer;

	 /*
	 Clearing the output buffer to white will show the
	 boarder areas not touched by the median filter. It also
	 provides a nice white frame for the output image.
	 */

	 ClearImageArea(OutputImageBuffer,MINCOLNUM,MINROWNUM,
			MAXCOLS,MAXROWS,WHITE);

	 /* Calculate border pixel to miss */
	 ColOffset = NeighborhoodCols/2;
	 RowOffset = NeighborhoodRows/2;

	 /* Compensate for edge effects */
	 Col += ColOffset;
	 Row += RowOffset;
	 Width  -= (NeighborhoodCols - 1);
	 Height -= (NeighborhoodRows - 1);

	 /* Calculate new range of pixels to act upon */
	 ColExtent = Col + Width;
	 RowExtent = Row + Height;

	 TotalPixels = (NeighborhoodCols*NeighborhoodRows);
	 MedianIndex = (NeighborhoodCols*NeighborhoodRows)/2;

	 /* allocate memory for pixel buffer */
	 PixelValues = (BYTE *) calloc(TotalPixels,(unsigned)sizeof(BYTE));

	 if (PixelValues == NULL)
	 {
	    restorecrtmode();
	    printf("Error Not enough memory for median filter pixel buffer\n");
	    return (ENoMemory);
	 }

	 for (ImageRow = Row; ImageRow < RowExtent; ImageRow++)
	 {
	    TempRow = ImageRow - RowOffset;
	    for (ImageCol = Col; ImageCol < ColExtent; ImageCol++)
	    {
	       TempCol = ImageCol - ColOffset;
	       PixelIndex = 0;
	       for (NeighborCol = 0; NeighborCol < NeighborhoodCols; NeighborCol++)
		  for (NeighborRow = 0; NeighborRow < NeighborhoodRows; NeighborRow++)
		     PixelValues[PixelIndex++] =
		       GetPixelFromImage(InImage,TempCol+NeighborCol,
						 TempRow+NeighborRow);

	       /*
	       Quick sort the brightness values into ascending order
	       and then pick out the median or middle value as
	       that for the pixel.
	       */
	       qsort(PixelValues,TotalPixels,sizeof(BYTE),ByteCompare);
	       PutPixelInImage(OutputImageBuffer,ImageCol,ImageRow,
			       PixelValues[MedianIndex]);
	    }
	 }
      }
      else
	 return(EKernelSize);
   }
   free(PixelValues);        /* give up the pixel value buffer */
   return(NoError);
}

/*
Sobel Edge Detection Function
*/

CompletionCode SobelEdgeDet(BYTE huge *InImage,
			    unsigned Col, unsigned Row,
			    unsigned Width, unsigned Height,
			    unsigned Threshold, unsigned Overlay,
			    BYTE huge * *OutImageBufPtr)
{
   register unsigned ColExtent, RowExtent;
   register unsigned ImageCol, ImageRow;
   unsigned PtA, PtB, PtC, PtD, PtE, PtF, PtG, PtH, PtI;
   unsigned LineAEIAveAbove, LineAEIAveBelow, LineAEIMaxDif;
   unsigned LineBEHAveAbove, LineBEHAveBelow, LineBEHMaxDif;
   unsigned LineCEGAveAbove, LineCEGAveBelow, LineCEGMaxDif;
   unsigned LineDEFAveAbove, LineDEFAveBelow, LineDEFMaxDif;
   unsigned MaxDif;
   BYTE huge *OutputImageBuffer;


   if (ParameterCheckOK(Col,Row,Col+Width,Row+Height,"Sobel Edge Detector"))
   {
      /* allocate far memory buffer for output image */
      OutputImageBuffer = (BYTE huge *)
			  farcalloc(RASTERSIZE,(unsigned long)sizeof(BYTE));

      if (OutputImageBuffer == NULL)
      {
	restorecrtmode();
	printf("Error Not enough memory for Sobel output buffer\n");
	return (ENoMemory);
      }

      /* Store address of output image buffer */
      *OutImageBufPtr = OutputImageBuffer;

      /*
      Clearing the output buffer
      */

      ClearImageArea(OutputImageBuffer,MINCOLNUM,MINROWNUM,
		     MAXCOLS,MAXROWS,BLACK);

      /* Compensate for edge effects of 3x3 pixel neighborhood */
      Col += 1;
      Row += 1;
      Width  -= 2;
      Height -= 2;

      /* Calculate new range of pixels to act upon */
      ColExtent = Col + Width;
      RowExtent = Row + Height;

      for (ImageRow = Row; ImageRow < RowExtent; ImageRow++)
	 for (ImageCol = Col; ImageCol < ColExtent; ImageCol++)
	 {
	   /* Get each pixel in 3x3 neighborhood */
	   PtA = GetPixelFromImage(InImage,ImageCol-1,ImageRow-1);
	   PtB = GetPixelFromImage(InImage,ImageCol  ,ImageRow-1);
	   PtC = GetPixelFromImage(InImage,ImageCol+1,ImageRow-1);
	   PtD = GetPixelFromImage(InImage,ImageCol-1,ImageRow  );
	   PtE = GetPixelFromImage(InImage,ImageCol  ,ImageRow  );
	   PtF = GetPixelFromImage(InImage,ImageCol+1,ImageRow  );
	   PtG = GetPixelFromImage(InImage,ImageCol-1,ImageRow+1);
	   PtH = GetPixelFromImage(InImage,ImageCol  ,ImageRow+1);
	   PtI = GetPixelFromImage(InImage,ImageCol+1,ImageRow+1);

	   /*
	   Calculate average above and below the line.
	   Take the absolute value of the difference.
	   */
	   LineAEIAveBelow = (PtD+PtG+PtH)/3;
	   LineAEIAveAbove = (PtB+PtC+PtF)/3;
	   LineAEIMaxDif = abs(LineAEIAveBelow-LineAEIAveAbove);

	   LineBEHAveBelow = (PtA+PtD+PtG)/3;
	   LineBEHAveAbove = (PtC+PtF+PtI)/3;
	   LineBEHMaxDif = abs(LineBEHAveBelow-LineBEHAveAbove);

	   LineCEGAveBelow = (PtF+PtH+PtI)/3;
	   LineCEGAveAbove = (PtA+PtB+PtD)/3;
	   LineCEGMaxDif = abs(LineCEGAveBelow-LineCEGAveAbove);

	   LineDEFAveBelow = (PtG+PtH+PtI)/3;
	   LineDEFAveAbove = (PtA+PtB+PtC)/3;
	   LineDEFMaxDif = abs(LineDEFAveBelow-LineDEFAveAbove);
	   /*
	   Find the maximum value of the absolute differences
	   from the four possibilities.
	   */
	   MaxDif = MAX(LineAEIMaxDif,LineBEHMaxDif);
	   MaxDif = MAX(LineCEGMaxDif,MaxDif);
	   MaxDif = MAX(LineDEFMaxDif,MaxDif);
	   /*
	   If maximum difference is above the threshold, set
	   the pixel of interest (center pixel) to white. If
	   below the threshold optionally copy the input image
	   to the output image. This copying is controlled by
	   the parameter Overlay.
	   */
	   if (MaxDif >= Threshold)
	     PutPixelInImage(OutputImageBuffer,ImageCol,ImageRow,WHITE);
	   else if (Overlay)
	     PutPixelInImage(OutputImageBuffer,ImageCol,ImageRow,PtE);
	 }
   }
   return(NoError);
}

