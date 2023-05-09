/**
 * @file leaderboard.c
 * @brief Leaderboard of the game
 */

/*
 *   Copyright 2023 Hélder Gomes, Humberto Gomes, Mariana Rocha, Pedro Pereira
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <views/leaderboard.h>
#include <views/main_menu.h>

#include <ncurses.h>
#include <string.h>

Player players[] = {
	{"Alice", 100},
	{"Bob", 200},
	{"Charlie", 300},
	{"Dave", 400},
	{"Eve", 500},
	{"Cristino", 600}
};

int current_player = 0;

void draw_leaderboard(WINDOW* win) {
	int i, j, x, y, rank_width = 4, name_width = 15, score_width = 6;
	int num_players = sizeof(players) / sizeof(players[0]);

	getmaxyx(win, y, x);
	if (y < num_players + 2 || x < rank_width + name_width + score_width + 2) {
		return;
	}

	werase(win);
	box(win, 0, 0);

	// Draw title
	attron(COLOR_PAIR(1));
	mvwprintw(win, 0, 2, "Leaderboard");
	attroff(COLOR_PAIR(1));

	// Draw title
	attron(COLOR_PAIR(1));
	mvwprintw(win, 1, (x-strlen("Leaderboard"))/2, "Leaderboard");
	attroff(COLOR_PAIR(1));

	mvwprintw(win, 2, 2, "%-*s %-*s %-*s", rank_width, "Rank", name_width, "Name", score_width, "Score");

	for (i = 0; i < num_players; i++) {
		mvwprintw(win, i+3, 2, "%*d. ", rank_width-1, i+1);

		if (i == current_player) {
			wattron(win, A_REVERSE);
		}
		mvwprintw(win, i+3, rank_width+3, "%-*s", name_width, players[i].name);
		wattroff(win, A_REVERSE);

		mvwprintw(win, i+3, rank_width+3+name_width+1, "%*d", score_width, players[i].score);
	}

	// Draw shading
	for (j = 1; j < y-1; j++) {
		for (i = 1; i < x-1; i++) {
			if (i >= rank_width+name_width+score_width+3 || j >= num_players+3) {
				wattron(win, A_DIM);
				mvwprintw(win, j, i, " ");
				wattroff(win, A_DIM);
			}
		}
	}

	// Draw "Back" button
	int button_y = y - 3;
	int button_x = x - 11;

	if (current_player == -1) {
		wattron(win, A_REVERSE);
	}

	mvwprintw(win, button_y, button_x, " Back ");

	if (current_player == -1) {
		wattroff(win, A_REVERSE);
	}

	// Draw shading for names
	for (i = 0; i < num_players; i++) {
		if (i == current_player) {
			wattron(win, A_REVERSE);
		}
		mvwprintw(win, i + 3, rank_width + 3, "%-*s", name_width, players[i].name);
		wattroff(win, A_REVERSE);
	}

	wrefresh(win);
}

void leaderboard() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	// Initialize colors
	if (has_colors()) {
		start_color();
		init_pair(1, COLOR_YELLOW, COLOR_BLUE);
		init_pair(2, COLOR_WHITE, COLOR_BLUE);
		init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	}

	// Enable mouse events
	mousemask(ALL_MOUSE_EVENTS, NULL);

	refresh();

	int y, x;
	getmaxyx(stdscr, y, x);

	int menuHeight = 14;
	int menuWidth = 40;
	int menuX = (x - menuWidth) / 2;
	int menuY = (y - menuHeight) / 2;

	// Create leaderboard window
	WINDOW* leaderboard = newwin(menuHeight, menuWidth, menuY, menuX);
	wbkgd(leaderboard, COLOR_PAIR(2));
	draw_leaderboard(leaderboard);
	mvwprintw(leaderboard, 0, 1, "Leaderboard");
	int num_players = (int)(sizeof(players) / sizeof(players[0]));

	int c;
	MEVENT event;
	while ((c = getch()) != KEY_F(1)) {
		switch (c) {
			case KEY_UP:
				if (current_player == -1) {
					current_player = num_players - 1;
				} else {
					current_player--;
					if (current_player < 0) {
						current_player = - 1;
					}
				}
				break;
			case KEY_DOWN:
				if (current_player == -1) {
					current_player = 0;
				} else if (current_player == num_players - 1) {
					current_player = -1;
				} else {
					current_player++;
				}
				break;
			case KEY_MOUSE:
					if (getmouse(&event) == OK) {
					int row = event.y - 3;
					if (row >= 0 && row < (int)(sizeof(players) / sizeof(players[0]))) {
						current_player = row;
					}
				}
				break;
			case 10: // Enter key
				if (current_player == -1) {
					clear();
					refresh();
					delwin(leaderboard);
					endwin();
					return;
				}
				break;
		}
		draw_leaderboard(leaderboard);
	}
	delwin(leaderboard);
	endwin();
}

