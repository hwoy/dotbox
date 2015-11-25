
#define D_SQR 4
#define LEN 4
#define BSIZE 512

#define P1 0
#define P2 1

#define FP stdout


#define PRINTTAB() fprintf(fp,"\t\t\t\t")



static void showscore(FILE *fp,struct dbs_game *game);
static int showErr (const char **str, int errno, const char *msg);
static unsigned int basename (const char *ch);
static void summary(FILE *fp,struct dbs_game *game);


static const char *idstr[]={"AI best move","AI worse move","AI random move","Hit score","Double tab","Game over","Normal",\
"Invalide line","Invalid line-x","Invalid line-y","AI no more move","Memmory can't be allocated",\
NULL};

static dbv_ai gai[]={dbf_aiv1_Jarvis,dbf_aiv2_Friday};
static const char *gainame[]={"Jarvis","Friday"};

static void showscore(FILE *fp,struct dbs_game *game)
{
	fprintf(fp,"%s:Score-< %u",game->player[P1].name,game->player[P1].score);
	fprintf(fp," >--VS--< ");
	fprintf(fp,"%u >-Score:%s\n",game->player[P2].score,game->player[P2].name);

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

static void summary(FILE *fp,struct dbs_game *game)
{
	const char *p1msg,*p2msg;
	static const char WIN[]="Win!",LOSE[]="Lose",DRAW[]="Draw";
	
	fprintf(fp,"\n\n");
	fprintf(fp,"%s\n\n","**************** [Game summary] ****************");
	fprintf(fp,"%-10s%10s\t%10s\n\n","","PLAYER1","PLAYER2");
	fprintf(fp,"%-10s%10s\t%10s\n","PNAME|",game->player[0].name,game->player[1].name);
	fprintf(fp,"%-10s%10u\t%10u\n","DBTAB|",game->player[0].doubletab,game->player[1].doubletab);
	fprintf(fp,"%-10s%10u\t%10u\n","PHITS|",game->player[0].score-2*game->player[0].doubletab,game->player[1].score-2*game->player[1].doubletab);
	fprintf(fp,"%-10s%10u\t%10u\n","SCORE|",game->player[0].score,game->player[1].score);
	
	if(game->player[0].score > game->player[1].score)
	{
		p1msg=WIN;
		p2msg=LOSE;
	}
	else if(game->player[0].score < game->player[1].score)
	{
		p2msg=WIN;
		p1msg=LOSE;		
	}
	else
	{
		p1msg=p2msg=DRAW;
	}
	fprintf(fp,"\n");
	fprintf(fp,"%-10s%10s\t%10s\n","RESULT",p1msg,p2msg);

	fprintf(fp,"\n");	
	fprintf(fp,"%s\n\n","**************** [Game summary] ****************");
}

