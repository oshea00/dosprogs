/* MOUSEST.C -- This is a very simple program to test the mouse
* package.
**
*/
#include <graphics.h>
#include <mouse.h>

main()
{
	int gdriver=DETECT;
	int gmode, c;

	initgraph(&gdriver,&gmode,"");
	if (!initmouse()) {
		hidemouse();
		outtextxy(0,0,"No mouse detected");
		showmouse();
	}
	hidemouse();
	outtextxy(0,10,"Press a mouse button to quit");
	outtextxy(0,20,"or the INS or DEL key if a mouse isn't present");
	showmouse();
	do {
		c = waitforinput(EITHER_BUTTON);
	} while (c>=0);
	hidemouse();
	closegraph();
	return(0);
}
