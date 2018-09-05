#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include <ncurses.h>
#include <sys/stat.h>

int sort_array(int c, char tosort[c][256])
{
	char sorted[c][256];
	char temp[256];
	for (int i = 0; i < c; i++)
	{
		//printf("%d: %s\n",i,tosort[i]);
		strcpy(sorted[i], tosort[i]);
	}

	for (int i = 0; i < c - 1; i++)
	{
		for (int j = i + 1; j < c; j++)
		{
			if (strcmp(sorted[i], sorted[j]) > 0)
			{
				strcpy(temp, sorted[i]);
				strcpy(sorted[i], sorted[j]);
				strcpy(sorted[j], temp);
			}
		}
	}

	printf("\t(%d Directory: %s)\n", 0, sorted[0]);
	for (int i = 1; i < c; i++)
	{
		printf("\t\t\t(%d Directory: %s)\n", i, sorted[i]);
	}
	return 0;
}

int main(void)
{
	pid_t child;
	DIR *d;
	struct dirent *de;
	struct stat statbuffer;
	int i, c, k;
	char s[356], cmd[256];
	time_t t;

	initscr();
	WINDOW *win = newwin(40, 150, 1, 1); //create window

	while (1)
	{
		werase(win);
		refresh();
		box(win, 0, 0); //put window
		wrefresh(win);
		start_color();  //start color
		t = time(NULL);

		init_pair(1, COLOR_GREEN, COLOR_BLACK); //initializing color
		attron(COLOR_PAIR(1));
		attron(A_BOLD);
		move(2, 65);
		printw("|| TEXT-BASED SHELL UTILITY ||");
		attroff(A_BOLD);
		attroff(COLOR_PAIR(1));

		mvwprintw(win, 4, 1, "Current Time: %s", ctime(&t));
		getcwd(s, 200);
		mvwprintw(win, 5, 1, "Current Working Directory: %s", s);

		mvwprintw(win, 9, 1, "Files:");
		d = opendir(".");
		c = 0;
		int y = 10, x = 5;
		while ((de = readdir(d)))
		{
			if ((de->d_type) & DT_REG)
			{
				if (stat(de->d_name, &statbuffer) == -1)
        			continue;
				mvwprintw(win, y++, x, "(%d File: %s)", c++, de->d_name);
				mvwprintw(win,y-1,50,"%d bytes", (intmax_t)statbuffer.st_size);
			}
			/*if ((c % 8) == 0)
			{
				printf("Hit N for Next\n");
				k = getchar();
			}*/
		}
		closedir(d);
		y++;
		mvwprintw(win, y++, 1, "Directories: ");
		d = opendir(".");
		c = 0;
		
		while ((de = readdir(d)))
		{
			if ((de->d_type) & DT_DIR)
			{
				if (stat(de->d_name, &statbuffer) == -1)
        			continue;
				mvwprintw(win,y++, x, "(%d Directory: %s)", c++, de->d_name);
				mvwprintw(win,y-1,50,"%d bytes", (intmax_t)statbuffer.st_size);
			}
		}
		closedir(d);

		y++;
		mvwprintw(win, y++, 1, "Operations:");
		mvwprintw(win, y++, x, "D Display");
		mvwprintw(win, y++, x, "E Edit");
		mvwprintw(win, y++, x, "R RUN");
		mvwprintw(win, y++, x, "C Change Directory");
		mvwprintw(win, y++, x, "S Sort Directory");
		mvwprintw(win, y++, x, "M Move Directory");
		mvwprintw(win, y++, x, "D Remove Directory");
		mvwprintw(win, y++, x, "Q Quit");

		refresh();
		wrefresh(win);
		y+=2; x=2;
		wmove(win,y,x);
		char z;
		wgetnstr(win, &z, 1);
		x=2;
		switch (z)
		{
		case 'q':
		case 'Q':
			endwin();
			system("clear");
			exit(0);
		case 'e':
		case 'E':
			mvwprintw(win, y++, x, "Edit what?:");
			scanf("%s", s);
			strcpy(cmd, "pico ");
			strcat(cmd, s);
			system(cmd);
			break;
		case 'r':
		case 'R':
			mvwprintw(win, y++, x, "Run what?:");
			scanf("%s", cmd);
			system(cmd);
			break;
		case 'c':
		case 'C':
			mvwprintw(win, y++, x, "Change To?:");
			wgetstr(win, cmd);
			chdir(cmd);
			break;
		case 'd':
		case 'D':
			mvwprintw(win, y++, x, "Remove which?:");
			wgetstr(win,cmd);
			rmdir(cmd);
			break;
		case 's':
			mvwprintw(win, y++, x, "Sorted Directories:");
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

			int tosort[c];
			d = opendir(".");
			c = 0;
			de = readdir(d);
			if (((de->d_type) & DT_DIR) & (stat(de->d_name, &statbuffer) == -1))
			{
				tosort[c] = (intmax_t)statbuffer.st_size;
				c++;
			}

			while ((de = readdir(d)))
			{
				if (((de->d_type) & DT_DIR) & (stat(de->d_name, &statbuffer) == -1))
				{
					tosort[c] = (intmax_t)statbuffer.st_size;
					c++;
				}
			}
			closedir(d);
			//sort_array(c, tosort);
			break;
		}
	}
	return 0;
}
