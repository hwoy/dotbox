
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

static void showscore(struct dbs_game *game)
{
	printf("%s:Score-< %u",game->player[P1].name,game->player[P1].score);
	printf(" >--VS--< ");
	printf("%u >-Score:%s\n",game->player[P2].score,game->player[P2].name);

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
