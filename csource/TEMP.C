	int scan;
	char ascii=0;
	int g_driver,g_mode,g_error;
	detectgraph(&g_driver,&g_mode);
	g_mode = 1; /* Override to EGA */
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(triplex_font)<0) exit(1);
	if (registerbgifont(small_font)<0) exit(1);
	initgraph(&g_driver,&g_mode,"");
	regs.h.ah = 0x03;
	regs.h.al = 0x05;
	regs.h.bh = 0x00;
	regs.h.bl = 0x00;
	int86(0x16,&regs,&regs);
	draw_cursor(ON);
	fflush(stdin);
	prompt("Cursor Mode.","Type 'e' to quit.","","");
	while (ascii!='e')
	{
		mgetch(&scan,&ascii);
		switch (scan)
		{
			case FUP   : draw_cursor(OFF);up(FR);draw_cursor(ON);break;
			case FDOWN : draw_cursor(OFF);down(FR);draw_cursor(ON);break;
			case FLEFT : draw_cursor(OFF);left(FR);draw_cursor(ON);break;
			case FRIGHT : draw_cursor(OFF);right(FR);draw_cursor(ON);break;
			case UP   : draw_cursor(OFF);up(SR);draw_cursor(ON);break;
			case DOWN : draw_cursor(OFF);down(SR);draw_cursor(ON);break;
			case LEFT : draw_cursor(OFF);left(SR);draw_cursor(ON);break;
			case RIGHT : draw_cursor(OFF);right(SR);draw_cursor(ON);break;
		}
		switch(ascii)
		{
			case 's' : draw_cursor(OFF);draw_square();break;
			case 'c' : draw_cursor(OFF);draw_circle();break;
			case 'f' : draw_cursor(OFF);draw_file();break;
			case 'p' : draw_cursor(OFF);draw_arrow();break;
			case 'l' : draw_cursor(OFF);draw_line();break;
			case 'm' : draw_cursor(OFF);manipulate_object();break;
			case 'g' : draw_cursor(OFF);get_save();break;
			case '=' : draw_cursor(OFF);rescale();break;
			case 'r' : refresh();break;
		}
		if (ascii=='s'||ascii=='r'||ascii=='c'||ascii=='f'||
			ascii=='p'||ascii=='l'||ascii=='m'||ascii=='g'||ascii=='=')
			prompt("Cursor Mode.","Type 'e' to quit.","","");
	}
	c_break();
	return 0;
