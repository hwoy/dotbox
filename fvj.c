#include <stdio.h>
#include <time.h>
#include "dotbox.h"
#include "dotbox_io.h"
#include "function.h"
#include "opt.h"

#define D_SQR 4
#define LEN 4
#define BSIZE 512

#define P1 0
#define P2 1


#define PRINTTAB() printf("\t\t\t\t")


static void showscore(struct dbs_game *game);
static void showHelp (const char *str, const char **param,const char **hparam);
static int showErr (const char **str, int errno, const char *msg);
static unsigned int basename (const char *ch);

static const char *idstr[]={"AI best move","AI worse move","AI random move","Hit score","Double tab","Game over","Normal",\
"Invalide line","Invalid line-x","Invalid line-y","AI no more move","Memmory can't be allocated",\
NULL};

static dbv_ai gai[]={dbf_aiv1_Jarvis,dbf_aiv2_Friday};
static const char *gainame[]={"Jarvis","Friday"};

static const char *cptrarr_param[] =
  { "-s:", "-h", NULL };
static const char *helpparam[] =
  { "Squar", "Help",
  NULL
};
 enum
{
  opt_s, opt_h
};

static const char *err_str[] =
  { "Invalid option", "Not an unsigned integer","Squar equal zero", NULL
};

enum
{
  err_inv, err_ni, err_sz
};

int main(int argc, const char *argv[])
{
	int i,aiid,gpid;
	unsigned int pindex;
	static char carray_buff[BSIZE];
  unsigned int ui_cindex;
	struct dbs_game game;
	struct dbs_line line;
	
	unsigned int squar;
	const char *p1name,*p2name;
	

	#ifndef _DEVRAND_
	dbf_srandom(time(NULL));
	#endif

	squar=D_SQR;
	p1name=gainame[P1];
	p2name=gainame[P2];
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
		
		case opt_h:
		showHelp (argv[0], cptrarr_param, helpparam);
		return 1;
		
		default:
		showHelp (argv[0], cptrarr_param, helpparam);
		return showErr (err_str, err_inv, carray_buff);
	}
	}
	
	dbf_init(&game,p1name,p2name,squar,NULL);

	putchar('\n');
	printTable(&game,LEN);
	putchar('\n');
	
do
{
	do{
		
		aiid=gai[pindex](&game,&line);

		putchar('\n');
		if(pindex==P2)PRINTTAB();printf("NAME = %s\n",game.player[pindex].name);
		if(pindex==P2)PRINTTAB();printf("AI_RID = %d*(%s)\n",aiid,idstr[aiid]);
		
		if(aiid==ai_nomove)
		{
			goto QUIT_GAME;	
		}
		
		gpid=dbf_gameplay(&game,&line,&game.player[pindex]);
		

		if(pindex==P2) PRINTTAB();
		printf("MOVE = (%u,%u) (%u,%u)\n",line.p1.x,line.p1.y,line.p2.x,line.p2.y);
		
		if(pindex==P2) PRINTTAB();
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
	
	if(gpid!=gp_hitscore)
	pindex=!pindex;

}while(gpid!=gp_gameover);


QUIT_GAME:
	dbf_destroy(&game);
	return 0;

}


static void showscore(struct dbs_game *game)
{
	printf("%s:Score= %u",game->player[P1].name,game->player[P1].score);
	printf(" <--VS--> ");
	printf("%u =Score:%s\n",game->player[P2].score,game->player[P2].name);

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
  fprintf (stderr, "\n");
}

static int showErr (const char **str, int errno, const char *msg)
{
  fprintf (stderr, "ERR %d: %s : %s\n", errno, msg, str[errno]);
  return -1 * (errno + 1);
}

static unsigned int
basename (const char *ch)
{
  unsigned int i, j;
  for (i = 0, j = 0; ch[i]; i++)
    {
      if (ch[i] == '\\' || ch[i] == '/')
	{
	  j = i;
	}
    }
  return (j == 0) ? 0 : j + 1;
}
