 /***************************************************************************
 *
 *                    HLLAPI sample program using  C
 *
 ****************************************************************************
 *
 *     This program makes a handful of HLLAPI requests to logon to a
 *     host.
 *
 *     ---------------------------------------------------------------
 *
 *     Digital Communications Associates
 *     (C) 1987-1989 Digital Communications Associates, Inc.
 *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"

void main(void);
int wait_search_send(int,char *,int,char *,int);
int send_param(char *,int);

#define MAX_TRIES 18

int api_func;                                           /* function requested */
int api_null = 0;                                         /* null data string */
int api_len;                                          /* length of the string */
int api_retc = 0;                                              /* return code */

struct query
{
        char    q_short_name;
        char    q_long_name[8];
        char    q_sess_type;
        int     q_ps_size;
};
struct query qp[12];


char parms[] = "NEWRET NOATTRB NOEAB STRLEN CONLOG ESC=@ AUTORESET SRCHALL SRCHFRWD";

char host_dca_logon[] = "LOGON";
char our_dca_logon[] = "kgpstso(spf028) logmode(t3278m2)@E";
char password_screen[] = "PASSWORD";
char our_password[] = "borland@E";


void main()
{
        register int i;
        int j;

        printf( "HLLAPI Sample program for C\n");
        printf( "(C) 1987-1989 Digital Communications Associates, Inc.\n\n");

        i = send_param( parms, sizeof(parms)-1 );
        if(i != 0 )
        {
                printf( "Error 000: send param failure\n");
                exit(1);
        }

        /*
         * issue query for session location
         */
        api_func = PCB_QUERY;
        api_len = sizeof(qp);
        hllc( &api_func, &qp[0], &api_len, &api_retc );
        if( api_len == 0 )
        {
                printf( "Error 005: No configured sessions\n");
                exit(1);
        }

        for( i = 0, j = 0; i < api_len; ++i )
        {
                if( qp[i].q_sess_type == 'H')
                {
                        j = 1;
                        break;
                }
        }
        if( j == 0 )
        {
                printf( "Error 010: No configured host session\n");
                exit(1);
        }


        /*
         * issue the connect request
         */
        api_func = PCB_CONNECT;
        hllc( &api_func, &qp[i].q_short_name, &api_len, &api_retc );
        switch( api_retc )
        {
        case PCB_RC_CONNECT_OK:
        case PCB_RC_CONNECT_OK_BUSY:
                break;

        default:
                printf( "Error 030: Connect failure, code %x\n",api_retc);
                exit(1);
        }

        /*
         * reserve the session
         */
        api_func = PCB_RESERVE;
        hllc(&api_func, &api_null, &api_len, &api_retc);
        if (api_retc != PCB_RC_RESERVE_OK)
        {
                printf( "Error 050: Reserve failure, code %x\n", api_retc);
      api_func = PCB_DISCONNECT;
      hllc( &api_func, &api_null, &api_null, &api_null );
                exit(1);
        }



        /*
         * search for the host logon message and logon
         */
        i = wait_search_send( MAX_TRIES,
            host_dca_logon, sizeof(host_dca_logon)-1,
            our_dca_logon,  sizeof(our_dca_logon )-1 );
        if(i != 0 )
        {
                printf( "Error 060: Logon failure\n");
      api_func = PCB_DISCONNECT;
      hllc( &api_func, &api_null, &api_null, &api_null );
                exit(1);
        }

         /* release the session */
        api_func = PCB_RELEASE;
        hllc(&api_func, &api_null, &api_len, &api_retc);
        if (api_retc != PCB_RC_RELEASE_OK)
        {
                printf( "Error 140: Release failure, code %x\n", api_retc);
      api_func = PCB_DISCONNECT;
      hllc( &api_func, &api_null, &api_null, &api_null );
                exit(1);
        }

        /* disconnect the session */
        api_func = PCB_DISCONNECT;
        hllc(&api_func, &api_null, &api_len, &api_retc);
        if (api_retc != PCB_RC_DISCONNECT_OK)
        {
                printf( "Error 150: Disconnect failure, code %x\n", api_retc);
                exit(1);
        }


        printf( "Application task completes with no errors\n");
        exit(1);
}




/*************************************************************************
 * Waits for keyboard free, searches for passed string, then writes string
 *
 * Returns:
 *              0 = no error
 *              1 = busy timeout
 *              2 = search failure
 *              3 = send failure
 *
 ************************************************************************/
wait_search_send( wait_count, search_string, search_count, send_string, send_count)
int wait_count;
char *search_string;
int search_count;
char *send_string;
int send_count;
{
        int i, j;
        int found;

        found = 0;
        for( i = 0; i < wait_count; i++ )
        {
                /*
                 * wait for screen to become unbusy
                 */
                for( j = 0; j < MAX_TRIES; j++ )
                {
                        api_func = PCB_WAIT;
                        hllc(&api_func,&api_null, &api_len, &api_retc);
                        if (api_retc == PCB_RC_WAIT_OK)
                        {
                                break;
                        }
                }
                if( api_retc != PCB_RC_WAIT_OK )
                {
                        printf( "Error 500: Wait busy failure, code %x\n", api_retc);
                        return(1);
                }

                /*
                 * search the presentation space for desired string
                 */
                if( search_count != 0 )
                {
                        api_func = PCB_SEARCH;
                        api_len = search_count;
                        api_retc = 1;
                        hllc(&api_func, search_string, &api_len, &api_retc);
                        if (api_retc == 0 )
                        {
                                found = 1;
                                break;
                        }
                }
                else
                {
                        found = 1;
                        break;
                }

        /*
         * check for successful search
         */
        }
        if( found != 1 )
        {
                printf( "Error 520: Search failure\n");
                return(2);
        }

        /*
         * send the key string to the emulator
         */
        if( send_count != 0 )
        {
                api_func = PCB_SEND_KEY;
                api_len = send_count;
                hllc(&api_func, send_string, &api_len, &api_retc);
                if (api_retc != PCB_RC_SEND_KEY_OK )
                {
                        printf( "Error 530: Send Key failure, code %x\n", api_retc);
                        return(3);
                }
        }
        return(0);
}



/***************************************************************************
 * Set the session parameters
 *
 * Returns:
 *              0 = no error
 *
 *************************************************************************/
send_param( send_string, send_count)
char *send_string;
int send_count;
{
        /*
         * send string
         */
        if( send_count != 0 )
        {
                api_func = PCB_SET_PARAMS;
                api_len = send_count;
                hllc(&api_func, send_string, &api_len, &api_retc);
                if (api_retc != PCB_RC_SET_PARAMS_OK )
                {
                        printf( "Error 600: Set params failure, code %x\n", api_retc);
                        return(1);
                }
        }
        return(0);
}
