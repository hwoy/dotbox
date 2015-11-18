#include <stdlib.h>
#include "dotbox.h"


struct dbs_game *dbf_init(struct dbs_game *game,const char *playername,unsigned int sqr)
{
	unsigned int x,y;
	game->playername=playername;
	game->sqr=sqr;
	game->point=(struct dbs_point *)malloc(sizeof(struct dbs_point)*(sqr+1)*(sqr+1));
	if(!game->point) return NULL;
	
	for(y=0;y<game->sqr+1;y++)
	{
		for(x=0;x<game->sqr+1;x++)
		{
			game->point[y*(game->sqr+1)+x].stamp=!STAMP;
			game->point[y*(game->sqr+1)+x].x=x;
			game->point[y*(game->sqr+1)+x].y=y;
		}
	}
	
	return game;
}

void *dbf_destroy(struct dbs_game *game)
{
	free(game->point);
}

void dbf_setpoint(struct dbs_game *game,unsigned int x,unsigned int y)
{
			game->point[y*(game->sqr+1)+x].stamp=STAMP;
			game->point[y*(game->sqr+1)+x].x=x;
			game->point[y*(game->sqr+1)+x].y=y;
}

int dbf_setline(struct dbs_game *game,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	if( ( ((int)(x2-x1))>1 || ((int)(x2-x1))<-1) &&(((int)(y2-y1))>1 || ((int)(y2-y1))<-1)) return -1;	
	if( game->point[y1*(game->sqr+1)+x1].stamp==STAMP ||  game->point[y2*(game->sqr+1)+x2].stamp==STAMP) return -2;
	
dbf_setpoint(game,x1,y1);
dbf_setpoint(game,x1,y1);

	return 0;
}

