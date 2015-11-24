#include <stdio.h>
#include <time.h>
#include "dotbox.h"
#include "dotbox_io.h"
#include "function.h"
#include "opt.h"

#include "common.h"
static void showHelp (const char *str, const char **param,const char **hparam);

static const char *cptrarr_param[] =
  { "-s:","-l:","-1","-2","-h", NULL };
static const char *helpparam[] =
  { "Squar","Squar length","AI V1-Jarvis go first","AI V2-Friday go first","Help",
  NULL
};
 enum
{
  opt_s,opt_l,opt_1,opt_2, opt_h
};

static const char *err_str[] =
  { "Invalid option", "Not an unsigned integer","Squar equal zero","Can not init game","Squar length equal zero", NULL
};

enum
{
  err_inv, err_ni, err_sz, err_initgame, err_lz
};

int main(int argc, const char *argv[])
{
	int i;
	unsigned int aiid,gpid;
	unsigned int pindex;
	unsigned int length;
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
	length=LEN;
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
		
		case opt_l:
		if (!isUint (carray_buff))
			return showErr (err_str, err_ni, carray_buff);
		length = s2ui (carray_buff);	
		if(!length)
			return showErr (err_str, err_lz, carray_buff);
		break;
		
		case opt_1:
		case opt_2:
		pindex=i-opt_1;
		break;
		
		case opt_h:
		showHelp (argv[0], cptrarr_param, helpparam);
		return 1;
		
		default:
		showHelp (argv[0], cptrarr_param, helpparam);
		return showErr (err_str, err_inv, carray_buff);
	}
	}
	
	if(!dbf_init(&game,p1name,p2name,squar,NULL))
		return showErr (err_str, err_initgame, "dbf_init");

	putchar('\n');
	printTable(&game,length);
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
			if(pindex==P2) PRINTTAB();fprintf(stderr,"Fatal Error: require quit game\n");
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
	printTable(&game,length);
	putchar('\n');
	
	if(gpid!=gp_hitscore && gpid!=gp_doubletab)
	pindex=!pindex;

}while(gpid!=gp_gameover);


QUIT_GAME:
	dbf_destroy(&game);
	return 0;

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
  fprintf (stderr, "%s%u\n", param[1], LEN);
  fprintf (stderr, "\n");
}


