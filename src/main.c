/**
 * @file main.c
 * @brief Contains the entry point to the program
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

#include <game_loop.h>
#include <map.h>
#include <entities.h>
#include <entities/rat.h>

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	double fps;
	entity_set entities; 
	map map;
	int offsetx, offsety;
} game_state;

/** @brief Handle user input (move text with arrows, exit program on escape) */
game_loop_callback_return_value oninput(void *s, int key) {
	game_state *state = (game_state *)s;

	switch (key)
	{
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

/** @brief Update the game state (keep count of the FPS) */
game_loop_callback_return_value onupdate(void *s, double elapsed) {
	game_state *state = (game_state *)s;
	state->fps = 1.0 / elapsed;
	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief Render the test game */
game_loop_callback_return_value onrender(void *s, int width, int height) {
	game_state *state = (game_state *)s;

	(void)width;
	(void)height;

	erase();

	map_render(state->map,
			   state->offsety, state->offsetx, 5, 2, 20, 20);

	entity_set_render(state->entities,
			   state->offsety, state->offsetx, 5, 2, 20, 20);

	/* Print the FPS on the top right corner */
	move(0, 0);
	printw("FPS: %d", (int)state->fps);

	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief The entry point for the game */
int main(void) {
	int err = game_loop_init_ncurses();
	if (err) {
		/* Don't handle errors. Just try to return to a canonical terminal mode */
		game_loop_terminate_ncurses();
		puts("Could not initialize ncurses!");
		return 1;
	}

	game_loop_callbacks callbacks = {
		.oninput = oninput,
		.onupdate = onupdate,
		.onrender = onrender,
		.onresize = NULL};

	game_state state = {.fps = 0.0, .map = map_allocate(1024, 1024)};

	entity_set entities = malloc(sizeof(struct entity_set));
	entities->next = NULL;
	entities->ent = entity_create_rat(5, 2, 100);
	state.entities = entities;

	if (state.map.data == NULL) {
		/* Don't handle errors. Just try to return to a canonical terminal mode */
		game_loop_terminate_ncurses();
		puts("Could not initialize ncurses!");
		return 1;
	}

	int arrcount = state.map.width * state.map.height;
	memset(state.map.data, 0, arrcount * sizeof(tile));
	tile wall = {.type = TILE_WALL};
	tile empty = {.type = TILE_EMPTY};
	for (int i = 0; i < arrcount; ++i)
		state.map.data[i] = ((rand() % 2) == 0) ? wall : empty;

	err = game_loop_run(&state, callbacks, 60);
	if (err) {
		/* Don't handle errors. Just try to return to a canonical terminal mode */
		game_loop_terminate_ncurses();
		puts("An error occurred in the game");
		return 1;
	}

	err = game_loop_terminate_ncurses();
	return err;
}