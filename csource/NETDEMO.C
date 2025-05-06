/*
** NETDEMO.C
**
** INPUT
**  None
**
** OUTPUT
**  Graphic demo of network and tutorial on network command usage
**
**
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <graphics.h>
#include <netbios.h>
#include <dos.h>


#define ENTER_KEY 0x0d
#define BACKSPACE 0x08

int main(int,char *[]);
void good_job(void);
void demo(void);
void c_break(void);
void draw_network();
void draw_server(int,int,char *,int);
void draw_client(int,int,char *,int);
void draw_printer(int,int,char *,int);
void draw_netbar(int,int,int,char *);
void put_piece(int,int,void *);
void remove_piece(int,int,void *);
void message_1(void);
void message_2(void);
void message_3(void);
void message_4(void);
void message_5(void);
void message_6(void);
void message_7(void);
void message_8(void);
void message_9(void);
void message_10(void);
void use_connect(void);
int try_use_connect(void);
void show_connections(void);
int try_use_disconnect(void);
void use_disconnect(void);
void printer_connect(void);
int try_printer_connect(void);
void after_use_try(int);
void after_use_discon(int);
void NetBios(NCB far *);

NCB test_ncb;
void * image_buffer;
void interrupt (*int_5C_vector)(void);
int net_installed = 1;

int main(int argc,char *argv[])
{
	int test,g_driver,g_mode,g_error;
	int_5C_vector = getvect(0x5C);
	if (int_5C_vector == (void far *) NULL)
	{
		printf("Network not active, demo will simulate network.\n");
		delay(2000);
		net_installed = 0;
	}

	memset(&test_ncb,0,sizeof(NCB));
	test_ncb.NCB_COMMAND = 0x7F;
	NetBios(&test_ncb);
	if (test_ncb.NCB_RETCODE != 03)
	{
		printf("Network not active, demo will simulate network.\n");
		delay(2000);
		net_installed = 0;
	}
	detectgraph(&g_driver,&g_mode);
	g_mode = 1; /* Override to EGA */
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(triplex_font)<0) exit(1);
	initgraph(&g_driver,&g_mode,"");
	delay(0);
	demo();
	if (argc > 1)
		if (!strcmpi(argv[1],"/t"))
			test = 1;
}

void demo(void)
{
	draw_network();
	message_1();
	c_break();
	message_2();
	c_break();
	message_3();
	c_break();
	message_4();
	c_break();
	draw_server(0,0,"SERVER01",1);
	draw_client(250,0,"CLIENT37",1);
	use_connect();
	delay(3000);
	message_5();
	c_break();
	while (try_use_connect());
	draw_network();
	message_6();
	c_break();
	draw_server(0,0,"SERVER01",1);
	draw_client(250,0,"CLIENT37",1);
	use_disconnect();
	delay(3000);
	message_7();
	c_break();
	while (try_use_disconnect());
	draw_network();
	message_8();
	c_break();
	draw_client(250,0,"CLIENT37",1);
	draw_server(250,180,"SERVER03",1);
	draw_printer(0,180,"LASERP02",1);
	printer_connect();
	message_9();
	c_break();
	while (try_printer_connect());
	message_10();
	c_break();
	closegraph();
}

void draw_network()
{
	draw_server(0,0,"SERVER01",0);
	setcolor(LIGHTMAGENTA);
	outtextxy(85,10,"OFFERINGS:");
	outtextxy(85,22,"\\WORD");
	outtextxy(85,31,"\\WG3333");
	draw_client(250,0,"CLIENT37",0);
	draw_printer(0,180,"LASERP02",0);
	draw_server(250,180,"SERVER03",0);
	setcolor(LIGHTMAGENTA);
	outtextxy(335,190,"OFFERINGS:");
	outtextxy(335,202,"\\LASERP02");
	draw_netbar(5,120,350,"Ethernet LAN (Corporate Technology)");
	setcolor(LIGHTRED);
	line(45,140,45,208);
	line(295,140,295,193);
	line(45,120,45,66);
	line(295,120,295,66);
}

void draw_server(int x, int y,char *servname,int highlight)
{
	setcolor(LIGHTGREEN);
	settextstyle(0,0,1);
	setlinestyle(SOLID_LINE,NULL,NORM_WIDTH);
	rectangle(x+23,y+15,x+67,y+40);
	rectangle(x+15,y+40,x+75,y+55);
	outtextxy(x+17,y+46,"Server");
	outtextxy(x+17,y+57,servname);
	if (highlight)
	{
		setfillstyle(SOLID_FILL,LIGHTCYAN);
		floodfill(x+24,y+16,LIGHTGREEN);
	}
	else
	{
		setfillstyle(SOLID_FILL,BLACK);
		floodfill(x+24,y+16,LIGHTGREEN);
	}
	arc(x+29,y+20,90,180,3);
	arc(x+29,y+35,180,270,3);
	arc(x+61,y+20,0,90,3);
	arc(x+61,y+35,270,360,3);
	line(x+(29-3),y+20,x+(29-3),y+35);
	line(x+(61+3),y+20,x+(61+3),y+35);
	line(x+29,y+(21-3),x+61,y+(21-3));
	line(x+29,y+(34+3),x+61,y+(34+3));
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	floodfill(x+32,y+30,LIGHTGREEN);
}

void draw_client(int x, int y,char *clntname,int highlight)
{
	setcolor(YELLOW);
	settextstyle(0,0,1);
	setlinestyle(SOLID_LINE,NULL,NORM_WIDTH);
	rectangle(x+23,y+15,x+67,y+40);
	rectangle(x+15,y+40,x+75,y+55);
	outtextxy(x+17,y+46,"User ");
	if (highlight)
	{
		setfillstyle(SOLID_FILL,LIGHTCYAN);
		floodfill(x+24,y+16,YELLOW);
	}
	else
	{
		setfillstyle(SOLID_FILL,BLACK);
		floodfill(x+24,y+16,YELLOW);
	}
	outtextxy(x+17,y+57,clntname);
	arc(x+29,y+20,90,180,3);
	arc(x+29,y+35,180,270,3);
	arc(x+61,y+20,0,90,3);
	arc(x+61,y+35,270,360,3);
	line(x+(29-3),y+20,x+(29-3),y+35);
	line(x+(61+3),y+20,x+(61+3),y+35);
	line(x+29,y+(21-3),x+61,y+(21-3));
	line(x+29,y+(34+3),x+61,y+(34+3));
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	floodfill(x+32,y+30,YELLOW);
}

void draw_printer(int x, int y,char *prntname,int highlight)
{
	setcolor(LIGHTCYAN);
	settextstyle(0,0,1);
	setlinestyle(SOLID_LINE,NULL,NORM_WIDTH);
	rectangle(x+23,y+30,x+67,y+40);
	if (highlight)
	{
		setfillstyle(SOLID_FILL,LIGHTCYAN);
		floodfill(x+24,y+31,LIGHTCYAN);
	}
	else
	{
		setfillstyle(SOLID_FILL,BLACK);
		floodfill(x+24,y+31,LIGHTCYAN);
	}
	rectangle(x+15,y+40,x+75,y+55);
	outtextxy(x+17,y+46,"Printer ");
	outtextxy(x+17,y+57,prntname);
}

void draw_netbar(int x,int y,int length,char *text)
{
	setcolor(LIGHTRED);
	settextstyle(0,0,1);
	ellipse(x+5,y+10,0,360,5,10);
	ellipse(x+5+length,y+10,270,90,5,10);
	line(x+5,y+(10-10),x+5+length,y+(10-10));
	line(x+5,y+(10+10),x+5+length,y+(10+10));
	setfillstyle(CLOSE_DOT_FILL,LIGHTRED);
	floodfill(x+15,y+10,LIGHTRED);
	setcolor(WHITE);
	outtextxy(x+17,y+10,text);
}

void message_1(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"Each machine on the   ");
	outtextxy(0,29,"network is called a   ");
	outtextxy(0,38,"\"node\".               ");
	outtextxy(0,47,"                      ");
	outtextxy(0,56,"The server nodes are  ");
	outtextxy(0,65,"of two types:         ");
	outtextxy(0,74,"file servers and print");
	outtextxy(0,83,"servers.              ");
	outtextxy(0,92,"A server provides     ");
	outtextxy(0,101,"offerings to the users.");
	outtextxy(0,110,"Print servers offer   ");
	outtextxy(0,119,"output devices such as");
	outtextxy(0,128,"printers and plotters.");
	outtextxy(0,137,"File servers provide  ");
	outtextxy(0,146,"file directories from");
	outtextxy(0,155,"which users may access");
	outtextxy(0,164,"files.                 ");
	outtextxy(0,173,"Every offering has a  ");
	outtextxy(0,182,"name.  The diagram    ");
	outtextxy(0,191,"Shows some of our     ");
	outtextxy(0,200,"Servers and their     ");
	outtextxy(0,209,"offering names.       ");
	outtextxy(0,218,"Although the print    ");
	outtextxy(0,227,"server offering looks ");
	outtextxy(0,236,"like a directory name,");
	outtextxy(0,245,"the name is the same  ");
	outtextxy(0,254,"as the network name for");
	outtextxy(0,263,"the output device:    ");
	outtextxy(0,272,"LASERP02.             ");
}

void message_2(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"How does a user get   ");
	outtextxy(0,29,"access to something on");
	outtextxy(0,38,"the network?          ");
	outtextxy(0,47,"                      ");
	outtextxy(0,56,"The following session ");
	outtextxy(0,65,"will give you training");
	outtextxy(0,74,"using the \"USE\"      ");
	outtextxy(0,83,"command to connect and");
	outtextxy(0,92,"disconnect from a      ");
	outtextxy(0,101,"server offering.");
}

void message_3(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"Like most PC users,   ");
	outtextxy(0,29,"you have a hard disk  ");
	outtextxy(0,38,"drive and one or more   ");
	outtextxy(0,47,"floppy disk drives.   ");
	outtextxy(0,56,"These drives are      ");
	outtextxy(0,65,"designated by a letter.");
	outtextxy(0,74,"                      ");
	outtextxy(0,83,"Example -             ");
	outtextxy(0,92,"Your hard disk drive  ");
	outtextxy(0,101,"is designated by C:    ");
	outtextxy(0,110,"Your floppy drive is  ");
	outtextxy(0,119,"designated by drive A:");
	outtextxy(0,128,"                      ");
	outtextxy(0,137,"DOS assigns letters to");
	outtextxy(0,146,"your disk drives     ");
	outtextxy(0,155,"starting at A: until  ");
	outtextxy(0,164,"there are no more      ");
	outtextxy(0,173,"drives to assign.     ");
	outtextxy(0,182,"                      ");
	outtextxy(0,191,"The remaining letters ");
	outtextxy(0,200,"up to Z: are unused by");
	outtextxy(0,209,"DOS.  These letters   ");
	outtextxy(0,218,"may be used by you to ");
	outtextxy(0,227,"connect to a file     ");
	outtextxy(0,236,"server.               ");
}

void message_4(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"How do you know which");
	outtextxy(0,29,"letters are unused?   ");
	outtextxy(0,38,"Well there is no easy  ");
	outtextxy(0,47,"way to find out, as   ");
	outtextxy(0,56,"your hard disk may be ");
	outtextxy(0,65,"partitioned.  However,");
	outtextxy(0,74,"it's safe to say that ");
	outtextxy(0,83,"letters J: to Z: are  ");
	outtextxy(0,92,"free for everyone.    ");
	outtextxy(0,101,"                      ");
	outtextxy(0,110,"You also have at least");
	outtextxy(0,119,"two parallel ports for");
	outtextxy(0,128,"hooking printers to.  ");
	outtextxy(0,137,"                      ");
	outtextxy(0,146,"These are designated ");
	outtextxy(0,155,"by LPT1: and LPT2:    ");
	outtextxy(0,164,"respectively.          ");
	outtextxy(0,173,"                      ");
	outtextxy(0,182,"LPT1: and LPT2: are   ");
	outtextxy(0,191,"used to hook to print ");
	outtextxy(0,200,"server offerings.     ");
	outtextxy(0,209,"                      ");
	outtextxy(0,218,"The following examples");
	outtextxy(0,227,"should help....       ");
}

void message_5(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"If no errors occured ");
	outtextxy(0,29,"the \"C:\\>\" prompt  ");
	outtextxy(0,38,"will return.           ");
	outtextxy(0,47,"                      ");
	outtextxy(0,56,"You may now access the");
	outtextxy(0,65,"J: drive as if it were");
	outtextxy(0,74,"any other disk drive  ");
	outtextxy(0,83,"on your PC.           ");
	outtextxy(0,92,"You may switch to it. ");
	outtextxy(0,101,"You may do a directory,");
	outtextxy(0,110,"copy files to and from");
	outtextxy(0,119,"drive J: etc.         ");
	outtextxy(0,128,"                      ");
	outtextxy(0,137,"If the server offering");
	outtextxy(0,146,"is read only, you will");
	outtextxy(0,155,"not be able to copy    ");
	outtextxy(0,164,"files to J: or delete ");
	outtextxy(0,173,"files on J:           ");
	outtextxy(0,182,"                      ");
	outtextxy(0,191,"Note: The space after ");
	outtextxy(0,200,"J: in the USE example.");
	outtextxy(0,209,"This is required.  Also");
	outtextxy(0,218,"server names always   ");
	outtextxy(0,227,"are preceded by \"\\\\\".");
}

void message_6(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"Any drive letter from ");
	outtextxy(0,29,"J: to Z: can be used. ");
	outtextxy(0,38,"Also, you may connect  ");
	outtextxy(0,47,"more than one drive   ");
	outtextxy(0,56,"letter.  However you  ");
	outtextxy(0,65,"must disconnect a used");
	outtextxy(0,74,"drive letter before   ");
	outtextxy(0,83,"reconnecting it.      ");
	outtextxy(0,92,"                      ");
	outtextxy(0,101,"What if you no longer  ");
	outtextxy(0,110,"need to have drive J: ");
	outtextxy(0,119,"connected?            ");
	outtextxy(0,128,"                      ");
	outtextxy(0,137,"The next example will ");
	outtextxy(0,146,"show how to disconnect");
	outtextxy(0,155,"a connected drive     ");
	outtextxy(0,164,"letter.  Another way  ");
	outtextxy(0,173,"to refer to connected ");
	outtextxy(0,182,"drive letters is,     ");
	outtextxy(0,191,"\"logical drive\".    ");
}

void message_7(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"The disconnect has ");
	outtextxy(0,29,"occurred.             ");
	outtextxy(0,38,"                      ");
	outtextxy(0,47,"If you forgot which   ");
	outtextxy(0,56,"drive letter is       ");
	outtextxy(0,65,"connected to what,    ");
	outtextxy(0,74,"typing \"USE\" by itself");
	outtextxy(0,83,"will produce a list of ");
	outtextxy(0,92,"connected drives and  ");
	outtextxy(0,101,"what they are connected");
	outtextxy(0,110,"to.                   ");
	outtextxy(0,119,"                      ");
	outtextxy(0,128,"                      ");
}

void message_8(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"Now that you are   ");
	outtextxy(0,29,"familiar with the USE ");
	outtextxy(0,38,"command for connecting");
	outtextxy(0,47,"to file servers, let's");
	outtextxy(0,56,"move on to connecting ");
	outtextxy(0,65,"network printer servers");
	outtextxy(0,74,"to our LPT1: and LPT2:");
	outtextxy(0,83,"port identifiers...   ");
}

void message_9(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"Connecting to a    ");
	outtextxy(0,29,"printer is almost     ");
	outtextxy(0,38,"exactly the same as   ");
	outtextxy(0,47,"connecting to a file  ");
	outtextxy(0,56,"server offering.      ");
	outtextxy(0,65,"                      ");
	outtextxy(0,74,"The difference is that  ");
	outtextxy(0,83,"you use LPT1:, LPT2:,  ");
	outtextxy(0,92,"and LPT3: instead of  ");
	outtextxy(0,101,"drive letters in the   ");
	outtextxy(0,110,"USE command.          ");
	outtextxy(0,119,"                      ");
}

void message_10(void)
{
	/* add 9 to y for additional lines */

	setcolor(CYAN);
	settextstyle(0,0,1);
	setviewport(455,11,639,299,1);
	clearviewport();
	outtextxy(0,20,"The way to disconnect ");
	outtextxy(0,29,"is the same as with   ");
	outtextxy(0,38,"a file server:         ");
	outtextxy(0,47,"USE LPT1: /d          ");
	outtextxy(0,56,"                      ");
	outtextxy(0,65,"The only question left");
	outtextxy(0,74,"is:                   ");
	outtextxy(0,83,"How do I know what    ");
	outtextxy(0,92,"servers are out there ");
	outtextxy(0,101,"and what do they offer?");
	outtextxy(0,110,"                      ");
	outtextxy(0,119,"The next portion of   ");
	outtextxy(0,128,"the tutorial will     ");
	outtextxy(0,137,"cover a utility that  ");
	outtextxy(0,146,"allows you to log on  ");
	outtextxy(0,155,"to a network server   ");
	outtextxy(0,164,"remotely and list it's");
	outtextxy(0,173,"available offerings.  ");
	outtextxy(0,182,"This module is done.  ");
	outtextxy(0,191,"Thank you!.           ");
}


void use_connect(void)
{
	setcolor(YELLOW);
	setviewport(0,280,454,349,1);
	clearviewport();
	outtextxy(0,0,"EXAMPLE 1 -");
	outtextxy(0,9,"Connect the \\WORD offering of SERVER01 to drive J:");
	delay(1500);
	setcolor(WHITE);
	outtextxy(0,25,"C:\\>USE J: \\\\SERVER01\\WORD");
	delay(2000);
	outtextxy(0,34,"C:\\>_");
}

int try_use_connect(void)
{
	static int trys = 0;
	char input[80];
	int i = 0;
	setcolor(YELLOW);
	setviewport(0,280,454,349,1);
	clearviewport();
	outtextxy(0,0,"Your turn to try EXERCISE 1 -");
	outtextxy(0,9,"Connect drive J: to the \\WORD offering of SERVER01.");
	setcolor(WHITE);
	outtextxy(0,25,"C:\\> ");
	fflush(stdin);
	input[i] = getch();
	while (input[i] != ENTER_KEY)
	{
		if (input[i] == BACKSPACE)
		{
			setcolor(BLACK);
			input[i] = NULL;
			outtextxy(33,25,input);
			input[i-1] = ' ';
			input[i] = NULL;
			setcolor(WHITE);
			outtextxy(33,25,input);
			i--;
		}
		else
		{
			input[i+1] = NULL;
			outtextxy(33,25,input);
			i++;
		}
		input[i] = getch();
	}
	input[i] = NULL;

	if (!strcmpi(input,"use j: \\\\server01\\word"))
	{
		delay(1000);
		outtextxy(0,34,"Good Job!");
		good_job();
		delay(500);
		clearviewport();
		after_use_try(1);
		return 0;
	}
	else
	{
		outtextxy(0,34,"C:\\>syntax error");
		trys++;

		if (trys == 2)
		{
			outtextxy(0,43,"try- USE <drive letter>: \\\\<servername>\\<offer>");
			delay(4000);
		}
		else
			if (trys == 3)
			{
				outtextxy(0,43,"ANSWER-  USE J: \\\\server01\\WORD");
				outtextxy(0,52,"You can always try this again!");
				good_job();
				delay(3000);
				clearviewport();
				after_use_try(0);
				return 0;
			}

		delay(1000);
		return 1;
	}
}


int try_use_disconnect(void)
{
	static int trys = 0;
	char input[80];
	int i = 0;
	setcolor(YELLOW);
	setviewport(0,280,454,349,1);
	clearviewport();
	outtextxy(0,0,"Your turn to try EXERCISE 2 -");
	outtextxy(0,9,"Disconnect drive J: from server SERVER01.");
	setcolor(WHITE);
	outtextxy(0,25,"C:\\> ");
	fflush(stdin);
	input[i] = getch();
	while (input[i] != ENTER_KEY)
	{
		if (input[i] == BACKSPACE)
		{
			setcolor(BLACK);
			input[i] = NULL;
			outtextxy(33,25,input);
			input[i-1] = ' ';
			input[i] = NULL;
			setcolor(WHITE);
			outtextxy(33,25,input);
			i--;
		}
		else
		{
			input[i+1] = NULL;
			outtextxy(33,25,input);
			i++;
		}
		input[i] = getch();
	}
	input[i] = NULL;

	if (!strcmpi(input,"use j: /d"))
	{
		delay(1000);
		outtextxy(0,34,"C:\\>drive disconnected");
		outtextxy(0,43,"Good Job!");
		good_job();
		delay(1500);
		clearviewport();
		after_use_discon(1);
		return 0;
	}
	else
	{
		outtextxy(0,34,"C:\\>syntax error");
		trys++;

		if (trys == 2)
		{
			outtextxy(0,43,"try-  USE <drive letter>: /d");
			delay(4000);
		}
		else
			if (trys == 3)
			{
				outtextxy(0,43,"ANSWER-  USE J: /D");
				outtextxy(0,52,"You can always try this again!");
				good_job();
				delay(3000);
				clearviewport();
				after_use_discon(0);
				return 0;
			}

		delay(1000);
		return 1;
	}
}

int try_printer_connect(void)
{
	static int trys = 0;
	char input[80];
	int i = 0;
	setcolor(YELLOW);
	setviewport(0,280,454,349,1);
	clearviewport();
	outtextxy(0,0,"Your turn to try EXERCISE 3 -");
	outtextxy(0,9,"Connect LPT1: to printer LASERP02 offered by SERVER03.");
	setcolor(WHITE);
	outtextxy(0,25,"C:\\> ");
	fflush(stdin);
	input[i] = getch();
	while (input[i] != ENTER_KEY)
	{
		if (input[i] == BACKSPACE)
		{
			setcolor(BLACK);
			input[i] = NULL;
			outtextxy(33,25,input);
			input[i-1] = ' ';
			input[i] = NULL;
			setcolor(WHITE);
			outtextxy(33,25,input);
			i--;
		}
		else
		{
			input[i+1] = NULL;
			outtextxy(33,25,input);
			i++;
		}
		input[i] = getch();
	}
	input[i] = NULL;

	if (!strcmpi(input,"use lpt1: \\\\server03\\laserp02"))
	{
		delay(1000);
		outtextxy(0,34,"Good Job!");
		good_job();
		delay(500);
		clearviewport();
		return 0;
	}
	else
	{
		outtextxy(0,34,"C:\\>syntax error");
		trys++;

		if (trys == 2)
		{
			outtextxy(0,43,"try- USE <parallel port>: \\\\<servername>\\<printeroffer>");
			delay(4000);
		}
		else
			if (trys == 3)
			{
				outtextxy(0,43,"ANSWER-  USE LPT1: \\\\SERVER03\\LASERP02");
				outtextxy(0,52,"You can always try this again!");
				good_job();
				delay(3000);
				clearviewport();
				return 0;
			}

		delay(1000);
		return 1;
	}
}


void use_disconnect(void)
{
	setcolor(YELLOW);
	setviewport(0,280,454,349,1);
	clearviewport();
	outtextxy(0,0,"EXAMPLE 2 -");
	outtextxy(0,9,"Disconnect the logical drive J: from the file server.");
	delay(1500);
	setcolor(WHITE);
	outtextxy(0,25,"C:\\>USE J: /D");
	delay(2000);
	outtextxy(0,34,"C:\\>drive disconnected");
}

void printer_connect(void)
{
	setcolor(YELLOW);
	setviewport(0,280,454,349,1);
	clearviewport();
	outtextxy(0,0,"EXAMPLE 3 -");
	outtextxy(0,9,"Connect LPT1: to printer LASERP02 offered by SERVER03.");
	delay(1500);
	setcolor(WHITE);
	outtextxy(0,25,"C:\\>USE LPT1: \\\\SERVER03\\LASERP02");
	delay(2000);
	outtextxy(0,34,"C:\\>_");
}


void put_piece(int x,int y,void *buffer)
{
	int adjx,adjy;
	adjx = 90 + ((x-1)*45);
	adjy = 33 + ((y-1)*35);
	putimage(adjx,adjy,buffer,0);
}

void remove_piece(int x,int y,void *buffer)
{
	int adjx,adjy;
	adjx = 90 + ((x-1)*45);
	adjy = 33 + ((y-1)*35);
	putimage(adjx,adjy,buffer,1);
}

void show_connections(void)
{
	int mode;
	mode = getgraphmode();
	restorecrtmode();
	printf("\nThese are your current connections:\n");
	system("use");
	printf("\nHit a key to resume tutorial\n");
	getch();
	setgraphmode(mode);
}

void after_use_try(int ok)
{
	int mode;
	mode = getgraphmode();
	restorecrtmode();
	if (ok)
	{
		if (net_installed)
		{
			system("use j: \\\\server01\\word");
		}
		else
		{
			printf("Simulated network connection...\n");
		}
		printf("\nNow that you have made the connection:\n\n");
	}
	else
	{
		printf("I'll make the connection to J: \\\\server01\\word\n");
		printf("in order to continue..\n\n");
		printf("C:\\>USE J: \\\\SERVER01\\WORD\n");
		if (net_installed)
		{
			system("use j: \\\\server01\\word");
		}
		else
		{
			printf("simulated network connection...\n");
		}

		printf("\n");
	}

	printf("Note the new connection to drive j: in the connection list\n\n");
	printf("I will type the USE command by itself to get this list\n");
	printf("C:\\>USE\n\n");
	if (net_installed)
	{
		system("use");
	}
	else
	{
		printf("        Local   Network\n");
		printf("Status  Device  Name\n");
		printf("--------------------------------------------\n");
		printf("         LPT1   \\\\SERVER03\\LASERP02\n");
		printf("         J      \\\\SERVER01\\WORD\n");
	}
	delay(13000);
	system("cls");
	printf("You may display a directory of drive j: \n");
	delay(2000);
	if (net_installed)
	{
		printf("C:\\>dir j:/w\n");
		system("dir j:/w");
	}
	else
	{
		system("dir c:/w");
	}
	printf("\nFor all purposes, you may use drive j: as if it were\n");
	printf("physically connected to your machine.\n\n");
	printf("Hit a key to continue with tutorial.\n");
	fflush(stdin);
	getch();
	setgraphmode(mode);
}

void after_use_discon(int ok)
{
	int mode;
	mode = getgraphmode();
	restorecrtmode();
	if (ok)
	{
		if (net_installed)
		{
			system("use j: /d");
		}
		else
		{
			printf("Simulated network connection...\n");
		}
		printf("\nNow that you have disconnected drive j:\n\n");
	}
	else
	{
		printf("I'll disconnect j:\n");
		printf("in order to continue..\n\n");
		printf("C:\\>USE J: /d\n");
		if (net_installed)
		{
			system("use j: /d");
		}
		else
		{
			printf("Simulated network connection...\n");
		}
		printf("\n");
	}

	printf("Note the absence of drive j: in the connection list\n\n");
	printf("I will type the USE command by itself to get this list\n");
	printf("C:\\>USE\n\n");
	if (net_installed)
	{
		system("use");
	}
	else
	{
		printf("        Local   Network\n");
		printf("Status  Device  Name\n");
		printf("--------------------------------------------\n");
		printf("         LPT1   \\\\SERVER03\\LASERP02\n");
	}
	delay(13000);
	system("cls");
	printf("Your current drive must be something other than the drive\n");
	printf("you are disconnecting.  In this example, our current drive is\n");
	printf("the C: drive.\n\nAttempting to disconnect from J: while it is the current\n");
	printf("drive, will result in an error.\n\n");
	printf("Hit a key to continue with tutorial.\n");
	fflush(stdin);
	getch();
	setgraphmode(mode);
}

void good_job(void)
{
	sound(1100);
	delay(20);
	nosound();
	sound(1600);
	delay(20);
	nosound();
}

void c_break(void)
{
	char string[80];
	setcolor(LIGHTGREEN);
	setviewport(455,300,639,349,1);
	sound(1300);
	delay(20);
	nosound();
	outtextxy(0,0,"Hit <ENTER> key to");
	outtextxy(0,9,"continue...");
	gets(string);
	clearviewport();
	setviewport(0,0,454,300,1);
}

void NetBios(NCB far *ncb_ptr)
{
	ncb_ptr->NCB_CMD_CPLT = 0xFF;
	_ES = FP_SEG(ncb_ptr);
	_BX = FP_OFF(ncb_ptr);
	_AX = 0x0100;
	geninterrupt(0x5C);
}



