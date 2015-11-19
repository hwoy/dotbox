#define D_SQR 2
#define D_DOT (D_SQR+1)
#define STAMP 1
#define UNSTAMP (!STAMP)

struct dbs_point
{
	unsigned int stampx;
	unsigned int stampy;
	unsigned int x;
	unsigned int y;
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

void dbf_setpointx(struct dbs_game *game,unsigned int x,unsigned int y);
void dbf_setpointy(struct dbs_game *game,unsigned int x,unsigned int y);

int dbf_setlinepoint(struct dbs_game *game,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void dbf_getpointx(struct dbs_game *game,unsigned int linenum,struct dbs_point *p1,struct dbs_point *p2);
void dbf_getpointy(struct dbs_game *game,unsigned int linenum,struct dbs_point *p1,struct dbs_point *p2);

int dbf_setlinex(struct dbs_game *game,unsigned int linenum);
int dbf_setliney(struct dbs_game *game,unsigned int linenum);

void dbf_srandom (void);
int dbf_rand(void);
int dbf_random (int min, int max);

int dbf_countsqr(struct dbs_game *game);


#ifdef _DEVRAND_
const char DEVRAND[]="/dev/urandom";
#endif