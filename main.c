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

static int cropui_key(struct dbs_game *game,const char *str,struct dbs_line *line);
static int _cropui(const char *str,char ch,unsigned int *num);
static const char *gameidstr(const char *str[],int id);
static char answer(const char *str,char *buff,unsigned int bsize,char dkey);
static void helpkey(void);
static void showscore(struct dbs_game *game);

static const char *idstr[NGPID+1+3]={"Invalide line","Invalid line-x","Invalid line-y",\
"AI no more move","Error malloc","Game over","Normal","AI best move","AI worse move","AI random move",\
NULL};

static const char *key[]={"x:","y:","-","+","t","h",NULL};
enum
{
	k_x,k_y,k_quit,k_new,k_tab,k_help
};

int main(int argc, const char *argv[])
{
	int i,n,gpid;
	unsigned int pindex;
	unsigned int ui_cindex;
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
			
		
		}while(cropui_key(&game,buff,&line)<0);
		
	break;
	}
	/************************** HUMAN **************************/
	
	
	
	
	
	
		gpid=dbf_gameplay(&game,&line,&game.player[pindex]);
		
	



	
		/************** Fatal Error(GP)(Require quit game) **************/
		switch(gpid)
		{
			case ai_errmalloc:
			case ai_nomove:
			printf("Error:%s\n",gameidstr(idstr,gpid));
			goto QUIT_GAME;
		}
		/************** Fatal Error(GP)(Require quit game) **************/
		
		
		/************** Tiny Error(GP) **************/
		if(gpid<=gp_invy) 
		{
			printf("Error:%s\n",gameidstr(idstr,gpid));
			continue;
		}
		/************** Tiny Error(GP) **************/
		
		
		
		
		if(pindex==COM) PRINTTAB();
		printf("%s: (%u,%u) (%u,%u)\n",game.player[pindex].name,line.p1.x,line.p1.y,line.p2.x,line.p2.y);
		
		if(pindex==COM) PRINTTAB();
		printf("GP_ID = %d*(%s)\n",gpid,gameidstr(idstr,gpid));
		
		
	}while( gpid<gp_gameover);
	
	putchar('\n');
	showscore(&game);
	putchar('\n');
	
	putchar('\n');
	printTable(&game,LEN);
	putchar('\n');
	
	pindex=!pindex;

}while(gpid!=gp_gameover);


QUIT_GAME:
	dbf_destroy(&game);
	return 0;
}









static int cropui_key(struct dbs_game *game,const char *str,struct dbs_line *line)
{
	unsigned int x,y;
	int i;
	if(str[0]==PREFIX_X)
	{
		
		if((i=_cropui(str,PREFIX_X,&x))<0) return i;
		
		if(x>=game->sqr*(game->sqr+1)) return -3;
			
		dbf_getpointlinex(game,x,line);
	}
	else if(str[0]==PREFIX_Y)
	{
		if((i=_cropui(str,PREFIX_Y,&y))<0) return i;
		
		if(y>=game->sqr*(game->sqr+1)) return -4;
		
		dbf_getpointliney(game,y,line);
	}
	else
	{
		return -5;
	}
	
	return 1;
}

static int _cropui(const char *str,char ch,unsigned int *num)
{
	if(str[0]==ch)
	{
		if(!isUint(&str[1])) return -1;
		*num=s2ui(&str[1]);
		return 1;
	}
	
	return -2;
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