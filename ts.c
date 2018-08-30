#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <curses.h>


int sort_array(int c,char tosort[c][256]){
	char sorted[c][256];
	printf("\n\nchhiryo\n");
	for(int i=0; i<c; i++)
	{
		//printf("%d: %s\n",i,tosort[i]);
		strcpy(sorted[i],tosort[i]);
	}

	printf("mathi wala Sorted Directories:\n");
	int i=0;
	for(int i=0; i<c; i++)
	{
		printf("%d: %s\n",i,sorted[i]);
	}
	return 0;
}

int main(void)
{
	pid_t child;
	DIR *d;
	struct dirent *de;
	int i, c, k;
	char s[356], cmd[256];
	time_t t;

	while (1)
	{
		t = time(NULL);
		printf("\nCurrent Time: %s\n", ctime(&t));

		getcwd(s, 200);
		printf("Current Working Directory: %s \n\n", s);

		printf("\nFiles:");
		d = opendir(".");
		c = 0;
		while ((de = readdir(d)))
		{
			if ((de->d_type) & DT_REG)
			{
				printf("\t\t(%d File: %s)\n", c++, de->d_name);
			}
			/*if ((c % 8) == 0)
			{
				printf("Hit N for Next\n");
				k = getchar();
			}*/
		}
		closedir(d);

		printf("\nDirectories: ");
		d = opendir(".");
		c = 0;
		de = readdir(d);
		if ((de->d_type) & DT_DIR)
			printf("\t(%d Directory: %s)\n", c++, de->d_name);
		while ((de = readdir(d)))
		{
			if ((de->d_type) & DT_DIR)
				printf("\t\t(%d Directory: %s)\n", c++, de->d_name);
		}
		closedir(d);

		printf("\nOperations:\tD Display\n\t\tE Edit\n\t\tR Run\n\t\tC Change Directory\n\t\tS Sort Directory Listing\n\t\tM Move to Directory\n\t\tR Remove File\n\t\tQ Quit\n\n");
		
		c = getchar();
		if(c=='\n'){c = getchar();}
		//getchar();
		switch (c)
		{
		case 'q':
			exit(0);
		case 'e':
			printf("Edit what?:");
			scanf("%s", s);
			strcpy(cmd, "pico ");
			strcat(cmd, s);
			system(cmd);
			break;
		case 'r':
			printf("Run what?:");
			scanf("%s", cmd);
			system(cmd);
			break;
		case 'c':
			printf("Change To?:");
			scanf("%s", cmd);
			chdir(cmd);
			break;
		case 's':
			printf("Sorted Directories:");
			printf("\nDirectories: ");
			d = opendir(".");
			c = 0;
			de = readdir(d);
			if ((de->d_type) & DT_DIR)
				c++;
				
			while ((de = readdir(d)))
			{
				if ((de->d_type) & DT_DIR)
					c++;
					
			}
			closedir(d);

			char tosort[c][256];
			d = opendir(".");
			c = 0;
			de = readdir(d);
			if ((de->d_type) & DT_DIR)
			{
				strcpy(tosort[c],de->d_name);
				printf("\t(%d Directory: %s)\n", c++, tosort[c-1]);
			}
				
			while ((de = readdir(d)))
			{
				if ((de->d_type) & DT_DIR)
				{
					strcpy(tosort[c],de->d_name);
					printf("\t\t(%d Directory: %s)\n", c++, tosort[c-1]);
				}
					
			}
			closedir(d);
			sort_array(c,tosort);
			break;
		}
	}
}


