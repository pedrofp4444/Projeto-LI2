/**
 * @file help.c
 * @brief Instructions of the game
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

#include <views/help.h>

#include <ncurses.h>
#include <string.h>

void draw_help(WINDOW* win) {
	int i, x, y;
	char* help_text[] = {
		"Welcome to Roguelite. In this turn-based adventure,",
		"you'll face off against a range of enemies, from the weak",
		"and easily defeatable rats to the cowardly goblins",
		"and the powerful Cristinos, who can even paralyze you.",
		"",
		"To defeat your enemies, you'll have a variety of weapons ",
		"at your disposal. Choose your weapons carefully, as each has its own strengths and weaknesses.",
		"","",
		"Usefull commands:"
		"",
		"Use the arrow keys on your keyboard to move your character or aim your weapon,",
		"Press 's' to skip your turn,",
		"Press enter to confirm your play, just be sure that no Cristino",
		"is around the corner.",
		"And here's a helpful tip: The worst enemy of Cristinos oddly are lanterns.",
		"Keep that in mind as you explore the game world and battle your way to victory!"
	};
	int num_help_lines = sizeof(help_text) / sizeof(help_text[0]);

	getmaxyx(win, y, x);
	if (y < num_help_lines + 2 || x < (int)(strlen("Help") + 2)) {
		return;
	}

	werase(win);

	// Draw box
	box(win, 0, 0);

	// Draw title
	attron(COLOR_PAIR(1));
	mvwprintw(win, 0, 2, "Help");
	attroff(COLOR_PAIR(1));

	// Draw help text
	for (i = 0; i < num_help_lines; i++) {
		int line_length = strlen(help_text[i]);
		int max_line_length = x - 4;
		int num_lines = (line_length + max_line_length - 1) / max_line_length;

		for (int j = 0; j < num_lines; j++) {
			int start_index = j * max_line_length;
			mvwprintw(win, i + 2 + j, 2, "%.*s", max_line_length, help_text[i] + start_index);
		}
	}

	// Draw "Back" button
	int button_y = y - 2;
	int button_x = x - 11;

	wattron(win, A_REVERSE);
	mvwprintw(win, button_y, button_x, " Back ");
	wattroff(win, A_REVERSE);

	wrefresh(win);
}


void help() {
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

	int menuHeight = 20;
	int menuWidth = 67;
	int menuX = (x - menuWidth) / 2;
	int menuY = (y - menuHeight) / 2;

	// Create help window
	WINDOW* help = newwin(menuHeight, menuWidth, menuY, menuX);
	wbkgd(help, COLOR_PAIR(2));
	draw_help(help);
	mvwprintw(help, 0, 1, "help");

	int c;
	while ((c = getch()) != KEY_F(1)) {
		if (c == 10) { // Enter key
			clear();
			refresh();
			delwin(help);
			endwin();
			return;
			break;
		}
		draw_help(help);
	}

	delwin(help);
	endwin();
}

