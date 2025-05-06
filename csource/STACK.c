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

NODE *createStack();
void destroyStack(NODE *head);
void push(NODE *head, int value);
int  pop(NODE *head);
void dumpStack(NODE *head);
int  isTail(NODE *node);
int  isEmpty(NODE *node);
NODE *findItem(NODE *head, int value);
void deleteItems(NODE *head, int value);
void deleteItem(NODE *head, NODE *item);

void main() {
	int items[] = {1,2,3,3,4,5};
	int itemCnt = sizeof(items) / sizeof(items[0]);
	int i=0;
	int search;
	NODE *stack;
	NODE *node;

	stack = createStack();

	/* do stuff with stack */
	for (i=0;i<itemCnt;i++)
	{
		push(stack,items[i]);
	}
	printf("Dumping stack...\n");
	dumpStack(stack);

	search = 3;
	deleteItems(stack, search);

	printf("Dumping stack...\n");
	dumpStack(stack);

	search = 9;
	deleteItems(stack, search);


	printf("Pop remaining items:\n");
	while (!isEmpty(stack))
	{
		printf("popped: %d\n",pop(stack));
	}

	printf("Dumping stack...\n");
	dumpStack(stack);
	if (isEmpty(stack))
		printf("Stack empty.\n");


	printf("Hit any key to exit.\n");
	getch();

	/* cleanup list */
	printf("Cleaning up...\n");
	destroyStack(stack);
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

void push(NODE *head, int value) {
	NODE *node = malloc(sizeof(NODE));
	node->value = value;
	node->next = head->next;
	head->next = node;
}


NODE *createStack() {
	NODE *head;
	NODE *tail;
	head = malloc(sizeof(NODE));
	tail = malloc(sizeof(NODE));
	head->next = tail;
	tail->next = tail;
	return head;
}

void destroyStack(NODE *head) {
	while (!isEmpty(head))
	{
		pop(head);
	}
	free(head->next);
	free(head);
}

void dumpStack(NODE *head) {
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

int isEmpty(NODE *node) {
	if (node->next->next == node->next)
		return 1;
	else
		return 0;
}

int pop(NODE *node)
{
	int value;
	NODE *item = node->next;
	value = item->value;
	node->next = item->next;
	free(item);
	return value;
}

