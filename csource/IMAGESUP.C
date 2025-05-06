/*  
Copyright 1990 by John Wiley & Sons, Inc.
          All Rights Reserved.
*/
/****************************************/
/* Image Processing Support Functions   */
/*       written in Turbo C 2.0         */
/*                by                    */
/*         Craig A. Lindley             */
/*                                      */
/*   Vers: 1.0  Last Update: 11/14/89   */
/****************************************/

#include <stdio.h>
#include <process.h>
#include <conio.h>
#include <dos.h>
#include <alloc.h>
#include <mem.h>
#include <graphics.h>
#include "misc.h"
#include "pcx.h"
#include "vga.h"
#include "imagesup.h"


extern struct PCX_File PCXData;
extern unsigned ImageWidth;
extern unsigned ImageHeight;

/*
Image Processing Support Functions - See text for details.
*/

/*
Copy a complete image from source buffer to destination buffer
*/

void CopyImage(BYTE huge *SourceBuf, BYTE huge *DestBuf)
{
   movedata(FP_SEG(SourceBuf),FP_OFF(SourceBuf),
	    FP_SEG(DestBuf),FP_OFF(DestBuf),
	    (unsigned) RASTERSIZE);
}


/*
NOTE: to index into the image memory like an array, the index
value must be a long variable type, NOT just cast to long.
*/

BYTE GetPixelFromImage(BYTE huge *Image, unsigned Col, unsigned Row)
{
  unsigned long PixelBufOffset;

  if((Col < ImageWidth) && (Row < ImageHeight))
  {
     PixelBufOffset  = Row;            /* done to prevent overflow */
     PixelBufOffset *= ImageWidth;
     PixelBufOffset += Col;
     return(Image[PixelBufOffset]);
  }
  printf("GetPixelFromImage Error: Coordinate out of range\n");
  printf("  Col = %d  Row = %d\n",Col,Row);
  return(FALSE);
}

CompletionCode PutPixelInImage(BYTE huge *Image, unsigned Col,
			       unsigned Row, unsigned Color)
{
  unsigned long PixelBufOffset;

  if((Col < ImageWidth) && (Row < ImageHeight))
  {
     PixelBufOffset  = Row;            /* done to prevent overflow */
     PixelBufOffset *= ImageWidth;
     PixelBufOffset += Col;
     Image[PixelBufOffset] = Color;
     return(TRUE);
  }
  else
  {
     printf("PutPixelInImage Error: Coordinate out of range\n");
     printf("  Col = %d  Row = %d\n",Col,Row);
     return(FALSE);
  }
}

/*
NOTE: A length of 0 is one pixel on. A length of 1 is two pixels
on. That is why length is incremented before being used.
*/

CompletionCode DrawHLine(BYTE huge *Image, unsigned Col, unsigned Row,
			 unsigned Length, unsigned Color)
{
   if ((Col < ImageWidth) && ((Col+Length) <= ImageWidth) &&
       (Row < ImageHeight))
   {
      Length++;
      while(Length--)
	PutPixelInImage(Image,Col++,Row,Color);
      return(TRUE);
   }
   else
   {
      printf("DrawHLine Error: Coordinate out of range\n");
      printf("  Col = %d  Row = %d  Length = %d\n",Col,Row,Length);
      return(FALSE);
   }
}


CompletionCode DrawVLine(BYTE huge *Image, unsigned Col, unsigned Row,
			 unsigned Length, unsigned Color)
{
   if ((Row < ImageHeight) && ((Row+Length) <= ImageHeight) &&
       (Col < ImageWidth))
   {
      Length++;
      while(Length--)
	PutPixelInImage(Image,Col,Row++,Color);
      return(TRUE);
   }
   else
   {
      printf("DrawVLine Error: Coordinate out of range\n");
      printf("  Col = %d  Row = %d  Length = %d\n",Col,Row,Length);
      return(FALSE);
   }
}

void ReadImageAreaToBuf (BYTE huge *Image, unsigned Col, unsigned Row,
			 unsigned Width, unsigned Height, BYTE huge *Buffer)
{
   unsigned long PixelBufOffset = 0L;
   register unsigned ImageCol, ImageRow;

   for (ImageRow=Row; ImageRow < Row+Height; ImageRow++)
      for (ImageCol=Col; ImageCol < Col+Width; ImageCol++)
         Buffer[PixelBufOffset++] =
	      GetPixelFromImage(Image,ImageCol,ImageRow);
}

void WriteImageAreaFromBuf (BYTE huge *Buffer, unsigned BufWidth,
			    unsigned BufHeight, BYTE huge *Image,
			    unsigned ImageCol, unsigned ImageRow)
{
   unsigned long PixelBufOffset;
   register unsigned BufCol, BufRow, CurrentImageCol;

   for (BufRow = 0; BufRow < BufHeight; BufRow++)
   {
      CurrentImageCol = ImageCol;
      for (BufCol = 0; BufCol < BufWidth; BufCol++)
      {
	 PixelBufOffset = (unsigned long)BufRow*BufWidth+BufCol;
	 PutPixelInImage(Image,CurrentImageCol,ImageRow,Buffer[PixelBufOffset]);
	 CurrentImageCol++;
      }
      ImageRow++;
   }
}


void ClearImageArea(BYTE huge *Image,unsigned Col, unsigned Row,
		    unsigned Width, unsigned Height,
		    unsigned PixelValue)
{
   register unsigned BufCol, BufRow;

   for (BufRow = 0; BufRow < Height; BufRow++)
      for (BufCol = 0; BufCol < Width; BufCol++)
	 PutPixelInImage(Image,BufCol+Col,BufRow+Row,PixelValue);
}


/*
This function checks to make sure the parameters passed to
the image processing functions are all within range. If so
a TRUE is returned. If not, an error message is output and
the calling program is terminated.
*/

CompletionCode ParameterCheckOK(unsigned Col, unsigned Row,
				unsigned ColExtent, unsigned RowExtent,
				char *FunctionName)
{
   if ((Col > MAXCOLNUM) || (Row > MAXROWNUM) ||
       (ColExtent > MAXCOLS) || (RowExtent > MAXROWS))
   {
      restorecrtmode();
      printf("Parameter(s) out of range in function: %s\n",FunctionName);
      printf("  Col = %d  Row = %d  ColExtent = %d  RowExtent = %d\n",
		Col, Row, ColExtent, RowExtent);

      exit(EBadParms);
   }
   return(TRUE);
}
