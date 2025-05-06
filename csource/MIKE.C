/*  set 1 excercises
**  Mike O'Shea Monday class
*/

main()
{
	ex1();
	ex2();
	ex3();
	ex6();
	ex7();
}

ex1()
{
	printf("EXERCISE 1 part a,b and c\n\n");
	printf("She sells C shells by the C shore.\n");
	printf("She\nsells\nC\nshells\nby\nthe\nC\nshore.\n");
	printf("*************************************\n");
	printf("* She sells C shells by the C shore *\n");
	printf("*************************************\n");
}
ex2()
{
	int a,b;
	printf("EXERCISE 2\n\n");
	printf("Hi there, type two integers seperated by a comma\n");
	scanf("%d,%d",&a,&b);
	printf("%d + %d = %d\n",a,b,a+b);
}
ex3()
{
	int age;
	long int hours, seconds;
	char name[30];
	printf("EXERCISE 3\n\n");
	printf("Please type your first name: ");
	scanf("%s",name);
	printf("\nThank you %s.\n",name);
	printf("Please type your age: ");
	scanf("%d",&age);
	hours = (age * 365L) * 24;
	seconds = hours * 1800;
	printf("Hello %s, you are you are approximately %ld hours old\n",name,hours);
	printf("which is equivalent to %ld seconds\n",seconds);
}
ex6()
{
	long int x1,x2,x3,x4,x5,x6,x7,sum;
	float avg;
	printf("EXERCISE 6\n\n");
	printf("type seven integers seperated by spaces: ");
	scanf("%ld %ld %ld %ld %ld %ld %ld",&x1,&x2,&x3,&x4,&x5,&x6,&x7);
	sum = x1 + x2 + x3 + x4 + x5 + x6 + x7;
	avg = sum / 7.0;
	printf("The sum is: %ld\n",sum);
	printf("The average is: %g\n",avg);
}
ex7()
{
	float degreeF, degreeC;
	printf("EXERCISE 7\n\n");
	printf("Type a temperature in degrees Fahrenheit: ");
	scanf("%f",&degreeF);
	degreeC = (5.0 / 9.0) * (degreeF - 32);
	printf("That is: %-6.2g degrees Centigrade",degreeC);
}
