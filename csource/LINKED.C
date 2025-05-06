#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct NODE {
  int value;
  struct NODE* next;
} NODE;


/*
[1,2,[3,[4,5]],6,[7,8,9],10]
*/

NODE *createList();
void destroyList(NODE *head);
void addNode(NODE *head, NODE *node);
void printList(NODE *head);
int  isTail(NODE *node);
NODE *findItem(NODE *head, int value);
void deleteItems(NODE *head, int value);
void deleteItem(NODE *head, NODE *item);

void main() {
	int items[] = {1,2,3,3,4,5};
	int itemCnt = 6;
	int i=0;
	int search = 3;
	int ch;
	NODE *list;
	NODE *node;

	/* Create list */
	list = createList();

	/* do stuff with list */
	for (i=0;i<itemCnt;i++)
	{
		node = malloc(sizeof(NODE));
		node->value = items[i];
		addNode(list,node);
	}
	printList(list);

	search = 3;
	deleteItems(list, search);

	printList(list);

	search = 9;
	deleteItems(list, search);


	/* cleanup list */
	destroyList(list);


	ch = getch();
}

void deleteItems(NODE *head, int value) {
	NODE *node;
	node = findItem(head, value);
	if (node == NULL) {
		printf("%d Not Found.\n",value);
		return;
	}
	while (node != NULL)
	{
		printf("Found %d.\n",value);
		deleteItem(head, node);
		node = findItem(head, value);
	}
	printf("Removed all %d.\n",value);
}

void addNode(NODE *head, NODE* node) {
	NODE *prev = head;
	NODE *next = head->next;
	while (!isTail(next))
	{
		prev = next;
		next = next->next;
	}
	prev->next = node;
	node->next = next;
}


NODE *createList() {
	NODE *head;
	NODE *tail;
	head = malloc(sizeof(NODE));
	tail = malloc(sizeof(NODE));
	head->next = tail;
	tail->next = tail;
	return head;
}

void destroyList(NODE *head) {
	NODE *prev = head;
	NODE *next = head->next;
	while (!isTail(next))
	{
		prev = next;
		next = next->next;
		free(prev);

	}
	free(next);
	free(head);
}

void printList(NODE *head) {
	NODE *prev = head;
	NODE *next = head->next;
	while (!isTail(next))
	{
		prev = next;
		next = next->next;
		printf("%d\n",prev->value);
	}
}

NODE *findItem(NODE *head, int value) {
	NODE *prev = head;
	NODE *next = head->next;
	while (!isTail(next))
	{
		prev = next;
		next = next->next;
		if (prev->value == value)
			return prev;
	}
	return NULL;
}

void deleteItem(NODE *head, NODE *item) {
	NODE *prev = head;
	NODE *next = head->next;
	while (!isTail(next))
	{
		prev = next;
		next = next->next;
		if (next == item)
		{
			prev->next = next->next;
			free(next);
			break;
		}
	}
}

int isTail(NODE *node) {
	if (node->next == node)
		return 1;
	else
		return 0;
}

