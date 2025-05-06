/*
** This is a skeleton for API programs using the HLLAPI_N
** interface for IRMA terminal emulators
**
** Each application should begin with this code.
**
** See the sample.c and apicalls.c code for further info.
*/

#include <pcb.h>

#define MAX_TRIES 18

int api_func;    /* function requested */
int api_len;     /* length of command string */
int api_retc;    /* return code from function */

struct query
{
	char q_short_name;    /* structure defined for query sessions */
	char q_long_name[8];
	char q_sess_type;
	int  q_ps_size;
};

struct query qp[12];           /* an array qp[12] of query structures */

char parms[] = "NEWRET NOATTRB NOEAB STRLEN CONLOG ESC=@ AUTORESET SRCHALL SRCHFRWD";

char host_dca_logon[] = "LOGON <userid>";
char our_dca_logon[] = "logon dusep stu@E";

/* DATA string sizes :
** model 2  1920       (double for copy ps commands)
** model 3  2560
** model 4  3440
** model 5  3560
**
*/

/* variables used to call HLLAPI3 must be initialized.
** example
** api_func = PCB_SET_PARAMS;
** api_str = 'conphys';
** api_len = strlen(api_str);
** api_retc = 0;
**
*/

/* calls to HLLAPI can use either function name hllc, or tclim
** hllc is compatible with IBM standard EEHLLAPI
** tclim is compatible with older versions of HLLAPI3
**
** example:
** hllc(&api_func, search_string, &api_len, &api_retc)
**
** if (api_retc == 0)
** {
**     found = 1; (or return 1)
**     break;
** }
**
*/


