#define CHX '-'
#define CHY '|'
#define CHSP 0x20

void printTable(FILE *fp,struct dbs_game *game,unsigned int len);
char dio_getch(char *buff,int size,char dkey);
char *dio_getstr(char *buff,int size);
