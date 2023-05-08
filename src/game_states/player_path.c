/**
 * @file player_path.c
 * @brief Draw and update the player movement path
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

#include <map.h>
#include <combat.h>
#include <game_states/main_game.h>
#include <game_states/msg_box.h>

#include <ncurses.h>

/**
 * @brief Verifies if a player position is valid (inside the map, not a wall, ...)
 * @param state A pointer to the main game state data.
 * @param x The x coordinate of the player.
 * @param y The y coordinate of the player.
 * @return 1 if the position is valid, 0 otherwise.
*/
int state_main_game_verify_player_position(state_main_game_data *state, int x, int y) {
	return (x >= 0 && y >= 0 &&
	        (unsigned)x < state->map.height && (unsigned)y < state->map.width &&
	        state->map.data[y * state->map.width + x].type != TILE_WALL &&
	        state->map.data[y * state->map.width + x].light != 0);
}

/**
 * @brief Gets the change of the x and y coordinates for a given pressed key.
 * @param key The ncurses' key.
 * @param dx A pointer to the variable to store the change in x coordinate.
 * @param dy A pointer to the variable to store the change in y coordinate.
*/
void get_dx_dy(int key, int *dx, int *dy) {

	if (key == KEY_UP   ) *dy = -1;
	if (key == KEY_DOWN ) *dy = 1;
	if (key == KEY_LEFT ) *dx = -1;
	if (key == KEY_RIGHT) *dx = 1;

}

void state_main_game_move_player(state_main_game_data *state, int key) {

	int x, y;
	animation_step last_pos;

	/*
	 * Define the current position and the last position. This depends on length of the
	 * animation.
	 */
	if (PLAYER(state).animation.length == 0) {
		last_pos.x = -1; last_pos.y = -1; /* Invalid position for later */
		x = PLAYER(state).x; y = PLAYER(state).y;
	}
	else if (PLAYER(state).animation.length == 1) {
		last_pos.x = PLAYER(state).x; last_pos.y = PLAYER(state).y;

		animation_step current_pos =
			PLAYER(state).animation.steps[PLAYER(state).animation.length - 1];
		x = current_pos.x; y = current_pos.y;

	} else {
		last_pos = PLAYER(state).animation.steps[PLAYER(state).animation.length - 2];

		animation_step current_pos =
			PLAYER(state).animation.steps[PLAYER(state).animation.length - 1];
		x = current_pos.x; y = current_pos.y;
	}

	/* Player movement based on pressed key */
	int dx = 0, dy = 0;
	get_dx_dy(key, &dx, &dy);
	if (dx == 0 && dy == 0) return;
	x += dx; y += dy;

	if (state_main_game_verify_player_position(state, x, y)) { /* Check if the movement is valid */
		animation_step step = { .x = x, .y = y };

		if (last_pos.x == step.x && last_pos.y == step.y) {
			/* Player wants to go back. Revert last movement */
			/* This won't trigger if the last postion is invalid */
			PLAYER(state).animation.length--;
		} else {
			animation_sequence_add_step(&PLAYER(state).animation, step);
		}
	} else {
		beep();
	}
}

void state_main_game_move_cursor(state_main_game_data *state, int key) {
	int dx = 0, dy = 0;
	get_dx_dy(key, &dx, &dy);

	int x = state->cursorx + dx, y = state->cursory + dy;

	/* Don't let cursor get out of bounds or out of the visible area */
	if (x >= 0 && y >= 0 && (unsigned) x < state->map.width && (unsigned) y < state->map.height
	    && state->map.data[y * state->map.width + x].light) {
		state->cursorx = x, state->cursory = y;
	} else {
		beep();
	}
}

void state_main_game_attack_cursor(state_main_game_data *state, game_state *box_state) {

	/* Get entity in the cursor postion */
	entity *target = NULL;
	for (size_t i = 1; i < state->entities.count; ++i) { /* Start at one not to attack player */
		if (state->entities.entities[i].health <= 0) continue;

		if (state->entities.entities[i].x == state->cursorx &&
		    state->entities.entities[i].y == state->cursory) {
			target = &state->entities.entities[i];
			break;
		}
	}

	/* Try to attack entity */
	if (target) {
		if (combat_can_attack(&PLAYER(state), target, &state->map)) {
			combat_attack(&PLAYER(state), target, &state->map);
			state->action = MAIN_GAME_ANIMATING_PLAYER_COMBAT;
		} else {
			const char *button = "OK";
			game_state msg = state_msg_box_create(*box_state, NULL, "Out of range weapon!",
				&button, 1, 0);
			state_switch(box_state, &msg, 0);
		}
	} else {
		const char *button = "OK";
		game_state msg = state_msg_box_create(*box_state, NULL, "No mob here!", &button, 1, 0);
		state_switch(box_state, &msg, 0);
	}
}

void state_main_game_draw_player_path(state_main_game_data *state,
                                      int map_top , int map_left,
                                      int term_top, int term_left,
                                      int height  , int width) {

	attron(COLOR_PAIR(COLOR_WHITE) | A_REVERSE);
	animation_sequence seq = PLAYER(state).animation;

	for (size_t i = (size_t) state->animation_step; i < seq.length; ++i) {
		animation_step step = seq.steps[i];

		if(step.x >= map_left        &&
		   step.x < map_left + width &&
		   step.y >= map_top         &&
		   step.y < map_top + height) { /* Don't draw out-of-screen paths */

			mvaddch(term_top + (step.y - map_top), term_left + (step.x - map_left), ' ');
		}
	}
	attrset(A_NORMAL);
}

void state_main_game_draw_cursor(state_main_game_data *state,
                                 int map_top , int map_left,
                                 int term_top, int term_left,
                                 int height  , int width) {

	int screenx = term_left + (state->cursorx - map_left),
	    screeny = term_top  + (state->cursory - map_top);

	if (screenx >= 0 && screenx < term_left + width &&
	    screeny >= 0 && screeny < term_top + height) {

		move(screeny, screenx);
		attron(COLOR_PAIR(COLOR_BLACK) | A_REVERSE);
		addch(' ');
		attroff(COLOR_PAIR(COLOR_BLACK) | A_REVERSE);
	}
}

