#include <stdio.h>
#include "dotbox.h"
#include "dotbox_io.h"
#include "function.h"

#define D_SQR 3
#define LEN 4
#define BSIZE 512

#define PREFIX_X 'x'
#define PREFIX_Y 'y'

static int cropui(struct dbs_game *game,const char *str,struct dbs_line *line);

int main(void)
{
	int n,j;
	unsigned int pindex;
	struct dbs_game game;
	struct dbs_line line;
	
	static char buff[BSIZE+1];
	
	pindex=0;
	dbf_init(&game,"Hwoy",D_SQR);


	putchar('\n');
	printTable(&game,LEN);
	putchar('\n');
	
do
{
	
	if(pindex==1)
	{
		n=game.ai(&game,&line);
	}
	else if(pindex==0)
	{
		do
		{
		dio_getch(buff,BSIZE,0);
		}while(cropui(&game,buff,&line)<0);
	}
	j=dbf_gameplay(&game,&line,&game.player[pindex]);
	
	if(pindex==1) printf("return AI = %d, (%u,%u)  (%u,%u)\n",n,line.p1.x,line.p1.y,line.p2.x,line.p2.y);
	printf("%s:Score= %u\n",game.player[0].name,game.player[0].score);
	printf("%s:Score= %u\n",game.player[1].name,game.player[1].score);
	
	putchar('\n');
	printTable(&game,LEN);
	putchar('\n');
	
	if(pindex==1) pindex=0;
	else pindex=1;

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

