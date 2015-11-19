#include <stdio.h>
#include "dotbox.h"
#include "dotbox_io.h"

#define LEN 4

int main(void)
{
	unsigned int x,y,i;
	struct dbs_game game;
	dbf_init(&game,"Hwoy",D_SQR+1);

	
for(i=0;i<game.sqr*(game.sqr);i++)
{
	dbf_setlinex(&game,i);
	dbf_setliney(&game,i);
}
printTable(&game,LEN);
dbf_countsqr(&game);
printf("\nsqr count = %u\n",game.count);




	dbf_destroy(&game);
	return 0;
}

