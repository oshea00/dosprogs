#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <ctype.h>
#include <dir.h>
#include <process.h>


#define  MENUSIZE 7
#define  SUB1SIZE 1
#define  SUB2SIZE 3
#define  SUBSIZE 10
#define  TEXTLENGTH 25
#define  UP 0x48
#define  DOWN 0x50
#define  LEFT 0x4b
#define  RIGHT 0x4d
#define  ENTER 0x0d
#define  END 0x4F
#define  DEL 0x53
#define  ESC 0x1B
#define  BACKSPACE 0x08
#define  F1  0x3b
#define  MESSAGESIZE 100
#define  MESX 3
#define  MESY 23

typedef struct {
	char headername[10];
	} file_header;

typedef struct {
	int x;
	int y;
	int highlight;
	char choice[TEXTLENGTH];
	} choicetype;

typedef struct {
	int year;
	char month[3];
	char day[3];
	char subject[26];
	char message1[51];
	char message2[51];
	char message3[51];
	char message4[51];
	char message5[51];
	} message_entry_type;

struct ffblk ffblk;
struct text_info r;
union REGS regs;
union REGS inregs,outregs;
union REGS in,out;
char input[81];
int entrycnt=0;
int currentry=0;
int activefile=0;

char screenbuffer[80*25*2];
char files[20][13];
int filescnt;
char selected_file[13];

file_header fileheader;

choicetype mainmenulst[MENUSIZE] =
{ {2,2,0,"New   "},
  {12,2,0,"Find   "},
  {22,2,0,"Update   "},
  {32,2,0,"Delete   "},
  {42,2,1,"fIle   "},
  {52,2,0,"Clear   "},
  {62,2,0,"Quit   "} };

char quickmain[10] = "NFUDICQ";

choicetype sub1menulst[SUB1SIZE] =
{ {2,2,7,"search Subjects"} };

char quicksub1[10] = "S";

choicetype sub2menulst[SUB2SIZE] =
{ {2,2,0,"Open file"},
  {2,3,0,"Save file"},
  {2,4,0,"Delete file"} };

char quicksub2[10] = "OSD";

message_entry_type messagelist[MESSAGESIZE];
message_entry_type message;

int main(void);
int mainmenu(void);
int sub1menu(void);
int sub2menu(void);
int getchoicelr(int,int,int,int,int,choicetype []);
int getchoiceud(int,int,int,int,int,choicetype [],char [],int);
void drawmainmenu(void);
void border(int,int,int,int,char *);
void placechar(int,int,int);
void placestr(int,int,char *);
void box(int,int,int,int);
void cursoroff(void);
void cursoron(void);
int mgetch(int *,char *);
void clear_fields(void);
void create_record(void);
int find_record(int);
void update_record(int);
void clear_record(message_entry_type *);
void getstrfld(int,int,int,int,int);
void fill_record(void);
void display_record(message_entry_type *);
int messagewait(char *);
int messagewaitinv(char *);
void messagepause(char *);
void getscreenchars(int,int,int);
int compare(char *,char *);
int fillfilelist(void);
int select_file_name(void);
void draw_file_box(void);
void list_files(int);
int get_file_name(void);
void save_file(void);
void open_file(void);
void delete_file(void);
int editfilename();

int main(void)
{
	textmode(C80);
	gettextinfo(&r);
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	clrscr();
	selected_file[0]=NULL;
	drawmainmenu();
	while (mainmenu());
	textattr(r.attribute);
	clrscr();
	cursoron();
	return 0;
}

int mainmenu(void)
{
	int choice,quit=9,subval,found;
	choice = getchoicelr(MENUSIZE,BLACK,LIGHTCYAN,BLUE,LIGHTCYAN,mainmenulst);
	switch (choice)
	{
		case  1  :  cursoron();create_record();cursoroff();break;   /*sub1menu()*/
		case  2  :  if (subval=sub1menu())
					{
						clear_fields();
						find_record(subval);
					}
					else
					{
						messagepause("Find aborted");
					}
					;break;
		case  3  :  if (currentry>=0)
					{
						memcpy(&message,&messagelist[currentry],sizeof(message_entry_type));
						cursoron();
						update_record(currentry);
						cursoroff();
					}
					else
					{
						messagepause("No record displayed");
					}
					;break;
		case  4  :  if (entrycnt>0&&currentry>=0)
					{
						if (!messagewait("Are you sure?"))
						{
							memcpy(&messagelist[currentry],&messagelist[entrycnt-1],sizeof(message_entry_type));
							entrycnt--;
							clear_fields();
						}
					};break;
		case  5  :  if (subval=sub2menu())
					{
						switch(subval)
						{
							case 1 : if (activefile)
									 {
										if (!messagewaitinv("Would you like to save the current file?"))
										{
											messagepause("Saving current file");
											save_file();
											if (get_file_name())
											{
												 messagepause("Opening file");
												 open_file();break;
											}
											else
											{
												messagepause("File operation aborted");break;
											}
										}
										else
										{
											if (get_file_name())
											{
												 messagepause("Opening file");
												 open_file();break;
											}
											else
											{
												messagepause("File operation aborted");break;
											}
										}
									 }
									 if (get_file_name())
									 {
										 messagepause("Opening file");
										 open_file();break;
									 }
									 else
									 {
										messagepause("File operation aborted");break;
									 }
							case 2 : if (get_file_name())
									 {
										messagepause("Saving file");
										save_file();break;
									 }
									 else
									 {
										messagepause("File operation aborted");break;
									 }
							case 3 : get_file_name();break;
						}
						break;
					}
					else
					{
						messagepause("File operation aborted");
					};break;
		case  6  :  if (messagewait("Clear all current entries"))
						break;
					else
					{
						entrycnt=0;
						selected_file[0]=NULL;
						clear_fields();
					};break;
		case  7  :  if (activefile)
					{
						if (!messagewaitinv("Would you like to save this?"))
						{
							if (strlen(selected_file))
							{
								messagepause("Saving current file");
								save_file();
							}
							else
							{
								 if (get_file_name())
								 {
									messagepause("Saving file");
									save_file();
								 }
								 else
								 {
									messagepause("File operation aborted");break;
								 }
							}
						}
					}
					quit=0;break;
	}
	return quit;
}

int sub1menu(void)
{
	int i,choice;
	gettext(1,1,80,25,screenbuffer);
	window(2,3,21,6);
	textcolor(YELLOW);
	textbackground(BLUE);
	clrscr();
	box(1,1,19,4);
	for (i=0;i<SUB1SIZE;i++)
	{
		placestr(sub1menulst[i].x,sub1menulst[i].y,sub1menulst[i].choice);
		textcolor(LIGHTCYAN);
		gotoxy(sub1menulst[i].x+sub1menulst[i].highlight,sub1menulst[i].y);
		putch(sub1menulst[i].choice[sub1menulst[i].highlight]);
		textcolor(YELLOW);
	}
	choice = getchoiceud(SUB1SIZE,BLUE,YELLOW,LIGHTGRAY,YELLOW,sub1menulst,quicksub1,SUB1SIZE);
	window(1,1,80,25);
	puttext(1,1,80,25,screenbuffer);
	switch (choice)
	{
		case  0  :  return 0;   /*  ESC taken */
		case  1  :  return 1;
		case  2  :  return 2;
		case  3  :  break;
		case  4  :  break;
	}
}

int sub2menu(void)
{
	int i,choice;
	gettext(1,1,80,25,screenbuffer);
	window(41,3,66,7);
	textcolor(BLACK);
	textbackground(GREEN);
	clrscr();
	box(1,1,25,5);
	for (i=0;i<SUB2SIZE;i++)
	{
		placestr(sub2menulst[i].x,sub2menulst[i].y,sub2menulst[i].choice);
		textcolor(LIGHTCYAN);
		gotoxy(sub2menulst[i].x+sub2menulst[i].highlight,sub2menulst[i].y);
		putch(sub2menulst[i].choice[sub2menulst[i].highlight]);
		textcolor(BLACK);
	}
	choice = getchoiceud(SUB2SIZE,GREEN,BLACK,LIGHTGRAY,YELLOW,sub2menulst,quicksub2,SUB2SIZE);
	window(1,1,80,25);
	puttext(1,1,80,25,screenbuffer);
	switch (choice)
	{
		case  0  :  return 0;   /*  ESC taken */
		case  1  :  return 1;
		case  2  :  return 2;
		case  3  :  return 3;
		case  4  :  break;
	}
}


void drawmainmenu(void)
{
	int i;
	char string[80];
	textbackground(BLACK);
	textcolor(CYAN);
	border(1,1,80,25," Messages ");
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	for (i=0;i<MENUSIZE;i++)
	{
		placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
		textcolor(WHITE);
		gotoxy(mainmenulst[i].x+mainmenulst[i].highlight,mainmenulst[i].y);
		putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
		textcolor(LIGHTCYAN);
	}
	/* Help line */
	textcolor(RED);
	textbackground(LIGHTGRAY);
	placestr(2,25,"F1");
	textcolor(BLACK);
	placestr(4,25,"-HELP");

	/* Screen Labels */
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	placestr(05,9," Subject: ");
	placestr(05,10," Message: ");
	placestr(05,11,"        : ");
	placestr(05,12,"        : ");
	placestr(05,13,"        : ");
	placestr(05,14,"        : ");
	textbackground(BLUE);
	placestr(65,25,"             ");
	textbackground(BLACK);
	clear_fields();
}

void clear_fields(void)
{
	textbackground(BLUE);
	placestr(15,9,"                         ");
	placestr(15,10,"                                                  ");
	placestr(15,11,"                                                  ");
	placestr(15,12,"                                                  ");
	placestr(15,13,"                                                  ");
	placestr(15,14,"                                                  ");
	currentry=-1;
}

int getchoicelr(int num,int orgback,int orgfore,int selback,int selfore,choicetype mainmenulst[MENUSIZE])
{
	int i=0,j,keycode;
	char ascii=0;
	textcolor(selfore);
	textbackground(selback);
	placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
	textcolor(WHITE);
	gotoxy(mainmenulst[i].x+mainmenulst[i].highlight,mainmenulst[i].y);
	putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
	textcolor(selfore);
	cursoroff();
	while (ascii!=ENTER)
	{
		mgetch(&keycode,&ascii);
		switch (keycode)
		{
			case LEFT : textcolor(orgfore);
						textbackground(orgback);
						placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
						textcolor(WHITE);
						gotoxy(mainmenulst[i].x+mainmenulst[i].highlight,mainmenulst[i].y);
						putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
						textcolor(orgfore);
						i-=1;
						if (i<0)
							i=num-1;
						textcolor(selfore);
						textbackground(selback);
						placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
						textcolor(WHITE);
						gotoxy(mainmenulst[i].x+mainmenulst[i].highlight,mainmenulst[i].y);
						putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
						textcolor(selfore);break;
			case RIGHT : textcolor(orgfore);
						textbackground(orgback);
						placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
						textcolor(WHITE);
						gotoxy(mainmenulst[i].x+mainmenulst[i].highlight,mainmenulst[i].y);
						putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
						textcolor(orgfore);
						i+=1;
						if (i>num-1)
							i=0;
						textcolor(selfore);
						textbackground(selback);
						placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
						textcolor(WHITE);
						gotoxy(mainmenulst[i].x+mainmenulst[i].highlight,mainmenulst[i].y);
						putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
						textcolor(selfore);break;
			case F1  :  spawnl(P_WAIT,"help","help.exe","message.hlp",NULL);break;
			default  :  if (ascii==ENTER)
						{
							textcolor(orgfore);
							textbackground(orgback);
							placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
							textcolor(WHITE);
							gotoxy(mainmenulst[i].x+mainmenulst[i].highlight,mainmenulst[i].y);
							putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
							textcolor(orgfore);
							break;
						}
						break;
		}
		if (isalpha(ascii))
		{
			for (j=0;j<MENUSIZE;j++)
			{
				if (quickmain[j]==toupper(ascii))
					return j+1;
			}
		}
	}
	return i+1;
}

int getchoiceud(int num,int orgback,int orgfore,int selback,int selfore,
				choicetype submenulst[SUBSIZE],char quicksub[10],int subsize)
{
	int i=0,j,keycode;
	char ascii=0;
	textcolor(selfore);
	textbackground(selback);
	placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);
	textcolor(LIGHTCYAN);
	gotoxy(submenulst[i].x+submenulst[i].highlight,submenulst[i].y);
	putch(submenulst[i].choice[submenulst[i].highlight]);
	textcolor(selfore);
	cursoroff();
	while (ascii!=ENTER&&ascii!=ESC)
	{
		mgetch(&keycode,&ascii);
		switch (keycode)
		{
			case UP :  textcolor(orgfore);
					   textbackground(orgback);
					   placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);
					   textcolor(LIGHTCYAN);
					   gotoxy(submenulst[i].x+submenulst[i].highlight,submenulst[i].y);
					   putch(submenulst[i].choice[submenulst[i].highlight]);
					   textcolor(orgfore);
					   i-=1;
					   if (i<0)
						   i=num-1;
					   textcolor(selfore);
					   textbackground(selback);
					   placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);
					   textcolor(LIGHTCYAN);
					   gotoxy(submenulst[i].x+submenulst[i].highlight,submenulst[i].y);
					   putch(submenulst[i].choice[submenulst[i].highlight]);
					   textcolor(selfore);break;
			case DOWN :  textcolor(orgfore);
					   textbackground(orgback);
					   placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);
					   textcolor(LIGHTCYAN);
					   gotoxy(submenulst[i].x+submenulst[i].highlight,submenulst[i].y);
					   putch(submenulst[i].choice[submenulst[i].highlight]);
					   textcolor(orgfore);
					   i+=1;
					   if (i>num-1)
						   i=0;
					   textcolor(selfore);
					   textbackground(selback);
					   placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);
					   textcolor(LIGHTCYAN);
					   gotoxy(submenulst[i].x+submenulst[i].highlight,submenulst[i].y);
					   putch(submenulst[i].choice[submenulst[i].highlight]);
					   textcolor(selfore);break;
			case RIGHT : ascii=ESC;break;
			case LEFT  : ascii=ESC;break;
			default   : break;
		}
		if (isalpha(ascii))
		{
			for (j=0;j<subsize;j++)
			{
				if (quicksub[j]==toupper(ascii))
					return j+1;
			}
		}
	}
	if (ascii==ESC)
		return 0;
	return i+1;
}


/*
** This creates a titled box by specifying the top left and bottom
** right coordinates and the title.
** Note: be sure that your coordinates do not exceed the
**       current text window, it will cause truncation.
**
**
*/

void border(int a,int b,int c,int d,char *str)
{
	int i;
	for (i=a;i<c;i++) placechar(i,b,205);
	for (i=a;i<c;i++) placechar(i,d-1,205);
	for (i=b;i<d-1;i++) placechar(a,i,186);
	for (i=b;i<d-1;i++) placechar(c,i,186);
	placechar(a,b,201);
	placechar(c,b,187);
	placechar(a,d-1,200);
	placechar(c,d-1,188);
	placechar(a,b+2,204);
	placechar(c,b+2,185);
	for (i=a+1;i<c;i++) placechar(i,b+2,205);
	gotoxy((c-strlen(str)+a)/2,b);
	textcolor(YELLOW);
	textbackground(BLACK);
	cprintf("%s",str);
}


/*  puts a character at coords a,b - character is passed as int */

void placechar(int a,int b,int c)
{
	gotoxy(a,b);
	cprintf("%c",c);
};

void placestr(int a,int b,char *str)
{
	gotoxy(a,b);
	cprintf(str);
}


/* Same as border only without the Title option */

void box(int a,int b,int c,int d)
{
    int i;
	for (i=a;i<=c;i++) placechar(i,b,196);
	for (i=a;i<=c;i++) placechar(i,d,196);
	for (i=b;i<=d;i++) placechar(a,i,179);
	for (i=b;i<=d;i++) placechar(c,i,179);
	placechar(a,b,218);
	placechar(c,b,191);
	placechar(a,d,192);
	placechar(c,d,217);
}

void cursoroff(void)
{
	inregs.h.ah = 0x01;
	inregs.h.ch = 0x20;
	int86(0x10,&inregs,&outregs);
}

int mgetch(int *scan,char *ascii)
{
	regs.h.ah = 0x00;
	int86(0x16,&regs,&regs);
	*scan=regs.h.ah;
	*ascii=regs.h.al;
}

void cursoron(void)
{
	inregs.h.ah = 0x01;
	inregs.h.ch = 6;
	inregs.h.cl = 7;
	int86(0x10,&inregs,&outregs);
}

void update_record(int recno)
{
	clear_fields();
	do
	{
		display_record(&message);
		fill_record();
	} while (messagewait("Done with changes?"));
	if (messagewaitinv("Save update?"))
	{
		clear_fields();
		messagepause("Update Aborted");
	}
	else
	{
		memcpy(&messagelist[recno],&message,sizeof(message_entry_type));
		activefile=1;
		messagepause("Update completed");
	}
	currentry=recno;
}

void create_record(void)
{
	clear_record(&message);
	clear_fields();
	do
	{
		display_record(&message);
		fill_record();
	} while (messagewait("Done with entry?"));
	if (messagewaitinv("Save entry?"))
	{
		clear_fields();
		messagepause("Entry Aborted");
	}
	else
	{
		memcpy(&messagelist[entrycnt],&message,sizeof(message_entry_type));
		entrycnt++;
		activefile=1;
		messagepause("Entry Saved");
		currentry=entrycnt-1;
	}

}

int find_record(int keytype)
{
	int keycode,i;
	char ascii;
	gettext(1,1,80,25,screenbuffer);
	cursoron();
	textcolor(LIGHTCYAN);
	textbackground(BLUE);
	window(12,3,40,6);
	clrscr();
	box(1,1,28,4);
	textcolor(YELLOW);
	textbackground(BLACK);
	placestr(2,2,"                         ");
	textcolor(LIGHTCYAN);
	textbackground(BLUE);
	placestr(2,3,"Enter all or part to find");
	getstrfld(2,2,25,BLACK,YELLOW);
	window(1,1,80,25);
	puttext(1,1,80,25,screenbuffer);
	textcolor(LIGHTCYAN);
	textbackground(BLUE);
	i=0;
	while(1)
	{
		while (i<entrycnt)
		{
			if (keytype==1)
			{
				if (compare(messagelist[i].subject,input))
				{
					display_record(&messagelist[i]);
					currentry=i;
					if (messagewait("Keep looking?")==1)
					{
						cursoroff();
						return i;
					}
					i++;
					break;
				}
				i++;
			}
		}
		if (i==entrycnt)
		{
			messagepause("End of matches");
			cursoroff();
			return -1;
		}
	}
}

void fill_record(void)
{
	getstrfld(15,9,25,BLUE,LIGHTCYAN);
	if (strlen(input)>0)
		strcpy(message.subject,input);
	getstrfld(15,10,50,BLUE,LIGHTCYAN);
	if (strlen(input)>0)
		strcpy(message.message1,input);
	getstrfld(15,11,50,BLUE,LIGHTCYAN);
	if (strlen(input)>0)
		strcpy(message.message2,input);
	getstrfld(15,12,50,BLUE,LIGHTCYAN);
	if (strlen(input)>0)
		strcpy(message.message3,input);
	getstrfld(15,13,50,BLUE,LIGHTCYAN);
	if (strlen(input)>0)
		strcpy(message.message4,input);
	getstrfld(15,14,50,BLUE,LIGHTCYAN);
	if (strlen(input)>0)
		strcpy(message.message5,input);
}

void clear_record(message_entry_type *message)
{
	message->subject[0]=NULL;
	message->message1[0]=NULL;
	message->message2[0]=NULL;
	message->message3[0]=NULL;
	message->message4[0]=NULL;
	message->message5[0]=NULL;
}

void display_record(message_entry_type *message)
{
	clear_fields();
	textcolor(LIGHTCYAN);
	placestr(15,9,message->subject);
	placestr(15,10,message->message1);
	placestr(15,11,message->message2);
	placestr(15,12,message->message3);
	placestr(15,13,message->message4);
	placestr(15,14,message->message5);
}
void getstrfld(int x,int y,int length,int backcolor,int forecolor)
{
	int keycode,i=0,j=0;
	char ascii;
	textcolor(forecolor);
	textbackground(backcolor);
	gotoxy(x,y);
	mgetch(&keycode,&ascii);
	while(ascii!=ENTER&&ascii!=ESC)
	{
		if (isprint(ascii))
		{
			if (i<length)
			{
				gotoxy(x+i,y);
				putch(ascii);
				i++;
			}
		}
		else
		if (ascii==BACKSPACE)
		{
			gotoxy(x+i,y);
			if (i<length)
				putch(' ');
			i--;
			if (i<0)
				i=0;
			gotoxy(x+i,y);
		}
		else
		if (keycode==LEFT)
		{
			gotoxy(x+i,y);
			i--;
			if (i<0)
				i=0;
			gotoxy(x+i,y);
		}
		else
		if (keycode==RIGHT)
		{
			gotoxy(x+i,y);
			i++;
			if (i>length)
				i=length;
			gotoxy(x+i,y);
		}
		else
		if (keycode==DEL)
		{
			gotoxy(x+i,y);
			for (j=x+i;j<x+length-1;j++)
			{
				getscreenchars(j+1,y,1);
				gotoxy(j,y);
				putch(input[0]);
			}
			if (j<x+length)
			{
				gotoxy(j,y);
				putch(' ');
				gotoxy(x+i,y);
			}
			else
				gotoxy(x+i,y);
		}
		else
		if (keycode==END)
		{
			gotoxy(x+i,y);
			for (j=x+length-1;j>=x;j--)
			{
				getscreenchars(j,y,1);
				if (isgraph(input[0]))
					break;
				gotoxy(j,y);
			}
			i=j+1-x;
			gotoxy(x+i,y);
		}
		mgetch(&keycode,&ascii);
	}
	for (j=x+length-1;j>=x;j--)
	{
		getscreenchars(j,y,1);
		if (isgraph(input[0]))
			break;
		gotoxy(j,y);
	}
	i=j+1-x;
	gotoxy(x+i,y);
	getscreenchars(x,y,(x+i)-x);
}

void messagepause(char *text)
{
	int x,y;
	x = wherex();
	y = wherey();
	gettextinfo(&r);
	textcolor(LIGHTCYAN);
	textbackground(BLUE);
	gettext(1,1,80,25,screenbuffer);
	gotoxy(MESX,MESY);
	cprintf("%s",text);
	delay(600);
	puttext(1,1,80,25,screenbuffer);
	gotoxy(x,y);
	textattr(r.attribute);
}

int messagewait(char *text)
{
	int x,y,keycode;
	char ascii;
	x = wherex();
	y = wherey();
	gotoxy(MESX,MESY);
	gettextinfo(&r);
	gettext(1,1,80,25,screenbuffer);
	cprintf("%s (y/n): ",text);
	putch('N');
	gotoxy(MESX+strlen(text)+8,MESY);
	mgetch(&keycode,&ascii);
	putch(toupper(ascii));
	puttext(1,1,80,25,screenbuffer);
	gotoxy(x,y);
	textattr(r.attribute);
	if (toupper(ascii)=='Y')
		return 0;
	else
		return 1;
}

int messagewaitinv(char *text)
{
	int x,y,keycode;
	char ascii;
	x = wherex();
	y = wherey();
	gotoxy(MESX,MESY);
	gettextinfo(&r);
	gettext(1,1,80,25,screenbuffer);
	cprintf("%s (y/n): ",text);
	putch('Y');
	gotoxy(MESX+strlen(text)+8,MESY);
	mgetch(&keycode,&ascii);
	putch(toupper(ascii));
	puttext(1,1,80,25,screenbuffer);
	gotoxy(x,y);
	textattr(r.attribute);
	if (toupper(ascii)=='N')
		return 1;
	else
		return 0;
}


void getscreenchars(int x,int y,int length)
{
	int i;
	in.h.ah = 0x08;
	in.h.bh = 0x00;
	for (i=0;i<length;i++)
	{
		gotoxy(x+i,y);
		int86(0x10,&in,&out);
		input[i] = out.h.al;
	}
	input[i]=NULL;
}

int compare(char *src,char *dest)
{
	int i=0;
	char hold[80];
	char hold2[80];
	strcpy(hold,src);
	strcpy(hold2,dest);
	for (i=0;hold[i]=toupper(hold[i]);i++);
	for (i=0;hold2[i]=toupper(hold2[i]);i++);
	return strstr(hold,hold2);
}

void open_file(void)
{
	int i=0;
	FILE *infile;
	if (strlen(selected_file))
	{
		if (editfilename())
		{
			if ((infile = fopen(selected_file,"rb"))==NULL)
			{
				messagepause("Error opening given file name");
			}
			else
			{
				clear_fields();
				fflush(infile);
				fread(&fileheader,sizeof(file_header),1,infile);
				if (!strcmpi(fileheader.headername,"message"))
				{
					while (fread(&messagelist[i],sizeof(message_entry_type),1,infile))
					{
						i++;
					}
					entrycnt=i++;
					activefile=1;
					fclose(infile);
					display_record(&messagelist[0]);
					currentry=0;
					placestr(65,25,"             ");
					placestr(65,25,selected_file);
				}
				else
				{
					messagepause("Invalid file format");
				}
			}
		}
	}
	else
	{
		messagepause("No file name given");
	}
}

void save_file(void)
{
	int i=0;
	FILE *outfile;
	strcpy(fileheader.headername,"message");
	clear_fields();
	fflush(outfile);
	if (strlen(selected_file)&&entrycnt>0)
	{
		if ((outfile = fopen(selected_file,"wb"))==NULL)
		{
			messagepause("Error opening output file");
		}
		else
		{
			activefile=0;
			fwrite(&fileheader,sizeof(file_header),1,outfile);
			for (i=0;i<entrycnt;i++)
			{
				fwrite(&messagelist[i],sizeof(message_entry_type),1,outfile);
			}
			fclose(outfile);
			entrycnt=0;
			placestr(65,25,"             ");

		}
	}
	else
	{
		messagepause("No file loaded to save");
	}

}

void delete_file(void)
{

}

int editfilename()
{
	return 1;
}

int get_file_name()
{
	int ret;
	gettext(1,1,80,25,screenbuffer);
	fillfilelist();
	ret = select_file_name();
	puttext(1,1,80,25,screenbuffer);
	return ret;
}

int fillfilelist()
{
	int done,i=0;
	done = findfirst("*.msg",&ffblk,0);
	if (!done)
	{
		while (!done)
		{
			strcpy(files[i],ffblk.ff_name);
			i++;
			done = findnext(&ffblk);
		}
		filescnt=i;
		return 1;
	}
	else
	{
		return 0;
	}
}

int select_file_name()
{
	int i;
	draw_file_box();
	list_files(0);
	gotoxy(3,2);
	textcolor(WHITE);
	cprintf("Enter file name: ");
	textbackground(BLACK);
	cursoron();
	gotoxy(20,2);
	cprintf("             ");
	getstrfld(20,2,13,BLACK,WHITE);
	cursoroff();
	if (strlen(input))
	{
		for (i=0;input[i]=toupper(input[i]);i++);
		for (i=0;input[i]!=NULL&&i<8;i++)
			if (input[i]=='.') break;
		input[i]='.';
		input[i+1]='M';
		input[i+2]='S';
		input[i+3]='G';
		input[i+4]=NULL;
		strcpy(selected_file,input);
		window(1,1,80,25);
		return 1;
	}
	else
	{
		window(1,1,80,25);
		return 0;
	}
}

void list_files(int starting)
{
	int i,j,c=0,end;
	j=starting;
	end=j+20;
	if (end>filescnt)
		end=filescnt;
	while (j<end)
	{
		for (i=0;i<5;i++)
		{
			gotoxy(14*c+3,i+4);
			cprintf("%s",files[j]);
			j++;
		}
		c++;
	}
}

void draw_file_box()
{
	window(5,5,76,13);
	textbackground(RED);
	clrscr();
	box(1,1,71,9);
}



