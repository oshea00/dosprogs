/*
** figures the towers of hanoi problem
**
*/

#include <stdio.h>
#include <stdlib.h>

int main(int, char *[]);
void solve_it(int,int,int,int);

long int num_moves = 0L;

int main(int argc,char *argv[])
{
    int n, source_peg = 1, aux_peg = 2, dest_peg = 3;

    if (argc != 2)
    {
	printf("Usage:\nhanoi num_disks\n");
	exit(1);
    }

    if ((n = atoi(argv[1])) < 0)
	n= -n;
    putchar('\n');

    printf("\n\nTowers of Hanoi solution for %d disks\n\n",n);

    solve_it(n,source_peg,aux_peg,dest_peg);
    printf("\nDone in %ld moves...\n",num_moves);
    exit(0);
}
void solve_it(int n,int source_peg,int aux_peg,int dest_peg)
{
    if (n!=0)
    {
	solve_it(n-1,source_peg,dest_peg,aux_peg);
	printf("Move disk %d from peg %d to peg %d\n",n,source_peg,dest_peg);
	++num_moves;
	solve_it(n-1,aux_peg,source_peg,dest_peg);
    }
}