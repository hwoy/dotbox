#include <stdio.h>
#include <string.h>
#include "dotbox.h"
#include "dotbox_io.h"

static void replacech(char *str,char ch,char rp);

void printTable(struct dbs_game *game,unsigned int len)
{
	unsigned int x,y,i,j;
	putchar('\n');
	for(y=0;y<game->sqr+1;y++)
	{
		for(j=0;j<len;j++)
		{
			putchar('\t');
		for(x=0;x<game->sqr+1;x++)
		{
			if(j==0)
			{
				putchar('*');
			if(game->point[y*(game->sqr+1)+x].next_x==STAMP && game->point[y*(game->sqr+1)+x+1].prev_x==STAMP && x<game->sqr)
			{
				for(i=0;i<len;i++)putchar('-');
			}
			else
			{
				for(i=0;i<len;i++)putchar(0x20);
			}
			}
			else
			{
				if(game->point[y*(game->sqr+1)+x].next_y==STAMP && game->point[(y+1)*(game->sqr+1)+x].prev_y==STAMP)
				{
					putchar('|');
				}
				else
				{
					putchar(0x20);
				}
				for(i=0;i<len;i++)putchar(0x20);
			}
		}
		if(y<game->sqr)putchar('\n');
		
		}
		
	}
	putchar('\n');
}



char dio_getch(char *buff,int size,char dkey)
{
		
	fgets(buff,size,stdin);
	replacech(buff,13,0);
	replacech(buff,10,0);
	if(!strlen(buff))
	{
		return dkey;
	}
	return buff[0];
		
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
