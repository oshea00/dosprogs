
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dos.h>

typedef struct {
unsigned drive_installed : 1;
unsigned math_installed  : 1;
unsigned board_ram       : 2;
unsigned initial_video   : 2;
unsigned number_of_drives : 2;
unsigned not_used1       : 1;
unsigned number_of_serial : 3;
unsigned game_adapter    : 1;
unsigned not_used2       : 1;
unsigned number_of_printers : 2;
} equipat;

typedef struct {
unsigned drive_installed : 1;
unsigned math_installed  : 1;
unsigned mouse_installed : 1;
unsigned not_used3       : 1;
unsigned initial_video   : 2;
unsigned number_of_drives : 2;
unsigned not_used1       : 1;
unsigned number_of_serial : 3;
unsigned not_used4        : 1;
unsigned modem_installed  : 1;
unsigned number_of_printers : 2;
} equips2;

equipat e;
equips2 p;
union REGS regs;
char board_ram[4];
char video_mode[14];

main()
{
	int86(0x11,&regs,&regs);
	memcpy(&e,&regs.x.ax,2);
	memcpy(&p,&regs.x.ax,2);
	regs.h.ah = 0xc0;
	int86(0x15,&regs,&regs);
	if (regs.h.ah == 0x80 || regs.h.ah == 0x86)
	{
		printf("%02X PC/XT/AT \n",regs.h.ah);
		pcat();
	}
	else
	{
		printf("%02X PS/2\n",regs.h.ah);
		pcps2();
	}
}



pcat()
{
	printf("Drive installed    = %s\n",e.drive_installed?"Yes":"No");
	printf("Math installed     = %s\n",e.math_installed?"Yes":"No");
	switch (e.board_ram)
	{
		case 0 : strcpy(board_ram,"16K");break;
		case 1 : strcpy(board_ram,"32K");break;
		case 2 : strcpy(board_ram,"48K");break;
		case 3 : strcpy(board_ram,"64K");break;
	}
	printf("Board RAM          = %s\n",board_ram);
	switch (e.initial_video)
	{
		case 1 : strcpy(video_mode,"40 x 25 color");break;
		case 2 : strcpy(video_mode,"80 x 25 color");break;
		case 3 : strcpy(video_mode,"80 x 25 mono ");break;
	}
	printf("Initial video mode = %s\n",video_mode);
	printf("Number of drives   = %d\n",e.number_of_drives?e.number_of_drives+1:0);
	printf("Number of serials  = %d\n",e.number_of_serial);
	printf("Game adapter inst. = %s\n",e.game_adapter?"Yes":"No");
	printf("Number of printers = %d\n",e.number_of_printers);
}

pcps2()
{
	printf("Drive installed    = %s\n",p.drive_installed?"Yes":"No");
	printf("Math installed     = %s\n",p.math_installed?"Yes":"No");
	printf("Mouse installed    = %s\n",p.mouse_installed?"Yes":"No");
	switch (p.initial_video)
	{
		case 1 : strcpy(video_mode,"40 x 25 color");break;
		case 2 : strcpy(video_mode,"80 x 25 color");break;
		case 3 : strcpy(video_mode,"80 x 25 mono ");break;
	}
	printf("Initial video mode = %s\n",video_mode);
	printf("Number of drives   = %d\n",p.number_of_drives?e.number_of_drives+1:0);
	printf("Number of serials  = %d\n",p.number_of_serial);
	printf("Modem installed    = %s\n",p.modem_installed?"Yes":"No");
	printf("Number of printers = %d\n",p.number_of_printers);
}


