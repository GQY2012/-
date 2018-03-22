#include "stdio.h"
#define g 9.795
#define L 0.973
#define D 0.000201

main()
{
	float m,x1,x2;
	int flag = 1;
	while(flag)
	{
		scanf("%f%f%f%d",&m,&x1,&x2,&flag);
		printf("%f",m*g*L/((3.1415926/4)*D*D*(x2 - x1)));
	}
 } 
