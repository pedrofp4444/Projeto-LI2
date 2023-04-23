/**
 * @file main_game_renderer.c
 * @brief Renderization of screen elements during the game
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

#include <game_states/main_game_renderer.h>
#include <string.h>
#include <ncurses.h>

#define SIDEBAR_WIDTH 20

/** @brief Renders the sidebar of the main game */
void main_game_render_sidebar(const state_main_game_data *state, int width, int height) {
	/* Draw vertical separation line */
	for (int y = 0; y < height; ++y)
		mvaddch(y, width - 1, '|');

	/* Draw FPS and number of renders */
	char txt[SIDEBAR_WIDTH];
	int len = sprintf(txt, "FPS: %d", state->fps_show);
	move(height - 2, (SIDEBAR_WIDTH - len) / 2);
	printw("%s", txt);

	len = sprintf(txt, "Renders: %d", state->renders_show);
	move(height - 1, (SIDEBAR_WIDTH - len) / 2);
	printw("%s", txt);
}

game_loop_callback_return_value state_main_game_onrender(void *s, int width, int height) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	if (!state->needs_rerender) return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
	state->needs_rerender = 0;
	erase();

	/* TODO - better define conditions for an invalid screen size */
	if (width < 40 || height < 15) {

		/* Print invalid layout in the middle */
		const char * const msg = "Invalid terminal size";
		int len = strlen(msg);
		move(height / 2, (width - len) / 2);
		printw("%s", msg);

		return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
	}

	/* Render game normally (valid screen) */
	main_game_render_sidebar(state, SIDEBAR_WIDTH, height);
	map_render(state->map,
	           state->offsety, state->offsetx,
	           0, SIDEBAR_WIDTH,
	           height, width - SIDEBAR_WIDTH);
	entity_set_render(state->entities, state->map,
	                  state->offsety, state->offsetx,
	                  0, SIDEBAR_WIDTH,
	                  height, width - SIDEBAR_WIDTH);


	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_loop_callback_return_value state_main_game_onresize(void *s, int width, int height) {
	(void) width; (void) height;

	state_main_game_data *state = state_extract_data(state_main_game_data, s);
	state->needs_rerender = 1;

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

