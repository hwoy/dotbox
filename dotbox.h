#define D_SQR 2
#define D_DOT (D_SQR+1)
#define STAMP 1
#define UNSTAMP (!STAMP)

#define POW2A(x) (1<<(x))

struct dbs_point
{
	unsigned int stampx;
	unsigned int stampy;
	unsigned int x;
	unsigned int y;
};

struct dbs_line
{
	struct dbs_point p1;
	struct dbs_point p2;
};

struct dbs_game
{
	const char *playername;
	unsigned int sqr;
	unsigned int count;
	struct dbs_point *point;
};

struct dbs_game *dbf_init(struct dbs_game *game,const char *playername,unsigned int sqr);
void *dbf_destroy(struct dbs_game *game);

void dbf_setpointx(struct dbs_game *game,struct dbs_point *point);
void dbf_setpointy(struct dbs_game *game,struct dbs_point *point);

int dbf_setlinepoint(struct dbs_game *game,struct dbs_line *line);

void dbf_getpointlinex(struct dbs_game *game,unsigned int linenum,struct dbs_line *line);
void dbf_getpointliney(struct dbs_game *game,unsigned int linenum,struct dbs_line *line);

int dbf_setlinex(struct dbs_game *game,unsigned int linenum);
int dbf_setliney(struct dbs_game *game,unsigned int linenum);

void dbf_srandom (void);
int dbf_rand(void);
int dbf_random (int min, int max);

int dbf_countsqr(struct dbs_game *game);

unsigned int dbf_getremain_one_line(struct dbs_game *game,struct dbs_line *line);

unsigned int dbf_countbit(unsigned int num);
int dbf_postzerobit(unsigned int num);


#ifdef _DEVRAND_
const char DEVRAND[]="/dev/urandom";
#endif