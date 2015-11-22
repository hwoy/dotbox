#include <stdio.h>
#include "dotbox.h"
#include "dotbox_io.h"
#include "function.h"

#define D_SQR 4
#define LEN 4
#define BSIZE 512

#define P1 0
#define P2 1
#define NGPID 7

#define PRINTTAB() printf("\t\t\t\t")

static const char *gameidstr(const char *str[],int id);
static void showscore(struct dbs_game *game);

static const char *idstr[NGPID+1+3]={"Invalide line","Invalid line-x","Invalid line-y",\
"AI no more move","Error malloc","Game over","Normal","AI best move","AI worse move","AI random move",\
NULL};

static dbv_ai gai[]={dbf_aiv1_Friday,dbf_aiv2_Jarvis};
static const char *gainame[]={"Friday","Jarvis"};



int main(int argc, const char *argv[])
{
	int i,n,gpid;
	unsigned int j;
	unsigned int pindex;
	static char buff[BSIZE+1];
	struct dbs_game game;
	struct dbs_line line;
	
	unsigned int squar;
	const char *p1name,*p2name;
	
	squar=D_SQR;
	
	
	#ifndef _DEVRAND_
	dbf_srandom(time(NULL));
	#endif

	
	p1name=gainame[P1];
	p2name=gainame[P2];
	
	dbf_init(&game,p1name,p2name,squar,NULL);
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
	
	case P2:
		n=gai[P2](&game,&line);
		
		putchar('\n');
		PRINTTAB();printf("NAME = %s\n",game.player[pindex].name);
		PRINTTAB();printf("AI_RID = %d*(%s)\n",n,gameidstr(idstr,n));
		
	break;
	
	/************************** COM **************************/
	
	/************************** HUMAN **************************/
	case P1:
		n=gai[P1](&game,&line);
		
		putchar('\n');
		printf("NAME = %s\n",game.player[pindex].name);
		printf("AI_RID = %d*(%s)\n",n,gameidstr(idstr,n));
		
	break;
	/************************** HUMAN **************************/
	
	}
	
	
	
	
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
		
		
		
		
		if(pindex==P2) PRINTTAB();
		printf("MOVE = (%u,%u) (%u,%u)\n",line.p1.x,line.p1.y,line.p2.x,line.p2.y);
		
		if(pindex==P2) PRINTTAB();
		printf("GP_RID = %d*(%s)\n",gpid,gameidstr(idstr,gpid));
		
		
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

static const char *gameidstr(const char *str[],int id)
{
	return str[id+NGPID];
}
static void showscore(struct dbs_game *game)
{
	printf("%s:Score= %u",game->player[P1].name,game->player[P1].score);
	printf(" <--VS--> ");
	printf("%u =Score:%s\n",game->player[P2].score,game->player[P2].name);

}

