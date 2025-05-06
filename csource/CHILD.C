#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>
#include <conio.h>

int main(void);
void child(void);
void parentenv(void);

unsigned int far *childenvptr, far *parentpspptr, far *parentpspenvptr,
			 far *parentenvptr;
unsigned int hold;
union REGS inregs,outregs;

/* Refer to the notes provided with the PARENT.C source.
   An interesting thing about this child() program.  If you
   run it explicitly at the DOS prompt, it will affect your
   DOS environment by replacing the first line with EEEE=EEEE.
   So this is really doing what I said it could.  While being
   run as a child process under another 'C' program, it only
   affects the Environment of the parent.
   Anyway, this can be expanded to do some things that the DOS
   SET command doesn't do.  I'll leave that for tinkering...

   Please excuse the sloppy code, this WAS a quick and dirty.
*/

int main(void)
{
	printf("\nCHILD PROCESSING");
	child();
	printf("END CHILD PROCESSING\n");
	return 0;
}

void child(void)
{
	/* call dos interrupt function 0x51 to get PSP segment pointer*/
	inregs.h.ah = 0x51;
	intdos(&inregs,&outregs);
	printf("\nCHILD PSP: %p:0000\n",outregs.x.bx);

	/* convert BX value to XXXX:0000 form of far pointer for child PSP*/
	hold=outregs.h.bh*0x100+outregs.h.bl;

	/* create far pointers to child environ and parent PSP */
	childenvptr = (unsigned int far *) MK_FP(hold,0x2c);
	parentpspptr = (unsigned int far *) MK_FP(hold,0x16);
	hold = *childenvptr;
	printf("Child environment = %X:0000\n",hold);
	hold = *parentpspptr;
	printf("parent psp = %X:0000\n",hold);

	/* get pointer to parents environment from parent PSP*/
	parentpspenvptr = (unsigned int far *) MK_FP(hold,0x2c);
	hold = *parentpspenvptr;
	printf("Parents environment pointer = %X:0000\n",hold);

	/* create far pointer to parents environment */
	parentenvptr = (unsigned int far *) MK_FP(hold,0);

	parentenv();

}

/*
   Display the parents first environment string and
   alter it.  Since I'm using the small model for
   this program, I'm bypassing the use of Turbo 'C'
   string handling functions as they are used to
   getting near pointers when in small model.
*/

void parentenv(void)
{
	char far *c;
	int i;
	c = (char far *) parentenvptr;

	/* Display parents first string and change it as we go....
	   Of course by now you can do almost anything you want with
	   the parents environment except increase it's size, since
	   the DOS loader creates only an exact size copy.

	   Actually this whole excercise is academic because the spawn
	   function allows you to set up an environment area for a child
	   process to use and pass it to the child.  The child can use
	   this area and the parent already knows where to look when the
	   child process returns.
	*/

	while (*c!=NULL)
	{
		putch((int)*c);
		/* Change the character */
		if (*c != '=')
			*c = 'E';
		c=(c+1);       /* Precedence keeps me from using += operator */
	}
	printf("\n");
}