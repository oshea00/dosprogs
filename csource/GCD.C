#include <stdio.h>

long gcd(long,long);

main()
{
	int x,y;
		while (scanf("%ld %ld",&x,&y) != EOF)
		{
			if (x>0 && y>0)
			{
				printf("x=%ld y=%ld gcd=%ld\n",x,y,gcd(x,y));
			}
		}
}

long gcd(long u,long v)
{
	long t;
	while (u>0)
	{
		if (u<v)
		{
			t=u;
			u=v;
			v=t;
		}
		u=u%v;
	}
	return v;
}