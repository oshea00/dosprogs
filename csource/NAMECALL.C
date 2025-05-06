#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>
#include <bios.h>
#include <conio.h>
#include <string.h>
#include <process.h>

    /* NETBIOS.H */

typedef unsigned char byte;
typedef unsigned int  word;

/* Network Control Block (NCB)  */

typedef struct
    {
    byte NCB_COMMAND;               /* command id                         */
    byte NCB_RETCODE;               /* immediate return code              */
    byte NCB_LSN;                   /* local session number               */
    byte NCB_NUM;                   /* network name number                */
    void far *NCB_BUFFER_PTR;       /* address of message packet          */
    word NCB_LENGTH;                /* length of message packet           */
    byte NCB_CALLNAME[16];          /* name of the other computer         */
    byte NCB_NAME[16];              /* our network name                   */
    byte NCB_RTO;                   /* receive time-out in 500 ms. incrs. */
    byte NCB_STO;                   /* send time-out - 500 ms. increments */
    void interrupt (*POST_FUNC)(void); /* address of POST routine         */
    byte NCB_LANA_NUM;              /* adapter number (0 or 1)            */
    byte NCB_CMD_CPLT;              /* final return code                  */
    byte NCB_RESERVE[14];           /* Reserved area                      */
    }
    NCB;


/* NetBIOS error return messages  */

char    *net_error_message[] = {
        "success",                      /*  00  */
        "invalid buffer length",        /*  01  */
        "ret code 02",                  /*  02  */
        "invalid command",              /*  03  */
        "ret code 04",                  /*  04  */
        "timed out",                    /*  05  */
        "buffer too small",             /*  06  */
        "ret code 07",                  /*  07  */
        "invalid session num",          /*  08  */
        "no resource",                  /*  09  */
        "session closed",               /*  0A  */
        "command cancelled",            /*  0B  */
        "ret code 0C",                  /*  0C  */
        "dupl. local name",             /*  0D  */
        "name table full",              /*  0E  */
        "active session",               /*  0F  */
        "ret code 10",                  /*  10  */
        "session table full",           /*  11  */
        "no one listening",             /*  12  */
        "invalid name num",             /*  13  */
        "no answer",                    /*  14  */
        "no local name",                /*  15  */
        "name in use",                  /*  16  */
        "name is deleted",              /*  17  */
        "abnormal end",                 /*  18  */
        "name conflict",                /*  19  */
        "ret code 1A",                  /*  1A  */
        "ret code 1B",                  /*  1B  */
        "ret code 1C",                  /*  1C  */
        "ret code 1D",                  /*  1D  */
        "ret code 1E",                  /*  1E  */
        "ret code 1F",                  /*  1F  */
        "ret code 20",                  /*  20  */
        "card busy",                    /*  21  */
        "too many cmds",                /*  22  */
        "invalid card num",             /*  23  */
        "cancel done",                  /*  24  */
        "ret code 25",                  /*  25  */
        "cannot cancel"                 /*  26  */
        };

/* Symbolic names for NetBIOS commands  */

#define RESET                           0x32
#define CANCEL                          0x35
#define STATUS                          0xb3
#define STATUS_WAIT                     0x33
#define TRACE                           0xf9
#define TRACE_WAIT                      0x79
#define UNLINK                          0x70
#define ADD_NAME                        0xb0
#define ADD_NAME_WAIT                   0x30
#define ADD_GROUP_NAME                  0xb6
#define ADD_GROUP_NAME_WAIT             0x36
#define DELETE_NAME                     0xb1
#define DELETE_NAME_WAIT                0x31
#define CALL                            0x90
#define CALL_WAIT                       0x10
#define LISTEN                          0x91
#define LISTEN_WAIT                     0x11
#define HANG_UP                         0x92
#define HANG_UP_WAIT                    0x12
#define SEND                            0x94
#define SEND_WAIT                       0x14
#define SEND_NO_ACK                     0xf1
#define SEND_NO_ACK_WAIT                0x71
#define CHAIN_SEND                      0x97
#define CHAIN_SEND_WAIT                 0x17
#define CHAIN_SEND_NO_ACK               0xf2
#define CHAIN_SEND_NO_ACK_WAIT          0x72
#define RECEIVE                         0x95
#define RECEIVE_WAIT                    0x15
#define RECEIVE_ANY                     0x96
#define RECEIVE_ANY_WAIT                0x16
#define SESSION_STATUS                  0xb4
#define SESSION_STATUS_WAIT             0x34
#define SEND_DATAGRAM                   0xa0
#define SEND_DATAGRAM_WAIT              0x20
#define SEND_BCST_DATAGRAM              0xa2
#define SEND_BCST_DATAGRAM_WAIT         0x22
#define RECEIVE_DATAGRAM                0xa1
#define RECEIVE_DATAGRAM_WAIT           0x21
#define RECEIVE_BCST_DATAGRAM           0xa3
#define RECEIVE_BCST_DATAGRAM_WAIT      0x23


#define  MSG_SIZE  1001

/* - - - - - - - - - - - - - - - - - - - - - - - */

char            message_out[MSG_SIZE];
char            message_in [MSG_SIZE];
char            string     [MSG_SIZE];
char            string2    [MSG_SIZE];
char            localname  [17];
char            remotename [17];
unsigned char   lsn;
unsigned char   name_number;
int             choice;
int             x;
int             cursor_x;
int             cursor_y;

/* - - - - - - - - - - - - - - - - - - - - - - - */

unsigned            ncb_segments[10];
unsigned            ncb_offsets [10];
unsigned volatile   ncb_head  = 0;
unsigned            ncb_tail  = 0;

/* - - - - - - - - - - - - - - - - - - - - - - - */

NCB     add_name_ncb;
NCB     delete_name_ncb;
NCB     temp_ncb;

/* - - - - - - - - - - - - - - - - - - - - - - - */

unsigned        es_reg, bx_reg;
void interrupt (*int_5C_vector)(void);
char    *state_msg [7] = {
        "                 ",
        "LISTEN pending",
        "CALL pending",
        "Active session",
        "HANG UP pending",
        "HANG UP complete",
        "Session abort"
        };

int     name_type;
char    *name_type_msg [2] = {
        "Unique name",
        "Group name "
        };

int     name_status;
char    *name_status_msg [8] = {
        "Reg. in progress   ",
        "                   ",
        "                   ",
        "                   ",
        "Registered         ",
        "De-registered      ",
        "Dupl detected      ",
        "Dupl; dereg pend.  "
        };

/* - - - - - - - - - - - - - - - - - - - - - - - */


void    NetBios(NCB far *ncb_ptr)
        {
        _ES    = FP_SEG(ncb_ptr);
        _BX    = FP_OFF(ncb_ptr);
        _AX    = 0x0100;
        geninterrupt(0x5c);
        }


void main()
{
    char net_name[17] = "SRV01";
    char p1,p2,p3;
    int_5C_vector = getvect(0x5C);
    if (int_5C_vector == (void far *) NULL)
    {
	printf("NetBios not loaded (Int5C not present).\n");
	exit(0);
    }

    memset(&temp_ncb, 0, sizeof(NCB));
    temp_ncb.NCB_COMMAND = 0x7F;
    NetBios(&temp_ncb);
    if (temp_ncb.NCB_RETCODE != 03)
    {
	printf("NetBios not loaded (No response from Int5C).\n");
	exit(0);
    }

    system("net pause prdr");

    for (p1='Y';p1 < '[';p1++)
    {
	for (p2='Y';p2 < '[';p2++)
	{
	    for (p3='Y';p3 < '[';p3++)
	    {
		net_name[5] = p1;
		net_name[6] = p2;
		net_name[7] = p3;
		net_name[8] = ' ';
		net_name[9] = ' ';
		net_name[10] = ' ';
		net_name[11] = ' ';
		net_name[12] = ' ';
		net_name[13] = ' ';
		net_name[14] = ' ';
		net_name[15] = ' ';
		net_name[16] = NULL;
		printf("trying %s\n",net_name);
		add_name(net_name);
		if (add_name_ncb.NCB_RETCODE != 00)
		    fprintf(stdprn,"%s %s\n",net_name,net_error_message[add_name_ncb.NCB_RETCODE]);
		else
		    delete_name(net_name);
	    }
	}
    }
}


add_name(char *localname)
{
    memset(&add_name_ncb, 0, sizeof(NCB));
    add_name_ncb.NCB_COMMAND = ADD_NAME_WAIT;
    strcpy(add_name_ncb.NCB_NAME, localname);
    add_name_ncb.POST_FUNC = NULL;
    NetBios(&add_name_ncb);
}

delete_name(char *localname)
{
    memset(&delete_name_ncb, 0, sizeof(NCB));
    delete_name_ncb.NCB_COMMAND = DELETE_NAME_WAIT;
    strcpy(delete_name_ncb.NCB_NAME, localname);
    delete_name_ncb.POST_FUNC = NULL;
    NetBios(&delete_name_ncb);
}



/* - - - - - - - - - - - - - - - - - - - - - - - */






