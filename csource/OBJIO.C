
#include <stdio.h>

int sqrcnt;
int crccnt;
int filcnt;
int arwcnt;
int lincnt;
char squares[10];


void save_drawing(void)
{
	FILE *sqr;
	/*,*crc,*fil,*arw,*lin;*/
	int i;

	if ((sqr=fopen("base.sqr","wb+")) == NULL)
	{	printf("Cannot open output file\n");  }
/*	if ((crc=fopen("base.crc","wb+")) == NULL)
	{	printf("Cannot open output file\n");  }
	if ((fil=fopen("base.fil","wb+")) == NULL)
	{	printf("Cannot open output file\n");  }
	if ((arw=fopen("base.arw","wb+")) == NULL)
	{	printf("Cannot open output file\n");  }
	if ((lin=fopen("base.lin","wb+")) == NULL)
	{	printf("Cannot open output file\n");  }*/

	for (i=0;i<sqrcnt;i++)
	{
		write(sqr,squares[i],sizeof(square_type));
	}
/*	for (i=0;i<crccnt;i++)
	{
		write(crc,circles[i],sizeof(circle_type));
	}
	for (i=0;i<filcnt;i++)
	{
		write(fil,files[i],sizeof(file_type));
	}
	for (i=0;i<arwcnt;i++)
	{
		write(arw,arrows[i],sizeof(arrow_type));
	}
	for (i=0;i<lincnt;i++)
	{
		write(lin,lines[i],sizeof(line_type));
	}  */
	fclose(sqr);
/*	fclose(crc);
	fclose(fil);
	fclose(arw);
	fclose(lin);*/
}

void get_drawing(void)
{
}
