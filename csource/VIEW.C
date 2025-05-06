/*
Copyright 1990 by John Wiley & Sons, Inc.
          All Rights Reserved.
*/
/****************************************/
/*  PCX/TIFF File View Utility Program  */
/*       written in Turbo C 2.0         */
/*                by                    */
/*         Craig A. Lindley             */
/*  Usage:                              */
/*   view [-v ?] filename[.pcx | .tif]  */
/*   Vers: 2.1  Last Update: 10/11/90   */
/****************************************/
/*
NOTE:
This example program must be compiled using Turbo's medium
memory model because of its size.
*/

#include <stdio.h>
#include <process.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <string.h>
#include <io.h>
#include "misc.h"
#include "pcx.h"
#include "tiffintf.h"

#define MAXFILENAMELENGTH 30 /* max supported length of filename */

extern struct PCX_File PCXData;

/* current revision level */
unsigned Release = 2;
unsigned Revision = 1;

/*
This function provides help in the advent of operator error.
Program terminates after help is given
*/

void ShowHelp( void )
{
   printf("\nView usage: view [-v ?] filename[.pcx | .tif] <cr>\n");
   printf("  -v displays image file information\n");
   printf("  ? or -?  displays this help text\n");
   printf("  filename is name given to PCX or TIFF image file\n\n");
   exit(EBadParms);
}

void main(unsigned argc, char *argv[])
{
   unsigned Verbose = FALSE;
   unsigned FileNameCounter, ArgIndex, StrLength;
   char    *ImageFileName;
   char    FileName[MAXFILENAMELENGTH];
   char    PCXFileName[MAXFILENAMELENGTH];
   char    TIFFFileName[MAXFILENAMELENGTH];

   clrscr();
   printf("View - PCX or TIFF Image File Display Program\n");
   printf("  Version: %d.%d by Craig A. Lindley\n\n",Release,Revision);

   /* parse all command line arguments */

   FileNameCounter = 0;                /* count of user specified filenames */
   for (ArgIndex=1; ArgIndex < argc; ArgIndex++)
   {
      if (*argv[ArgIndex] != '-')      /* if not a cmd line switch */
      {                                /* must be a filename */
	 if (*argv[ArgIndex] == '?')   /* help requested ? */
	    ShowHelp();
	 if (FileNameCounter > 1)      /* only one filename allowed */
            ShowHelp();                /* if more then error exit */
	 ImageFileName = argv[ArgIndex];  /* save image filename */
         FileNameCounter++;            /* inc count for error check */
      }
      else                             /* its a cmd line switch */
      {
         switch (*(argv[ArgIndex]+1))     /* parse the cmd line */
         {
            case 'v':
            case 'V':
              Verbose = TRUE;
              break;
            case '?':
	      ShowHelp();
              break;
	    default:
	      printf("Error - invalid cmd line switch encountered\n");
	      ShowHelp();
         }
      }
   }
   if (FileNameCounter != 1)
   {
      printf("Error: a single PCX or TIFF filename must be specified\n");
      ShowHelp();
   }

   printf("Press the <Enter> key to terminate display\n\n\n");
   delay(1000);
   /*
   Check for which type of file to display. This
   sometimes means adding filename extensions if
   one was not specified by the user.
   */
   strupr(ImageFileName);                    /* Convert to upper case */
   if (strstr(ImageFileName,".PCX"))         /* does it have a .PCX ext ? */
      DisplayPCXFile(ImageFileName,Verbose); /* display PCX file */
   else if (strstr(ImageFileName,".TIF"))    /* does it have a .TIF ext ? */
      DisplayTIFFFile(ImageFileName,Verbose);/* display TIFF file */
   else
   {
      strcpy(FileName,"");                   /* make storage empty */

      /* find filename length minus the extension */
      StrLength = strcspn(ImageFileName,".");

      if (StrLength == 0)                    /* no ext specified */
	 strncat(FileName,ImageFileName,MAXFILENAMELENGTH); /* copy filename completely */
      else                                   /* an ext was specified */
	 strncat(FileName,ImageFileName,StrLength); /* copy name only */
      /*
      Copy the processed file name to each of the
      filename storage areas and append the appropriate string.
      */
      strcpy(PCXFileName,FileName);
      strcat(PCXFileName,".PCX");
      strcpy(TIFFFileName,FileName);
      strcat(TIFFFileName,".TIF");
      /*
      Determine is files with these extensions really
      exist. If so display with appropriate
      function.
      */
      if (access(PCXFileName,0) == 0)          /* does PCX file exist ? */
	 DisplayPCXFile(PCXFileName,Verbose);  /* yes then display */
      else if (access(TIFFFileName,0) == 0)    /* does TIFF file exist ? */
	 DisplayTIFFFile(TIFFFileName,Verbose);/* yes then display */
      else
      {
	 printf("Neither file %s nor %s found\n",
		 PCXFileName,TIFFFileName);
	 exit(EFileNotFound);
      }
   }
   getchar();
   restorecrtmode();
   closegraph();
}

