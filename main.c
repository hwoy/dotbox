#include <stdio.h>
#include "dotbox.h"
#include "dotbox_io.h"

#define D_SQR 3
#define LEN 4

int main(void)
{
	unsigned int x,y,i;
	int n,j;
	struct dbs_game game;
	struct dbs_line line;
	
	dbf_init(&game,"Hwoy",D_SQR);

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


do
{
	
	n=game.ai(&game,&line);
	j=dbf_gameplay(&game,&line,&game.player[1]);
	
	printf("return AI = %d, (%u,%u)  (%u,%u)\n",n,line.p1.x,line.p1.y,line.p2.x,line.p2.y);
	printf("%s:Score= %u\n",game.player[1].name,game.player[1].score);
	
	printTable(&game,LEN);

}while(j!=gp_gameover);


	dbf_destroy(&game);
	return 0;
}

