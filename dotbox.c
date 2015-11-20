#include <stdlib.h>
#include <time.h>
#include "dotbox.h"

static const char CNAME[]="COMPUTER";

struct dbs_game *dbf_init(struct dbs_game *game,const char *playername,unsigned int sqr)
{
	unsigned int x,y;
	
	game->point=(struct dbs_point *)malloc(sizeof(struct dbs_point)*(sqr+1)*(sqr+1));
	if(!game->point) return NULL;
	
	#ifndef _DEVRAND_
	dbf_srandom(time(NULL));
	#endif
	
	game->player[0].name=playername;
	game->player[0].score=0;
	
	#ifdef _COMNAME_
		for(x=0;COMNAME[x];x++);
		game->player[1].name=x?COMNAME[dbf_random(0,x-1)]:CNAME;
	#else
		game->player[1].name=CNAME;
	#endif
	
	game->player[1].score=0;	
	
	game->sqr=sqr;
	game->ai=dbf_ai;
	
	for(y=0;y<game->sqr+1;y++)
	{
		for(x=0;x<game->sqr+1;x++)
		{
			game->point[y*(game->sqr+1)+x].prev_x=UNSTAMP;
			game->point[y*(game->sqr+1)+x].next_x=UNSTAMP;
			game->point[y*(game->sqr+1)+x].prev_y=UNSTAMP;
			game->point[y*(game->sqr+1)+x].next_y=UNSTAMP;
			game->point[y*(game->sqr+1)+x].x=x;
			game->point[y*(game->sqr+1)+x].y=y;
		}
	}
	
	return game;
}

void dbf_destroy(struct dbs_game *game)
{
	free(game->point);
}




void dbf_setpoint_prev_x(struct dbs_game *game,struct dbs_point *point)
{
			game->point[point->y*(game->sqr+1)+point->x].prev_x=STAMP;
}
void dbf_setpoint_next_x(struct dbs_game *game,struct dbs_point *point)
{
			game->point[point->y*(game->sqr+1)+point->x].next_x=STAMP;
}

void dbf_setpoint_prev_y(struct dbs_game *game,struct dbs_point *point)
{
			game->point[point->y*(game->sqr+1)+point->x].prev_y=STAMP;
}
void dbf_setpoint_next_y(struct dbs_game *game,struct dbs_point *point)
{
			game->point[point->y*(game->sqr+1)+point->x].next_y=STAMP;
}




int dbf_setlinepoint(struct dbs_game *game,struct dbs_line *line)
{
	if( ( ((int)(line->p2.x-line->p1.x))>1 || ((int)(line->p2.x-line->p1.x))<-1) && \
	(((int)(line->p2.y-line->p1.y))>1 || ((int)(line->p2.y-line->p1.y))<-1)) return gp_invline;	
	
	if(line->p1.y==line->p2.y)
	{
		if( game->point[line->p1.y*(game->sqr+1)+line->p1.x].next_x==STAMP &&  \
		game->point[line->p2.y*(game->sqr+1)+line->p2.x].prev_x==STAMP) return gp_invx;
		
		dbf_setpoint_next_x(game,&line->p1);
		dbf_setpoint_prev_x(game,&line->p2);

	}
	else if(line->p1.x==line->p2.x)
	{
		if( game->point[line->p1.y*(game->sqr+1)+line->p1.x].next_y==STAMP &&  \
		game->point[line->p2.y*(game->sqr+1)+line->p2.x].prev_y==STAMP) return gp_invy;
		
		dbf_setpoint_next_y(game,&line->p1);
		dbf_setpoint_prev_y(game,&line->p2);
	}
	

	return 0;
}


struct dbs_line *dbf_getpointlinex(struct dbs_game *game,unsigned int linenum,struct dbs_line *line)
{
	line->p1.y=linenum/(game->sqr);
	line->p1.x=linenum%(game->sqr);
	
	line->p2.y=line->p1.y;
	line->p2.x=line->p1.x+1;
	
	line->p1.next_x=game->point[line->p1.y*(game->sqr+1)+line->p1.x].next_x;
	line->p1.prev_x=game->point[line->p1.y*(game->sqr+1)+line->p1.x].prev_x;
	
	line->p2.next_x=game->point[line->p2.y*(game->sqr+1)+line->p2.x].next_x;
	line->p2.prev_x=game->point[line->p2.y*(game->sqr+1)+line->p2.x].prev_x;
	
	return line;
}

struct dbs_line *dbf_getpointliney(struct dbs_game *game,unsigned int linenum,struct dbs_line *line)
{
	line->p1.x=linenum/(game->sqr);
	line->p1.y=linenum%(game->sqr);
	
	line->p2.x=line->p1.x;
	line->p2.y=line->p1.y+1;
	
	line->p1.next_y=game->point[line->p1.y*(game->sqr+1)+line->p1.x].next_y;
	line->p1.prev_y=game->point[line->p1.y*(game->sqr+1)+line->p1.x].prev_y;
	
	line->p2.next_y=game->point[line->p2.y*(game->sqr+1)+line->p2.x].next_y;
	line->p2.prev_y=game->point[line->p2.y*(game->sqr+1)+line->p2.x].prev_y;
	

	
	return line;
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

void dbf_srandom (int seed)
{
  srand (seed);
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

unsigned int dbf_countsqr(struct dbs_game *game)
{
	unsigned int x,y,count;
	count=0;
	for(y=0;y<game->sqr;y++)
	{
		for(x=0;x<game->sqr;x++)
		{
			if(game->point[y*(game->sqr+1)+x].next_x==STAMP && game->point[y*(game->sqr+1)+x+1].prev_x==STAMP &&\
			   game->point[(y+1)*(game->sqr+1)+x].next_x==STAMP && game->point[(y+1)*(game->sqr+1)+x+1].prev_x==STAMP &&\
			   game->point[y*(game->sqr+1)+x].next_y==STAMP && game->point[(y+1)*(game->sqr+1)+x].prev_y==STAMP &&\
			   game->point[y*(game->sqr+1)+x+1].next_y==STAMP && game->point[(y+1)*(game->sqr+1)+x+1].prev_y==STAMP
			   )
			   count++;
		}
	}
return count;	
}

unsigned int dbf_getremainline(struct dbs_game *game,struct dbs_line *line,unsigned int rcount)
{
	unsigned int x,y,i,n,flag,count;
	
	count=0;
	for(y=0;y<game->sqr;y++)
	{
		for(x=0;x<game->sqr;x++)
		{
			flag=0;
			for(i=0;i<2;i++)
			{
				if(game->point[(y+i)*(game->sqr+1)+x].next_x==STAMP && game->point[(y+i)*(game->sqr+1)+x+1].prev_x==STAMP)
					flag|=POW2A(i);
			}
			
			for(i=0;i<2;i++)
			{
				if(game->point[y*(game->sqr+1)+x+i].next_y==STAMP && game->point[(y+1)*(game->sqr+1)+x+i].prev_y==STAMP)
					flag|=POW2A(i+2);
			}
			
			if(dbf_countbit(flag)==(4-rcount))
			{
				for(n=0;n<4;n++)
				{
					if(!(flag&POW2A(n)))
					{
				switch(n)
				{
					case 0:
					case 1:
					line[count].p1.x=x;
					line[count].p1.y=y+n;
					line[count].p2.x=x+1;
					line[count].p2.y=y+n;
					break;
					case 2:
					case 3:
					line[count].p1.x=x+n-2;
					line[count].p1.y=y;
					line[count].p2.x=x+n-2;
					line[count].p2.y=y+1;
					break;
				}
				count++;
					}
				}
			}
			

		}
	}
	return count;
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

int dbf_issetline(struct dbs_game *game,struct dbs_line *line)
{
	if(line->p1.y==line->p2.y)
	{
		if(game->point[line->p1.y*(game->sqr+1)+line->p1.x].next_x==STAMP && game->point[line->p2.y*(game->sqr+1)+line->p2.x+1].prev_x==STAMP)
			return 1;
	}
	else if(line->p1.x==line->p2.x)
	{
		if(game->point[line->p1.y*(game->sqr+1)+line->p1.x].next_y==STAMP && game->point[(line->p2.y+1)*(game->sqr+1)+line->p2.x].prev_y==STAMP)
			return 2;
	}
	
	return 0;
}

struct dbs_line *dbf_copyline(struct dbs_line *dsk,struct dbs_line *src)
{
	dsk->p1.x=src->p1.x;
	dsk->p1.y=src->p1.y;
	
	dsk->p2.x=src->p2.x;
	dsk->p2.y=src->p2.y;
	
	return dsk;
}

int dbf_ai(struct dbs_game *game,struct dbs_line *line)
{
	struct dbs_line *lbuff;
	unsigned int i,j;
	
	lbuff=(struct dbs_line *)malloc(sizeof(struct dbs_line)* (game->sqr)*(game->sqr) *4);
	if(!lbuff) return ai_errmalloc;
	
	for(j=1;j<=4;j+=2)
	{
	if((i=dbf_getremainline(game,lbuff,j)))
	{
		dbf_copyline(line,&lbuff[dbf_random(0,i-1)]);
		free(lbuff);
		return (j==1)?ai_best:ai_random;
	}
	}
	
	for(j=4;j>=1;j-=2)
	{
	if((i=dbf_getremainline(game,lbuff,j)))
	{
		dbf_copyline(line,&lbuff[dbf_random(0,i-1)]);
		free(lbuff);
		return (j==2)?ai_worse:ai_random;
	}
	}
		
	free(lbuff);
	return ai_invalid;
}

int dbf_gameplay(struct dbs_game *game,struct dbs_line *line,struct dbs_player *player)
{
	int result;
	unsigned int count;
	
	count=dbf_countsqr(game);
	if((result=dbf_setlinepoint(game,line))<0) return result;
	if(dbf_countsqr(game) > count) player->score += dbf_countsqr(game)-count;
	
	return dbf_isgameover(game);
}

int dbf_isgameover(struct dbs_game *game)
{
	unsigned int i;
	struct dbs_line linex,liney;
	
	for(i=0;i<(game->sqr)*(game->sqr+1);i++)
	{
		dbf_getpointlinex(game,i,&linex);
		dbf_getpointliney(game,i,&liney);
		
		if(	!(linex.p1.next_x==STAMP && linex.p2.prev_x==STAMP) )
			return gp_gamenormal;
		
		if( !(liney.p1.next_y==STAMP && liney.p2.prev_y==STAMP) )
			return gp_gamenormal;
	}
	
	return gp_gameover;
}

