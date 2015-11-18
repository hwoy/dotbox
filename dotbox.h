#define S_SQR 2
#define S_DOT (S_SQR+1)
#define STAMP 1;

struct dbs_point
{
	unsigned int stamp;
	unsigned int x;
	unsigned int y;
};


struct dbs_game
{
	const char *playername;
	struct dbs_point point[S_DOT][S_DOT];
};

struct dbs_game *dbf_init(struct dbs_game *game,const char *playername);

