/*  
Copyright 1990 by John Wiley & Sons, Inc.
          All Rights Reserved.
*/
/****************************************/
/*        PCX Function Library          */
/*       written in Turbo C 2.0         */
/*                by                    */
/*         Craig A. Lindley             */
/*                                      */
/*   Vers: 1.0  Last Update: 11/21/89   */
/****************************************/

#include <stdio.h>
#include <string.h>
#include <process.h>
#include <conio.h>
#include <dos.h>
#include <alloc.h>
#include <graphics.h>
#include "misc.h"
#include "pcx.h"
#include "vga.h"

/* Externally Accessable Global Variables */
struct   PCX_File PCXData;        /* PCX File Hdr Variable */
unsigned ImageWidth, ImageHeight;

/* Variables global to this file only */
static FILE    *PCXFile;       /* file handle */
static BYTE     ScanLine[MAXBYTESPERSCAN];
static BYTE     PixelColorNum[MAXSCREENWIDTH];
static unsigned Is256ColorFile;
static struct   ExtendedPalette Color256Palette;


/* Start of Functions */

CompletionCode ReadPCXFileHdr (char *FileName, int Verbose)
{
   unsigned Index;
   char     String[80];


   Is256ColorFile = FALSE;             /* initialize mode variable */

   if (!strchr(FileName,'.'))          /* is there an ext ? */
   {
      strcpy(String,FileName);         /* copy filename to buffer */
      FileName = String;               /* FileName now pts at buffer */
      strcat(FileName,".pcx");         /* if not add .pcx ext */
   }
   /* try to open the PCX file */
   if ((PCXFile = fopen(FileName,"rb")) == NULL)
   {
      printf("PCX file: %s not found\n",FileName);
      return(EFileNotFound);
   }
   /* try to read the file header record */
   if (fread(&PCXData,sizeof(struct PCX_File),1,PCXFile) != 1)
   {
      printf("Error reading PCX file header\n");
      return(EReadFileHdr);
   }
   /* check to make sure its a PCX file */
   if (PCXData.PCXHeader.Header != PCXHdrTag)
   {
      printf("Error not a PCX file\n");
      return(ENotPCXFile);
   }
   /* Yep, we've got a PCX file OK. Display info if requested */
   if (Verbose)
   {
      clrscr();
      printf("PCX Image Information for file: %s\n\n",FileName);
      printf("\tVersion: %d\n", PCXData.PCXHeader.Version);
      printf("\tCompression: %s\n",
              PCXData.PCXHeader.Encode == 0 ? "None":"RLL");
      printf("\tBits Per Pixel: %d\n",PCXData.PCXHeader.BitPerPix);
      printf("\tX1: %d\n",PCXData.PCXHeader.X1);
      printf("\tY1: %d\n",PCXData.PCXHeader.Y1);
      printf("\tX2: %d\n",PCXData.PCXHeader.X2);
      printf("\tY2: %d\n",PCXData.PCXHeader.Y2);
      printf("\tHoriz Resolution: %d\n",PCXData.PCXHeader.Hres);
      printf("\tVert  Resolution: %d\n",PCXData.PCXHeader.Vres);
      printf("\tVMode: %d\n",PCXData.Info.Vmode);
      printf("\tNumber of Planes: %d\n",PCXData.Info.NumOfPlanes);
      printf("\tBytes Per Scan Line One Plane: %d\n",PCXData.Info.BytesPerLine);
      printf("\nHit any key to proceed\n");
      getch();                       /* wait for operator input */

      clrscr();
      printf("Color Register Values for PCX file: %s\n\n",FileName);
      for (Index = 0; Index < MAXPALETTECOLORS; Index++)
      {
         printf("Palette Index: %2d  R = %2x G = %2x B = %2x\n",
		Index,PCXData.Palette[Index].Red,
		      PCXData.Palette[Index].Green,
		      PCXData.Palette[Index].Blue);
      }
      printf("\nHit <Enter> to proceed - ^C to abort\n");
      getchar();                       /* wait for operator input */
   }
   return(NoError);
}

static CompletionCode ExpandScanLine (FILE *InFile)
{
   register short    BitNum;
   register unsigned ByteNum;
   register short    CharRead;
   unsigned          InPtr,RepCount,PixelsData;
   unsigned          BytesToRead,PlaneNum,ShiftCount;
   unsigned          ByteOffset, BitOffset;

   BytesToRead = PCXData.Info.NumOfPlanes * PCXData.Info.BytesPerLine;

   InPtr = ShiftCount = 0;             /* initialize vars */
   do
   {
      CharRead = getc(InFile);         /* read a byte from the file */
      if (CharRead == EOF)             /* error should never read EOF */
         return(FALSE);                /* abort picture */

      if ((CharRead & 0xC0) == 0xC0)  /* a repeat tag ? */
      {
         RepCount = CharRead & ~0xC0;  /* repeat 1..63 */
         CharRead = getc(InFile);      /* read byte to repeat */
         if (CharRead == EOF)          /* error should never read EOF */
            return(FALSE);             /* abort picture */

         while (RepCount--)            /* expand byte */
            ScanLine[InPtr++] =        /* RepCount times */
              CharRead;
      }
      else                             /* just a byte of data */
        ScanLine[InPtr++] = CharRead;  /* store in buffer */
   } while (InPtr < BytesToRead);      /* expand a full scan line */
/*
When we get here, we have an array, ScanLine, which is composed of
NumOfPlanes sections each BytesPerLine long. For a normal EGA/VGA image
this works out to be 4 planes of 80 bytes each. For a 256 color VGA image
it is 1 plane of 320 bytes. For the normal image we must merge each of these
bit planes into the array PixelColorNum so that we can display the
resultant image. Each entry into this array corresponds to a pixel on
a single scan line of the monitor. For a 256 color image, the ScanLine is
simply copied into the PixelColorNum array because there is no interleaving
of bit planes.
*/

   if (PCXData.PCXHeader.X2 == 319)    /* if 256 color image */
      memcpy(PixelColorNum,ScanLine,ImageWidth);
   else                                /* normal image file */
   {
      /* clear PixelColorNum array to zeros */
      memset(PixelColorNum,'\0',ImageWidth);

      for (PlaneNum=0; PlaneNum < PCXData.Info.NumOfPlanes; PlaneNum++)
      {
         ByteOffset = PlaneNum * PCXData.Info.BytesPerLine;
         for (ByteNum=0; ByteNum < PCXData.Info.BytesPerLine; ByteNum++)
         {
            /* read 8 bits of pixel data for one plane */
           PixelsData = ScanLine[ByteOffset+ByteNum];
           BitOffset = ByteNum * BITSPERBYTE;

           for (BitNum=BITSPERBYTE-1; BitNum >= 0; BitNum--)
           {
	      if (PixelsData & (1 << BitNum))
	      {
		 /* OR in each component of the color */
		 PixelColorNum[BitOffset + (7 - BitNum)] |=
		    (1 << ShiftCount);
	      }
	   }
         }
         ShiftCount++;
      }
   }
/*
When we get here, the PixelColorNum array has a byte color value for each
pixel on the display. Return an indication that this operation went
smoothly.
*/

   return(TRUE);
}

unsigned InstallPCXFilePalette(void)
{
   struct   palettetype palette;
   union    REGS regs;
   unsigned Index;

   /*
   Always load the VGA palette as long as the version is not 3 which
   doesn't have palette information in the file. If version 3
   use the default palette
   */

   if (PCXData.PCXHeader.Version != 3)
   {
      if (Is256ColorFile)	       /* if a mode 13h file */
      {
         /*
         When we get here, we have a mode 13h file image. In this
         VGA mode, the palette mechanism is bypassed. The color
         registers are loaded from the extended palette in the
	 PCX file. The values in the file's palette are four
	 times their actual values. They must be scaled before
	 being used. All 256 of the color registers must be loaded.
         */

	 for (Index=0; Index < MAX256PALETTECOLORS; Index++)
	 {
	     Color256Palette.Palette[Index].Red   >>= 2;
	     Color256Palette.Palette[Index].Green >>= 2;
	     Color256Palette.Palette[Index].Blue  >>= 2;
	 }

	 /* set a block of Color Registers */
	 regs.h.ah = 0x10;
	 regs.h.al = 0x12;
	 regs.x.bx = 0;
	 regs.x.cx = MAX256PALETTECOLORS;
	 _ES = FP_SEG(&Color256Palette.Palette);
	 regs.x.dx =FP_OFF(&Color256Palette.Palette);
	 int86(VIDEO,&regs,&regs);
         return(TRUE);       /* indicate palette installed successfully */
      }
      else
      {
         /*
         When we get here we have a 16 color VGA image. We must
         build a palette data structure with data loaded from the PCX
         file. The palette is set up in sequential order and the
	 color register are set from values in the file. The file
	 values are scaled before being installed.
         */

	 palette.size = MAXPALETTECOLORS;

	 for (Index = 0; Index < MAXPALETTECOLORS; Index++)
	 {
	    palette.colors[Index] = Index;
	    PCXData.Palette[Index].Red   >>= 2;
	    PCXData.Palette[Index].Green >>= 2;
	    PCXData.Palette[Index].Blue  >>= 2;
	 }

	 /* set a block of Color Registers */
	 regs.h.ah = 0x10;
	 regs.h.al = 0x12;
	 regs.x.bx = 0;
	 regs.x.cx = MAXPALETTECOLORS;
	 _ES = FP_SEG(&PCXData.Palette);
	 regs.x.dx =FP_OFF(&PCXData.Palette);
	 int86(VIDEO,&regs,&regs);

	 /* enable the palette we just read from the file */
         setallpalette(&palette);
         return(TRUE);       /* indicate palette installed successfully */
      }
   }
   else
      return(FALSE);         /* no palette info to load */
}


/*
This function reads a PCX file into a buffer in memory. It does not
alter the palette currently used for the VGA display.
*/

CompletionCode ReadPCXFileToBuf (char *FileName, BYTE huge * *BufferPtr)
{
   register unsigned ScanNum;          /* scan line being expanded
                                          and displayed */
   register unsigned ColNum;	       /* pixel being read */
   int      PCXError;
   BYTE huge *ImageMemory;             /* memory block where image */
                                       /* will be stored */
   unsigned long PixelBufOffset;

   if ((PCXError = ReadPCXFileHdr(FileName,FALSE)) != NoError)
      return(PCXError);

   /* Header has been read, now we are ready to read the PCX image */
   /* PCC files cannot be displayed */

   if ((PCXData.PCXHeader.X1 != 0) || (PCXData.PCXHeader.Y1 != 0))
   {
      printf("Error PCC file not PCX file\n");
      return (EPCCFile);
   }

   /*
   From the header information determine the size of the buffer
   required to store the image. Set the global vars ImageWidth
   and ImageHeight are accordingly.
   */

   if (PCXData.PCXHeader.X2 == 319)
   {
      ImageWidth  = 320;
      ImageHeight = 200;
   }
   else
   {
      ImageWidth = 640;
      switch(PCXData.PCXHeader.Y2)
      {
	 case 479: ImageHeight = 480;
      		   break;
	 case 349: ImageHeight = 350;
		   break;
	 case 199: ImageHeight = 200;
		   break;
      }
   }

   /* allocate far memory for the image */
   ImageMemory = (BYTE huge *) farcalloc((long) ImageWidth * ImageHeight,
                                          sizeof(BYTE));
   if (ImageMemory == NULL)
   {
      printf("Error Not enough memory for PCX buffer\n");
      return (ENoMemory);
   }
   /*
   Proceed to unpack and store the PCX data. A scan line at
   a time.
   */

   for (ScanNum=0; ScanNum < ImageHeight; ScanNum++)
   {
      if (ExpandScanLine(PCXFile) != TRUE)
      {
         printf("Error Scanline corrupt in PCX file\n");
	 return(ECorrupt);
      }
      PixelBufOffset = (long) ScanNum * ImageWidth;
      for (ColNum=0; ColNum < ImageWidth; ColNum++)
      {
         ImageMemory[PixelBufOffset + ColNum] =
                    PixelColorNum[ColNum];
      }
   }
   /*
   Determine if the PCX file is a mode 13h extended color file by
   trying to read the extended palette record located after the
   raster data. If EOF is read then no extended palette info
   is included in the file.
   */

   Is256ColorFile = FALSE;   /* set global flag to indicate file type */

   if (fread(&Color256Palette,sizeof(struct ExtendedPalette),1,PCXFile) == 1)
      /* Extended palette read ok. Now check tag. */
      if (Color256Palette.ExtendedPalette == PCX256ColorTag)
         /*
         Tag is ok, extended palette RGB values in Color256Palette
         structure.
         */
         Is256ColorFile = TRUE;

   /* file has been read prepare to close up shop */

   fclose(PCXFile);
   *BufferPtr = ImageMemory;           /* return the buffer address */
   return(NoError);
}


/*
This function displays an image in a buffer. If SetMode is TRUE,
the graphics mode will be set and the palette will be loaded. If FALSE,
neither will be performed, the image will be displayed with current
settings. If Pause is requested, this function will wait for a key
press before returning to the calling code.
*/

void DisplayImageInBuf(BYTE huge *Image, unsigned SetMode, unsigned Pause)
{
   register unsigned ScanNum, PixelNum;
   unsigned long PixelBufOffset;

   if (SetMode)
      InitGraphics();        /* initialize graphic system if required */
   if (ImageWidth == 320)    /* a 256 color image ? */
   {
      if (SetMode)           /* if a mode set is required */
      {                      /* set mode and load palette */
	 Set256ColorMode();
	 InstallPCXFilePalette();
      }
      for (ScanNum=0; ScanNum < ImageHeight; ScanNum++)
	for (PixelNum=0; PixelNum < ImageWidth; PixelNum++)
	{
	   PixelBufOffset  = ScanNum;  /* done to prevent overflow */
	   PixelBufOffset *= ImageWidth;
	   PixelBufOffset += PixelNum;
	   PutPixel256(PixelNum,ScanNum,Image[PixelBufOffset]);
	}
   }
   else
   {
      if (SetMode)           /* if mode set required */
      {
	 switch(ImageHeight) /* determine VGA mode */
	 {
	    case 480:  setgraphmode(VGAHI);
		       break;
	    case 350:  setgraphmode(VGAMED);
		       break;
	    case 200:  setgraphmode(VGALO);
		       break;
	 }
	 InstallPCXFilePalette();
      }
      for (ScanNum=0; ScanNum < ImageHeight; ScanNum++)
	for (PixelNum=0; PixelNum < ImageWidth; PixelNum++)
	{
	   PixelBufOffset  = ScanNum;  /* done to prevent overflow */
	   PixelBufOffset *= ImageWidth;
	   PixelBufOffset += PixelNum;
	   putpixel(PixelNum,ScanNum,Image[PixelBufOffset]);
	}
   }
   if (Pause)                /* if pause requested wait for key */
      getch();
}

/*
This function reads and displays a PCX file.
*/

void DisplayPCXFile (char *FileName, int Verbose)
{
   register unsigned ScanNum;          /* scan line being expanded
                                          and displayed */
   register unsigned ColNum;	       /* pixel being read */
   int      PCXError;


   if ((PCXError = ReadPCXFileHdr(FileName,Verbose)) != NoError)
      exit(PCXError);

   /* Header has been read, now we are ready to display the PCX image */
   /* PCC files cannot be displayed */

   if ((PCXData.PCXHeader.X1 != 0) || (PCXData.PCXHeader.Y1 != 0))
   {
      printf("Error PCC file not PCX file\n");
      exit(EPCCFile);
   }

   InitGraphics();             /* initialize graphs subsystem */

   /* From the header information determine which mode the display should
      be in. If width is 320 then its mode 13 hex 256 colors. Otherwise
      its a VGA mode with a width of 640. */

   if (PCXData.PCXHeader.X2 == 319)
   {
      Set256ColorMode();
      ImageWidth  = 320;
      ImageHeight = 200;
   }
   else
   {
      ImageWidth = 640;
      switch(PCXData.PCXHeader.Y2)
      {
         case 479: setgraphmode(VGAHI);
		   ImageHeight = 480;
      		   break;
         case 349: setgraphmode(VGAMED);
		   ImageHeight = 350;
		   break;
	 case 199: setgraphmode(VGALO);
		   ImageHeight = 200;
		   break;
      }
   }


   /* proceed to unpack and diplay the PCX file */
   for (ScanNum=0; ScanNum < ImageHeight; ScanNum++)
   {
      if (ExpandScanLine(PCXFile) != TRUE)
      {
         printf("Scanline corrupt in PCX file\n");
         exit(ECorrupt);
      }
      if (ImageWidth == 320) /* 256 color mode */
      {
         for (ColNum=0; ColNum < ImageWidth; ColNum++)
            PutPixel256(ColNum,ScanNum,(int) PixelColorNum[ColNum]);
      }
      else                   /* normal 16 color modes */
      {
         for (ColNum=0; ColNum < ImageWidth; ColNum++)
            putpixel(ColNum,ScanNum,(int) PixelColorNum[ColNum]);
      }
   }
   /*
   Determine if the PCX file is a mode 13h extended color file by
   trying to read the extended palette record located after the
   raster data. If EOF is read then no extended palette info
   is included in the file.
   */

   Is256ColorFile = FALSE;   /* set global flag to indicate file type */

   if (fread(&Color256Palette,sizeof(struct ExtendedPalette),1,PCXFile) == 1)
      /* Extended palette read ok. Now check tag. */
      if (Color256Palette.ExtendedPalette == PCX256ColorTag)
         /*
         Tag is ok, extended palette RGB values in Color256Palette
         structure.
         */
         Is256ColorFile = TRUE;

   /* file has been read prepare to close up shop */

   /* Install palette read from file */
   InstallPCXFilePalette();

   fclose(PCXFile);
}


/*
The following routines create a PCX file from a raster image
or a memory buffer and writes it to disk.
*/

CompletionCode WritePCXHdr(char *FileName, unsigned BitsPerPixel,
			   unsigned MaxX,  unsigned MaxY, unsigned Planes,
			   unsigned BytesPerLine)
{
   struct palettetype palette;
   unsigned    Index;
   union REGS  regs;
   char        String[80];

   if (!strchr(FileName,'.'))          /* is there an ext ? */
   {                                   /* if not ... */
      strcpy(String,FileName);         /* copy filename to buffer */
      FileName = String;               /* FileName now pts at buffer */
      strcat(FileName,".pcx");         /* add .pcx ext */
   }

   if ((PCXFile = fopen(FileName,"w+b")) == NULL)
   {
      restorecrtmode();
      printf("Could not open output PCX file\n");
      return (EWrtOutFile);
   }

   /* initialize the PCX file header info */
   PCXData.PCXHeader.Header    = PCXHdrTag;
   PCXData.PCXHeader.Version   = 5;
   PCXData.PCXHeader.Encode    = 1;
   PCXData.PCXHeader.BitPerPix = BitsPerPixel;
   PCXData.PCXHeader.X1        = 0;
   PCXData.PCXHeader.Y1        = 0;
   PCXData.PCXHeader.X2        = MaxX-1;
   PCXData.PCXHeader.Y2        = MaxY-1;
   PCXData.PCXHeader.Hres      = MaxX;
   PCXData.PCXHeader.Vres      = MaxY;
   ImageWidth                  = MaxX;
   ImageHeight                 = MaxY;
   PCXData.Info.Vmode          = 0;
   PCXData.Info.NumOfPlanes    = Planes;
   PCXData.Info.BytesPerLine   = BytesPerLine;

   /*
   Initialize the palette structure in the PCX file data. The palette
   will be written to the PCX file regardless of whether it is a 16
   or 256 color image. If its a 256 color image an extended palette
   structure will be written at the end of the PCX raster data.
   The palette values must be scaled up by four before being
   written to the file.
   */

   getpalette(&palette);
   for (Index = 0; Index < palette.size; Index++)
   {
      regs.h.ah = 0x10;
      regs.h.al = 0x15;
      regs.x.bx = palette.colors[Index];
      int86(VIDEO,&regs,&regs);
      PCXData.Palette[Index].Red   = regs.h.dh <<= 2;
      PCXData.Palette[Index].Green = regs.h.ch <<= 2;
      PCXData.Palette[Index].Blue  = regs.h.cl <<= 2;
   }

   /* clear the unused area at the end of the PCX header */
   memset(&PCXData.Info.unused,'\0',sizeof(PCXData.Info.unused));

   /* now write the file header to the physical file */
   if (fwrite(&PCXData,sizeof(struct PCX_File),1,PCXFile) != 1)
   {
      restorecrtmode();
      printf("Error writing PCX file header\n");
      return(EWrtFileHdr);
   }
   return(NoError);
}


static CompletionCode CompressScanLine(FILE *OutFile)
{
   register unsigned OutPtr,RepCount,RepChar;
   register unsigned BytesToWrite;

   BytesToWrite = PCXData.Info.NumOfPlanes * PCXData.Info.BytesPerLine;

   OutPtr = 0;                         /* ptr to data to compress */
   do
   {
      RepChar = ScanLine[OutPtr++];    /* get byte to start compression */
      RepCount = 1;                    /* byte seen once at this point */
      while ((ScanLine[OutPtr]==RepChar) &&
             (RepCount < MaxRepCount)    &&
	     (OutPtr < BytesToWrite))
      {
         RepCount++;                   /* count all repetitions of char */
         OutPtr++;                     /* bump ptr and check again */
      }

      /* repeat sequence found or if chars has either or both MSBs set
         than must process as a repetition count and char sequence */

      if ((RepCount > 1) || (RepChar > 0xBF))
      {
         RepCount |= 0xC0;             /* set two MSBs */
         if (putc(RepCount,OutFile) == EOF) /* write count to file */
            return(FALSE);             /* if error return error */
      }
      if (putc(RepChar,OutFile) == EOF)/* write char to file */
         return(FALSE);                /* if error return error */
   } while (OutPtr < BytesToWrite);    /* until all bytes in scan
                                          are compressed */
   return(TRUE);                       /* indicate operation successful */
}

/*
This function writes a PCX file to disk contained in a buffer in
memory. The current palette being used for display is written to
the PCX file. All entries in the PCX header are from the image
which was originally read into the buffer under the assumption
that the content of the image might change but not its basic
parameters.
*/

CompletionCode WritePCXFileFromBuf (char *FileName, BYTE huge *ImageMemory)
{
   register unsigned PlaneNum, BitNum, ByteNum, PData;
   register unsigned ScanLineNum,PixelNum, Index;
   int PCXError;
   unsigned long PixelBufOffset;
   union    REGS regs;

   /* write out PCX header and palette */
   if ((PCXError = WritePCXHdr(FileName, PCXData.PCXHeader.BitPerPix,
		   PCXData.PCXHeader.X2+1,PCXData.PCXHeader.Y2+1,
                   PCXData.Info.NumOfPlanes,
                   PCXData.Info.BytesPerLine)) != NoError)
      return(PCXError);

/*
At this point we will read the image from the buffer a scanline
at at time. For 320x200 256 color images there is only a single bit plane
so the data read from the buffer is placed directly into the ScanLine array
for compressing. For normal VGA images, the color value returned need to be
separated into their component parts which will be compressed separately.
In essense, the single array of 640 bytes corresponding to 640 separate
pixels on the scan line is broken up into four separate arrays 80 bytes
apiece (all contained in ScanLine[]). These arrays are the i,r,g,b components
of the pixel values. The separates components or planes are then compressed.
The components are written to disk in the following order: blue, green,
red and then intensity.
*/

   for (ScanLineNum=0; ScanLineNum < ImageHeight; ScanLineNum++)
   {
      PixelBufOffset = (long) ScanLineNum * ImageWidth;
      if (PCXData.PCXHeader.X2 == 319)    /* if 256 color image */
      {
	 for (PixelNum=0; PixelNum < ImageWidth; PixelNum++)
	    ScanLine[PixelNum] = ImageMemory[PixelBufOffset + PixelNum];
      }
      else                                /* normal image file */
      {
	 /* clear ScanLine array to zeros for each scan line */
	 /* this is an array of NumOfPlanes * BytesPerLine bytes */
	 memset(ScanLine,'\0',MAXBYTESPERSCAN);

	 for (PixelNum=0; PixelNum < ImageWidth; PixelNum++)
	 {
            /* get pixel value from buffer */
	    PData = ImageMemory[PixelBufOffset + PixelNum];
	    ByteNum = PixelNum/BITSPERBYTE;         /* calc byte offset */
	    BitNum  = 7 - (PixelNum % BITSPERBYTE); /* calc bit offset */
	    for (PlaneNum=0; PlaneNum < PCXData.Info.NumOfPlanes; PlaneNum++)
	      if (PData & (1<<PlaneNum))           /* if bit in plane is 1 */
		 ScanLine[(PlaneNum * PCXData.Info.BytesPerLine) + ByteNum] |=
			  (1<<BitNum);
	 }
      }

      if (CompressScanLine(PCXFile) != TRUE)    /* compress a complete scan */
      {                                         /* line */
         restorecrtmode();
         printf("Error writing a compressed scan line\n");
         return(EWrtScanLine);
      }
   }

   /*
   Determine if the PCX file is a mode 13h extended color image.
   If so, write an extended palette record to the file after the
   raster data.
   */

   if (ImageWidth == 320)    /* 256 color mode 13h image ? */
   {                         /* yes it is */
      /*
      Read the 256 color register RGB values and store them in
      the Color256Palette structure before writing them to the
      PCX file. This structure is tagged to assure validity.
      */

      Color256Palette.ExtendedPalette = PCX256ColorTag;

      /* get a block of Color Registers */
      regs.h.ah = 0x10;
      regs.h.al = 0x17;
      regs.x.bx = 0;
      regs.x.cx = MAX256PALETTECOLORS;
      _ES = FP_SEG(&Color256Palette.Palette);
      regs.x.dx =FP_OFF(&Color256Palette.Palette);
      int86(VIDEO,&regs,&regs);

      /*
      The palette data must be scaled up by four before
      being written to the file.
      */
      for (Index=0; Index < MAX256PALETTECOLORS; Index++)
      {
	Color256Palette.Palette[Index].Red   <<= 2;
	Color256Palette.Palette[Index].Green <<= 2;
	Color256Palette.Palette[Index].Blue  <<= 2;
      }

      /*
      With all of the color register values read, write the
      extended palette structure to the PCX file.
      */

      if (fwrite(&Color256Palette,
                 sizeof(struct ExtendedPalette),1,PCXFile) != 1)
      {
         restorecrtmode();
         printf("Error writing extended palette structure\n");
         fclose(PCXFile);              /* close the PCX file */
         farfree((BYTE far *) ImageMemory);  /* return buffer memory */
         return(EWrtExtPal);
      }
   }
   /* file has been written prepare to close up shop */
   fclose(PCXFile);                    /* close the completed PCX file */
   farfree((BYTE far *) ImageMemory);  /* return buffer memory */
   return(NoError);
}

/*
This function writes a PCX file to disk from the image currently
being displayed on the monitor.
*/

void WritePCXFile (char *FileName, unsigned BitsPerPixel,
		   unsigned MaxX,  unsigned MaxY, unsigned Planes,
		   unsigned BytesPerLine)
{
   register unsigned PlaneNum, BitNum, ByteNum, PData;
   register unsigned ScanLineNum,PixelNum;
   int PCXError;
   unsigned Index;
   union    REGS regs;


   /* write out PCX header and palette */
   if ((PCXError = WritePCXHdr(FileName, BitsPerPixel,
		   MaxX, MaxY, Planes, BytesPerLine)) != NoError)
      exit(PCXError);

/*
At this point we will read the displayed image from the screen a scanline
at at time. For 320x200 256 color images there is only a single bit plane
so the data read from the screen is placed directly into the ScanLine array
for compressing. For normal VGA images, the color value returned need to be
separated into their component parts which will be compressed separately.
In essense, the single array of 640 bytes corresponding to 640 separate
pixels on the scan line is broken up into four separate arrays 80 bytes
apiece (all contained in ScanLine[]). These arrays are the i,r,g,b components
of the pixel values. The separates components or planes are then compressed.
The components are written to disk in the following order: blue, green,
red and then intensity.
*/

   for (ScanLineNum=0; ScanLineNum < ImageHeight; ScanLineNum++)
   {
      if (PCXData.PCXHeader.X2 == 319)    /* if 256 color image */
      {
	 for (PixelNum=0; PixelNum < ImageWidth; PixelNum++)
	    ScanLine[PixelNum] = GetPixel256(PixelNum,ScanLineNum);
      }
      else                                /* normal image file */
      {
	 /* clear ScanLine array to zeros for each scan line */
	 /* this is an array of NumOfPlanes * BytesPerLine bytes */
	 memset(ScanLine,'\0',MAXBYTESPERSCAN);

	 for (PixelNum=0; PixelNum < ImageWidth; PixelNum++)
	 {
	    PData = getpixel(PixelNum,ScanLineNum); /* get pixel value */
	    ByteNum = PixelNum/BITSPERBYTE;         /* calc byte offset */
	    BitNum  = 7 - (PixelNum % BITSPERBYTE); /* calc bit offset */
	    for (PlaneNum=0; PlaneNum < PCXData.Info.NumOfPlanes; PlaneNum++)
	      if (PData & (1<<PlaneNum))           /* if bit in plane is 1 */
		 ScanLine[(PlaneNum * PCXData.Info.BytesPerLine) + ByteNum] |=
			  (1<<BitNum);
	 }
      }

      if (CompressScanLine(PCXFile) != TRUE)    /* compress a complete scan */
      {                                         /* line */
         restorecrtmode();
         printf("Error writing a compressed scan line\n");
         exit(EWrtScanLine);
      }
   }
   /*
   Determine if the PCX file is a mode 13h extended color image.
   If so, write an extended palette record to the file after the
   raster data.
   */

   if (ImageWidth == 320)    /* 256 color mode 13h image ? */
   {                         /* yes it is */
      /*
      Read the 256 color register RGB values and store them in
      the Color256Palette structure before writing them to the
      PCX file. This structure is tagged to assure validity.
      */

      Color256Palette.ExtendedPalette = PCX256ColorTag;

      for (Index = 0; Index < MAX256PALETTECOLORS; Index++)
      {
         regs.h.ah = 0x10;
         regs.h.al = 0x15;
         regs.x.bx = Index;  /* get this color reg RGB values */
         int86(VIDEO,&regs,&regs);
	 Color256Palette.Palette[Index].Red   = regs.h.dh <<= 2;
	 Color256Palette.Palette[Index].Green = regs.h.ch <<= 2;
	 Color256Palette.Palette[Index].Blue  = regs.h.cl <<= 2;
      }
      /*
      With all of the color register values read, write the
      extended palette structure to the PCX file.
      */

      if (fwrite(&Color256Palette,
                 sizeof(struct ExtendedPalette),1,PCXFile) != 1)
      {
         restorecrtmode();
         printf("Error writing extended palette structure\n");
      }
   }
   /* file has been written prepare to close up shop */

   fclose(PCXFile);                    /* close the completed PCX file */
}

/*
This function reads a raw image file into memory. The amount
of data read from the file is determined from the specified image
dimensions. Various error codes are returns in the advent
of an error reading the image file. If the read is successful,
a pointer to the image data and the NoError status will be
returned to the calling code. No assumptions are made about the
format of the data read other than that it should be stored
in sequential image buffer memory locations.
*/

CompletionCode ReadRawImageFileToBuf(char *FileName,
				     unsigned ImageWidth,
				     unsigned ImageHeight,
				     BYTE huge * *BufferPtr)
{
   unsigned long RasterSize, Index;
   BYTE huge    *ImageBuffer;
   FILE         *ImageDataFile;
   int           DataRead;

   /* Assigned an error value until read is completed successfully */
   *BufferPtr = NULL;

   /* Calculate size of buffer required to store image */
   RasterSize = (long)ImageWidth * ImageHeight;

   /* Check for sufficient memory for buffer */
   if (RasterSize >= farcoreleft())
   {
      printf("Not enough memory for image!\n");
      return(ENoMemory);
   }
   /* Allocate buffer from FAR heap */
   ImageBuffer = (BYTE huge *)farcalloc(RasterSize,
				       (unsigned long) sizeof(BYTE));

   /* Attempt to open the raw image data file */
   if ((ImageDataFile = fopen(FileName,"rb")) == NULL)
   {
     printf("Cannot open image data file: %s\n",FileName);
     farfree((char far *) ImageBuffer);
     return(EFileNotFound);
   }
   /* With file open, read RasterSize number of bytes from file */
   for (Index=0; Index < RasterSize; Index++)
   {
      /* Check each read for an error */
      if((DataRead = fgetc(ImageDataFile)) == EOF)
      {
	 fclose(ImageDataFile);   /* close file */
	 farfree((char far *) ImageBuffer);
	 return(ECorrupt);        /* return error code */
      }
      /* If all is well, store the BYTE read into the buffer */
      ImageBuffer[Index] = (BYTE) DataRead;
   }
   /* Everythings ok. Close file, store ptr and return NoError */

   fclose(ImageDataFile);
   *BufferPtr = ImageBuffer;      /* return ptr to buffer */
   return(NoError);
}

/*
This function writes to a file the raw image data produced
by the digitizer. NOTE: This function will convert the column
by column image data returned by the digitizer to a row
by row raster data format if Transpose is TRUE. Otherwise,
the output file is written in the same manner as the data
returned by the digitizer; column by column.
*/

CompletionCode WriteRawImageFileFromBuf(char *FileName,
					unsigned ImageWidth,
					unsigned ImageHeight,
					unsigned Transpose,
					BYTE huge *ImageBuffer)
{
   unsigned long RasterSize, Index, Col, Row;
   FILE         *ImageDataFile;
   BYTE          WriteData;

   /* Attempt to open the raw image output data file */
   if ((ImageDataFile = fopen(FileName,"wb")) == NULL)
   {
     printf("Cannot open image data output file: %s\n",FileName);
     return(EWrtOutFile);
   }

   if (!Transpose)                /* write data in normal format */
   {
      /* Calculate number of bytes to write */
      RasterSize = (long)ImageWidth * ImageHeight;

      /* With file open, write RasterSize number of bytes to file */
      for (Index=0; Index < RasterSize; Index++)
      {
	 /* Get the byte to write */
	 WriteData = ImageBuffer[Index];

	 /* Check each write for an error */
	 if (fputc(WriteData,ImageDataFile) != WriteData)
	 {
	    fclose(ImageDataFile);   /* close file */
	    return(EWrtOutFile);     /* return error code */
	 }
      }
   }
   else                           /* data must be in raster format */
   {
      for(Row=0; Row < ImageHeight; Row++)
	 for (Col=0; Col < ImageWidth; Col++)
	 {
	    /* Get the byte to write */
	    WriteData = ImageBuffer[(Col * ImageHeight) + Row];

	    /* Check each write for an error */
	    if (fputc(WriteData,ImageDataFile) != WriteData)
	    {
	       fclose(ImageDataFile);   /* close file */
	       return(EWrtOutFile);     /* return error code */
	    }
	 }
   }
   /* Everythings ok. Close file, free buffer memory and return NoError */

   fclose(ImageDataFile);
   farfree((char far*) ImageBuffer);   /* free memory */
   return(NoError);
}
