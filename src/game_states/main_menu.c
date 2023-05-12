/**
 * @file  main_menu.c
 * @brief The main menu of the game
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

#include <game_state.h>
#include <game_states/main_menu.h>
#include <game_states/main_game.h>
#include <game_states/help.h>
#include <game_states/leaderboard.h>
#include <menu_tools.h>

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define MAIN_MENU_BUTTON_COUNT 4 /**< @brief Number of buttons on main menu */
const char * MAIN_MENU_BUTTONS[MAIN_MENU_BUTTON_COUNT] = {
	"New Game", "Help", "Leaderboard", "Leave"
}; /**< @brief Text of the buttons on the main menu */

/** @brief Height of the main menu (includes contours and spacing) */
#define MAIN_MENU_HEIGHT (MAIN_MENU_BUTTON_COUNT + 6)
#define MAIN_MENU_WIDTH 30 /**< @brief Width of the main menu */

game_loop_callback_return_value state_main_menu_oninput(void *s, int key) {
	state_main_menu_data *state = state_extract_data(state_main_menu_data, s);

	switch (key) {
		/* Respond to arrow keys for button switching (with bounds checking) */
		case KEY_UP:
			state->button = menu_update_button(MAIN_MENU_BUTTON_COUNT, state->button, -1);
			state->needs_rerender = 1;
			break;

		case KEY_DOWN:
			state->button = menu_update_button(MAIN_MENU_BUTTON_COUNT, state->button, 1);
			state->needs_rerender = 1;
			break;

		/* Button choice (enter) */
		case '\r': {
			/* Create the game state for the chosen button */
			game_state new;
			switch (state->button) {
				case 0: /* New game */
					new = state_main_game_create();
					break;
				case 1: /* Help screen */
					new = state_help_create();
					break;
				case 2: /* Leaderboard */
					new = state_leaderboard_create();
					break;
				case 3: /* Leave */
					return GAME_LOOP_CALLBACK_RETURN_BREAK;
				default: /* Not supposed to happen */
					return GAME_LOOP_CALLBACK_RETURN_ERROR;
			}

			/* Change to the new state (or leave the game) */
			state_switch((game_state *) s, &new, 1);
			break;
		}

		case '\x1b': /* Escape */
			return GAME_LOOP_CALLBACK_RETURN_BREAK;

		default:
			break;
	}

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;

}

/** @brief Renders the main menu (only if re-rendering is needed) */
game_loop_callback_return_value state_main_menu_onrender(void *s, int width, int height) {
	state_main_menu_data *state = state_extract_data(state_main_menu_data, s);

	if (!state->needs_rerender)
		return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
	state->needs_rerender = 0;

	erase();

	/* Menu position and contours */
	int left = (width - MAIN_MENU_WIDTH) / 2, top = (height - MAIN_MENU_HEIGHT) / 2;
	menu_draw_box(left, top, MAIN_MENU_WIDTH, MAIN_MENU_HEIGHT);

	/* Draw game name */
	const char *game_name = "Roguelite";
	int len = strlen(game_name);
	move(top + 2, left + (MAIN_MENU_WIDTH - len) / 2); /* Centered with vertical spacing */
	printw("%s", game_name);

	/* Draw buttons */
	for (int i = 0; i < MAIN_MENU_BUTTON_COUNT; ++i) {
		if (i == state->button) {
			/* Highlight the whole of the line if this is the selected button */
			attron(A_REVERSE);
			move(top + i + 4, left + 1);
			for (int i = 1; i < MAIN_MENU_WIDTH - 1; ++i) addch(' ');
		}

		/* Print button text centered */
		len = strlen(MAIN_MENU_BUTTONS[i]);
		move(top + i + 4, left + (MAIN_MENU_WIDTH - len) / 2);
		printw("%s", MAIN_MENU_BUTTONS[i]);

		attroff(A_REVERSE);
	}

	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief Called when the terminal window is resized to request for a menu redraw */
game_loop_callback_return_value state_main_menu_onresize(void *s, int width, int height) {
	(void) width; (void) height;

	state_main_menu_data *state = state_extract_data(state_main_menu_data, s);
	state->needs_rerender = 1;

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_state state_main_menu_create(void) {

	state_main_menu_data data = {
		.needs_rerender = 1,
		.button = 0,
	};
	state_main_menu_data *data_ptr = malloc(sizeof(state_main_menu_data));
	*data_ptr = data;

	game_loop_callbacks callbacks = {
		.oninput  = state_main_menu_oninput,
		.onupdate = NULL,
		.onrender = state_main_menu_onrender,
		.onresize = state_main_menu_onresize,
	};

	game_state ret = {
		.data = data_ptr,
		.destroy = state_main_menu_destroy,
		.callbacks = callbacks
	};

	return ret;
}

void state_main_menu_destroy(game_state* state) {
	free(state->data);
}

