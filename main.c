#include <stdio.h>
#include "dotbox.h"
#include "dotbox_io.h"

int main(void)
{
	unsigned int x,y;
	struct dbs_game game;
	dbf_init(&game,"Hwoy",D_SQR+1);
	/*
	for(y=0;y<game.sqr+1;y++)
	{
		for(x=0;x<game.sqr+1;x++)
		{
			printf("%u,%u,%u\t",game.point[y*(game.sqr+1)+x].stamp,game.point[y*(game.sqr+1)+x].y,game.point[y*(game.sqr+1)+x].x);
		}
		putchar('\n');
	}
	*/
	/*
	for(y=0;y<game.sqr+1;y++)
	{
		for(x=0;x<game.sqr+1;x++)
		{
			game.point[y*(game.sqr+1)+x].stamp=STAMP;
		}
	}
	*/
	printTable(&game,4);
	
	dbf_destroy(&game);
	return 0;
}

