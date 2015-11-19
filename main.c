#include <stdio.h>
#include "dotbox.h"
#include "dotbox_io.h"

#define LEN 4

int main(void)
{
	unsigned int x,y,i;
	struct dbs_game game;
	struct dbs_line line;
	
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

printTable(&game,LEN);
dbf_countsqr(&game);
printf("\nsqr count = %u\n",game.count);
if(dbf_getremain_one_line(&game,&line))
{
	printf("%u,%u  %u,%u\n",line.p1.x,line.p1.y,line.p2.x,line.p2.y);
}



	dbf_destroy(&game);
	return 0;
}

