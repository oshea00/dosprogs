/*
** WRITOBJ.C
**
** MIke O'Shea
**
** Used to create object database for TEXTSHEL
**
**
*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>

typedef struct	{
	char name[20];
	char descfile[12];
	char number[4];
	char size[4];
	char location[4];
	char value[4];
	char visible[2];
	char attach[2];
	char attachto[4];
	char sizelim[4];
	char character[2];
	char solved[2];
		} OBJREC;

OBJREC Ob, *Obptr;

OBJREC work[99];

int objcnt,currobj = 0;

int main()
{
	int menu_process();
	Obptr = &Ob;
	reset_window();
	while (menu_process());
	clrscr();
	return 0;
}

int menu_process()
{
	char done[2],*endptr = NULL;
	clrscr();
	border(1,1,80,25,"Object Maintenance Menu");
	placestr(30,5,"1 ..... Create Object");
	placestr(30,7,"2 ..... Update Object");
	placestr(30,9,"3 ..... Delete Object");
	placestr(30,11,"4 ..... Display Object");
	placestr(30,13,"5 ..... Load Object File");
	placestr(30,15,"6 ..... Save Object File");
	placestr(30,17,"Q ..... Quit");
	placestr(30,19,"Option? :");
	getstrfld(40,19,&done);
	switch (strtol(done,endptr,10))
		{ case  1 : create_object();
			    break;
		  case  2 : update_object();
			    break;
		  case  3 : delete_object();
			    break;
		  case  4 : display_object();
			    break;
		  case  5 : load_objects();
			    break;
		  case  6 : save_objects();
			    break;
		  default : return 0;
		};
	return 1;
}

int screen_enter(OBJREC *Obptr)
{
	char done[2];
	border(1,1,80,25,"Object Entry Screen");
	placestr(2,5,"Object Number:");
	getstrfld(17,5,&Obptr->number);
	placestr(2,6,"Object Size  :");
	getstrfld(17,6,&Obptr->size);
	placestr(2,7,"Location     :");
	getstrfld(17,7,&Obptr->location);
	placestr(2,8,"Value        :");
	getstrfld(17,8,&Obptr->value);
	placestr(2,9,"Visible      :");
	getstrfld(17,9,&Obptr->visible);
	placestr(2,10,"Attach Hook  :");
	getstrfld(17,10,&Obptr->attach);
	placestr(2,11,"Attached To  :");
	getstrfld(17,11,&Obptr->attachto);
	placestr(2,12,"Size Limit   :");
	getstrfld(17,12,&Obptr->sizelim);
	placestr(2,13,"Object Name  :");
	getstrfld(17,13,&Obptr->name);
	placestr(2,14,"Character    :");
	getstrfld(17,14,&Obptr->character);
	placestr(2,15,"Solved       :");
	getstrfld(17,15,&Obptr->solved);
	placestr(2,16,"Desc File    :");
	getstrfld(17,16,&Obptr->descfile);
	placestr(2,18,"Information Correct? (y/n):");
	getstrfld(30,18,&done);
	return (strcmpi(done,"y"));
}

int screen_display(OBJREC *Obptr)
{
	char done[2];
	border(1,1,80,25,"Object Display Screen");
	placestr(2,5,"Object Number:");
	placestr(17,5,&Obptr->number);
	placestr(2,6,"Object Size  :");
	placestr(17,6,&Obptr->size);
	placestr(2,7,"Location     :");
	placestr(17,7,&Obptr->location);
	placestr(2,8,"Value        :");
	placestr(17,8,&Obptr->value);
	placestr(2,9,"Visible      :");
	placestr(17,9,&Obptr->visible);
	placestr(2,10,"Attach Hook  :");
	placestr(17,10,&Obptr->attach);
	placestr(2,11,"Attached To  :");
	placestr(17,11,&Obptr->attachto);
	placestr(2,12,"Size Limit   :");
	placestr(17,12,&Obptr->sizelim);
	placestr(2,13,"Object Name  :");
	placestr(17,13,&Obptr->name);
	placestr(2,14,"Character    :");
	placestr(17,14,&Obptr->character);
	placestr(2,15,"Solved       :");
	placestr(17,15,&Obptr->solved);
	placestr(2,16,"Desc File    :");
	placestr(17,16,&Obptr->descfile);
	placestr(2,18,"Done? (y/n):");
	getstrfld(15,18,&done);
	return (strcmpi(done,"y"));
}

int screen_update(OBJREC *Obptr)
{
	char done[2];
	border(1,1,80,25,"Object Update Screen");
	placestr(2,5,"Object Number:");
	placestr(17,5,&Obptr->number);
	placestr(2,6,"Object Size  :");
	placestr(17,6,&Obptr->size);
	placestr(2,7,"Location     :");
	placestr(17,7,&Obptr->location);
	placestr(2,8,"Value        :");
	placestr(17,8,&Obptr->value);
	placestr(2,9,"Visible      :");
	placestr(17,9,&Obptr->visible);
	placestr(2,10,"Attach Hook  :");
	placestr(17,10,&Obptr->attach);
	placestr(2,11,"Attached To  :");
	placestr(17,11,&Obptr->attachto);
	placestr(2,12,"Size Limit   :");
	placestr(17,12,&Obptr->sizelim);
	placestr(2,13,"Object Name  :");
	placestr(17,13,&Obptr->name);
	placestr(2,14,"Character    :");
	placestr(17,14,&Obptr->character);
	placestr(2,15,"Solved       :");
	placestr(17,15,&Obptr->solved);
	placestr(2,16,"Desc File    :");
	placestr(17,16,&Obptr->descfile);
	getstrfld(17,5,&Obptr->number);
	getstrfld(17,6,&Obptr->size);
	getstrfld(17,7,&Obptr->location);
	getstrfld(17,8,&Obptr->value);
	getstrfld(17,9,&Obptr->visible);
	getstrfld(17,10,&Obptr->attach);
	getstrfld(17,11,&Obptr->attachto);
	getstrfld(17,12,&Obptr->sizelim);
	getstrfld(17,13,&Obptr->name);
	getstrfld(17,14,&Obptr->character);
	getstrfld(17,15,&Obptr->solved);
	getstrfld(17,16,&Obptr->descfile);
	placestr(2,18,"Done? (y/n):");
	getstrfld(15,18,&done);
	return (strcmpi(done,"y"));
}

reset_window()
{
	window(1,1,80,25);
	textmode(C80);
	clrscr();
}

/*
** This creates a titled box by specifying the top left and bottom
** right coordinates and the title.
** Note: be sure that your coordinates do not exceed the
**       current text window, it will cause truncation.
**
**
*/

border(int a,int b,int c,int d,char *str)
{
	int i;
	for (i=2;i<c;i++) placechar(i,b,205);
	for (i=2;i<c;i++) placechar(i,d-1,205);
	for (i=2;i<d-1;i++) placechar(a,i,186);
	for (i=2;i<d-1;i++) placechar(c,i,186);
	placechar(a,b,201);
	placechar(c,b,187);
	placechar(a,d-1,200);
	placechar(c,d-1,188);
	placechar(a,b+2,204);
	placechar(c,b+2,185);
	for (i=2;i<c;i++) placechar(i,b+2,205);
	gotoxy((c-strlen(str))/2,b+1);
	cprintf("%s",str);
}

create_object()
{
	int screen_enter();
	clrscr();
	while (screen_enter(&work[objcnt]));
	putroom();
}

display_object()
{
	int screen_display();
	char roomnum[3];
	int i;
	clrscr();
	pick_from_list(&roomnum);
	for (i=0;(i<objcnt);i++)
		if (!strcmpi(roomnum,work[i].number)) break;
	clrscr();
	if (i<objcnt)
	    while (screen_display(&work[i]));
}

update_object()
{
	int screen_update();
	char roomnum[3];
	int i;
	clrscr();
	pick_from_list(&roomnum);
	for (i=0;(i<objcnt);i++)
		if (!strcmpi(roomnum,work[i].number)) break;
	clrscr();
	if (i<objcnt)
	  while (screen_update(&work[i]));
}

delete_object()
{
	char roomnum[3];
	int i;
	clrscr();
	pick_from_list(&roomnum);
	for (i=0;(i<objcnt);i++)
		if (!strcmpi(roomnum,work[i].number)) break;
	clrscr();
	if (i<objcnt)
	    if (i==(objcnt-1))
		objcnt--;
	    else
		{
		for(i=i;i+1<objcnt;i++)
		    work[i] = work[i+1];
		objcnt--;
		};
}

putroom()
{
	int i = objcnt;
	if (strcmpi(work[i].number,""))
	    {
	    objcnt++;
	    };
}

pick_from_list(char *selected)
{
	int x,y,i=0;
	clrscr();
	border(1,1,80,25,"Select From Object List");
	for (x=5;x<45;x=x+4)
		for(y=5;y<15;y++)
			if (i<objcnt)
			    placestr(x,y,work[i++].number);
	placestr(5,17,"What Object Number? :");
	getstrfld(27,17,selected);
}

/*  puts a character at coords a,b - character is passed as int */

placechar(int a,int b,int c)
{
	gotoxy(a,b);
	cprintf("%c",c);
};

placestr(int a,int b,char *str)
{
	gotoxy(a,b);
	cprintf(str);
}

getstrfld(int a,int b,char *strin)
{
	char hold[80];
	gotoxy(a,b);
	gets(hold);
	if (strcmpi(hold,""))
	    strcpy(strin,hold);
}


load_objects()
{
	FILE *in;
	char string[100];
	int i = 0;
	if ((in = fopen("OBJFILE.TXT","r")) == NULL)
		return 1;
	else
	{
		while (!feof(in))
		  {
		    fgets(string,101,in);
		    if (!feof(in))
			put_record(&string,&i);
		  };
		objcnt = i;
		fclose(in);
		return 0;
	};
}

put_record(char *string,int *i)
{
    strcpy(work[*i].name,strtok(string,";\n"));
    strcpy(work[*i].descfile,strtok(NULL,";\n"));
    strcpy(work[*i].number,strtok(NULL,";\n"));
    strcpy(work[*i].size,strtok(NULL,";\n"));
    strcpy(work[*i].location,strtok(NULL,";\n"));
    strcpy(work[*i].value,strtok(NULL,";\n"));
    strcpy(work[*i].visible,strtok(NULL,";\n"));
    strcpy(work[*i].attach,strtok(NULL,";\n"));
    strcpy(work[*i].attachto,strtok(NULL,";\n"));
    strcpy(work[*i].sizelim,strtok(NULL,";\n"));
    strcpy(work[*i].character,strtok(NULL,";\n"));
    strcpy(work[*i].solved,strtok(NULL,";\n"));
    *i = *i + 1;
}

save_objects()
{
	FILE *out;
	int i = 0;
	system("del objfile.bak");
	system("ren objfile.txt objfile.bak");
	if ((out = fopen("OBJFILE.TXT","w+")) == NULL)
		return 1;
	else
	{
		while (i<objcnt)
		  {
		    fputs(work[i].name,out);
		    fputc(';',out);
		    fputs(work[i].descfile,out);
		    fputc(';',out);
		    fputs(work[i].number,out);
		    fputc(';',out);
		    fputs(work[i].size,out);
		    fputc(';',out);
		    fputs(work[i].location,out);
		    fputc(';',out);
		    fputs(work[i].value,out);
		    fputc(';',out);
		    fputs(work[i].visible,out);
		    fputc(';',out);
		    fputs(work[i].attach,out);
		    fputc(';',out);
		    fputs(work[i].attachto,out);
		    fputc(';',out);
		    fputs(work[i].sizelim,out);
		    fputc(';',out);
		    fputs(work[i].character,out);
		    fputc(';',out);
		    fputs(work[i].solved,out);
		    fputc('\n',out);
		    i++;
		  };
		fclose(out);
		return 0;
	};
}
