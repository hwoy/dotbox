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

#define YES 'y'
#define NO 'n'

#define K_QUIT '-'
#define K_NEW '+'
#define K_TABLE 't'
#define K_HELP 'h'


#define PRINTTAB() printf("\t\t\t\t")

#define NGPID 7

static int cropui(struct dbs_game *game,const char *str,struct dbs_line *line);
static const char *gameidstr(const char *str[],int id);
static char answer(const char *str,char *buff,unsigned int bsize,char dkey);
static void helpkey(void);
static void showscore(struct dbs_game *game);

static const char *idstr[NGPID+1+3]={"Invalide line","Invalid line-x","Invalid line-y",\
"AI no more move","Error malloc","Game over","Normal","AI best move","AI worse move","AI random move",\
NULL};

int main(void)
{
	int n,j;
	unsigned int pindex;
	struct dbs_game game;
	struct dbs_line line;
	
	static char buff[BSIZE+1];
	
NEW_GAME:	
	dbf_init(&game,"YOU",D_SQR+1,dbf_aiv2);
	pindex=dbf_random(0,1);

	putchar('\n');
	printTable(&game,LEN);
	putchar('\n');
	
do
{
	do{
		
	switch(pindex)
	{
		
	/************************** COM **************************/	
	
	case COM:
		n=game.ai(&game,&line);
		
		putchar('\n');
		PRINTTAB();printf("AI_ID = %d*(%s)\n",n,gameidstr(idstr,n));
		
	break;
	
	/************************** COM **************************/
	
	/************************** HUMAN **************************/
	case YOU:
		do
		{
		putchar('\n');		
		printf("Enter a line --> ");dio_getch(buff,BSIZE,0);
		
		
		
		if(sLen(buff)==1)
		{
			/********************* Key*********************/
			switch(buff[0])
			{
				case K_NEW:
				dbf_destroy(&game);
				goto NEW_GAME;
				
				case K_QUIT:
				if(answer("(Y/n)--> ",buff,BSIZE,YES)==YES) goto QUIT_GAME;
				else  break;
				
				case K_HELP:
				helpkey();
				break;
				
				case K_TABLE:
				putchar('\n');
				showscore(&game);
				putchar('\n');
				
				putchar('\n');
				printTable(&game,LEN);
				putchar('\n');
				break;
			}
			
			/********************* Key*********************/
		}
			
		
		}while(cropui(&game,buff,&line)<0);
		
	break;
	}
	/************************** HUMAN **************************/
	
	
	
	
	
	
		j=dbf_gameplay(&game,&line,&game.player[pindex]);
		
	



	
		/************** Fatal Error(GP)(Require quit game) **************/
		switch(j)
		{
			case ai_errmalloc:
			case ai_nomove:
			printf("Error:%s\n",gameidstr(idstr,j));
			goto QUIT_GAME;
		}
		/************** Fatal Error(GP)(Require quit game) **************/
		
		
		/************** Tiny Error(GP) **************/
		if(j<=gp_invy) 
		{
			printf("Error:%s\n",gameidstr(idstr,j));
			continue;
		}
		/************** Tiny Error(GP) **************/
		
		
		
		
		if(pindex==COM) PRINTTAB();
		printf("%s: (%u,%u) (%u,%u)\n",game.player[pindex].name,line.p1.x,line.p1.y,line.p2.x,line.p2.y);
		
		if(pindex==COM) PRINTTAB();
		printf("GP_ID = %d*(%s)\n",j,gameidstr(idstr,j));
		
		
	}while( j<gp_gameover);
	
	putchar('\n');
	showscore(&game);
	putchar('\n');
	
	putchar('\n');
	printTable(&game,LEN);
	putchar('\n');
	
	pindex=!pindex;

}while(j!=gp_gameover);


QUIT_GAME:
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
		
		if(x>=game->sqr*(game->sqr+1)) return -2;
			
		dbf_getpointlinex(game,x,line);
	}
	else if(str[0]==PREFIX_Y)
	{
		if(!isUint(&str[1])) return -1;
		y=s2ui(&str[1]);
		
		if(y>=game->sqr*(game->sqr+1)) return -3;
		
		dbf_getpointliney(game,y,line);
	}
	else
	{
		return -4;
	}
	
	return 1;
}

static const char *gameidstr(const char *str[],int id)
{
	return str[id+NGPID];
}

static char answer(const char *str,char *buff,unsigned int bsize,char dkey)
{
	do
	{
	printf("%s",str);
	dio_getch(buff,bsize,0);
	if(!sLen(buff))
	{
		buff[0]=dkey;
		buff[1]=0;
	}
		
	}while(!((sLen(buff)==1)&&(buff[0]==YES || buff[0]==NO)));
		
	return buff[0];
}

static void helpkey(void)
{
	putchar('\n');
	printf("\t%c:\t NEW GAME\n",K_NEW);
	printf("\t%c:\t QUIT GAME\n",K_QUIT);
	printf("\t%c:\t SHOW HELP\n",K_HELP);
	printf("\t%c:\t SHOW TABLE\n",K_TABLE);
	putchar('\n');
}

static void showscore(struct dbs_game *game)
{
	printf("%s:Score= %u",game->player[YOU].name,game->player[YOU].score);
	printf(" <--VS--> ");
	printf("%u =Score:%s\n",game->player[COM].score,game->player[COM].name);

}