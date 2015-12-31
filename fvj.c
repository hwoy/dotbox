#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "dotbox.h"
#include "dotbox_io.h"
#include "function.h"
#include "opt.h"

#include "common.h"
static void showHelp (const char *str, const char **param,const char **hparam);

static const char *cptrarr_param[] =
  { "-s:","-l:","-1","-2","-f:","-h", NULL };
static const char *helpparam[] =
  { "Squar","Squar length","AI V1-Jarvis go first","AI V2-Friday go first","Out put to a file","Help",
  NULL
};
 enum
{
  opt_s,opt_l,opt_1,opt_2,opt_f, opt_h
};

static const char *err_str[] =
  { "Invalid option", "Not an unsigned integer","Squar equal zero","Can not init game","Squar length equal zero","Can not assign a file", NULL
};

enum
{
  err_inv, err_ni, err_sz, err_initgame, err_lz ,err_file
};

int main(int argc, const char *argv[])
{
	int i;
	FILE *fp;
	unsigned int aiid,gpid;
	unsigned int pindex;
	unsigned int length;
	static char carray_buff[BSIZE],filename[BSIZE];
	unsigned int ui_cindex;
	struct dbs_game game;
	struct dbs_line line;
	
	unsigned int squar;
	const char *p1name,*p2name;
	

	#ifndef _DEVRAND_
	dbf_srandom(time(NULL));
	#endif

	fp=FP;
	filename[0]=0;
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
		
		case opt_f:
		strcpy(filename,carray_buff);
		break;
		
		case opt_h:
		showHelp (argv[0], cptrarr_param, helpparam);
		return 1;
		
		default:
		showHelp (argv[0], cptrarr_param, helpparam);
		return showErr (err_str, err_inv, carray_buff);
	}
	}
	
	if(filename[0])
	{
		if(!(fp=fopen(filename,"w")))
			return showErr (err_str, err_file, filename);
	}
	
	if(!dbf_init(&game,p1name,p2name,squar,NULL))
	{
		fclose(fp);
		return showErr (err_str, err_initgame, "dbf_init");
	}

	fputc('\n',fp);
	printTable(fp,&game,length);
	fputc('\n',fp);
	
do
{
	do{
		
		aiid=gai[pindex](&game,&line);

		fputc('\n',fp);
		if(pindex==P2)PRINTTAB();fprintf(fp,"NAME = %s\n",game.player[pindex].name);
		if(pindex==P2)PRINTTAB();fprintf(fp,"AI_RID = %d*(%s)\n",aiid,idstr[aiid]);
		
		if(aiid==ai_nomove)
		{
			if(pindex==P2) PRINTTAB();fprintf(stderr,"AI Error[PINDEX=%u]: Quit game for breaking point\n",pindex);
			goto QUIT_GAME;	
		}
		
		gpid=dbf_gameplay(&game,&line,&game.player[pindex]);
		

		if(pindex==P2) PRINTTAB();
		fprintf(fp,"MOVE = (%u,%u) (%u,%u)\n",line.p1.x,line.p1.y,line.p2.x,line.p2.y);
		
		if(pindex==P2) PRINTTAB();
		fprintf(fp,"GP_RID = %d*(%s)\n",gpid,idstr[gpid]);
		
		
		/************** Fatal Error(GP)(Require quit game) **************/
		switch(gpid)
		{
			case ai_errmalloc:
			case ai_nomove:
			if(pindex==P2) PRINTTAB();fprintf(stderr,"GP Fatal Error[PINDEX=%u]: require quit game\n",pindex);
			goto QUIT_GAME;
		}
		/************** Fatal Error(GP)(Require quit game) **************/
		
		
		/************** Tiny Error(GP) **************/
		if(gpid>=gp_invline) 
		{
			if(pindex==P2) PRINTTAB();fprintf(stderr,"GP Error[PINDEX=%u]: Quit game for breaking point\n",pindex);
			goto QUIT_GAME;
		}
		/************** Tiny Error(GP) **************/		
		
	}while(gpid>gp_gamenormal);
	
	fputc('\n',fp);
	showscore(fp,&game);
	fputc('\n',fp);
	
	fputc('\n',fp);
	printTable(fp,&game,length);
	fputc('\n',fp);
	
	if(gpid!=gp_hitscore && gpid!=gp_doubletab)
	pindex=!pindex;

}while(gpid!=gp_gameover);

summary(fp,&game);

QUIT_GAME:
	fclose(fp);
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


