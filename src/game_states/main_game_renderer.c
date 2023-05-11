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

#include <combat.h>
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
	/* Draw centered entity name and weapon */
	char name[128];
	int len = sprintf(name, "%s (%s)", entity_get_name(ent.type), weapon_get_name(ent.weapon));
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
	attron(A_BOLD); printw("%s", game_name); attroff(A_BOLD);

	/* Draw player weapon */
	const char *equiped = "Weapon";
	move(2, (SIDEBAR_WIDTH - strlen(equiped)) / 2);
	attron(A_BOLD); printw("%s", equiped); attroff(A_BOLD);

	equiped = weapon_get_name(PLAYER(state).weapon);
	move(3, (SIDEBAR_WIDTH - strlen(equiped)) / 2);
	printw("%s", equiped);

	/* Draw health of surronding enemies */
	int max_health_bars = (height - 8) / 3;
	entity_set health_entities =
		entity_get_closeby(PLAYER(state), state->entities, max_health_bars, &state->map);

	for (size_t i = 0; i < health_entities.count; ++i) {
		main_game_render_health(health_entities.entities[i], 5 + i * 3);
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

/** @brief Renders the overlay on top of the map */
void main_game_render_overlay(ncurses_char *overlay, int width, int height) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width - SIDEBAR_WIDTH; ++x) {
			if (overlay->chr) { /* Don't draw '\0' */
				attron(overlay->attr);
				mvaddch(y, SIDEBAR_WIDTH + x, overlay->chr);
				attroff(overlay->attr);
			}

			overlay++;
		}
	}
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


	/* Draw combat overlay, after cleaning it and drawing it */
	if (state->action == MAIN_GAME_ANIMATING_PLAYER_COMBAT ||
	    state->action == MAIN_GAME_ANIMATING_PLAYER_COMBAT) {

		if (!state->overlay) /* Allocate overlay if not allocated (after game overs) */
			state->overlay = malloc((width - SIDEBAR_WIDTH) * height * sizeof(ncurses_char));

		memset(state->overlay, 0, (width - SIDEBAR_WIDTH) * height * sizeof(ncurses_char));
		combat_entity_set_animate(state->entities, state->animation_step, state->overlay,
	                                map_top, map_left,
	                                height, width - SIDEBAR_WIDTH);

		main_game_render_overlay(state->overlay, width, height);
	}

	if (state->action == MAIN_GAME_COMBAT_INPUT)
		state_main_game_draw_cursor(state,
		                            map_top, map_left,
		                            0, SIDEBAR_WIDTH,
		                            height, width - SIDEBAR_WIDTH);


	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_loop_callback_return_value state_main_game_onresize(void *s, int width, int height) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);
	state->needs_rerender = 1;

	/* Reallocate the overlay (if it has been allocated already) */
	const size_t bytes = (width - SIDEBAR_WIDTH) * height * sizeof(ncurses_char);
	if (state->overlay)
		state->overlay = realloc(state->overlay, bytes);
	else
		state->overlay = malloc(bytes);

	memset(state->overlay, 0, bytes);

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

