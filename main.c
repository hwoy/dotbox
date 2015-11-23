#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dotbox.h"
#include "dotbox_io.h"
#include "function.h"


#include "common.h"


#define YOU P1
#define COM P2

#define PREFIX_X 'x'
#define PREFIX_Y 'y'

#define YES 'y'
#define NO 'n'




static char answer(const char *str,char *buff,unsigned int bsize,char dkey);
static void helpkey(const char *key[],const char *keystr[]);
static int key_option(const char *str,const char *key[],char *buff);


static const char *key[]={"x","y","s","1","2","-","+","t","h",NULL};
static const char *keystr[]={"Enter a x line","Enter a y line","Enter a squar value",\
"AI version 1","AI version 2","Quit game","New Game","Show game table","Show keys help",NULL};
enum
{
	k_x,k_y,k_s,k_1,k_2,k_quit,k_new,k_tab,k_help
};

static const char d_p1name[]="YOU";

int main(int argc, const char *argv[])
{
	int i;
	unsigned int aiid,gpid;
	unsigned int j;
	unsigned int pindex;
	unsigned int ui_cindex;
	unsigned int x,y;
	static char buff[BSIZE+1],carray_buff[BSIZE];
	struct dbs_game game;
	struct dbs_line line;
	
	unsigned int squar;
	const char *p1name,*p2name;
	dbv_ai ai;
	
	squar=D_SQR;
	p1name=d_p1name;
	
	
	#ifndef _DEVRAND_
	dbf_srandom(time(NULL));
	#endif
	
NEW_GAME:

	
	j=dbf_random(0,1);
	ai=gai[j];
	p2name=gainame[j];
	
	dbf_init(&game,p1name,p2name,squar,ai);
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
		aiid=game.ai(&game,&line);
		
		putchar('\n');
		PRINTTAB();printf("NAME = %s\n",game.player[pindex].name);
		PRINTTAB();printf("AI_RID = %d*(%s)\n",aiid,idstr[aiid]);
		
		if(aiid==ai_nomove)
		{
			goto QUIT_GAME;	
		}
		
	break;
	
	/************************** COM **************************/
	
	/************************** HUMAN **************************/
	case YOU:
		do
		{
		putchar('\n');		
		printf("Enter a line (%s=help) --> ",key[8]);
		
		dio_getstr(buff,BSIZE);
		i=key_option(buff,key,carray_buff);
	  switch(i)
	  {
		  case k_x:
		if(!isUint(carray_buff)) 		{i*=-1;continue;}
		x=s2ui(carray_buff);
		if(x>=game.sqr*(game.sqr+1)) 	{i*=-1;continue;}	
		dbf_getpointlinex(&game,x,&line);
		break;
		
		
		  case k_y:
		if(!isUint(carray_buff)) 		{i*=-1;continue;}
		y=s2ui(carray_buff);
		if(y>=game.sqr*(game.sqr+1)) 	{i*=-1;continue;}
		dbf_getpointliney(&game,y,&line);		  
		  break;
		  
		  case k_1:
		  case k_2:
		  ai=gai[i-k_1];
		  game.ai=ai;
		  p2name=gainame[i-k_1];
		  game.player[COM].name=p2name;
		  printf("AI codename: %s activated!",p2name);
		  
		  case k_s:
		if(!isUint(carray_buff)) 		{i*=-1;continue;}
		squar=s2ui(carray_buff);
		if(squar==0)					{i*=-1;continue;}
		  
		  case k_new:
		dbf_destroy(&game);
		goto NEW_GAME;
		  
		  case k_tab:
		putchar('\n');
		showscore(&game);
		putchar('\n');
	
		putchar('\n');
		printTable(&game,LEN);
		putchar('\n');
		break;

		  case k_help:
		  helpkey(key,keystr);
		  break;		
		 
		  case k_quit:
		if(answer("Do you want to quit this game?\n(Y/n)",buff,BSIZE,YES)==YES)
			goto QUIT_GAME;
		break;
		  
	  }
	  

			
		
		}while( i!=k_x && i!=k_y );
	putchar('\n');
	printf("NAME = %s\n",game.player[pindex].name);	
	break;
	}
	/************************** HUMAN **************************/
	
	
	
	
	
	
		gpid=dbf_gameplay(&game,&line,&game.player[pindex]);
		

		if(pindex==COM) PRINTTAB();
		printf("MOVE = (%u,%u) (%u,%u)\n",line.p1.x,line.p1.y,line.p2.x,line.p2.y);
		
		if(pindex==COM) PRINTTAB();
		printf("GP_RID = %d*(%s)\n",gpid,idstr[gpid]);
		
		/************** Fatal Error(GP)(Require quit game) **************/
		switch(gpid)
		{
			case ai_errmalloc:
			case ai_nomove:
			goto QUIT_GAME;
		}
		/************** Fatal Error(GP)(Require quit game) **************/
		
		
		/************** Tiny Error(GP) **************/
		if(gpid>=gp_invline) 
		{
			continue;
		}
		/************** Tiny Error(GP) **************/
		
		
	}while(gpid>gp_gamenormal);
	
	putchar('\n');
	showscore(&game);
	putchar('\n');
	
	putchar('\n');
	printTable(&game,LEN);
	putchar('\n');
	
	if(gpid!=gp_hitscore && gpid!=gp_doubletab)
	pindex=!pindex;

}while(gpid!=gp_gameover);


QUIT_GAME:
	dbf_destroy(&game);
	return 0;
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

static void helpkey(const char *key[],const char *keystr[])
{
	unsigned int i;
	fputc('\n',stderr);

	fprintf(stderr,"%s\n","[KEYS]");
	fputc('\n',stderr);
	
	for(i=0;key[i];i++)
		fprintf(stderr,"%5s\t%s\n",key[i],keystr[i]);

	fputc('\n',stderr);
	
	fprintf(stderr,"%s\n","[EXAMPLE]");
	fputc('\n',stderr);
	
	fprintf(stderr,"%5s%u\tEnter x line %u\n",key[0],i,i=dbf_random(0,D_SQR*(D_SQR+1)-1));
	fprintf(stderr,"%5s%u\tEnter y line %u\n",key[1],i,i=dbf_random(0,D_SQR*(D_SQR+1)-1));
	fprintf(stderr,"%5s%u\tEnter a squar value %u\n",key[2],i,i=dbf_random(2,10));
	fprintf(stderr,"%5s\tQuit Game\n",key[5]);
	fprintf(stderr,"%5s\tNew Game\n",key[6]);
	
	fputc('\n',stderr);
}


static int key_option(const char *str,const char *key[],char *buff)
{
	unsigned int i,j,k;
	for(i=0;key[i];i++)
	{
		if(!strncmp(key[i],str,strlen(key[i])))
		{
			for(k=0,j=strlen(key[i]);str[j];j++,k++)
			{
				buff[k]=str[j];
			}
			buff[k]=0;
			return i;
		}
	}
	
	return -1;
}