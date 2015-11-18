#include <stdio.h>
#include "dotbox.h"

int main(void)
{
	unsigned int x,y;
	struct dbs_game game;
	dbf_init(&game,"Hwoy");
	for(y=0;y<S_DOT;y++)
	{
		for(x=0;x<S_DOT;x++)
		{
			printf("%u,%u,%u\t",game.point[y][x].stamp,game.point[y][x].y,game.point[y][x].x);
		}
		putchar('\n');
	}
	
	return 0;
}

