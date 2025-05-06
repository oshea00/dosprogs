/************************************************
* DLL.C - A collection of functions to manage	*
* doubly linked lists							*
*												*
* Anthony D. Ennis								*
************************************************/

#include <malloc.h>
#include <stdio.h>
#include "dll.h"

#define TRUE (1==1)
#define FALSE (!(TRUE))
#define FOREVER TRUE

/************************************************
* dllGetLastError - return the last error code	*
* from a DLL library function.  DLL_SUCCESS is	*
* returned if no errors have occurred since the	*
* last call.									*
*												*
* OUT:	DLL error status code					*
************************************************/
static dllStat _dllLastErr = DLL_SUCCESS;
dllStat dllGetLastError(void)
	{
	dllStat temp;
	temp=_dllLastErr;			/* Store error	*/
	_dllLastErr = DLL_SUCCESS;	/* Clear buffer	*/
	return temp;				/* Return error	*/
	}

/************************************************
* dllCreateList - create and initialize a list	*
* control block.								*
*												*
* OUT:	The address of a list control block, or	*
*		NULL if error							*
************************************************/
List *dllCreateList(void)
	{
	List *retval;

	/* allocate RAM for List Control Block */
	retval=(List *)malloc(sizeof(List));
	if (retval==NULL)
		{
		_dllLastErr=DLL_BAD_NORAM;
		return NULL;
		}

	/* Now initialize the list control block. */
	retval->head = NULL;
	retval->curr = NULL;

	return retval;
	}

/************************************************
* dllKillList - clear and destroy a list		*
*												*
* INP:	list - the handle of the list to kill	*
* OUT:	standard error code						*
************************************************/
dllStat dllKillList(List *list)
	{
	if (list->head != NULL)
		return _dllLastErr = DLL_LIST_NOT_EMPTY;
	free((void *)list);
	return DLL_SUCCESS;
	}

/************************************************
* dllAdd - add an element to the specified list	*
*												*
* INP:	list - the list to add to				*
*		dir - whether to insert before or after	*
*			the current node					*
*		data - pointer to the data item to be	*
*			inserted into the list				*
* OUT:	0 if no error, error code otherwise		*
************************************************/
dllStat dllAdd(List *list, int dir, void *data)
	{
	dllLink *new;

	/* Get link storage from the OS. */
	new = (dllLink *)malloc(sizeof(dllLink));
	if (!new) return _dllLastErr=DLL_BAD_NORAM;

	/* Check for degenerate list */
	if (! list->curr)
		{	/* degenerate */
	    list->head = new;
	    list->curr = new;
	    new->next = new;
	    new->prev = new;
	    new->data = data;
	    }
	else
		{	/* non-degenerate */
		if (dir == DLL_PREV)
			{
			/* Inserting before the head check	*/
			if (list->curr == list->head)
				list->head = new;
			/* Move current pointer so all adds	*/
			/* are 'add afters'.				*/
			list->curr = list->curr->prev;
			}

		/* Now we can do an 'add after' */
	    new->next = list->curr->next;
	    new->prev = list->curr;
	    new->next->prev = new;
	    new->prev->next = new;

	    /* And assign the user's data item. */
	    new->data = data;

	    /* and mark it as the current element. */
	    list->curr = new;
	    }

	return DLL_SUCCESS;
	}

/************************************************
* dllDelete - delete an item from the list, and	*
* return a pointer to it's data.				*
*												*
* INP:	list - the list to delete from			*
*		direction - whether the previous or next*
*			item will become current after del.	*
* OUT:	pointer to user's data, or NULL on error*
************************************************/
void *dllDelete(List *list,Position direction)
	{
	void *retval;
	dllLink *tmp;

	/* Check for empty list */
	if (!list->head || !list->curr)
		{
		_dllLastErr = DLL_EMPTY_LIST;
		return NULL;
		}

	/* Capture the address of the data. */
	retval = list->curr->data;

	/* Is head being deleted? */
	if (list->curr==list->head)
		{	/* Yes, delete head, choose new one	*/
		if (direction == DLL_NEXT)
			list->head = list->curr->next;
		else	
			list->head = list->curr->prev;

		/* Was the head the only element? */
	    if (list->curr == list->head)
	    	list->head = NULL;	/* Yes */
	    }		

	/* Remove and free the link, only if we did	*/
	/* not remove it (if only link)				*/
	if (list->head)
		{	
	    list->curr->prev->next = list->curr->next;
	    list->curr->next->prev = list->curr->prev;
	    }

	/* Return the deleted link to the OS. */
	tmp = list->curr;
	if (direction == DLL_NEXT)
		list->curr = list->curr->next;
	else
		list->curr = list->curr->prev;
	free((void *)tmp);
	return retval;
	}

/************************************************
* dllGetData - get address of data at cur elem	*
*												*
* OUT:	address of data, NULL on error			*
************************************************/
void *dllGetData(List *list)
    {
    /* Make the easy check. */
    if (!list)
    	{
    	_dllLastErr = DLL_BAD_LIST;
    	return NULL;
    	}

    return list->curr->data;
    }

/************************************************
* dllSetPos - set the current element pointer	*
* to one of the four major positions			*
*												*
* INP:	list - the list to adjust				*
*		pos  - the position to use				*
* OUT:	the error code (0=success)				*
************************************************/
Position dllSetPos(List *list, Position dir)
	{
	/* A degenerate list is always ready. */
	if (!list->head) return DLL_TAIL;

	if (dir == DLL_NEXT)
		list->curr = list->curr->next;
	else if (dir == DLL_PREV)
		list->curr = list->curr->prev;
	else if (dir == DLL_HEAD)
		list->curr = list->head;
	else if (dir == DLL_TAIL)
		list->curr = list->head->prev;

	/* Change the return code, if major pt found*/
	if (list->curr==list->head)
		dir=DLL_HEAD;
	else if (list->curr==list->head->prev)
		dir=DLL_TAIL;

	return dir;
    }

/************************************************
* dllSearch - search list for specific node		*
*												*
* INP: 	list - the list to be searched			*
*		start - starting point of search		*
*		end - ending point of search			*
*		dir - direction of search				*
*		cmp - comparison function to guide srch	*
*		udata - ptr to key data for search		*
************************************************/

#define SET_POS(var, ptr) if (var == DLL_HEAD) \
	ptr = list->head; else if (var == DLL_TAIL)\
	ptr = list->head->prev;	else if (var ==    \
	DLL_CURR) ptr = list->curr;

#define CHECK_POS(var) (var != DLL_HEAD && var \
	!= DLL_TAIL && var != DLL_CURR)

dllStat dllSearch(List *list, Position begin,
	Position stop, Position dir,
	int (*cmp)(void *, void *),void *udata)
	{
	int cmp_status;		/* status of comparison */
	dllLink *end;		/* last element to test */
	dllLink *save_curr;	/* save current element	*/
						/* in case of no match	*/

	/* Check search direction */
	if (dir != DLL_PREV && dir != DLL_NEXT)
		return _dllLastErr=DLL_BAD_DIRECTION;

	/* Ensure begin & stop positions are valid	*/
	if CHECK_POS(begin)
		return _dllLastErr=DLL_BAD_START;
	if CHECK_POS(stop)
		return _dllLastErr=DLL_BAD_END;

	/* Check for reasonable list & search function*/
	if (!list)
		return _dllLastErr=DLL_BAD_LIST;
	if (!cmp)
		return _dllLastErr=DLL_BAD_CMPFUNC;

	/* Store current position, in case search	*/
	/* fails & it needs to be restored.			*/
	save_curr = list->curr;

	/* Initialize starting and ending positions	*/
	SET_POS(begin, list->curr);
	SET_POS(stop, end);

	/* Make sure list is not empty */
	if (!list->curr)
		return _dllLastErr=DLL_NOMATCH;
	
	/* Perform the actual search */
	while( FOREVER )
		{
		cmp_status=(*cmp)(list->curr->data, udata);

		/* Did we find a match? */
		if ( cmp_status == DLL_MATCH)
			{
			/* If last element, return warning	*/
			if (list->curr == end)
				return _dllLastErr=DLL_MATCH_EOL;
			else
				return _dllLastErr=DLL_MATCH;
			}

		/* Did we have a bad parameter? */
		else if (cmp_status == DLL_BAD_PARAM)
			return _dllLastErr=DLL_BAD_PARAM;

		/* Did we find the end of the list? */
		else if (list->curr == end)
			{
			/* Restore the old current pointer. */
			list->curr = save_curr;
			return _dllLastErr=DLL_NOMATCH;
			}
		else	/* Advance to the next item. */
			{
			if (dir == DLL_PREV)
				list->curr = list->curr->prev;
			else
				list->curr = list->curr->next;
			}
		}
	}
