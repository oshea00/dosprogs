/*
** reads keyboard for status flags
** and prints them
*/

#include <stdio.h>
#include <dos.h>


main()
{
    long int del = 10000;
    unsigned int huge *keyb;
    keyb = 0x0417;
    fflush(stdin);
    while (del--)
    printf("\n flag at 417h  = %X",*keyb);
}