
/*
** WRITROOM.C
**
** Mike O'Shea
**
** Utility used to create a room database for TEXTSHEL.
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
	char sizelim[4];
	char north[4];
	char south[4];
	char east[4];
	char west[4];
	char up[4];
	char down[4];
	char visible[2];
	char directo[4];
	char solved[2];
		} ROOMREC;

ROOMREC Rm, *Rptr;

ROOMREC work[99];

int roomcnt,curroom = 0;

int main()
{
	int menu_process();
	Rptr = &Rm;
	reset_window();
	while (menu_process());
	clrscr();
	return 0;
}

int menu_process()
{
	char done[2];
	clrscr();
	border(1,1,80,25,"Room Maintenance Menu");
	placestr(30,5,"1 ..... Create Room");
	placestr(30,7,"2 ..... Update Room");
	placestr(30,9,"3 ..... Delete Room");
	placestr(30,11,"4 ..... Display Room");
	placestr(30,13,"5 ..... Load Room File");
	placestr(30,15,"6 ..... Save Room File");
	placestr(30,17,"Q ..... Quit");
	placestr(30,19,"Option? :");
	getstrfld(40,19,&done);
	switch (atoi(done))
		{ case  1 : create_room();
			    break;
		  case  2 : update_room();
			    break;
		  case  3 : delete_room();
			    break;
		  case  4 : display_room();
			    break;
		  case  5 : load_rooms();
			    break;
		  case  6 : save_rooms();
			    break;
		  default : return(0);
		};
	return(1);
}

int screen_enter()
{
	char done[2];
	border(1,1,80,25,"Room Entry Screen");
	placestr(2,5,"Room Number:");
	getstrfld(16,5,&Rm.number);
	placestr(2,6,"Room Size  :");
	getstrfld(16,6,&Rm.sizelim);
	placestr(2,7,"North Room :");
	getstrfld(16,7,&Rm.north);
	placestr(2,8,"South Room :");
	getstrfld(16,8,&Rm.south);
	placestr(2,9,"East Room  :");
	getstrfld(16,9,&Rm.east);
	placestr(2,10,"West Room  :");
	getstrfld(16,10,&Rm.west);
	placestr(2,11,"Up Room    :");
	getstrfld(16,11,&Rm.up);
	placestr(2,12,"Down Room  :");
	getstrfld(16,12,&Rm.down);
	placestr(2,13,"Room Name  :");
	getstrfld(16,13,&Rm.name);
	placestr(2,14,"Visible    :");
	getstrfld(16,14,&Rm.visible);
	placestr(2,15,"Direct to  :");
	getstrfld(16,15,&Rm.directo);
	placestr(2,16,"Solved     :");
	getstrfld(16,16,&Rm.solved);
	placestr(2,17,"Desc File  :");
	getstrfld(16,17,&Rm.descfile);
	placestr(2,19,"Information Correct? (y/n):");
	getstrfld(30,19,&done);
	return (strcmpi(done,"y"));
}

int screen_display()
{
	char done[2];
	border(1,1,80,25,"Room Display Screen");
	placestr(2,5,"Room Number:");
	placestr(16,5,Rm.number);
	placestr(2,6,"Room Size  :");
	placestr(16,6,Rm.sizelim);
	placestr(2,7,"North Room :");
	placestr(16,7,Rm.north);
	placestr(2,8,"South Room :");
	placestr(16,8,Rm.south);
	placestr(2,9,"East Room  :");
	placestr(16,9,Rm.east);
	placestr(2,10,"West Room  :");
	placestr(16,10,Rm.west);
	placestr(2,11,"Up Room    :");
	placestr(16,11,Rm.up);
	placestr(2,12,"Down Room  :");
	placestr(16,12,Rm.down);
	placestr(2,13,"Room Name  :");
	placestr(16,13,Rm.name);
	placestr(2,14,"Visibile   :");
	placestr(16,14,Rm.visible);
	placestr(2,15,"Direct to  :");
	placestr(16,15,Rm.directo);
	placestr(2,16,"Solved     :");
	placestr(16,16,Rm.solved);
	placestr(2,17,"Desc File  :");
	placestr(16,17,Rm.descfile);
	placestr(2,19,"Done? (y/n):");
	getstrfld(15,19,&done);
	return (strcmpi(done,"y"));
}

int screen_update()
{
	char done[2];
	border(1,1,80,25,"Room Update Screen");
	placestr(2,5,"Room Number:");
	placestr(16,5,Rm.number);
	placestr(2,6,"Room Size  :");
	placestr(16,6,Rm.sizelim);
	placestr(2,7,"North Room :");
	placestr(16,7,Rm.north);
	placestr(2,8,"South Room :");
	placestr(16,8,Rm.south);
	placestr(2,9,"East Room  :");
	placestr(16,9,Rm.east);
	placestr(2,10,"West Room  :");
	placestr(16,10,Rm.west);
	placestr(2,11,"Up Room    :");
	placestr(16,11,Rm.up);
	placestr(2,12,"Down Room  :");
	placestr(16,12,Rm.down);
	placestr(2,13,"Room Name  :");
	placestr(16,13,Rm.name);
	placestr(2,14,"Visibile   :");
	placestr(16,14,Rm.visible);
	placestr(2,15,"Direct to  :");
	placestr(16,15,Rm.directo);
	placestr(2,16,"Solved     :");
	placestr(16,16,Rm.solved);
	placestr(2,17,"Desc File  :");
	placestr(16,17,Rm.descfile);
	getstrfld(16,5,&Rm.number);
	getstrfld(16,6,&Rm.sizelim);
	getstrfld(16,7,&Rm.north);
	getstrfld(16,8,&Rm.south);
	getstrfld(16,9,&Rm.east);
	getstrfld(16,10,&Rm.west);
	getstrfld(16,11,&Rm.up);
	getstrfld(16,12,&Rm.down);
	getstrfld(16,13,&Rm.name);
	getstrfld(16,14,&Rm.visible);
	getstrfld(16,15,&Rm.directo);
	getstrfld(16,16,&Rm.solved);
	getstrfld(16,17,&Rm.descfile);
	placestr(2,19,"Done? (y/n):");
	getstrfld(15,19,&done);
	return (strcmpi(done,"y"));
}

reset_window()
{
	window(1,1,80,25);
	textmode(C80);
	clrscr();
}

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

create_room()
{
	int screen_enter();
	clrscr();
	while (screen_enter());
	memcpy(&work[roomcnt],&Rm,sizeof(ROOMREC));
	putroom();
}

display_room()
{
	int screen_display();
	char roomnum[3];
	int i;
	clrscr();
	pick_from_list(&roomnum);
	for (i=0;(i<roomcnt);i++)
		if (!strcmpi(roomnum,work[i].number)) break;
	clrscr();
	if (i<roomcnt)
	    {
	    memcpy(&Rm,&work[i],sizeof(ROOMREC));
	    while (screen_display());
	    };
}

update_room()
{
	int screen_update();
	char roomnum[3];
	int i;
	clrscr();
	pick_from_list(&roomnum);
	for (i=0;(i<roomcnt);i++)
		if (!strcmpi(roomnum,work[i].number)) break;
	clrscr();
	if (i<roomcnt)
	    {
	    memcpy(&Rm,&work[i],sizeof(ROOMREC));
	    while (screen_update());
	    memcpy(&work[i],&Rm,sizeof(ROOMREC));
	    };
}

delete_room()
{
	char roomnum[3];
	int i;
	clrscr();
	pick_from_list(&roomnum);
	for (i=0;(i<roomcnt);i++)
		if (!strcmpi(roomnum,work[i].number)) break;
	clrscr();
	if (i<roomcnt)
	    if (i==(roomcnt-1))
		roomcnt--;
	    else
		{
		for(i=i;i+1<roomcnt;i++)
		    work[i] = work[i+1];
		roomcnt--;
		};
}

putroom()
{
	int i = roomcnt;
	if (strcmpi(work[i].number,""))
	    {
	    roomcnt++;
	    };
}

pick_from_list(char *selected)
{
	int x,y,i=0;
	clrscr();
	border(1,1,80,25,"Select From Room List");
	for (x=5;x<45;x=x+4)
		for(y=5;y<15;y++)
			if (i<roomcnt)
			    placestr(x,y,work[i++].number);
	placestr(5,17,"What Room Number? :");
	getstrfld(25,17,selected);
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

load_rooms()
{
	FILE *in;
	char string[100];
	int i = 0;
	if ((in = fopen("ROOMFILE.TXT","r")) == NULL)
		return 1;
	else
	{
		while (!feof(in))
		  {
		    fgets(string,101,in);
		    if (!feof(in))
			put_record(&string,&i);
		  };
		roomcnt = i;
		fclose(in);
		return 0;
	};
}

put_record(char *string,int *i)
{
	strcpy(work[*i].name,strtok(string,";\n"));
	strcpy(work[*i].descfile,strtok(NULL,";\n"));
	strcpy(work[*i].number,strtok(NULL,";\n"));
	strcpy(work[*i].sizelim,strtok(NULL,";\n"));
	strcpy(work[*i].north,strtok(NULL,";\n"));
	strcpy(work[*i].south,strtok(NULL,";\n"));
	strcpy(work[*i].east,strtok(NULL,";\n"));
	strcpy(work[*i].west,strtok(NULL,";\n"));
	strcpy(work[*i].up,strtok(NULL,";\n"));
	strcpy(work[*i].down,strtok(NULL,";\n"));
	strcpy(work[*i].visible,strtok(NULL,";\n"));
	strcpy(work[*i].directo,strtok(NULL,";\n"));
	strcpy(work[*i].solved,strtok(NULL,";\n"));
	*i = *i + 1;
}

save_rooms()
{
	FILE *out;
	int i = 0;
	system("del roomfile.bak");
	system("ren roomfile.txt roomfile.bak");
	if ((out = fopen("ROOMFILE.TXT","w+")) == NULL)
		return 1;
	else
	{
		while (i<roomcnt)
		  {
		    fputs(work[i].name,out);
		    fputc(';',out);
		    fputs(work[i].descfile,out);
		    fputc(';',out);
		    fputs(work[i].number,out);
		    fputc(';',out);
		    fputs(work[i].sizelim,out);
		    fputc(';',out);
		    fputs(work[i].north,out);
		    fputc(';',out);
		    fputs(work[i].south,out);
		    fputc(';',out);
		    fputs(work[i].east,out);
		    fputc(';',out);
		    fputs(work[i].west,out);
		    fputc(';',out);
		    fputs(work[i].up,out);
		    fputc(';',out);
		    fputs(work[i].down,out);
		    fputc(';',out);
		    fputs(work[i].visible,out);
		    fputc(';',out);
		    fputs(work[i].directo,out);
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