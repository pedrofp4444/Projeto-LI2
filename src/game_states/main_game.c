/**
 * @file  main_game.c
 * @brief The implementation for main game state (where you catually play the game)
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

#include <game_states/main_game.h>

#include <stdlib.h>
#include <ncurses.h>

game_loop_callback_return_value state_main_game_oninput(void *s, int key) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	switch (key) {
		case '\x1b':
			return GAME_LOOP_CALLBACK_RETURN_BREAK; /* Exit game on escape */

		case KEY_UP:
			state->offsety--;
			break;
		case KEY_DOWN:
			state->offsety++;
			break;

		case KEY_LEFT:
			state->offsetx--;
			break;
		case KEY_RIGHT:
			state->offsetx++;
			break;
	}

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_loop_callback_return_value state_main_game_onrender(void *s, int width, int height) {
	(void) width; (void) height;
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	erase();

	move(height / 2 + state->offsety, width / 2 + state->offsetx);
	addch('@');

	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_state state_main_game_create(void) {
	state_main_game_data data = {
		.offsetx = 0, .offsety = 0
	};
	state_main_game_data *data_ptr = malloc(sizeof(state_main_game_data));
	*data_ptr = data;

	game_loop_callbacks callbacks = {
		.oninput  = state_main_game_oninput,
		.onupdate = NULL,
		.onrender = state_main_game_onrender,
		.onresize = NULL
	};

	game_state ret = {
		.data = data_ptr,
		.destroy = state_main_game_destroy,
		.callbacks = callbacks
	};
	return ret;
}

void state_main_game_destroy(const game_state* state) {
	free(state->data);
}

