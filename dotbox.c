#include "dotbox.h"

struct dbs_game *dbf_init(struct dbs_game *game,const char *playername)
{
	unsigned int x,y;
	game->playername=playername;
	
	for(y=0;y<S_DOT;y++)
	{
		for(x=0;x<S_DOT;x++)
		{
			game->point[y][x].stamp=!STAMP;
			game->point[y][x].x=x;
			game->point[y][x].y=y;
		}
	}
	
	return game;
}

