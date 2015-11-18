#include <stdio.h>
#include "dotbox.h"

int main(void)
{
	unsigned int x,y;
	struct dbs_game game;
	dbf_init(&game,"Hwoy",D_SQR);
	for(y=0;y<game.sqr+1;y++)
	{
		for(x=0;x<game.sqr+1;x++)
		{
			printf("%u,%u,%u\t",game.point[y*(game.sqr+1)+x].stamp,game.point[y*(game.sqr+1)+x].y,game.point[y*(game.sqr+1)+x].x);
		}
		putchar('\n');
	}
	
	dbf_destroy(&game);
	return 0;
}

