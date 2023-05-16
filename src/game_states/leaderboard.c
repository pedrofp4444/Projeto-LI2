/**
 * @file  leaderboard.c
 * @brief The game state to show the leaderboard to the player
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
#include <game_states/leaderboard.h>
#include <game_states/main_menu.h>
#include <menu_tools.h>

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/** @brief Height of the leaderboard (includes contours and spacing) */
#define LEADERBOARD_HEIGHT (SCORE_LIST_MAX + 6)
#define LEADERBOARD_WIDTH  (SCORE_NAME_MAX + 20) /**< @brief Width of the leaderboard */

game_loop_callback_return_value state_leaderboard_oninput(void *s, int key) {
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

/** @brief Renders the leaderboard (only if re-rendering is needed) */
game_loop_callback_return_value state_leaderboard_onrender(void *s, int width, int height) {
	state_leaderboard_data *state = state_extract_data(state_leaderboard_data, s);

	if (!state->needs_rerender)
		return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
	state->needs_rerender = 0;

	erase();

	/* Leaderboard position and contours */
	int left = (width - LEADERBOARD_WIDTH) / 2, top = (height - LEADERBOARD_HEIGHT) / 2;
	menu_draw_box(left, top, LEADERBOARD_WIDTH, LEADERBOARD_HEIGHT);

	/* Draw leaderboard text */
	const char *menu_name = "Leaderboard";
	int len = strlen(menu_name);
	move(top + 2, left + (LEADERBOARD_WIDTH - len) / 2); /* Centered with vertical spacing */
	printw("%s", menu_name);

	/* Draw scores (ignore unfilled scores) */
	for (int i = 0; i < SCORE_LIST_MAX && state->scores.scores[i].score != 0; ++i) {
		/* Player name (aligned left) */
		move(top + i + 4, left + 2);
		printw("%s", state->scores.scores[i].name);

		/* Player score (aligned right) */
		char score_str[32];
		len = sprintf(score_str, "%d", state->scores.scores[i].score);
		move(top + i + 4, left + LEADERBOARD_WIDTH - 2 - len);
		printw("%s", score_str);
	}

	/* Draw user guidance message */
	const char *esc_message = "Press ESC to go back";
	len = strlen(esc_message);
	move(height - 2, (width - len) / 2);
	printw("%s", esc_message);

	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief Called when the terminal window is resized to request for a leaderboard redraw */
game_loop_callback_return_value state_leaderboard_onresize(void *s, int width, int height) {
	(void) width; (void) height;

	state_leaderboard_data *state = state_extract_data(state_leaderboard_data, s);
	state->needs_rerender = 1;

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_state state_leaderboard_create(void) {

	state_leaderboard_data data = {
		.needs_rerender = 1,
	};
	score_list_load(&data.scores);

	state_leaderboard_data *data_ptr = malloc(sizeof(state_leaderboard_data));
	*data_ptr = data;

	game_loop_callbacks callbacks = {
		.oninput  = state_leaderboard_oninput,
		.onupdate = NULL,
		.onrender = state_leaderboard_onrender,
		.onresize = state_leaderboard_onresize,
	};

	game_state ret = {
		.data = data_ptr,
		.destroy = state_leaderboard_destroy,
		.callbacks = callbacks
	};

	return ret;
}

void state_leaderboard_destroy(game_state* state) {
	free(state->data);
}

