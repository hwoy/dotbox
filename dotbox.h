#define D_SQR 2
#define D_DOT (D_SQR+1)
#define STAMP 1

struct dbs_point
{
	unsigned int stamp;
	unsigned int x;
	unsigned int y;
};


struct dbs_game
{
	const char *playername;
	unsigned int sqr;
	struct dbs_point *point;
};

struct dbs_game *dbf_init(struct dbs_game *game,const char *playername,unsigned int sqr);
void *dbf_destroy(struct dbs_game *game);
void dbf_setpoint(struct dbs_game *game,unsigned int x,unsigned int y);
int dbf_setline(struct dbs_game *game,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

