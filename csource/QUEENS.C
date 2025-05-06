/*
** QUEENS.C
**
** Michael R. O'Shea   'C' class level II - Monday off-hours
**
** INPUT
**  "/s" switch on command line causes program to reveal
**   process of solution graphically (slower run).
**
** OUTPUT
**  All 92 solutions to the 8 queens chess problem
**
**
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <dos.h>
#include <graphics.h>
#include <time.h>

typedef struct
{
	int x;
	int y;
} coord_array;

int main(int,char *[]);
void c_break(void);
int can_be_attacked(int,int,int,int);
void check_board_row(void);
int check_queens_list(coord_array [],int);
void print_queens_list(coord_array [],int);
void setup(void);
void draw_board(void);
void draw_queen(void);
void save_queen(void);
void put_piece(int,int,void *);
void remove_piece(int,int,void *);
void time_delay(clock_t);

int show = 0;
clock_t MOVE_DELAY  = (clock_t)((float)CLK_TCK * 0.08);
clock_t SHOW_DELAY  = (clock_t)((float)CLK_TCK * 1.0);
clock_t SOUND_DELAY = (clock_t)((float)CLK_TCK * 0.08);
coord_array  queen[8];
int row_point = 1;
int queens_placed = 1;
void * queen_buffer;

int main(int argc,char *argv[])
{
	int i,j,x,y;
	int g_driver,g_mode,g_error;
	queen[0].x = 0;
	queen[0].y = 0;
	detectgraph(&g_driver,&g_mode);
	g_mode = 1;
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(triplex_font)<0) exit(1);
	initgraph(&g_driver,&g_mode,"");
	setup();
	if (argc > 1)
		if (!strcmpi(argv[1],"/s"))
			show = 1;

	if (show)
	{
		put_piece(queen[row_point].y+1,queen[row_point].x+1,queen_buffer);
	}

	/* Look for solutions forever */

	while (1)
	{
		while (queens_placed < 8)
		{
			check_board_row();
		}

		/* A solution is found when 8 queens are placed */
		print_queens_list(queen,8);

		if (show)
		{
			put_piece(queen[0].y+1,queen[0].x+1,queen_buffer);
		}

		/* Move bottom queen over one and continue looking
		   for solutions */

		queens_placed--;
		row_point--;

		/* If bottom queen goes off board move up a row and move
		   that queen over one */

		if (++queen[7].y == 8)
		{
			queens_placed--;
			row_point--;
			queen[7].y = -1;
			queen[6].y++;
		}
	}
}

void time_delay(clock_t ticks)
{
	clock_t start = clock();
	while (clock() - start < ticks);
}

void check_board_row(void)
{
	int i;
	queen[row_point].x = row_point;
	for (i=queen[row_point].y+1;i<8;i++)
	{
		if (show)
		{
			put_piece(queen[row_point].y+1,queen[row_point].x+1,queen_buffer);
			time_delay(MOVE_DELAY);
			remove_piece(queen[row_point].y+1,queen[row_point].x+1,queen_buffer);
		}
		queen[row_point].y = i;
		if (show)
		{
			put_piece(queen[row_point].y+1,queen[row_point].x+1,queen_buffer);
			time_delay(MOVE_DELAY);
		}
		if (!check_queens_list(queen,row_point+1))
			break;
		if (show)
			remove_piece(queen[row_point].y+1,queen[row_point].x+1,queen_buffer);
	}

	if (i == 8)   /* no safe positions were found on the row */
	{
		if (row_point != 0)
		{
			/* backtrack one row and continue */

			if (show)
			{
				put_piece(queen[row_point].y+1,queen[row_point].x+1,queen_buffer);
				time_delay(MOVE_DELAY);
				remove_piece(queen[row_point].y+1,queen[row_point].x+1,queen_buffer);
			}
			queen[row_point].y = -1;
			row_point--;
			queens_placed--;
		}
		else
		{
			/* If backtracking has moved to the top row
			   all solutions have been found */

			closegraph();
			exit(0);
		}
	}
	else
	{
		/* A safe spot has been found on row.  Place queen
		   and go to next row */

		row_point++;
		queens_placed++;
	}
}

/*
** This routine checks the list of queens against each other
** to see if they are attacking.
*/

int check_queens_list(coord_array ptr[],int cnt)
{
	int i;

	/* List could be displayed here to show interm. result */

	for (i=0;i<cnt-1;i++)
	{
		if (can_be_attacked(ptr[i].x,ptr[i].y,ptr[cnt-1].x,ptr[cnt-1].y))
		{
			return i+1;   /* queen being attacked */
		}
	}
	return 0;   /* No queens attacking */
}

/*
** A queen can attack another queen if it shares a row or column,
** or if the absolute value of the slope of the line
** between them is 1.
*/

int can_be_attacked(int x,int y,int x1,int y1)
{
	if (x == x1)
		return 1;
	else
	if (y == y1)
		return 1;
	else
	if (fabs((x-x1)/(float)(y-y1)) == 1)
		return 1;
	else
		return 0;
}

void print_queens_list(coord_array ptr[],int cnt)
{
	int i;
	static int count = 0;
	count++;
	line(465,163,465+count,163);
	sound(1300);
	time_delay(SOUND_DELAY);
	nosound();
	for (i=0;i<cnt;i++)
	{
		put_piece(ptr[i].y+1,ptr[i].x+1,queen_buffer);
	}

	time_delay(SHOW_DELAY);

	if (kbhit())
		c_break();

	for (i=0;i<cnt;i++)
	{
		remove_piece(ptr[i].y+1,ptr[i].x+1,queen_buffer);
	}
}

void setup(void)
{
	draw_board();
	draw_queen();
	save_queen();
	setlinestyle(0,0,3);
	setcolor(10);
}

void draw_board(void)
{
	setcolor(14);
	settextstyle(1,0,3);
	outtextxy(180,05,"EIGHT QUEENS");
	setcolor(3);
	settextstyle(0,0,1);
	setcolor(7);
	outtextxy(461,160,"I");
	outtextxy(556,160,"I");
	setcolor(3);
	outtextxy(455,200,"This is showing all");
	outtextxy(455,209,"92 solutions to the");
	outtextxy(455,218,"Eight Queens chess");
	outtextxy(455,227,"puzzle.");
	outtextxy(455,240,"Invoking the program");
	outtextxy(455,249,"with a \"/s\" will");
	outtextxy(455,258,"cause the program to");
	outtextxy(455,267,"reveal the solution");
	outtextxy(455,276,"process graphically.");
	outtextxy(455,305,"HIT SPACE BAR TO QUIT.");
	outtextxy(455,314,"DISPLAY WILL HALT");
	outtextxy(455,323,"ON NEXT SOLUTION.");
	setlinestyle(0,0,3);
	setcolor(4);
	line(89,30,445,30);
	line(89,65,445,65);
	line(89,100,445,100);
	line(89,135,445,135);
	line(89,170,445,170);
	line(89,205,445,205);
	line(89,240,445,240);
	line(89,275,445,275);
	line(89,310,445,310);
	line(87,29,87,311);
	line(132,30,132,310);
	line(177,30,177,310);
	line(222,30,222,310);
	line(267,30,267,310);
	line(312,30,312,310);
	line(357,30,357,310);
	line(402,30,402,310);
	line(447,29,447,311);
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

void c_break(void)
{
	char string[80];
	outtextxy(455,50,"Hit <ENTER> key to");
	outtextxy(455,59,"return to DOS.");
	gets(string);
	closegraph();
	printf("Program aborted....\n");
	exit(0);
}




