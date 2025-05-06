/*
** A 'C' routine called from prolog
*/

/* Note that the storage class identifier 'extern' on the zwf
** function is optional.  It is used here to remind someone that
** this function is defined in another obj file.  In this case
** Prolog.
*/

extern void zwf(char *, ...);
void sum_0(double,double,double *);


/* the _0 is to denote that this function defines the first flow
** pattern denoted in the calling Prolog program.  Subsequent flow
** patterns are assigned left to right to _1, _2 etc. in the 'C'
** program.  This allows writing functions in 'C' for any desired flow
** pattern and having the resultant function appear 'Prolog-like' in
** handling seperate flow patterns.
*/

void sum_0(double flt1,double flt2,double *result)
{
	*result = flt1 + flt2;
	zwf("This is the sum function: parm1 = %g\nparm2 = %g\n",flt1,flt2);
	zwf("The sum is: %g",*result);
}
