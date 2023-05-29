/**
 * @file  name_input.c
 * @brief Where a player inputs their name before playing
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
#include <game_states/name_input.h>
#include <game_states/main_menu.h>
#include <game_states/main_game.h>
#include <menu_tools.h>
#include <score.h>

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/** @brief The height of the content on the center on the screen (title + spacing + input box) */
#define INPUT_HEIGHT (1 + 1 + 3)
/** @brief The width of the input content (maximum name size + input box margins) */
#define INPUT_WIDTH  (32 + 2)

/**
 * @brief Responds to user input in the name input field
 * @author A104348 Humberto Gomes
 */
game_loop_callback_return_value state_name_input_oninput(void *s, int key) {
	state_name_input_data *state = state_extract_data(state_name_input_data, s);

	(void) state;
	switch (key) {
		case '\x1b': { /* Escape - return to main menu */
			game_state new = state_main_menu_create();
			state_switch((game_state *) s, &new, 1);
			break;
		}

		case '\r': { /* Enter - proceed to game if the name is not empty */
			game_state new = state_main_game_create(state->name);
			state_switch((game_state *) s, &new, 1);
			break;
		}

		case KEY_BACKSPACE: { /* Backspace */
			int len = strlen(state->name);
			if (len > 0) state->name[len - 1] = '\0';
			state->needs_rerender = 1;
			break;
		}

		default: {
			int len = strlen(state->name);
			if ((('a' <= key && key <= 'z') || ('A' <= key && key <= 'Z') ||
			    ('0' <= key && key <= '9') || key == ' ' || key == '-' || key == '_') &&
			    len < SCORE_NAME_MAX) {

				state->name[len] = key;
				state->name[len + 1] = '\0';
				state->needs_rerender = 1;
			}

			break;
		}
	};

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/**
 * @brief Renders the input box (only if re-rendering is needed)
 * @author A104348 Humberto Gomes
 */
game_loop_callback_return_value state_name_input_onrender(void *s, int width, int height) {
	state_name_input_data *state = state_extract_data(state_name_input_data, s);

	if (!state->needs_rerender)
		return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
	state->needs_rerender = 0;

	erase();

	/* Position of the input */
	int top = (height - INPUT_HEIGHT) / 2, left = (width - INPUT_WIDTH) / 2;

	/* Ask user to input text */
	const char *input_request = "Enter your name";
	int len = strlen(input_request);
	move(top, (width - len) / 2);
	printw("%s", input_request);

	/* Text input field */
	menu_draw_box(left, top + 2, INPUT_WIDTH, 3);
	move(top + 3, left + 1);
		printw("%s", state->name);

	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/**
 * @brief Called when the terminal window is resized to request for an input box redraw
 * @author A104348 Humberto Gomes
 */
game_loop_callback_return_value state_name_input_onresize(void *s, int width, int height) {
	(void) width; (void) height;

	state_name_input_data *state = state_extract_data(state_name_input_data, s);
	state->needs_rerender = 1;

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_state state_name_input_create(void) {
	state_name_input_data data = {
		.needs_rerender = 1,
	};
	memset(data.name, 0, SCORE_NAME_MAX + 1);

	state_name_input_data *data_ptr = malloc(sizeof(state_name_input_data));
	*data_ptr = data;

	game_loop_callbacks callbacks = {
		.oninput  = state_name_input_oninput,
		.onupdate = NULL,
		.onrender = state_name_input_onrender,
		.onresize = state_name_input_onresize,
	};

	game_state ret = {
		.data = data_ptr,
		.destroy = state_name_input_destroy,
		.callbacks = callbacks
	};

	return ret;
}

void state_name_input_destroy(game_state *state) {
	free(state->data);
}

