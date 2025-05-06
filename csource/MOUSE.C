/*
** MOUSE.C -- Routines to support a Microsoft compatible mouse.  These functions also
** provide keyboard emulation of a mouse, if one does not exist.  This package
** assumes that you are running under graphics mode.  To move the emulated cursor, use
** the arrow keys on the keyboard and the INS and DEL keys as the left and right mouse
** buttons, respectively.  The gray + and - jkeys can be used to change the amount the
** emulated mouse cursor is moved by.
*/

#include <alloc.h>
#include <math.h>
#include <dos.h>
#include <graphics.h>
#include <process.h>
#include <stdio.h>
#include <conio.h>
#include <mouse.h>

#define TRUE 1
#define FALSE 0
#define _MAXINC 32 /* Largest increment amount of emulated mouse cursor */
int mouseexists;   /* Internal variable set true if a mouse driver is detected */
				   /* during initialization. This variable is used to */
				   /* select between the mouse code (if a mouse exists) and the */
				   /* keyboard's emulated mouse. */
/* All of the variables and functions that begin with an underscore
** are used to simulate a mouse when one is not present.
*/
int _cx, _cy;		/* Internal variables used to maintain */
					/* the cursor location when the mouse is not used */
int _cinc;			/* Internal variable used for increment amount of */
					/* the nonmouse cursor */
char *_cursor;		/* Points to image of the emulated mouse */
char *_undercursor;	/* Area saved under emulated mouse */
int  _cursoron;      /* TRUE if cursor currently visible */
int _cminx, _cmaxx;	/* Minimum, maximum x coordinates for internal cursor */
int _cminy, _cmaxy; /* Minimum, maximum y coordinates for internally */
					/* maintained cursor */
int _lbuttonpress;	/* True if simulated left or right */
int _rbuttonpress;	/* button is pressed */
int _numlpress;	/* Count the number of button presss */
int _numrpress;
int _numlrelease;	/* Count the number of button releases */
int _numrrelease;
int _cdeltax, _cdeltay;	/* Keeps track of emulated mouse's movements */
/* This routine provides the communication between the mouse driver and an application
** program. There are several predefined mouse functions supported by the MIcrosoft
** mouse -- see accompanying text. Parameters are sent back and forth
** to the mouse driver through the ax, bx, cx, and dx registers.
*/
void mouse(int *m1,int *m2,int *m3,int *m4)
{
	union REGS inregs, outregs;

	inregs.x.ax = *m1;  inregs.x.bx = *m2;
	inregs.x.cx = *m3;  inregs.x.dx = *m4;
	int86(0x33,&inregs,&outregs);
	*m1 = outregs.x.ax;  *m2 = outregs.x.bx;
	*m3 = outregs.x.cx;  *m4 = outregs.x.dx;
}

/* Call this routine at the beginnning of your program, after the graphics adapter has
** been initialized. It will initialize the mouse and display the mouse cursor at the
** top left of the screen, if one is present. Otherwise it will cause the emulated
** mouse to appear and the keyboard will be used to move the "mouse" cursor.
*/
int initmouse(void)
{
	int gmode;
	char far *memory = (char far *)0x004000049L;
	mouseexists = TRUE;
	if (resetmouse())
	{
		gmode = getgraphmode();
		if (gmode == HERCMONOHI)
		{
			*memory = 0x06;
			resetmouse();
		}
		movemouse(0,0);
		showmouse();
		return(1);
	}
	else
	{
		mouseexists = FALSE;
		_cx = _cy = 0;
		_cinc = 1; _cminx = 0;
		_cmaxx = getmaxx()-1;
		_cminy = 0;
		_cmaxy = getmaxy()-1;
		_cdeltax = _cdeltay = 0;
		_lbuttonpress = FALSE;
		_rbuttonpress = FALSE;
		_numlpress = 0;
		_numrpress = 0;
		_numlrelease = 0;
		_numrrelease = 0;
		_initcursor();
		_cursoron = FALSE;
		showmouse();
		return (0);
	}
}

/* Resets the mouse cursor to; screen center, mouse hidden, using arrow cursor and
** with minimum and maximum ranges set to full virtual screen dimensions. If a mouse
** driver exists, this function returns a -1 otherwise it returns a 0.
*/
int resetmouse(void)
{
	int m1,m2,m3,m4;

	m1 = RESET_MOUSE;
	mouse(&m1,&m2,&m3,&m4);
	return(m1);
}

/* Moves the mouse to the location (x,y)
*/
void movemouse(int x, int y)
{
	int m1, m2;

	if (!mouseexists)
	{
		hidemouse();
		_cx = x; _cy = y;
		showmouse();
		_cdeltax = _cdeltay = 0;
		return;
	}
	m1 = SET_MOUSE_COORD;
	if (getmaxx() == 319) x *= 2;
	mouse(&m1,&m2,&x,&y);
}
/* Removes the mouse cursor from the screen. Call this function before you write or
** draw anything to the screen.  It is also a good idea to turn off the mouse at the
** end of a program. Use showmouse() to restore the mouse on the screen. The mouse
** movement will bemaintained while the mouse is not visible. Due to a peculiarity of
** the mouse friver, make sure you don't call hidemouse() if the mouse is not already
** visible. See text discussion for more on this.
*/
void hidemouse(void)
{
	int m1,m2,m3,m4;

	if (!mouseexists)
	{
		_togglecursor();
	}
	else
	{
		m1 = HIDE_MOUSE;
		mouse(&m1,&m2,&m3,&m4);
	}
}

/* Display the mouse cursor. Normally, you should not call this
** function if the mouse is already visible. The keyboard mouse
** is clipped to the minimum and maximum ranges in this routine.
*/

void showmouse(void)
{
	int m1,m2,m3,m4;

	if (!mouseexists)
		_togglecursor();
	else
	{
		m1 = SHOW_MOUSE;
		mouse(&m1,&m2,&m3,&m4);
	}
}

/* Get the current location of the mouse cursor */
void getmousecoords(int *x, int *y)
{
	int m1,m2;

		if (!mouseexists)
		{
			*x = _cx; *y = _cy;
			return;
		}
		m1 = GET_MOUSE_STATUS;
		mouse(&m1,&m2,x,y);
		if (getmaxx() == 319) (*x) /= 2;
}

/* Find out how far the mouse has moved since the last call to this function */
void getmousemovement(int *x,int *y)
{
	int m1,m2;
	if (!mouseexists)
	{
		*x = _cdeltax; *y = _cdeltay;
		_cdeltax = _cdeltay = 0;
		return;
	}
	m1 = GET_MOUSE_MOVEMENT;
	mouse(&m1,&m2,x,y);
	if (getmaxx() == 319) (*x) /= 2;
}

/* Test if a button has been released since the last call to this
** function. If so, return 1, otherwise return 0. In the keyboard mode,
** button release is simulated by striking the button key again.
*/
int mousebuttonreleased(int whichbutton)
{
	if (!mouseexists)
	{
		return (_simulatedbuttonreleased(whichbutton));
	}
	return (testbutton(CHECK_BUTTON_RELEASE,whichbutton));

}

/* Return a 1 if the mouse button specified has been pressed since the
** last check with this function.  If the button has not been pressed, return a 0.
*/
int mousebuttonpressed(int whichbutton)
{
	if (!mouseexists)
		return (_simulatedbuttonpressed(whichbutton));
	return (testbutton(CHECK_BUTTON_PRESS,whichbutton));
}

/* Called by mousebuttonpressed() and mousebuttonreleased() to explicitly
** test the mouse button states.  The function returns TRUE if the specified
** mouse button (in whichbutton) performed the specified actoin (as indicated by testtype).
** Otherwise the function returns FALSE which means that the action tested for did not occur.
*/

int testbutton(int testtype, int whichbutton)
{
	int m1,m2,m3,m4;
	m1 = testtype;

	if (whichbutton == LEFT_BUTTON || whichbutton == EITHER_BUTTON)
	{
		m2 = LEFT_BUTTON;
		mouse(&m1,&m2,&m3,&m4);
		if (m2) return(TRUE);
	}
	if (whichbutton == RIGHT_BUTTON || whichbutton == EITHER_BUTTON)
	{
		m1 = testtype;
		m2 = RIGHT_BUTTON;
		mouse(&m1,&m2,&m3,&m4);
		if (m2) return(TRUE);
	}
	return(FALSE);
}

/* Returns TRUE if the specified simulated button has been pressed */
int _simulatedbuttonpressed(int whichbutton)
{
	if ((whichbutton == LEFT_BUTTON || whichbutton == EITHER_BUTTON) && _numlpress)
	{
		_numlpress--;
		return(TRUE);
	}
	else if ((whichbutton==RIGHT_BUTTON || whichbutton==EITHER_BUTTON) && _numrpress)
	{
		_numrpress--;
		return(TRUE);
	}
	/* If there isn't already a button pressed, chaeck and see if
	** the user just pressed one, and if so repeat the tests above.
	*/
	else if (_getkbinteraction(whichbutton) < 0)
	{
		if (whichbutton == LEFT_BUTTON || whichbutton == EITHER_BUTTON)
		{
			return(TRUE);
		}
		else if (whichbutton == RIGHT_BUTTON || whichbutton == EITHER_BUTTON)
		{
			return(TRUE);
		}
	}
	return(FALSE);
}

/* Returns TRUE if the specified simulated button has been released */
int _simulatedbuttonreleased(int whichbutton)
{
	if ((whichbutton == LEFT_BUTTON ||
		 whichbutton == EITHER_BUTTON) &&
		 _numlrelease)
	{
		_numlrelease--;
		return(TRUE);
	}
	else if ((whichbutton==RIGHT_BUTTON ||
			  whichbutton== EITHER_BUTTON) &&
			  _numrrelease)
	{
			  _numrrelease--;
			  return(TRUE);
	}
	/* If there isn't already a button released, check and see if
	** the user just released one, and if so repeat the tests above.
	*/
	else if (_getkbinteraction(whichbutton)<0)
	{
		if (whichbutton == LEFT_BUTTON || whichbutton == EITHER_BUTTON)
			return(TRUE);
		else if (whichbutton == RIGHT_BUTTON || whichbutton == EITHER_BUTTON)
			return(TRUE);
	}
	return(FALSE);
}

/* Test if the mouse cursor is within the box specified. Returns TRUE
** if the mouse is in the box; otherwise the function returns FALSE.
*/
int mouseinbox(int left, int top, int right, int bottom, int x, int y)
{
	return((x>=left&&x<=right&&y>=top&&y<=bottom) ? 1 : 0);
}

/* Returns a character if the key has been pressed, or -1 if a mouse button has
** been pressed, or a zero if none of the above. If a mouse exists, this routine
** favors any leyboard action.
*/
int getinput(int whichbutton)
{
	int c;

	if (!mouseexists)
	{
		c = _getkbinteraction(whichbutton);
		return(c);
	}
	if (kbhit())
		return(getch());
	else {
		if (mousebuttonpressed(whichbutton)) {
		  while (!mousebuttonreleased(whichbutton)) ;
		  return(-1);
		}
		else if (mousebuttonreleased(whichbutton))
			return(-1);
		return(0);
	}
}

/* Continue to call getinput() until a button or key has been pressed */
int waitforinput(int whichbutton)
{
	int c;

	while ((c=getinput(whichbutton)) == 0);
	return(c);
}

/* This routine is only used if a mouse does not exist. */

int _getkbinteraction(int whichbutton)
{
	int c;

	if (kbhit()) {
		c = _getkb();
		hidemouse();
		switch (c) {
			case 0x5200 :
				_lbuttonpress = (_lbuttonpress) ? FALSE : TRUE;
				showmouse();
				if (whichbutton == LEFT_BUTTON || whichbutton == EITHER_BUTTON)
					return(-1);
				if (_lbuttonpress) _numlpress++;
					else _numlrelease++;
				return(0);
			case 0x5300 :
				showmouse();
				_rbuttonpress = (_rbuttonpress) ? FALSE : TRUE;
				if (whichbutton == RIGHT_BUTTON || whichbutton == EITHER_BUTTON)
					return(-1);
				if (_rbuttonpress) _numrpress++;
					else _numrrelease++;
				return(0);
			case 0x002B :
				_cinc = (_cinc < _MAXINC) ? _cinc + 6 : _MAXINC;
				break;
			case 0x002D :
				_cinc = (_cinc > 1 + 6) ? _cinc - 6 : 1;
				break;
			case 0x4800 :
				if ((_cy -= _cinc) < _cminy) _cy = _cminy;
				_cdeltay -= _cinc;
				break;
			case 0x5000 :
				if ((_cy += _cinc) > _cmaxy) _cy = _cmaxy;
				_cdeltay += _cinc;
				break;
			case 0x4B00 :
				if ((_cx -= _cinc) < _cminx) _cx = _cminx;
				_cdeltax -= _cinc;
				break;
			case 0x4D00 :
				if ((_cx += _cinc) > _cmaxx) _cx = _cmaxx;
				_cdeltax += _cinc;
				break;
			case 0x4700 :
				if ((_cy -= _cinc) < _cminy) _cy = _cminy;
				if ((_cx -= _cinc) < _cminx) _cx = _cminx;
				_cdeltax -= _cinc;  _cdeltay -= _cinc;
				break;
			case 0x4900 :
				if ((_cy -= _cinc) < _cminy) _cy = _cminy;
				if ((_cx += _cinc) > _cmaxx) _cx = _cmaxx;
				_cdeltax += _cinc;  _cdeltay -= _cinc;
				break;
			case 0x4F00 :
				if ((_cy += _cinc) > _cmaxy) _cy = _cmaxy;
				if ((_cx -= _cinc) < _cminx) _cx = _cminx;
				_cdeltax -= _cinc;  _cdeltay += _cinc;
				break;
			case 0x5100 :
				if ((_cy += _cinc) > _cmaxy) _cy = _cmaxy;
				if ((_cx += _cinc) > _cmaxx) _cx = _cmaxx;
				_cdeltax += _cinc;  _cdeltay += _cinc;
				break;
			default : showmouse(); return(c);
		}
		showmouse();
	}
	return(0);
}

/* Low-level keyboard input that retrieves the sum of the extended codes produced by
** the arrow an grey keys. This routine does not support all the extended key codes.
*/

int _getkb(void)
{
	int ch1,ch2;

	ch1 = getch();
	if (ch1 == 0) {
		ch2 = getch();
		ch2 = ch2 << 8;
		ch2 |= ch1;
		return(ch2);
	}
	else
		return(ch1);
}
/* Creates the image of the simulated mouse. It calls drawcursor()
** which is a routine that you must supply to actually draw the cursor.
*/

void _initcursor(void)
{
/* Allocate space for the simulated mouse cursor and for the space
** below it.  If not enough memory, quit the program.
*/
	_cursor = malloc(imagesize(0,0,CURSWIDTH,CURSHEIGHT));
	_undercursor = malloc(imagesize(0,0,CURSWIDTH,CURSHEIGHT));
	if (_cursor == NULL || _undercursor == NULL) {
		closegraph();
		printf("Not enough memory for program.\n");
		exit(1);
	}
/* Save the image of the screen where the cursor image will be created.
** clear this space and call drawcursor() to draw the cursor.
*/
	getimage(_cx,_cy,_cx+CURSWIDTH,_cy+CURSHEIGHT,_undercursor);
	setlinestyle(SOLID_LINE,0,0);
	setfillstyle(SOLID_FILL,BLACK);
	bar(_cx,_cy,_cx+CURSWIDTH,_cy+CURSHEIGHT);
	_drawcursor(_cx,_cy);
/* Save the image of the cursor and overwrite the screen area where the
** cursor is with the original screen image.
*/
	getimage(_cx,_cy,_cx+CURSWIDTH,_cy+CURSHEIGHT,_cursor);
	putimage(_cx,_cy,_undercursor,COPY_PUT);
}

/* Used by the simulated mouse to turn the mouse cursor on and off. */

void _togglecursor(void)
{
	struct viewporttype vp;
	int oldx, oldy;
	getviewsettings(&vp);
	oldx = getx(); oldy = gety();
	setviewport(0,0,getmaxx(),getmaxy(),1);
	if (_cursoron) {
		putimage(_cx,_cy,_undercursor,COPY_PUT);
		_cursoron = FALSE;
	}
	else {
		getimage(_cx,_cy,_cx+CURSWIDTH,_cy+CURSHEIGHT,_undercursor);
		putimage(_cx,_cy,_cursor,COPY_PUT);
		_cursoron = TRUE;
	/* Reset the viewport settings and it's cp */
	setviewport(vp.left,vp.top,vp.right,vp.bottom,1);
	moveto(oldx,oldy);
	}
}
/* This routine draws a cursor if a mouse does not exists.  It draws
** a small arrow.
*/
void _drawcursor(int x,int y)
{
	setcolor(getmaxcolor());
	line(x+1,y+1,x+CURSWIDTH-1,y+CURSHEIGHT-1);
	line(x+2,y+1,x+CURSWIDTH-1,y+CURSHEIGHT-2);
	line(x+1,y+2,x+CURSWIDTH-2,y+CURSHEIGHT-1);
	line(x+2,y+1,x+2,y+5);
	line(x+1,y+1,x+1,y+5);
	line(x+1,y+1,x+5,y+1);
	line(x+1,y+2,x+5,y+2);
}

