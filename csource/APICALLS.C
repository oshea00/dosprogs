/****************************************************************************
 *
 *  A sample program to allow a user to test and learn the HLLAPI interface.
 *
 *
 *  (c) Copyright 1987-89  Digital Communications Associates, Inc.
 *
 ***************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "pcb.h"

void showstr(unsigned char *,int,int);
void wsctrl(void);
void get_sp(unsigned int);
void main(void);

struct query
{
   unsigned char q_short_name;
   unsigned char q_long_name[8];
   unsigned char q_sess_type;
   int           q_ps_size;
} *qp;


struct query_type_2
{
   int  emulator_type;                   /* type of emulator, hex */
   unsigned char emulator_version_msb;   /* version number, msb hex */
   unsigned char emulator_version_lsb;   /* version number, lsb hex */
   int  pc_type;                         /* As read in on config file, hex */
   unsigned char kb_type;                /* 0 = normal, 1 = 3270pc, hex */
   int  printer_type;                    /* As read in on config file, hex */
                                         /* 0xff = no printer */
   unsigned char printer_emulation;      /* 0 = dos printer, 1 = 3287 printer */
   unsigned char printer_bound;          /* 0 = not bound,   1 = not bound */
   int  printer_port;                    /* As read in on config file, hex */
   int  printer_interface;               /* 0 = parallel, 1 = serial */
   unsigned char current_active_session; /* current active sess, short name */
   unsigned char zoomed;                 /* 0 = unzoomed, 1 = zoomed, hex */
   unsigned char country_code;           /* As read in on config file, hex */
   int  reserved_1;                      /* reserved */
   int  reserved_2;                      /* reserved */
   int  reserved_3;                      /* reserved */
} *qp2;

unsigned char input_line[10];
unsigned char api_str[255];
unsigned char api_screen[7128];          /* big enough for model 5 with eab */

unsigned int api_func;                   /* hllapi function */
unsigned int api_len;                    /* hllapi length field */
unsigned int api_retc;                   /* hllapi return code */

unsigned char api_buffer[7128];          /* big enough for model 5 with eab */
unsigned int api_buffer_len;             /* hllapi length field */


void main()
{
        register int i;

        /* Make sure HLLAPI is present by issuing Set Session Parameters. */
        /* The parameter does not exist so the current settings are not changed. */
         api_func = 9;
         api_len = 8;
         api_retc = -1;                           /* make sure rc is not zero */
         hllc(&api_func, "xxxxxxxx", &api_len, &api_retc);
         if (api_retc != 0)
         {
                 fprintf(stderr, "\nHLLAPI is not loaded - rc = %04x\n", api_retc);
                 return;
         }

   printf("\nDCA sample HLLAPI application\n");
   printf("(C) 1987, Digital Communications Associates, Inc.\n\n");


        while (1)
        {
                printf("                            Function codes:\n\n");
                printf("  1 - Connect               2 - Disconnect            3 - Send Key\n");
                printf("  4 - Wait                  5 - Copy Entire PS        6 - Search PS\n");
                printf("  7 - Get Cursor Posn       8 - Copy String from PS   9 - Set Session Params\n");
                printf(" 10 - Query Sessions       11 - Reserve              12 - Release\n");
                printf(" 13 - Copy OIA             14 - Get Fld Attr         15 - Copy String to PS\n");
                printf(" 16 - Wsctrl               17 - Storage Manager      18 - Pause\n");
                printf(" 20 - Query System         21 - Reset System         22 - Query Session Status\n");
                printf(" 23 - Start Host Notify    24 - Query Host Update    25 - Stop Host Notify\n");
                printf(" 30 - Search Fld           31 - Find Fld Pos         32 - Find Fld Len\n");
                printf(" 33 - Copy String to Fld   34 - Copy Fld to String\n");
                printf(" 90 - Send File            91 - Receive File         92 - Invoke Dos Program\n");
                printf(" 93 - Dos Redirect         99 - Convert Row/Col     255 - DCA Query\n\n");
                printf("\n\n 0   - To exit\n");
                printf("\nEnter the function code: ");


                gets(input_line);
                api_func = atoi(input_line);
                printf("\n");

                switch(api_func)
                {
                case 0:                                  /* Exit from program */
                        exit(0);

                case 1:                                         /* Connect PS */
                case 24:
                case 25:
                        printf("Enter Session Id: ");
                        gets(api_str);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        printf("API Returncode = %d\n\n",api_retc);
         break;

                case 5:                                            /* Copy PS */
                        printf("Model 2 = 1920,   Model 3 = 2560\n");
                        printf("Model 4 = 3440,   Model 5 = 3564\n");
                        printf("Enter Length of Presentation Space Size: ");
                        gets(input_line);
                        hllc(&api_func, api_screen, &api_len, &api_retc);
                        printf("API Screen\n");
                        showstr(api_screen, atoi(input_line), 0);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 10:                                    /* Query Sessions */
                        api_len = sizeof(api_str);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        if (api_retc == 0)         /* 0 probably means NEWRET */
                                api_retc = api_len;
                        printf("There are %d sessions\n", api_retc);
                        qp = (struct query *) api_str;
                        for(i = 0; i < api_retc; ++i)
                        {
                                printf("%c  %.8s  %c  %d\n",
                                        qp->q_short_name,
                                        qp->q_long_name,
                                        qp->q_sess_type,
                                        qp->q_ps_size);
                                ++qp;
                        }
                        break;

                case 2:                                      /* Disconnect PS */
                case 4:                                               /* Wait */
                case 7:                              /* Query Cursor Location */
                case 11:                                           /* Reserve */
                case 12:                                           /* Release */
                case 21:                                      /* Reset System */
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        if (api_func == 7)
                                printf("API Length     = %d\n",api_len);
                        printf("API Returncode = %d\n\n",api_retc);
         break;

                case 3:                                           /* Send Key */
                        printf("Enter String of keystrokes: ");
                        gets(api_str);
                        api_len = strlen(api_str);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 6:                                          /* Search PS */
                case 30:                                      /* Search Field */
                        printf("Enter String to search for: ");
                        gets(api_str);
                        api_len = strlen(api_str);
                        printf("Enter starting Position: ");
                        gets(input_line);
                        api_retc = atoi(input_line);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        printf("API Length     = %d\n",api_len);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 8:                                  /* Copy PS to String */
                case 34:                              /* Copy Field to String */
                        printf("Enter Offset in PS to the string: ");
                        gets(input_line);
                        api_retc = atoi(input_line);
                        printf("Enter String length: ");
                        gets(input_line);
                        api_len = atoi(input_line);
                        hllc(&api_func, api_screen, &api_len, &api_retc);
                        showstr(api_screen, api_len, 1);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 9:                             /* Set Session Parameters */
                        printf("Enter Parameter string: ");
                        gets(api_str);
                        api_len = strlen(api_str);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 13:                                          /* Copy OIA */
                        api_len = 103;
                        hllc(&api_func, api_screen, &api_len, &api_retc);
                        showstr(api_screen, api_len, 1);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 14:                             /* Query Field Attribute */
                        printf("Enter Position: ");
                        gets(input_line);
                        api_retc = atoi(input_line);
                        hllc(&api_func, api_screen, &api_len, &api_retc);
                        printf("Attribute = %04x\n",api_len);
                        printf("API Length     = %d\n",api_len);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 15:                                 /* Copy String to PS */
                case 33:                              /* Copy String to Field */
                        printf("Enter String: ");
                        gets(api_screen);
                        api_len = strlen(api_screen);
                        printf("Enter Position: ");
                        gets(input_line);
                        api_retc = atoi(input_line);
                        hllc(&api_func, api_screen, &api_len, &api_retc);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;


                case 16:                                            /* WSCTRL */
                        wsctrl();
                        break;

                case 17:
                        printf(" 1 - Get Storage\n");
                        printf(" 2 - Free Storage\n");
                        printf(" 3 - Query Free Storage\n");
                        printf(" 4 - Free All Storage\n");
                        printf("Enter Storage Manager sub-function:  ");
                        gets(input_line);
                        i = api_retc = atoi(input_line);
                        if (api_retc == 1)
                        {
                                printf("Enter size to be allocated:  ");
                                gets(input_line);
                                api_len = atoi(input_line);
                        }
                        if (api_retc == 2)
                        {
                                printf("Enter block id to be freed: ");
                                gets(input_line);
                                api_len = atoi(input_line);
                        }
                        hllc(&api_func, input_line, &api_len, &api_retc);
                        if (api_retc == 0)
                        {
                                if (i == 1)
                                {
                                        printf("Block Id: %d\n", api_len);
                                        printf("Address: %02x%02x:%02x%02x\n",
                                                input_line[3], input_line[2], input_line[1], input_line[0]);
                                }
                                if (i == 3)
                                        printf("Size of largest available block: %d\n", api_len);
                        }
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 18:                                             /* Pause */
                        printf("Enter pause duration (in half seconds): ");
                        gets(input_line);
                        api_len = atoi(input_line);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 20:
                        api_len = 35;
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        showstr(api_str, api_len, 1);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 22:                              /* Query Session Status */
                        printf("Enter Session Id: ");
                        gets(api_str);
                        api_len = 18;
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        showstr(api_str, api_len, 1);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 23:                           /* Start Host Notification */
                         printf("Enter Session Id and parm: ");
                         gets(api_screen);
                         api_str[0] = api_screen[0];
                         api_str[1] = api_screen[1];
                         api_func = 23;
                         api_len = 2;
                         hllc(&api_func, api_str, &api_len, &api_retc);
                         printf("API Returncode = %d\n\n",api_retc);
          break;

                case 31:                               /* Find Field Position */
                case 32:                                 /* Find Field Length */
                        printf("Enter Position offset: ");
                        gets(input_line);
                        api_retc = atoi(input_line);
                        printf("Enter direction (blank, T, P, or N) and field type (blank, P, U): ");
                        gets(input_line);
                        if (strlen(input_line) <= 1)
                        {
                                input_line[1] = ' ';
                                if (strlen(input_line) < 1)
                                        input_line[0] = ' ';
                        }
                        api_len = 2;
                        hllc(&api_func, input_line, &api_len, &api_retc);
                        printf("API Length     = %d\n",api_len);
                        printf("API Returncode = %d\n\n",api_retc);
                        break;

                case 90:
                        printf("Enter parameters for SEND:\n");
                        gets(api_str);
                        api_len = strlen(api_str);
                        printf("Enter drive number of SEND program: ");
                        gets(input_line);
                        api_retc = atoi(input_line);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        printf("\nAPI Returncode = %d\n\n",api_retc);
                        break;

                case 91:
                        printf("Enter parameters for RECEIVE:\n");
                        gets(api_str);
                        api_len = strlen(api_str);
                        printf("Enter drive number of RECEIVE program: ");
                        gets(input_line);
                        api_retc = atoi(input_line);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        printf("\nAPI Returncode = %d\n\n",api_retc);
                        break;

                case 92:
                        printf("Enter program name (including extension) and its parameters:\n");
                        gets(api_str);
                        api_len = strlen(api_str);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        printf("\nAPI Returncode = %d\n\n",api_retc);
                        break;

                case 93:
                        printf("Enter the Dos command line:\n");
                        gets(api_str);
                        api_len = strlen(api_str);
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        printf("\nAPI Returncode = %d\n\n",api_retc);
                        break;

                case 99:                       /* Convert Position or Row Col */
                        printf("Enter Session and P or R (position or row/column): ");
                        gets(input_line);
                        if (input_line[1] == 'R')
                        {
                                printf("Enter Row: ");
                                gets(input_line + 2);
                                api_len = atoi(input_line + 2);
                                printf("Enter Column: ");
                        }
                        else
                                printf("Enter Position: ");
                        gets(input_line + 2);
                        api_retc = atoi(input_line + 2);
                        hllc(&api_func, input_line, &api_len, &api_retc);
                        printf("API Length     = %d\n",api_len);
                        printf("API Statuscode = %d\n\n",api_retc);
                        break;

                case 255:                                        /* DCA Query */
                        api_func = 20;
                        api_len = 35;
                        hllc(&api_func, api_str, &api_len, &api_retc);
                        api_func = 255;

                        if (api_str[13] == 'C')
                        {
                             api_len = sizeof(struct query_type_2);
                             hllc(&api_func, api_str, &api_len, &api_retc);
                             printf("API Returncode = %d\n",api_retc);

                             if (api_retc == 0)
                             {
                                qp2 = (struct query_type_2 *) api_str;

                                printf("Emulator Type = %d       Emulator Version %x.%02x\n",
                                        qp2->emulator_type, qp2->emulator_version_msb,
                                        qp2->emulator_version_lsb);

                                printf("PC Type = %d             Keyboard Type = %d  Printer Type = %d\n",
                                        qp2->pc_type, qp2->kb_type, qp2->printer_type);

                                printf("Printer Emulation = %d   Printer Port = %d   Printer Interface = %d\n",
                                        qp2->printer_emulation, qp2->printer_port,
                                        qp2->printer_interface);

                                printf("Printer Bind State = %d\n", qp2->printer_bound);

                                printf("Active Session = %c      Zoomed = %d         Country Code = %d\n",
                                        qp2->current_active_session, qp2->zoomed,
                                        qp2->country_code);
                             }
                             break;
                        }
                        else
                        {
                             api_len = 24;
                             hllc(&api_func, api_str, &api_len, &api_retc);
                             printf("API Returncode = %d\n\n",api_retc);
                             if (api_retc == 0)
                                    showstr(api_str, api_len, 1);
                             break;
                        }

                default :
                        printf("\nInvalid function code\n");
                        break;
                }
        };
}


/*
 *      Print the output of a HLLAPI call.       The api string is also printed
 *      in hex if the hexflag is non-zero.      The format is vertical, that is
 *      the character, same column next line is the high nibble, same column
 *      on the third line is the low nibble.
 */
void showstr(strptr, len, hexflag)
unsigned char *strptr;                       /* ptr to string to be displayed */
int  len;                                           /* length to be displayed */
int  hexflag;                                        /* show hex chars or not */
{
        register int i;                                   /* char index value */
        int cnt;                              /* number of chars on this line */
        int pos;                            /* starting position on this line */
        int end = 0;                          /* ending position on this line */

        printf("API String - Length = %d\n", len);

        while (len)
        {
                pos = end;
                if ((cnt = 80) > len)
                        cnt = len;
                end += cnt;

                for (i=pos ; i<end; i++)
                {
                        if (strptr[i] < ' ')
                                printf(" ");
                        else
                                printf("%c",strptr[i]);
                }
                if (cnt != 80)
                        printf("\n");

                if (hexflag)
                {
                        for (i=pos; i<end; i++)
                                printf("%x",strptr[i] >> 4);

                        if (cnt != 80)
                                printf("\n");

                        for (i=pos; i<end; i++)
                                printf("%x",strptr[i] & 0x0f);

                        if (cnt != 80)
                                printf("\n");
                }
                len -= cnt;
        }
        printf("\n");
}


void wsctrl()
{
        unsigned int i;                                       /* loop control */
        unsigned int cmd_num;                               /* command number */
        struct cmd_struct                         /* data for wsctrl commands */
        {
                unsigned char *cmd;
                unsigned int len;
        };

        static struct cmd_struct cmds[] =
        {
                "",    0,                              /* return to main menu */
                "AW=", 5,
                "CA=", 5,
                "CE=", 3,
                "CC=", 6,
                "CW=", 14,
                "CS=", 4,
                "CB=", 11,
                "DW=", 5,
                "RD=", 4,
                "QA=", 5,
                "QC=", 6,
                "QE=", 6,
                "QW=", 14,
                "QN=", 25
        };

        /*
         *      Loop until an invalid number is entered
         */
        while (1)
        {
                /*
                 *      Fill the string with spaces for largest command and
                 *      ask the user what to do.
                 */
                for (i = 0; i < 25; i++)
                        api_str[i] = ' ';

                printf("\n");
                printf("                       Work Station Control\n\n");
                printf("    0: Return to Main Menu\n");
                printf("    1: AW - Add Window                 8: DW - Delete Window\n");
                printf("    2: CA - Change Active Window       9: RD - Redraw\n");
                printf("    3: CE - Change Enlarge State      10: QA - Query Active Window\n");
                printf("    4: CC - Change Background Color   11: QC - Query Background Color\n");
                printf("    5: CW - Change Window             12: QE - Query Enlarge\n");
                printf("    6: CS - Clear Screen              13: QW - Query Window\n");
                printf("    7: CB - Copy Block                14: QN - Query Window Names\n");
                printf("\nSelect one: ");
                gets(input_line);

                cmd_num = atoi(input_line);
                switch(cmd_num)
                {
                case 3:                                                         /* Change Enlarge State */
                case 10:                                                        /* Query Active Window */
                case 12:                                                        /* Query Enlarge */
                        break;

                case 6:                                                         /* Clear Screen */
                case 9:                                                         /* Redraw */
                case 11:                                                        /* Query Background Color */
                case 14:                                                        /* Query Names */
                        get_sp(0);
                        break;

                case 1:                                                         /* Add Window */
                case 2:                                                         /* Change Active Window */
                case 8:                                                         /* Delete Window */
                case 13:                                                        /* Query Window */
                        get_sp(2);
                        break;

                case 4:                                                         /* Change Background Color */
                        get_sp(0);
                        printf("Enter Color: ");
                        gets(input_line);
                        api_str[5] = input_line[0];
                        break;

                case 5:                                                         /* Change Window */
                        get_sp(2);
                        printf("Enter Number of Rows: ");
                        gets(input_line);
                        api_str[5] = (char) atoi(input_line);
                        printf("Enter Number of Cols: ");
                        gets(input_line);
                        api_str[6] = (char) atoi(input_line);
                        printf("Enter Row Position of upper left corner: ");
                        gets(input_line);
                        api_str[7] = (char) atoi(input_line);
                        printf("Enter Col Position of upper left corner: ");
                        gets(input_line);
                        api_str[8] = (char) atoi(input_line);
                        printf("Enter Window Color Code: ");
                        gets(input_line);
                        api_str[9] = (char) atoi(input_line);
                        printf("Enter Border Color Code: ");
                        gets(input_line);
                        api_str[10] = (char) atoi(input_line);
                        printf("Enter Control Flags: ");
                        gets(input_line);
                        api_str[11] = (char) atoi(input_line);
                        printf("Enter Row on the Presentation Space: ");
                        gets(input_line);
                        api_str[12] = (char) atoi(input_line);
                        printf("Enter Col on the Presentation Space: ");
                        gets(input_line);
                        api_str[13] = (char) atoi(input_line);
                        break;

                case 7:                                                         /* Copy Block */
                        printf("Enter Source Window: ");
                        gets(input_line);
                        api_str[3] = input_line[0];
                        printf("Enter Starting Offset: ");
                        gets(input_line);
                        i = atoi(input_line);
                        api_str[4] = (char) (i & 0x00ff);
                        api_str[5] = (char) (i >> 8);
                        printf("Enter Ending Offset: ");
                        gets(input_line);
                        i = atoi(input_line);
                        api_str[6] = (char) (i & 0x00ff);
                        api_str[7] = (char) (i >> 8);
                        printf("Enter Target Window: ");
                        gets(input_line);
                        api_str[8] = input_line[0];
                        printf("Enter Starting Offset on Target Window: ");
                        gets(input_line);
                        i = atoi(input_line);
                        api_str[9] = (char) (i & 0x00ff);
                        api_str[10] = (char) (i >> 8);
                        break;

                default:
                        return;
                }

                /*
                 *      Fill in the 1st part of the command and the length.
                 *      Call HLLAPI
                 */
                for (i = 0; i < 3; i++)
                        api_str[i] = cmds[cmd_num].cmd[i];
                api_len = cmds[cmd_num].len;
                hllc(&api_func, api_str, &api_len, &api_retc);

                /*
                 *      Print the result
                 */
                printf("API Returncode = %d\n\n",api_retc);
                if ((cmd_num >= 10) && (api_retc == 0))
                        showstr(api_str, api_len, 1);
        }
}

/*
 *      Get the Screen Profile and the Presentation Space (window).
 *      If the parameter 'both' is zero it gets only the Screen Profile, otherwise
 *      it gets the Presentation Space as well.
 */
void get_sp(both)
unsigned int both;                     /* 0 - gets only S, otherwise get both */
{
        printf("Enter Screen Profile number: ");
        gets(input_line);
        api_str[3] = input_line[0];

        if (both)
        {
                printf("Enter Window: ");
                gets(input_line);
                api_str[4] = input_line[0];
        }
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/
