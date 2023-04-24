/**
 * @file player_path.c
 * @brief Draw player path
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
#include <game_states/main_game.h>

#include <ncurses.h>
/**
 * @brief Returns the opposite key given a key.
 * @param key Key.
 * @return The opposite key.
*/
int opposite_key (int key) {
	if (key == KEY_UP   ) return KEY_DOWN;
	if (key == KEY_DOWN ) return KEY_UP;
	if (key == KEY_RIGHT) return KEY_LEFT;
	if (key == KEY_LEFT ) return KEY_RIGHT;
	return 0;
}

/**
 * @brief Verifies if a move is valid.
 * @param state A pointer to the main game state data.
 * @param x The x coordinate of the move.
 * @param y The y coordinate of the move.
 * @return 1 if the move is valid, 0 otherwise.
*/
int verify_move (state_main_game_data *state, int x, int y) {
	return (x >= 0 && y >= 0 &&
	        (unsigned)x < state->map.height && (unsigned)y < state->map.width &&
	        state->map.data[y * state->map.width + x].type != TILE_WALL);
}

/**
 * @brief Gets the change in x and y coordinates given a key.
 * @param key The key.
 * @param dx A pointer to the variable to store the change in x coordinate.
 * @param dy A pointer to the variable to store the change in y coordinate.
*/
void get_dx_dy(int key, int *dx, int *dy) {

	if (key == KEY_UP   ) *dy = -1;
	if (key == KEY_DOWN ) *dy = 1;
	if (key == KEY_LEFT ) *dx = -1;
	if (key == KEY_RIGHT) *dx = 1;

}

void draw_path(state_main_game_data *state, int move) {

	/* Checks if the move is valid */
	switch (move) {
		case KEY_UP:
			if (!verify_move(state, state->x, state->y-1)) return;
			(state->y)--;
			break;

		case KEY_DOWN:
			if (!verify_move(state, state->x, state->y+1)) return;
			(state->y)++;
			break;

		case KEY_LEFT:
			if (!verify_move(state, state->x-1, state->y)) return;
			(state->x)--;
			break;

		case KEY_RIGHT:
			if (!verify_move(state, state->x+1, state->y)) return;
			(state->x)++;
			break;

		default:
			return;
	}

	int last_move = state->move_count - 1;
	if (last_move >= 0 && state->moves[last_move] == opposite_key(move)) {
		/* Removes the point and restores the previous TILE */
		int dx_l = 0, dy_l = 0;
		int dx_m = 0, dy_m = 0;

		get_dx_dy(last_move, &dx_l, &dy_l);
		get_dx_dy(move, &dx_m, &dy_m);

		state->x -= (dx_l + dx_m);
		state->y -= (dy_l + dy_m);

		tile_type previous_tile = state->history[last_move];
		state->map.data[state->y * state->map.width + state->x].type = previous_tile;

		state->history[last_move] = previous_tile;
		state->moves[last_move] = 0;
		state->move_count--;
	}

	else {
		/* Saves the current TILE */
		tile_type current_tile = state->map.data[state->y * state->map.width + state->x].type;

		state->map.data[state->y * state->map.width + state->x].type = TILE_DOT;
		state->moves[state->move_count] = move;
		state->move_count++;

		/* Saves current TILE in history */
		state->history[state->move_count - 1] = current_tile;
	}
}

void player_move (state_main_game_data *state) {
	int new_x = state->entities.entities[0].x, new_y = state->entities.entities[0].y;
	for (int i = 0; i < state->move_count; i++) {
		if (state->moves[i] == KEY_UP) new_y--;
		if (state->moves[i] == KEY_DOWN) new_y++;
		if (state->moves[i] == KEY_RIGHT) new_x++;
		if (state->moves[i] == KEY_LEFT) new_x--;

		state->entities.entities[0].x = new_x;
		state->entities.entities[0].y = new_y;

		state->map.data[new_y * state->map.width + new_x].type = state->history[i];
	}
	state->move_count = 0;
}

