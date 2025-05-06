
/*
** KNIGHTDE.C
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>


int main(void);
void setup(void);
void draw_board(void);
void draw_knight(void);
void draw_queen(void);
void save_knight(void);
void save_queen(void);
void put_piece(int,int,void *);
void remove_piece(int,int,void *);
void leave_number(int,int,int);
void str_int(char [],int);
void play_moves(int *,int);


void * knight_buffer;
void * queen_buffer;

int play[128] = {8,8,6,7,8,6,7,8,5,7,3,8,1,7,2,5,1,3,
			  2,1,4,2,6,1,8,2,7,4,6,2,8,1,7,3,8,5,
			  7,7,5,8,6,6,8,7,6,8,7,6,8,4,6,5,4,6,
			  5,4,7,5,8,3,7,1,6,3,5,1,7,2,6,4,5,2,
			  3,1,1,2,3,3,4,1,5,3,4,5,3,7,1,8,2,6,
			  1,4,2,2,3,4,5,5,4,7,2,8,3,6,4,4,5,6,
			  4,8,2,7,1,5,2,3,1,1,3,2,2,4,4,3,3,5,
			  1,6};



int main()
{
	int g_driver,g_mode,g_error;
	detectgraph(&g_driver,&g_mode);
	g_mode = 1;
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(triplex_font)<0) exit(1);
	initgraph(&g_driver,&g_mode,"\\tc\\lib");
	setup();
	play_moves(play,64);
	setcolor(11);
	settextstyle(0,0,0);
	outtextxy(455,150,"Press <ENTER> to return");
	outtextxy(455,159,"to DOS.");
	gets(NULL);
	closegraph();
	return 0;
}


void setup(void)
{
	draw_board();
	draw_knight();
	save_knight();
}

void play_moves(int *playlist,int count)
{
	int i,j = 0,move =0,x,y;

	for (i=0;i<count;i++)
	{
		x = *(playlist + j);
		j++;
		y = *(playlist + j);
		j++;
		put_piece(x,y,knight_buffer);
		delay(150);
		remove_piece(x,y,knight_buffer);
		move++;
		leave_number(x,y,move);
	}
}

void draw_board(void)
{
	setcolor(3);
	settextstyle(1,0,3);
	outtextxy(180,05,"KNIGHT'S TOUR");
	setlinestyle(0,0,3);
	setcolor(4);
	line(90,30,444,30);
	line(90,65,444,65);
	line(90,100,444,100);
	line(90,135,444,135);
	line(90,170,444,170);
	line(90,205,444,205);
	line(90,240,444,240);
	line(90,275,444,275);
	line(90,310,444,310);
	line(87,30,87,310);
	line(132,30,132,310);
	line(177,30,177,310);
	line(222,30,222,310);
	line(267,30,267,310);
	line(312,30,312,310);
	line(357,30,357,310);
	line(402,30,402,310);
	line(447,30,447,310);
	setcolor(63);
	setlinestyle(0,0,1);
}

void draw_queen(void)
{
	circle(20,58,2);
	circle(23,58,2);
	circle(26,58,2);
	circle(29,58,2);
	circle(32,58,2);
	circle(35,58,2);
	circle(28,56,3);
	floodfill(28,56,63);
	circle(28,62,3);
	floodfill(28,62,63);
	circle(24,65,3);
	floodfill(24,65,63);
	circle(32,65,3);
	floodfill(32,65,63);
	line(27,64,21,60);
	line(30,63,35,60);
	line(26,64,26,79);
	line(30,64,30,79);
	arc(26,80,90,180,6);
	arc(30,80,0,90,6);
	line(17,82,39,82);
	line(17,81,39,81);
	floodfill(28,74,63);
	floodfill(32,61,63);
	floodfill(24,61,63);
}

void draw_knight(void)
{
	line(10,50,49,50);
	line(49,50,49,47);
	line(10,50,10,47);
	line(10,47,15,47);
	line(49,47,45,47);
	line(15,47,20,41);
	arc(0,47,0,40,45);
	line(34,26,31,21);
	line(31,21,28,26);
	line(28,26,20,31);
	line(20,41,12,41);
	line(12,41,12,35);
	line(12,35,28,26);
	circle(27,30,2);
	arc(29,37,220,10,8);
	arc(17,37,145,200,6);
	floodfill(27,34,63);
}

void save_knight(void)
{
	unsigned size;
	size = imagesize(10,21,49,50);
	knight_buffer = malloc(size);
	getimage(10,21,49,50,knight_buffer);
}

void save_queen(void)
{
	unsigned size;
	size = imagesize(10,53,49,82);
	queen_buffer = malloc(size);
	getimage(10,53,49,82,queen_buffer);
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

void leave_number(int x,int y,int number)
{
	int adjx, adjy;
	char count[3];
	adjx = 103 + ((x-1) * 45);
	adjy = 32  + ((y-1) * 35);
	str_int(count,number);
	outtextxy(adjx,adjy,count);
}

void str_int(char *count,int number)
{
	int d1,d2;
	d1 = number/10;
	d2 = number - (d1*10);
	*(count+0) = d1+0x30;
	*(count+1) = d2+0x30;
	*(count+2) = NULL;
}


