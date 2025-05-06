/************************************************
* TICKET.C - DLL functions example program		*
* Print list of speeders						*
************************************************/

#include <stdio.h>		/* fputs()				*/
#include <stdlib.h>		/* exit()				*/
#include "dll.h"

#define ERROR(x) {fputs(x,stderr); exit(1); }
#define MAXNAME		40	/* Maximum name length	*/
#define TRUE		(1==1)

typedef struct _spdrec {
	char name[MAXNAME];	/* Person's name		*/
	int  speed;			/* Average driving speed*/
} SpdRec;

/************************************************
* BuildList - read name & average speed from	*
* input, and build linked list					*
************************************************/
List *BuildList(void) {
	List *l;
	SpdRec *tmp;

	/* Create list */
	l = dllCreateList();
	if (l==NULL) ERROR("Can't create list!")

	/* Read input & store until no more left */
	while (TRUE) {
		tmp = (SpdRec *)malloc(sizeof(SpdRec));
		if (tmp == NULL) ERROR("Ran out of RAM!")
		if (scanf("%d %38s",&tmp->speed,tmp->name)
					 == EOF) {
			free(tmp);	/* don't need last one	*/
			break;
		}
		if (dllAdd(l,DLL_NEXT,tmp))
			ERROR("Error adding to list!")
	}
	return l;
}

/************************************************
* PrintList - print all the data in the list	*
************************************************/
void PrintList(List *list) {
	SpdRec *tmp;

	dllSetPos(list,DLL_HEAD);
	do	{
		tmp=(SpdRec *)dllGetData(list);
		printf("Name: %s, speed: %d\n",
				tmp->name,  tmp->speed);
	} while (dllSetPos(list,DLL_NEXT) != DLL_HEAD);
}

/************************************************
* _KOShelper - the search function that finds	*
* people who obey the speed limit				*
************************************************/
int _KOShelper(void *k1, void *k2) {
	if (((SpdRec *)k1)->speed<=55) return DLL_MATCH;
	return DLL_NOMATCH;
}

/************************************************
* KeepOnlySpeeders - find all non-speeders, and	*
* delete them from the list						*
************************************************/
void KeepOnlySpeeders(List *lst) {
	dllStat tmp;

	dllSetPos(lst, DLL_HEAD);
	do	{
		tmp = dllSearch(lst, DLL_CURR, DLL_TAIL,
				DLL_NEXT,_KOShelper, NULL);
		if (tmp==DLL_MATCH || tmp==DLL_MATCH_EOL)
			dllDelete(lst, DLL_NEXT);
		} while (tmp!=DLL_MATCH_EOL &&
				 tmp!=DLL_NOMATCH);
}

void main(void) {
	List *SpeedList;
	SpeedList=BuildList();
	puts("All traffic information:");
	PrintList(SpeedList);
	KeepOnlySpeeders(SpeedList);
	puts("\nAll lawbreakers found:");
	PrintList(SpeedList);
}
