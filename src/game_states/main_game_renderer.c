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
#include <game_states/player_path.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

#define SIDEBAR_WIDTH 20
#define HEALTHBAR_WIDTH (SIDEBAR_WIDTH - 5)

/**
 * @brief Draws the health of an entity on the side bar
 */
void main_game_render_health(entity ent, int y) {
	/* Draw centered entity name */
	const char *name = entity_get_name(ent.type);

	int len = strlen(name);
	move(y, (SIDEBAR_WIDTH - len) / 2);
	printw("%s", name);

	/* Draw health bar */
	int health_dots = round(HEALTHBAR_WIDTH * ((float) ent.health / (float) ent.max_health));
	move(y + 1, 1);
	addch('[');

	for (int i = 1; i <= HEALTHBAR_WIDTH; ++i) {
		if (i <= health_dots) {
			attron(COLOR_PAIR(COLOR_RED) | A_REVERSE);
		} else {
			attroff(A_REVERSE);
		}
		addch(' ');
	}
	attroff(COLOR_PAIR(COLOR_RED) | A_REVERSE);
	addch(']');
}

/** @brief Renders the sidebar of the main game */
void main_game_render_sidebar(const state_main_game_data *state, int height) {
	/* Draw vertical separation line */
	for (int y = 0; y < height; ++y)
		mvaddch(y, SIDEBAR_WIDTH - 1, '|');

	/* Draw game name */
	const char *game_name = "Roguelite";
	move(0, (SIDEBAR_WIDTH - strlen(game_name)) / 2);
	printw("%s", game_name);

	/* Draw health of surronding enemies */
	int max_health_bars = (height - 5) / 3;
	entity_set health_entities =
		entity_get_closeby(PLAYER(state), state->entities, max_health_bars, &state->map);

	for (size_t i = 0; i < health_entities.count; ++i) {
		main_game_render_health(health_entities.entities[i], 2 + i * 3);
	}

	free(health_entities.entities); /* Don't use entity_set_free not to free entity data */

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
	int map_top  = PLAYER(state).y - (height / 2);
	int map_left = PLAYER(state).x - ((width - SIDEBAR_WIDTH) / 2);

	main_game_render_sidebar(state, height);

	map_render(state->map,
	           map_top, map_left,
	           0, SIDEBAR_WIDTH,
	           height, width - SIDEBAR_WIDTH);

	state_main_game_draw_player_path(state,
	                  map_top, map_left,
	                  0, SIDEBAR_WIDTH,
	                  height, width - SIDEBAR_WIDTH);


	entity_set_render(state->entities, state->map,
	                  map_top, map_left,
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

