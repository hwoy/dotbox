#include <stdio.h>
#include "dotbox.h"
#include "dotbox_io.h"

#define LEN 4

int main(void)
{
	unsigned int x,y,i,j;
	int n;
	struct dbs_game game;
	struct dbs_line line;
	
	dbf_init(&game,"Hwoy",D_SQR+2);

dbf_setlinex(&game,3);
dbf_setliney(&game,0);
dbf_setliney(&game,3);

dbf_setlinex(&game,1);
dbf_setliney(&game,6);

dbf_setlinex(&game,8);
dbf_setlinex(&game,11);
dbf_setliney(&game,8);

dbf_setlinex(&game,2);


printTable(&game,LEN);


while((n=dbf_ai(&game,&line))>-1)
{
	printf("================================");
	dbf_setlinepoint(&game,&line);
	printTable(&game,LEN);
	dbf_setlinepoint(&game,&line);
	printf("return AI = %d %u,%u  %u,%u\n",n,line.p1.x,line.p1.y,line.p2.x,line.p2.y);
}


	dbf_destroy(&game);
	return 0;
}

