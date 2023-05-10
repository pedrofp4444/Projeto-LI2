/**
 * @file main_menu.c
 * @brief Main menu of the game
 */

/*
 *   Copyright 2023 Hélder Gomes, Humberto Gomes, Mariana Rocha, Pedro Pereira
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *	   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <views/leaderboard.h>
#include <views/main_menu.h>
#include <views/help.h>

#include <game_states/main_game.h>
#include <game_state.h>

#include <ncurses.h>
#include <string.h>

int selectedButton = 0;

void draw_menu(WINDOW* win) {
	int i, x, y;
	int num_buttons = 4;
	const char *buttonLabels[] = {
		"New Game",
		"Help",
		"Leaderboard",
		"Leave"
	};

	getmaxyx(win, y, x);
	if (y < num_buttons + 2 || x < 12) {
		return;
	}

	werase(win);
	box(win, 0, 0);

	// Draw title
	attron(COLOR_PAIR(1));
	mvwprintw(win, 0, 2, "Menu");
	attroff(COLOR_PAIR(1));

	for (i = 0; i < num_buttons; i++) {
		if (i == selectedButton) {
			wattron(win, A_REVERSE);
		}

		int x_offset = (x - strlen(buttonLabels[i])) / 2;
		mvwprintw(win, i + 2, x_offset, "%s", buttonLabels[i]);

		if (i == selectedButton) {
			wattroff(win, A_REVERSE);
		}
	}

	wrefresh(win);
}

int menu() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	int num_buttons = 4;

	// Initialize colors
	if (has_colors()) {
		start_color();
		init_pair(1, COLOR_YELLOW, COLOR_BLUE);
		init_pair(2, COLOR_WHITE, COLOR_BLUE);
		init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	}

	refresh();

	int y, x;
	getmaxyx(stdscr, y, x);

	int menuHeight = 10;
	int menuWidth = 40;
	int menuX = (x - menuWidth) / 2;
	int menuY = (y - menuHeight) / 2;

	// Create menu window
	WINDOW* menu_win = newwin(menuHeight, menuWidth, menuY, menuX);
	wbkgd(menu_win, COLOR_PAIR(2));

	// Draw menu
	draw_menu(menu_win);

	// Enable mouse events
	mousemask(ALL_MOUSE_EVENTS, NULL);

	int c;
	MEVENT event;
	while ((c = getch()) != KEY_F(1)) {
		switch (c) {
			case KEY_UP:
				selectedButton = (selectedButton - 1 + num_buttons) % num_buttons;
				break;
			case KEY_DOWN:
				selectedButton = (selectedButton + 1) % num_buttons;
				break;
			case KEY_MOUSE:
				if (getmouse(&event) == OK) {
					int row = event.y - 2;
					if (row >= 0 && row < num_buttons) {
						selectedButton = row;
					}
				}
				break;
			case 10: // Enter key
				switch (selectedButton) {
					case 0:
						delwin(menu_win);
						endwin();
						return 1;
					case 1:
						help();
						break;
					case 2:
						leaderboard();
						break;
					case 3:
						delwin(menu_win);
						endwin();
						return 2;
				}
				break;
		}
		draw_menu(menu_win);
	}

	delwin(menu_win);
	endwin();

	return 0;
}

