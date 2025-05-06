/*
** PARENT.C
**
** Michael O'Shea
** 393-6495 Boeing Computer Services
**
** INPUT
**
** NONE
**
** OUTPUT
**
** Display parents PSP address and environment address.
** Display parents environment copy from dos
** Display parents environment after child() is called
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <process.h>
#include <conio.h>

int main(void);
void pspaddr(void);
void listenv(void);

union REGS inregs,outregs;

unsigned int far *parentenvptr;
unsigned char far *currenv,far *c;
unsigned int hold;

/*
   Note we are NOT using the environment copy set up by 'C'.
   Thus no: main(int argc,char *argv[],char *env[]).
   DOS provides an environment thru the PSP when EXEC is invoked.
   THAT is the environment we will use....

   Also, a structure is not really needed as long as we know the offsets.
   As the Assembler listing showed, the offset in the PSP for the parent
   PSP segment address is 0x16.  The offset for the current environment
   segment address is 0x2c.

   You may wonder why the PSP only allows 2 bytes to address the parent PSP
   and the current environment.  Thats because DOS makes sure to allocate
   the PSP and Environments on a paragraph boundary (divisible by 0x16) so
   it may always be expressed as NNNN:0000.  This is why I'm using
   MK_FP (make far pointer) to convert these values to far pointers that
   'C' can use.
*/

int main()
{
	int i;
	printf("PARENT PROCESSING\n");
	pspaddr();
	printf("\nEnvironment Before:\n");
	listenv();
	spawnl(P_WAIT,"child",NULL);
	printf("PARENT PROCESSING\n");
	printf("Environment After:\n");
	listenv();
	printf("Test Complete\n");
	return 0;
}

void pspaddr(void)
{
	/* get address of this processes' PSP */
	inregs.h.ah = 0x51;
	intdos(&inregs,&outregs);
	printf("PARENT PSP: %p  ",outregs.x.bx);

	/* BX returns a pointer that is resolved to a segment address with
	   offset zero as follows: i.e XXXX:0000 */
	hold=outregs.h.bh*0x100+outregs.h.bl;

	/* derive far pointer to current environment */
	parentenvptr = (unsigned int far *) MK_FP(hold,0x2c);
	hold = *parentenvptr;
	printf("Parents Environment Address = %X:0000\n",hold);
	currenv = (unsigned char far *) MK_FP(hold,0x00);
}

/* The environment is an array of chars delimited by NULLs and
   is ended by a double NULL.  The following routine prints
   the array. */

void listenv(void)
{
	c = currenv;
	while (*c!=NULL)
	{
		while (*c!=NULL)
		{
			putch(*c);
			c = (c + 1);
		}
		c = (c + 1);
		printf("\n");
	}
}
