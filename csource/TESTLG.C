#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main()
{
	char name1[20],name2[20];
	printf("\n");
	printf("Enter name 1\n");
	gets(name1);
	printf("Enter name 2\n");
	gets(name2);
	 if ((strcmpi(name1,"Mike") == 0))  {
          if ((strcmpi(name2,"George") == 0))  {
               /* rule 1 */
               printf("Incompatible Attributes");;
          }
          else {
               /* rule 2 */
               printf("Are you a democrat?");;
          }
     }
     else {
          if ((strcmpi(name2,"George") == 0))  {
               /* rule 3 */
               printf("Are you a republican?");;
          }
          else {
               if ((strcmpi(name1,"George") == 0))  {
                    if ((strcmpi(name2,"Mike") == 0))  {
                         /* rule 5 */
                         printf("Incompatible Attributes");;
                    }
                    else {
                         /* rule 6 */
                         printf("Are you a republican?");;
                    }
               }
               else {
                    if ((strcmpi(name2,"Mike") == 0))  {
                         /* rule 7 */
                         printf("Are you a democrat?");;
                    }
                    else {
                         /* rule 4 */
                         printf("Please enter Mike or George.");;
                    }
               }
          }
	 }
}

