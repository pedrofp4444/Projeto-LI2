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
#include <game_states/player_action.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

#define SIDEBAR_WIDTH 20 /**< @brief Width of the sidebar (includes vertical separation line)*/

/** @brief Width of an health bar excluding brackets */
#define HEALTHBAR_WIDTH  (SIDEBAR_WIDTH - 5)
#define HEALTHBAR_HEIGHT 3 /**< @brief Height of a health bar (includes spacing between bars) */

/**
 * @brief The number of lines on the sidebar before the health bars
 * @details Currently seven:
 *
 * 1. Game name
 * 2. Empty line for spacing
 * 3. Score: Number
 * 4. Empty line for spacing
 * 5. Weapon
 * 6. Name of the player's weapon
 * 7. Space between top lines and health bars
 */
#define SIDEBAR_TOP_LINES 7

/**
 * @brief The number of lines on the sidebar after the health bars
 * @details Currently three:
 *
 * 1. Space between bottom lines and health bars
 * 2. FPS
 * 3. Number of renders
 */
#define SIDEBAR_BOTTOM_LINES 3

/**
 * @brief The number of lines on the sidebar occupied by data other than health bars
 */
#define SIDEBAR_TOP_BOTTOM_LINES (SIDEBAR_TOP_LINES + SIDEBAR_BOTTOM_LINES)

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
	/* Example: [███     ] */
	int health_dots = round(HEALTHBAR_WIDTH * ((float) ent.health / (float) ent.max_health));
	move(y + 1, 1);
	addch('[');

	for (int i = 1; i <= HEALTHBAR_WIDTH; ++i) {
		if (i <= health_dots) {
			attron(COLOR_PAIR(COLOR_RED) | A_REVERSE); /* Red background (health) */
		} else {
			attroff(A_REVERSE); /* Empty (lost health points) */
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

	/* Draw game name (centered) */
	const char *game_name = "Roguelite";
	move(0, (SIDEBAR_WIDTH - strlen(game_name)) / 2);
	attron(A_BOLD); printw("%s", game_name); attroff(A_BOLD);

	/* Draw score */
	char score[SIDEBAR_WIDTH + 1];
	int len = sprintf(score, "Score: %d", state->score.score);
	move(2, (SIDEBAR_WIDTH - len) / 2);
	printw("%s", score);

	/* Draw player weapon (centered)
	 *
	 * 1.    Weapon
	 * 2.  Weapon name
	 */

	/* 1. Weapon word centered */
	const char *equiped = "Weapon";
	move(4, (SIDEBAR_WIDTH - strlen(equiped)) / 2);
	attron(A_BOLD); printw("%s", equiped); attroff(A_BOLD);

	/* 2. Weapon name centered */
	equiped = weapon_get_name(PLAYER(state).weapon);
	move(5, (SIDEBAR_WIDTH - strlen(equiped)) / 2);
	printw("%s", equiped);



	/* Draw health of surronding enemies */
	int max_health_bars = (height - SIDEBAR_TOP_BOTTOM_LINES) / HEALTHBAR_HEIGHT;
	entity_set health_entities =
		entity_get_closeby(PLAYER(state), state->entities, max_health_bars, &state->map);

	for (size_t i = 0; i < health_entities.count; ++i) {
		main_game_render_health(health_entities.entities[i],
			SIDEBAR_TOP_LINES + i * HEALTHBAR_HEIGHT);
	}

	free(health_entities.entities); /* Don't use entity_set_free not to free entity data */

	/* Draw FPS and number of renders */
	char txt[SIDEBAR_WIDTH + 1];
	len = sprintf(txt, "FPS: %d", state->fps_show);
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
				move(y, SIDEBAR_WIDTH + x);
				ncurses_char_print(*overlay);
			}

			overlay++;
		}
	}
}

/** @brief Draws tips for the player on how to play the game */
void state_main_game_draw_tips(state_main_game_action act, const map_window *wnd) {

	/* Choose tip message */
	const char *message[2];
	switch (act) {
		case MAIN_GAME_MOVEMENT_INPUT:
			message[0] = "Use the arrow keys to move. Press ENTER to confirm.";
			message[1] = "Press S to skip movement";
			break;
		case MAIN_GAME_COMBAT_INPUT:
			message[0] = "Use the arrow keys to choose a mob. Press ENTER to confirm";
			message[1] = "Press S to skip combat";
			break;
		case MAIN_GAME_ANIMATING_MOBS_MOVEMENT:
		case MAIN_GAME_ANIMATING_MOBS_COMBAT:
			message[0] = "";
			message[1] = "Now it's the turn for other mobs to move";
			break;
		default:
			message[0] = "";
			message[1] = "";
	}

	/* Print message on the bottom center */
	for (int i = 0; i < 2; ++i) {
		int len = strlen(message[i]);
		move(wnd->height - 3 + i, wnd->term_left + (wnd->width - len) / 2);
		printw("%s", message[i]);
	}
}

game_loop_callback_return_value state_main_game_onrender(void *s, int width, int height) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	if (!state->needs_rerender) return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
	state->needs_rerender = 0;
	erase();

	if (width < 80 || height < 24) {

		/* Terminal too small: print invalid layout in the middle */
		const char * const msg = "Invalid terminal size";
		int len = strlen(msg);
		move(height / 2, (width - len) / 2);
		printw("%s", msg);

		return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
	}

	/* Render game normally (valid screen) */

	map_window wnd = { /* Region of the screen for the map (exclude sidebar) */
		.map_top  = PLAYER(state).y - (height / 2),
		.map_left = PLAYER(state).x - ((width - SIDEBAR_WIDTH) / 2),
		.term_top = 0, .term_left = SIDEBAR_WIDTH,
		.height = height, .width = width - SIDEBAR_WIDTH
	};

	main_game_render_sidebar(state, height);

	map_render(state->map, &wnd);

	state_main_game_draw_player_path(state, &wnd);

	entity_set_render(state->entities, state->map, &wnd);

	/* Draw combat overlay, after cleaning it and drawing it */
	if (state->action == MAIN_GAME_ANIMATING_PLAYER_COMBAT ||
	    state->action == MAIN_GAME_ANIMATING_MOBS_COMBAT) {

		if (!state->overlay) /* Allocate overlay if not allocated (after game overs) */
			state->overlay = malloc(wnd.width * wnd.height * sizeof(ncurses_char));

		memset(state->overlay, 0, wnd.width * wnd.height * sizeof(ncurses_char));
		combat_entity_set_animate(state->entities, state->animation_step, state->overlay,
	                                &wnd);

		main_game_render_overlay(state->overlay, width, height);
	}

	state_main_game_draw_tips(state->action, &wnd);

	if (state->action == MAIN_GAME_COMBAT_INPUT)
		state_main_game_draw_cursor(state, &wnd);

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

