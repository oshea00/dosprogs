/*
** FARTHEST.C
**
*/

int max_xdistance = 0;
int max_ydistance = 0;

void farthestxy(void);

void farthestxy(void)
{
	int i;
	int xdistance;
	int ydistance;

	/* Locate farthest object */

	if (sqrcnt > 0)
	{
		for (i=0;i<sqrcnt;i++)
		{
			xdistance = squares[i].x;
			ydistance = squares[i].y;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}

	if (crccnt > 0)
	{
		for (i=0;i<crccnt;i++)
		{
			xdistance = circles[i].x;
			ydistance = circles[i].y;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}

	if (filcnt > 0)
	{
		for (i=0;i<filcnt;i++)
		{
			xdistance = files[i].x;
			ydistance = files[i].y;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}

	if (arwcnt > 0)
	{
		for (i=0;i<arwcnt;i++)
		{
			xdistance = arrows[i].x1;
			ydistance = arrows[i].y1;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
			xdistance = arrows[i].x2;
			ydistance = arrows[i].y2;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}

	if (lincnt > 0)
	{
		for (i=0;i<lincnt;i++)
		{
			xdistance = lines[i].x1;
			ydistance = lines[i].y1;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
			xdistance = lines[i].x2;
			ydistance = lines[i].y2;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}
}

