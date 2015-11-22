#define STAMP 1
#define UNSTAMP (!STAMP)

#define POW2A(x) (1<<(x))


struct dbs_point
{
	unsigned int prev_x,next_x;
	unsigned int prev_y,next_y;
	unsigned int x;
	unsigned int y;
};

struct dbs_line
{
	struct dbs_point p1;
	struct dbs_point p2;
};

struct dbs_player
{
	const char *name;
	unsigned int score;
	
};

struct dbs_game
{
	unsigned int sqr;
	struct dbs_point *point;
	struct dbs_player player[2];
	int (*ai)(struct dbs_game *game,struct dbs_line *line);
};

typedef int (*dbv_ai)(struct dbs_game *game,struct dbs_line *line);

struct dbs_game *dbf_init(struct dbs_game *game,const char *p1name,const char *p2name,unsigned int sqr,dbv_ai ai);
void dbf_destroy(struct dbs_game *game);


void dbf_setpoint_prev_x(struct dbs_game *game,struct dbs_point *point);
void dbf_setpoint_next_x(struct dbs_game *game,struct dbs_point *point);
void dbf_setpoint_prev_y(struct dbs_game *game,struct dbs_point *point);
void dbf_setpoint_next_y(struct dbs_game *game,struct dbs_point *point);


int dbf_setlinepoint(struct dbs_game *game,struct dbs_line *line);

struct dbs_line *dbf_getpointlinex(struct dbs_game *game,unsigned int linenum,struct dbs_line *line);
struct dbs_line *dbf_getpointliney(struct dbs_game *game,unsigned int linenum,struct dbs_line *line);

int dbf_setlinex(struct dbs_game *game,unsigned int linenum);
int dbf_setliney(struct dbs_game *game,unsigned int linenum);

void dbf_srandom (int seed);
int dbf_rand(void);
int dbf_random (int min, int max);

unsigned int dbf_countsqr(struct dbs_game *game);

unsigned int dbf_getremainline(struct dbs_game *game,struct dbs_line *line,unsigned int rcount);


unsigned int dbf_countbit(unsigned int num);
int dbf_postzerobit(unsigned int num);


int dbf_issetline(struct dbs_game *game,struct dbs_line *line);


struct dbs_line *dbf_copyline(struct dbs_line *dsk,struct dbs_line *src);
int dbf_aiv1_Friday(struct dbs_game *game,struct dbs_line *line);
int dbf_aiv2_Jarvis(struct dbs_game *game,struct dbs_line *line);

int dbf_gameplay(struct dbs_game *game,struct dbs_line *line,struct dbs_player *player);

int dbf_isgameover(struct dbs_game *game);



enum
{
gp_invline=-7,gp_invx=-6,gp_invy=-5,ai_nomove=-4,ai_errmalloc=-3,gp_gameover=-2,gp_gamenormal=-1,ai_best=0,ai_worse=1,ai_random=2
};

#ifdef _DEVRAND_
static const char DEVRAND[]="/dev/urandom";
#endif
