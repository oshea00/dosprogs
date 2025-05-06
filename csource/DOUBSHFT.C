/*
** generates a double shift combination
*/

#include <stdio.h>
#include <dos.h>


main()
{
    unsigned int huge *keyb;  /* assign a pointer to 0000:0417h */
    keyb = 0x0417;
    *keyb = 0x03;             /* set both shift flags to 1  */
}