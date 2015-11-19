#include <stdio.h>
#include "dotbox.h"
#include "dotbox_io.h"

#define LEN 4

int main(void)
{
	unsigned int x,y,i,j;
	struct dbs_game game;
	struct dbs_line line[(D_SQR+1)*(D_SQR+1)];
	
	dbf_init(&game,"Hwoy",D_SQR+1);

/*	
for(i=0;i<game.sqr*(game.sqr+1);i++)
{
	dbf_setlinex(&game,i);
	dbf_setliney(&game,i);
*/
/*dbf_setlinex(&game,0);*/
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
dbf_countsqr(&game);
printf("\nsqr count = %u\n",game.count);
i=dbf_getremainline(&game,line,2);
for(j=0;j<i;j++)
{
	dbf_setlinepoint(&game,&line[j]);
	printf("%u,%u  %u,%u\n",line[j].p1.x,line[j].p1.y,line[j].p2.x,line[j].p2.y);
}
printTable(&game,LEN);


	dbf_destroy(&game);
	return 0;
}

