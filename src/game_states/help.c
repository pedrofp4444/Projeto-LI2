/**
 * @file  help.c
 * @brief The help screen accessible by the main menu
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
#include <game_states/help.h>
#include <game_states/main_menu.h>
#include <menu_tools.h>

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define HELP_TEXT_LINE_COUNT 17 /**< @brief Number of buttons on main menu */
const char * HELP_TEXT[HELP_TEXT_LINE_COUNT] = {
	"Welcome to roguelite!",
	"",
	"This is a turn-based game. First, you move, then attack, then mobs",
	"move, then mobs attack. Your goal is roam around the map killing mobs.",
	"",
	"Weapons:",
	"",
	"Hand, Dagger and iPad: Close combat, with increasing range and damage",
	"Arrows: You must be aligned vertically or horizontally with a mob,",
	"        without walls between you and them",
	"Bombs: Throw to any entity you can see",
	"",
	"Mobs:",
	"",
	"Rats      (blue  R): Easy to kill",
	"Goblins   (green G): Tough luck, my friend",
	"Cristinos (pink  M): RUN FOR YOUR LIFE"
}; /**< @brief Text of the buttons on the main menu */

/**
 * @brief Responds to player input in the help screen
 * @author A104348 Humberto Gomes
 */
game_loop_callback_return_value state_help_oninput(void *s, int key) {
	switch (key) {
		case '\x1b': { /* Escape - return to main menu */
			game_state new = state_main_menu_create();
			state_switch((game_state *) s, &new, 1);
			break;
		}

		default:
			break;
	}

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/**
 * @brief Renders the help message (only if re-rendering is needed)
 * @author A104348 Humberto Gomes
 */
game_loop_callback_return_value state_help_onrender(void *s, int width, int height) {
	state_help_data *state = state_extract_data(state_help_data, s);

	if (!state->needs_rerender)
		return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
	state->needs_rerender = 0;

	erase();

	/* Help message dimensions and position */
	int message_width = 0; /* Width of widest line */
	for (int i = 0; i < HELP_TEXT_LINE_COUNT; ++i) {
		int len = strlen(HELP_TEXT[i]);
		if (len > message_width)
			message_width = len;
	}
	int left = (width - message_width) / 2, top = (height - HELP_TEXT_LINE_COUNT) / 2;

	/* Draw message */
	for (int i = 0; i < HELP_TEXT_LINE_COUNT; ++i) {
		move(top + i, left);
		printw("%s", HELP_TEXT[i]);
	}

	/* Draw user guidance message */
	const char *esc_message = "Press ESC to go back";
	int len = strlen(esc_message);
	move(height - 2, (width - len) / 2);
	printw("%s", esc_message);

	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}



/**
 * @brief Called when the terminal window is resized to request for an help message redraw
 * @author A104348 Humberto Gomes
 */
game_loop_callback_return_value state_help_onresize(void *s, int width, int height) {
	(void) width; (void) height;

	state_help_data *state = state_extract_data(state_help_data, s);
	state->needs_rerender = 1;

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_state state_help_create(void) {
	state_help_data data = {
		.needs_rerender = 1,
	};
	state_help_data *data_ptr = malloc(sizeof(state_help_data));
	*data_ptr = data;


	game_loop_callbacks callbacks = {
		.oninput  = state_help_oninput,
		.onupdate = NULL,
		.onrender = state_help_onrender,
		.onresize = state_help_onresize,
	};

	game_state ret = {
		.data = data_ptr,
		.destroy = state_help_destroy,
		.callbacks = callbacks
	};

	return ret;
}

void state_help_destroy(game_state* state) {
	free(state->data);
}

