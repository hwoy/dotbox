#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dotbox.h"
#include "dotbox_io.h"
#include "function.h"
#include "opt.h"


#include "common.h"


#define YOU P1
#define COM P2

#define PREFIX_X 'x'
#define PREFIX_Y 'y'

#define YES 'y'
#define NO 'n'

#define NLENGTH 10

static char answer(FILE *fp,const char *str,char *buff,unsigned int bsize,char dkey);
static void helpkey(const char *key[],const char *keystr[]);
static int key_option(const char *str,const char *key[],char *buff,unsigned int size);
static void showHelp (const char *str, const char **param,const char **hparam);

static const char d_p1name[]="YOU";

/*********************************************** Keys ***********************************************/

static const char *key[]={"x","y","s:","l:","n:","1","2","-","+","t","h",NULL};
static const char *keystr[]={"Enter a x line","Enter a y line","Enter a squar value","Enter a squar length","Rename",\
"AI version 1","AI version 2","Quit Game","New Game","Show game table","Show keys help",NULL};
enum
{
	k_x,k_y,k_s,k_l,k_n,k_1,k_2,k_quit,k_new,k_t,k_help
};

/*********************************************** Keys ***********************************************/

/*********************************************** Options ***********************************************/

static const char *cptrarr_param[] =
  { "-s:", "-n:","-1","-2","-y","-c","-l:","-h", NULL };
static const char *helpparam[] =
  { "Squar", "Player Name","AI version 1","AI version 2","You go first","Com go first","Squar length",\
  "Help",NULL
};
 enum
{
  opt_s, opt_n,opt_1,opt_2,opt_y,opt_c,opt_l,opt_h
};

/*********************************************** Options ***********************************************/

/*********************************************** Error massages ***********************************************/
static const char *err_str[] =
  { "Invalid option", "Not an unsigned integer","Squar equal zero","Can not init game",\
  "Over maximum name length","Squar length equal zero","Line X over limit","Line Y over limit",\
  NULL
};

enum
{
  err_inv, err_ni, err_sz, err_initgame,err_nl,err_lz,err_xo,err_yo
};

/*********************************************** Error massages ***********************************************/

int main(int argc, const char *argv[])
{
	int i;
	FILE *fp;
	unsigned int aiid,gpid;
	unsigned int j;
	unsigned int pindex;
	unsigned int ui_cindex;
	unsigned int x,y;
	unsigned int length;
	static char buff[BSIZE+1],carray_buff[BSIZE],op1name[NLENGTH+1];
	char ch;
	struct dbs_game game;
	struct dbs_line line;
	
	unsigned int squar;
	const char *p1name,*p2name;
	dbv_ai ai;
	
	#ifndef _DEVRAND_
	dbf_srandom(time(NULL));
	#endif
	
	fp=FP;
	squar=D_SQR;
	p1name=d_p1name;
	length=LEN;
	
	j=dbf_random(0,1);
	ai=gai[j];
	p2name=gainame[j];
	
	pindex=dbf_random(0,1);


	  for (ui_cindex = DSTART; (i =
		       opt_action (argc, argv, cptrarr_param, carray_buff,
				   BSIZE, DSTART)) != e_optend; ui_cindex++)
    {
      switch (i)
	{
		case opt_s:
		if (!isUint (carray_buff))
			return showErr (err_str, err_ni, carray_buff);
		squar = s2ui (carray_buff);	
		if(!squar)
			return showErr (err_str, err_sz, carray_buff);
		break;
		
		case opt_n:
		if(sLen(carray_buff)>NLENGTH)
		{
			j=showErr (err_str, err_nl, carray_buff);
			fprintf(stderr,"\nMax name length = %u\n",NLENGTH);
			return j;
		}
		strcpy(op1name,carray_buff);
		p1name=op1name;
		break;
		
		case opt_1:
		case opt_2:
		ai=gai[i-opt_1];
		p2name=gainame[i-opt_1];
		break;
		
		case opt_y:
		pindex=YOU;
		break;
		
		case opt_c:
		pindex=COM;
		break;
		
		case opt_l:
		if (!isUint (carray_buff))
			return showErr (err_str, err_ni, carray_buff);
		length = s2ui (carray_buff);	
		if(!length)
			return showErr (err_str, err_lz, carray_buff);
		break;
		
		case opt_h:
		showHelp (argv[0], cptrarr_param, helpparam);
		return 1;
		
		default:
		showHelp (argv[0], cptrarr_param, helpparam);
		return showErr (err_str, err_inv, carray_buff);
	}
	}	
	
	
NEW_GAME:
do		/* When Game is over answer YES/NO NO=QUIT */
{	
	if(!dbf_init(&game,p1name,p2name,squar,ai))
		return showErr (err_str, err_initgame, "dbf_init");

	fputc('\n',fp);
	printTable(fp,&game,length);
	fputc('\n',fp);
	
do		/* Exit loop when gpid==gp_gameover */
{
	do		/* NORNAL ID <--gp_gamenormal--> ERROR ID*/
	{
		
	switch(pindex)
	{
		
	/************************** COM **************************/	
	
	case COM:
		aiid=game.ai(&game,&line);
		
		fputc('\n',fp);
		PRINTTAB();fprintf(fp,"NAME = %s\n",game.player[pindex].name);
		PRINTTAB();fprintf(fp,"AI_RID = %d*(%s)\n",aiid,idstr[aiid]);
		
		if(aiid==ai_nomove)
		{
			goto QUIT_GAME;	
		}
		
	break;
	
	/************************** COM **************************/
	
	/************************** HUMAN **************************/
	case YOU:
		do		/* Exit loop when complete x or y line */
		{
		fputc('\n',fp);		
		fprintf(fp,"Enter a line (%s=help) --> ",key[10]);
		
		dio_getstr(buff,BSIZE);
		i=key_option(buff,key,carray_buff,BSIZE);
	  switch(i)
	  {
/*************** Key X(line-x) ***************/
		  case k_x:
		if(!isUint(carray_buff)) 		
		{
			showErr (err_str, err_ni, carray_buff);
			i++;i*=-1;continue;
		}
		x=s2ui(carray_buff);
		if(x>=game.sqr*(game.sqr+1))
		{
			showErr (err_str, err_xo, carray_buff);
			i++;i*=-1;continue;
		}			
		dbf_getpointlinex(&game,x,&line);
		break;
		
/*************** Key Y(line-y) ***************/		
		  case k_y:
		if(!isUint(carray_buff))
		{
			showErr (err_str, err_ni, carray_buff);
			i++;i*=-1;continue;
		}
		y=s2ui(carray_buff);
		if(y>=game.sqr*(game.sqr+1))
		{
			showErr (err_str, err_yo, carray_buff);
			i++;i*=-1;continue;
		}
		dbf_getpointliney(&game,y,&line);		  
		  break;
		  
/*************** Key 1(v1-Jarvis) or 2(v2-Friday) ***************/		  
		  case k_1:
		  case k_2:
		  ai=gai[i-k_1];
		  game.ai=ai;
		  p2name=gainame[i-k_1];
		  game.player[COM].name=p2name;
		  fprintf(fp,"AI codename: %s activated!",p2name);
		  break;
		  
		  case k_n:
			if(sLen(carray_buff)>NLENGTH)
				{
					showErr (err_str, err_nl, carray_buff);
					fprintf(stderr,"\nMax name length = %u\n",NLENGTH);
					i++;i*=-1;continue;
				}
			strcpy(op1name,carray_buff);
			p1name=op1name;
			game.player[pindex].name=p1name;
			fprintf(fp,"Setting Your name to %s\n",p1name);
			break;
			

/*************** Key S:(Squar) ***************/			
		  case k_s:
		if(!isUint(carray_buff))
		{
			showErr (err_str, err_ni, carray_buff);
			i++;i*=-1;continue;
		}
		squar=s2ui(carray_buff);
		if(squar==0)
		{
			showErr (err_str, err_sz, carray_buff);
			i++;i*=-1;continue;
		}

/*************** Key +(New Game) ***************/		
		  case k_new:
		dbf_destroy(&game);
		
		j=dbf_random(0,1);
		ai=gai[j];
		p2name=gainame[j];
		
		pindex=dbf_random(0,1);
	
		goto NEW_GAME;

/*************** Key L:(Squar Length) ***************/

		  case k_l:
		if(!isUint(carray_buff))
		{
			showErr (err_str, err_ni, carray_buff);
			i++;i*=-1;continue;
		}
		length=s2ui(carray_buff);
		if(length==0)
		{
			showErr (err_str, err_lz, carray_buff);
			i++;i*=-1;continue;
		}
		
/*************** Key T(Table) ***************/		
		  case k_t:
		fputc('\n',fp);
		showscore(fp,&game);
		fputc('\n',fp);
	
		fputc('\n',fp);
		printTable(fp,&game,length);
		fputc('\n',fp);
		break;
/*************** Key H(Help) ***************/
		  case k_help:
		  helpkey(key,keystr);
		  break;		

/*************** Key -(Quit) ***************/		  
		  case k_quit:
		if(answer(fp,"Do you want to quit this game?\n(Y/n)",buff,BSIZE,YES)==YES)
			goto QUIT_GAME;
		break;
		  
	  }
	  

			
		
		}while( i!=k_x && i!=k_y );		/* Exit loop when complete x or y line */
	fputc('\n',fp);
	fprintf(fp,"NAME = %s\n",game.player[pindex].name);	
	break;
	}
	/************************** HUMAN **************************/
	
	
	
	
	
	
		gpid=dbf_gameplay(&game,&line,&game.player[pindex]);
		

		if(pindex==COM) PRINTTAB();
		fprintf(fp,"MOVE = (%u,%u) (%u,%u)\n",line.p1.x,line.p1.y,line.p2.x,line.p2.y);
		
		if(pindex==COM) PRINTTAB();
		fprintf(fp,"GP_RID = %d*(%s)\n",gpid,idstr[gpid]);
		
		/************** Fatal Error(GP)(Require quit game) **************/
		switch(gpid)
		{
			case ai_errmalloc:
			case ai_nomove:
			if(pindex==COM) PRINTTAB();fprintf(stderr,"Fatal Error[PINDEX=%u]: require quit game\n",pindex);
			goto QUIT_GAME;
		}
		/************** Fatal Error(GP)(Require quit game) **************/
		
		
		/************** Tiny Error(GP) **************/
		if(gpid>=gp_invline) 
		{
			continue;
		}
		/************** Tiny Error(GP) **************/
		
		
	}while(gpid>gp_gamenormal);		/* NORNAL ID <--gp_gamenormal--> ERROR ID*/
	
	fputc('\n',fp);
	showscore(fp,&game);
	fputc('\n',fp);
	
	fputc('\n',fp);
	printTable(fp,&game,length);
	fputc('\n',fp);
	
	if(gpid!=gp_hitscore && gpid!=gp_doubletab)
	pindex=!pindex;

}while(gpid!=gp_gameover);		/* Exit loop when gpid==gp_gameover */

summary(fp,&game);


	if((ch=answer(fp,"Do you want to continue this game?\n(Y/n)",buff,BSIZE,YES))==YES)
	{
		dbf_destroy(&game);
		
		j=dbf_random(0,1);
		ai=gai[j];
		p2name=gainame[j];
		
		pindex=dbf_random(0,1);
	
	}
	
	
}while(ch!=NO);		/* When Game is over answer YES/NO NO=QUIT */
	


QUIT_GAME:
	fclose(fp);
	dbf_destroy(&game);
	return 0;
}






static char answer(FILE *fp,const char *str,char *buff,unsigned int bsize,char dkey)
{
	do
	{
	fprintf(fp,"%s",str);
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
	
	i=dbf_random(0,D_SQR*(D_SQR+1)-1);
	fprintf(stderr,"%5s%u\tEnter x line %u\n",key[0],i,i);
	i=dbf_random(0,D_SQR*(D_SQR+1)-1);
	fprintf(stderr,"%5s%u\tEnter y line %u\n",key[1],i,i);
	i=dbf_random(2,10);
	fprintf(stderr,"%5s%u\tEnter a squar value %u\n",key[2],i,i);
	fprintf(stderr,"%5s\tQuit Game\n",key[7]);
	fprintf(stderr,"%5s\tNew Game\n",key[8]);
	
	fputc('\n',stderr);
}


static int key_option(const char *str,const char *key[],char *buff,unsigned int size)
{
	unsigned int i,j,k;
	for(i=0;key[i];i++)
	{
		if(!strncmp(key[i],str,strlen(key[i])))
		{
			for(k=0,j=strlen(key[i]);str[j];j++,k++)
			{
				if(k>size-1)
					return -2;
				
				buff[k]=str[j];
			}
			buff[k]=0;
			return i;
		}
	}
	
	return -1;
}

static void showHelp (const char *str, const char **param, const char **hparam)
{
	unsigned int i;
  fprintf (stderr, "\nUSAGE: %s [option list]\n\n", &str[basename (str)]);

  fprintf (stderr, "[OPTIONS]\n");

  for (i = 0; param[i] && hparam[i]; i++)
    {
      fprintf (stderr, "%s\t\t%s\n", param[i], hparam[i]);
    }
  fprintf (stderr, "\n");

  fprintf (stderr, "[DEFAULT]\n");
  fprintf (stderr, "%s%u\n", param[0], D_SQR);
  fprintf (stderr, "%s%s\n", param[1], d_p1name);
  fprintf (stderr, "%s%u\n", param[6], LEN);
  fprintf (stderr, "\n");
}
