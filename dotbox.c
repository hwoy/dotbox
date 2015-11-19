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

void dbf_setpointx(struct dbs_game *game,struct dbs_point *point)
{
			game->point[point->y*(game->sqr+1)+point->x].stampx=STAMP;
}

void dbf_setpointy(struct dbs_game *game,struct dbs_point *point)
{
			game->point[point->y*(game->sqr+1)+point->x].stampy=STAMP;
}

int dbf_setlinepoint(struct dbs_game *game,struct dbs_line *line)
{
	if( ( ((int)(line->p2.x-line->p1.x))>1 || ((int)(line->p2.x-line->p1.x))<-1) && \
	(((int)(line->p2.y-line->p1.y))>1 || ((int)(line->p2.y-line->p1.y))<-1)) return -1;	
	
	if(line->p1.y==line->p2.y)
	{
		if( game->point[line->p1.y*(game->sqr+1)+line->p1.x].stampx==STAMP &&  \
		game->point[line->p2.y*(game->sqr+1)+line->p2.x].stampx==STAMP) return -2;
		
		dbf_setpointx(game,&line->p1);
		dbf_setpointx(game,&line->p2);

	}
	else if(line->p1.x==line->p2.x)
	{
		if( game->point[line->p1.y*(game->sqr+1)+line->p1.x].stampy==STAMP &&  \
		game->point[line->p2.y*(game->sqr+1)+line->p2.x].stampy==STAMP) return -3;
		
		dbf_setpointy(game,&line->p1);
		dbf_setpointy(game,&line->p2);
	}
	

	return 0;
}

void dbf_getpointlinex(struct dbs_game *game,unsigned int linenum,struct dbs_line *line)
{
	line->p1.y=linenum/(game->sqr);
	line->p1.x=linenum%(game->sqr);
	
	line->p2.y=line->p1.y;
	line->p2.x=line->p1.x+1;
}

void dbf_getpointliney(struct dbs_game *game,unsigned int linenum,struct dbs_line *line)
{
	line->p1.x=linenum/(game->sqr);
	line->p1.y=linenum%(game->sqr);
	
	line->p2.x=line->p1.x;
	line->p2.y=line->p1.y+1;
}

int dbf_setlinex(struct dbs_game *game,unsigned int linenum)
{
	struct dbs_line line;
	dbf_getpointlinex(game,linenum,&line);
	return dbf_setlinepoint(game,&line);
}

int dbf_setliney(struct dbs_game *game,unsigned int linenum)
{
	struct dbs_line line;
	dbf_getpointliney(game,linenum,&line);
	return dbf_setlinepoint(game,&line);
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

int dbf_getremain_one_line(struct dbs_game *game,struct dbs_line *line)
{
	unsigned int x,y,i,j,flag;
	for(y=0;y<game->sqr;y++)
	{
		for(x=0;x<game->sqr;x++)
		{
			flag=0;
			for(i=0;i<2;i++)
			{
				if(game->point[(y+i)*(game->sqr+1)+x].stampx==STAMP && game->point[(y+i)*(game->sqr+1)+x+1].stampx==STAMP)
					flag|=POW2A(i);
			}
			
			for(i=0;i<2;i++)
			{
				if(game->point[y*(game->sqr+1)+x+i].stampy==STAMP && game->point[(y+1)*(game->sqr+1)+x+i].stampy==STAMP)
					flag|=POW2A(i+2);
			}
			
			if(dbf_countbit(flag)==(4-1))
			{
				switch(j=dbf_postzerobit(flag))
				{
					case 0:
					case 1:
					line->p1.x=x;
					line->p1.y=y+j;
					line->p2.x=x+1;
					line->p2.y=y+j;
					break;
					case 2:
					case 3:
					line->p1.x=x+j-2;
					line->p1.y=y;
					line->p2.x=x+j-2;
					line->p2.y=y+1;
					break;
				}
				return 1;
			}
			

		}
	}
	return 0;
}

unsigned int dbf_countbit(unsigned int num)
{
	unsigned int i,j;
	for(j=0,i=0;i<(sizeof(int)*8);i++)
	{
		if(num&POW2A(i)) j++;
	}
	
	return j;
}

int dbf_postzerobit(unsigned int num)
{
	unsigned int i;
	for(i=0;i<(sizeof(int)*8);i++)
	{
		if(!(num&POW2A(i))) return i;
	}
	
	return -1;
}
