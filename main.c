#include <stdio.h>
#include "dotbox.h"
#include "dotbox_io.h"
#include "function.h"

#define D_SQR 3
#define LEN 4
#define BSIZE 512

#define YOU 0
#define COM 1

#define PREFIX_X 'x'
#define PREFIX_Y 'y'

static int cropui(struct dbs_game *game,const char *str,struct dbs_line *line);
static const char *gameidstr(const char *str[],int id);

static const char *idstr[]={"Game over","Normal","Invalide line","Invalid line-x","Invalid line-y","AI no more move","Error malloc","AI best move","AI worse move","AI random move",NULL};

int main(void)
{
	int n,j;
	unsigned int pindex;
	struct dbs_game game;
	struct dbs_line line;
	
	static char buff[BSIZE+1];
	
	dbf_init(&game,"YOU",D_SQR);
	pindex=dbf_random(0,1);

	putchar('\n');
	printTable(&game,LEN);
	putchar('\n');
	
do
{
	do{
		
	if(pindex==COM)
	{
		n=game.ai(&game,&line);
		printf("AI_ID = %d*(%s)\n",n,gameidstr(idstr,n));
	}
	
	else if(pindex==YOU)
	{
		do
		{
		dio_getch(buff,BSIZE,0);
		}while(cropui(&game,buff,&line)<0);
		
	}
	
		j=dbf_gameplay(&game,&line,&game.player[pindex]);
		if(j>=gp_invline && j<=ai_invalid) 
		{
			printf("Error:%s\n",gameidstr(idstr,j));
			continue;
		}
		
		printf("%s: (%u,%u) (%u,%u)\n",game.player[pindex].name,line.p1.x,line.p1.y,line.p2.x,line.p2.y);
		printf("GP_ID = %d*(%s)\n",j,gameidstr(idstr,j));
		
		
	}while( j>=gp_invline && j<=ai_invalid );
	
	putchar('\n');
	printf("%s:Score= %u",game.player[YOU].name,game.player[YOU].score);
	printf(" <--VS--> ");
	printf("%u =Score:%s\n",game.player[COM].score,game.player[COM].name);
	
	putchar('\n');
	printTable(&game,LEN);
	putchar('\n');
	
	if(pindex==COM) pindex=YOU;
	else pindex=COM;

}while(j!=gp_gameover);


	dbf_destroy(&game);
	return 0;
}

static int cropui(struct dbs_game *game,const char *str,struct dbs_line *line)
{
	unsigned int x,y;
	if(str[0]==PREFIX_X)
	{
		if(!isUint(&str[1])) return -1;
		x=s2ui(&str[1]);
		dbf_getpointlinex(game,x,line);
	}
	else if(str[0]==PREFIX_Y)
	{
		if(!isUint(&str[1])) return -1;
		y=s2ui(&str[1]);
		dbf_getpointliney(game,y,line);
	}
	else
	{
		return -2;
	}
	
	return 1;
}

static const char *gameidstr(const char *str[],int id)
{
	return str[id+7];
}

