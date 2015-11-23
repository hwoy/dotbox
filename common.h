
#define D_SQR 4
#define LEN 4
#define BSIZE 512

#define P1 0
#define P2 1


#define PRINTTAB() printf("\t\t\t\t")



static void showscore(struct dbs_game *game);


static const char *idstr[]={"AI best move","AI worse move","AI random move","Hit score","Double tab","Game over","Normal",\
"Invalide line","Invalid line-x","Invalid line-y","AI no more move","Memmory can't be allocated",\
NULL};

static dbv_ai gai[]={dbf_aiv1_Jarvis,dbf_aiv2_Friday};
static const char *gainame[]={"Jarvis","Friday"};

static void showscore(struct dbs_game *game)
{
	printf("%s:Score= %u",game->player[P1].name,game->player[P1].score);
	printf(" <--VS--> ");
	printf("%u =Score:%s\n",game->player[P2].score,game->player[P2].name);

}