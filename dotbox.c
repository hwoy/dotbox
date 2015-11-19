#include <stdlib.h>
#include "dotbox.h"


struct dbs_game *dbf_init(struct dbs_game *game,const char *playername,unsigned int sqr)
{
	unsigned int x,y;
	game->playername=playername;
	game->sqr=sqr;
	game->count=0;
	game->point=(struct dbs_point *)malloc(sizeof(struct dbs_point)*(sqr+1)*(sqr+1));
	if(!game->point) return NULL;
	
	for(y=0;y<game->sqr+1;y++)
	{
		for(x=0;x<game->sqr+1;x++)
		{
			game->point[y*(game->sqr+1)+x].stampx=UNSTAMP;
			game->point[y*(game->sqr+1)+x].stampy=UNSTAMP;
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

void dbf_setpointx(struct dbs_game *game,unsigned int x,unsigned int y)
{
			game->point[y*(game->sqr+1)+x].stampx=STAMP;
}

void dbf_setpointy(struct dbs_game *game,unsigned int x,unsigned int y)
{
			game->point[y*(game->sqr+1)+x].stampy=STAMP;
}

int dbf_setlinepoint(struct dbs_game *game,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	if( ( ((int)(x2-x1))>1 || ((int)(x2-x1))<-1) &&(((int)(y2-y1))>1 || ((int)(y2-y1))<-1)) return -1;	
	
	if(y1==y2)
	{
		if( game->point[y1*(game->sqr+1)+x1].stampx==STAMP &&  game->point[y2*(game->sqr+1)+x2].stampx==STAMP) return -2;
		dbf_setpointx(game,x1,y1);
		dbf_setpointx(game,x2,y2);

	}
	else if(x1==x2)
	{
		if( game->point[y1*(game->sqr+1)+x1].stampy==STAMP &&  game->point[y2*(game->sqr+1)+x2].stampy==STAMP) return -3;
		dbf_setpointy(game,x1,y1);
		dbf_setpointy(game,x2,y2);
	}
	

	return 0;
}

void dbf_getpointx(struct dbs_game *game,unsigned int linenum,struct dbs_point *p1,struct dbs_point *p2)
{
	p1->y=linenum/(game->sqr);
	p1->x=linenum%(game->sqr);
	
	p2->y=p1->y;
	p2->x=p1->x+1;
}

void dbf_getpointy(struct dbs_game *game,unsigned int linenum,struct dbs_point *p1,struct dbs_point *p2)
{
	p1->x=linenum/(game->sqr);
	p1->y=linenum%(game->sqr);
	
	p2->x=p1->x;
	p2->y=p1->y+1;
}

int dbf_setlinex(struct dbs_game *game,unsigned int linenum)
{
	struct dbs_point p1,p2;
	dbf_getpointx(game,linenum,&p1,&p2);
	return dbf_setlinepoint(game,p1.x,p1.y,p2.x,p2.y);
}

int dbf_setliney(struct dbs_game *game,unsigned int linenum)
{
	struct dbs_point p1,p2;
	dbf_getpointy(game,linenum,&p1,&p2);
	return dbf_setlinepoint(game,p1.x,p1.y,p2.x,p2.y);
}

void dbf_srandom (void)
{
  srand (time (NULL));
}


int dbf_rand(void)
{
#ifdef _DEVRAND_
        FILE           *fp;
        int             i,j,k;
        fp = fopen(DEVRAND, "rb");
        if (!fp)
                return 0;
		for(k=0,j=sizeof(char);j<sizeof(i);j+=sizeof(char),k++)
        ((char *)&i)[k] = fgetc(fp);
	
        fclose(fp);
        return i;
#else
                        return  rand   ();
#endif
}

int dbf_random (int min, int max)
{
  return min <= max ? min + (dbf_rand () % (max - min + 1)) : -1;
}

int dbf_countsqr(struct dbs_game *game)
{
	unsigned int x,y,count;
	count=game->count;
	for(y=0;y<game->sqr;y++)
	{
		for(x=0;x<game->sqr;x++)
		{
			if(game->point[y*(game->sqr+1)+x].stampx==STAMP && game->point[y*(game->sqr+1)+x+1].stampx==STAMP &&\
			   game->point[(y+1)*(game->sqr+1)+x].stampx==STAMP && game->point[(y+1)*(game->sqr+1)+x+1].stampx==STAMP &&\
			   game->point[y*(game->sqr+1)+x].stampy==STAMP && game->point[(y+1)*(game->sqr+1)+x].stampy==STAMP &&\
			   game->point[y*(game->sqr+1)+x+1].stampy==STAMP && game->point[(y+1)*(game->sqr+1)+x+1].stampy==STAMP
			   )
			   game->count++;
		}
	}
return game->count-count;	
}
