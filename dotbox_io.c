#include <stdio.h>
#include <string.h>
#include "dotbox.h"
#include "dotbox_io.h"

static void replacech(char *str,char ch,char rp);

void printTable(FILE *fp,struct dbs_game *game,unsigned int len)
{
	unsigned int x,y,i,j;
	fputc('\n',fp);
	for(y=0;y<game->sqr+1;y++)
	{
		for(j=0;j<len+1;j++)
		{
			fputc('\t',fp);
		for(x=0;x<game->sqr+1;x++)
		{
			if(j==0)
			{
				fputc('*',fp);
			if(game->point[y*(game->sqr+1)+x].next_x==STAMP && game->point[y*(game->sqr+1)+x+1].prev_x==STAMP && x<game->sqr)
			{
				for(i=0;i<len;i++)fputc('-',fp);
			}
			else
			{
				for(i=0;i<len;i++)fputc(0x20,fp);
			}
			}
			else
			{
				if(game->point[y*(game->sqr+1)+x].next_y==STAMP && game->point[(y+1)*(game->sqr+1)+x].prev_y==STAMP)
				{
					fputc('|',fp);
				}
				else
				{
					fputc(0x20,fp);
				}
				for(i=0;i<len;i++)fputc(0x20,fp);
			}
		}
		if(y<game->sqr)fputc('\n',fp);
		
		}
		
	}
	fputc('\n',fp);
}




char dio_getch(char *buff,int size,char dkey)
{
	dio_getstr(buff,size);
	if(!strlen(buff))
	{
		return dkey;
	}
	return buff[0];
		
}

char *dio_getstr(char *buff,int size)
{
		
	fgets(buff,size,stdin);
	replacech(buff,13,0);
	replacech(buff,10,0);
	
	return buff;
		
}

static void replacech(char *str,char ch,char rp)
{
	int i;
	for(i=0;str[i];i++)
	{
		if(str[i]==ch)
		{
			str[i]=rp;
		}
	}
}
