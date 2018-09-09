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

//function to sort array by their size
void sort_size(struct dirent **tosort, int size)
{
	struct dirent *temp;
	struct stat buffer_m, buffer_c;
	int i, j, m;
	for (i = 0; i < size - 1; i++)
	{
		for (j = i + 1; j < size; j++)
		{
			stat(tosort[m]->d_name, &buffer_m);
			stat(tosort[j]->d_name, &buffer_c);
			if (buffer_c.st_size < buffer_m.st_size)
				m = j;
		}
		temp = tosort[m];
		tosort[m] = tosort[i];
		tosort[i] = temp;
	}
}

//function to sort array by their last modified date
void sort_date(struct dirent **tosort, int size)
{
	struct dirent *temp;
	struct stat buffer_m, buffer_c;
	int i, j, m;
	for (i = 0; i < size - 1; i++)
	{
		m = i;
		for (j = i + 1; j < size; j++)
		{
			stat(tosort[m]->d_name, &buffer_m);
			stat(tosort[j]->d_name, &buffer_c);
			if (difftime(buffer_c.st_mtime, buffer_m.st_mtime) < 0)
			{
				m = j;
			}
		}
		temp = tosort[m];
		tosort[m] = tosort[i];
		tosort[i] = temp;
	}
}

int main(void)
{
	pid_t child;
	DIR *d;
	struct dirent *de;
	struct dirent *directories[1024]; //array to store list of directory
	struct dirent *files[1024];		  //array to store list of files
	int fsize;						  //to store total files in array
	int dsize;						  //to store total directory in array
	struct stat statbuffer;			  //stat buffer to return file attributes
	int i = 0, c;
	int dcount, fcount; //counter to manage current directory || file list view
	char s[1024];
	char cmd[1024];
	char from[1024];   //char array to handle move directory feature
	char to[1024];	 //char array to handle move directory feature
	char sortby = 'x'; //char to manage sorting options
	time_t t;

	initscr();
	WINDOW *win = newwin(40, 150, 1, 5); //create window 40x150 at 5,1

	while (1)
	{
		werase(win); //clear the window
		refresh();
		box(win, 0, 0); //put window
		wrefresh(win);  //refresh the window

		start_color(); //enable color features

		init_pair(1, COLOR_GREEN, COLOR_BLACK); //initializing color pair 1
		attron(COLOR_PAIR(1));					//enable color pait 1
		attron(A_BOLD);							//enable bold text
		move(2, 65);
		printw("|| TEXT-BASED SHELL UTILITY ||");
		attroff(A_BOLD);		//disable bold text
		attroff(COLOR_PAIR(1)); //disable bold text

		t = time(NULL);
		mvwprintw(win, 4, 1, "Current System Time: %s", ctime(&t)); //display current system time
		getcwd(s, 200);
		mvwprintw(win, 5, 1, "Current Working Directory: %s", s); //display current working directory

		//count total number of directories
		dsize = 0;
		d = opendir(".");
		while ((de = readdir(d)))
		{
			if ((de->d_type) & DT_DIR)
			{
				directories[dsize] = de;
				dsize++;
			}
		}
		closedir(d);

		//count total numvber of files
		fsize = 0;
		d = opendir(".");
		while ((de = readdir(d)))
		{
			if ((de->d_type) & DT_REG)
			{
				files[fsize] = de;
				fsize++;
			}
		}
		closedir(d);

		//check for sort directories option
		if (sortby == 's')
		{
			sort_size(directories, dsize);
			sort_size(files, fsize);
		}
		else if (sortby == 'd')
		{
			sort_date(directories, dsize);
			sort_date(files, fsize);
		}

		int y = 8, x = 5; //initializing the x and y co-ordinates
		//display all files
		mvwprintw(win, y++, 1, "Files: ");
		d = opendir(".");
		c = 0;

		for (i = fcount; i < (fcount + 5); i++)
		{
			if (i >= fsize)
				break;

			stat(files[i]->d_name, &statbuffer);

			mvwprintw(win, y++, x, "(%d File: %s)", i, files[i]->d_name);		 //listing file names
			mvwprintw(win, y - 1, 50, "%d bytes", (intmax_t)statbuffer.st_size); //listing file size
			t = statbuffer.st_mtime;
			mvwprintw(win, y - 1, 65, "%s", ctime(&t)); //listing file's last modified date
		}
		if (i == 5)
			mvwprintw(win, y++, x, "l - NEXT");
		else if (i > 5 && i < fsize)
			mvwprintw(win, y++, x, "k- PREVIOUS\tl - NEXT");
		else if (fsize > 5)
			mvwprintw(win, y++, x, "k- PREVIOUS");

		closedir(d);

		y++;
		//display all directory
		mvwprintw(win, y++, 1, "Directories: ");
		d = opendir(".");
		c = 0;

		for (i = dcount; i < (dcount + 5); i++)
		{
			if (i >= dsize)
				break;

			stat(directories[i]->d_name, &statbuffer);

			mvwprintw(win, y++, x, "(%d Directory: %s)", i, directories[i]->d_name); //listing directory names
			mvwprintw(win, y - 1, 50, "%d bytes", (intmax_t)statbuffer.st_size);	 //listing directory size
			t = statbuffer.st_mtime;
			mvwprintw(win, y - 1, 65, "%s", ctime(&t)); //listing directory's last modified date
		}
		if (i == 5)
			mvwprintw(win, y++, x, "n - NEXT");
		else if (i > 5 && i < dsize)
			mvwprintw(win, y++, x, "p- PREVIOUS\tn - NEXT");
		else if (dsize > 5)
			mvwprintw(win, y++, x, "p - PREVIOUS");

		closedir(d);

		y += 2;
		mvwprintw(win, y++, 1, "Operations:");
		mvwprintw(win, y++, x, "V Display");
		mvwprintw(win, y++, x, "E Edit");
		mvwprintw(win, y++, x, "R RUN");
		mvwprintw(win, y++, x, "C Change Directory");
		mvwprintw(win, y++, x, "S Sort Directory");
		mvwprintw(win, y++, x, "M Move Directory");
		mvwprintw(win, y++, x, "D Remove Directory");
		mvwprintw(win, y++, x, "Q Quit");

		refresh();
		wrefresh(win); //refreshing window to display new updates on window
		y += 2;
		x = 2;
		wmove(win, y, x); //changing x,y co-ordinates in window
		char z;
		wgetnstr(win, &z, 1); //get a character to operate corresponding functionm
		x = 2;
		switch (z)
		{
		case 'n':
		case 'N':
			if (dsize >= 6 && dcount < (dsize - 5))
				dcount += 5;
			break;
		case 'p':
		case 'P':
			if (dcount != 0)
				dcount -= 5;
			break;
		case 'l':
		case 'L':
			if (fsize >= 6 && fcount < (fsize - 5))
				fcount += 5;
			break;
		case 'k':
		case 'K':
			if (fcount != 0)
				fcount -= 5;
			break;
		case 'q':
		case 'Q':
			endwin();
			system("clear");
			exit(0);
		case 'e':
		case 'E':
			mvwprintw(win, y++, x, "Edit what?:");
			wgetstr(win, s);
			strcpy(cmd, "pico ");
			strcat(cmd, s);
			system(cmd);
			break;
		case 'r':
		case 'R':
			mvwprintw(win, y++, x, "Run what?:");
			wgetstr(win, cmd);
			system(cmd);
			break;
		case 'c':
		case 'C':
			mvwprintw(win, y++, x, "Change To?:");
			wgetstr(win, cmd);
			dcount = 0;
			fcount = 0;
			chdir(cmd);
			break;
		case 'd':
		case 'D':
			mvwprintw(win, y++, x, "Remove which?:");
			wgetstr(win, cmd);
			rmdir(cmd); 
			break;
		case 'v':
		case 'V':
			mvwprintw(win, y++, x, "Display which?:");
			wgetstr(win, s);
			system("clear");
			strcpy(cmd, "cat ");
			strcat(cmd, s);
			system(cmd);
			getch();
			system("clear");
			break;
		case 'm':
		case 'M':
			mvwprintw(win, y++, x, "Move which?:");
			wgetstr(win, from);
			mvwprintw(win, y++, x, "Move where?:");
			wgetstr(win, to);
			strcpy(cmd, "mv ");
			strcat(cmd, from);
			strcat(cmd, " ");
			strcat(cmd, to);
			system("clear");
			system(cmd);
			break;
		case 's':
		case 'S':
			mvwprintw(win, y++, x, "Sort by? s - size || d - date || other - default:");
			wgetnstr(win, &sortby, 1);
			break;
		default:
			mvwprintw(win, y++, x, "Invalid choice!");
			wgetstr(win, cmd);
			break;
		}
	}
	return 0;
}
