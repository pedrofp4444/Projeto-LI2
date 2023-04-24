/**
 * @file  main_game.c
 * @brief The implementation for main game state (where you actually play the game)
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
#include <game_states/main_game_renderer.h>
#include <game_states/player_path.h>

#include <entities/rat.h>

#include <time.h>
#include <stdlib.h>
#include <ncurses.h>

#define CIRCLE_CENTER_X 10
#define CIRCLE_CENTER_Y 10
#define CIRCLE_RADIUS 6
#define MOVES_MAX 100

/* @brief **DEGUB** function for drawing a circle of light on the map */
void state_main_game_circle_light_map(map m, unsigned x, unsigned y, unsigned r) {
	unsigned rsquared = r * r;
	for (unsigned yp = y - r; yp <= y + r; ++yp) {
		unsigned disty = (yp - y) * (yp - y);

		for (unsigned xp = x - r; xp <= x + r; ++xp) {
			if (xp < m.width && yp < m.height) {
				unsigned dist = (xp - x) * (xp - x) + disty;
				if (dist <= rsquared)
					m.data[yp * m.width + xp].light = dist < rsquared / 2 ? 2 : 1;
			}
		}
	}
}

/* @brief **DEGUB** function for clearing a circle of light on the map */
void state_main_game_circle_clean_light_map(map m, unsigned x, unsigned y, unsigned r) {
	for (unsigned yp = y - r; yp <= y + r; ++yp)
		for (unsigned xp = x - r; xp <= x + r; ++xp)
			m.data[yp * m.width + xp].light = 0;
}

/** @brief Responds to the passage of time in the game to measure FPS */
game_loop_callback_return_value state_main_game_onupdate(void *s, double elapsed) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	state->elapsed_fps += elapsed;
	if (state->elapsed_fps > 1) {
		/*
		 * When a second passes, update the number of frames (and renders) being displayed,
		 * and reset the count for the next second
		 */

		state->fps_show = state->fps_count;
		state->fps_count = 0;

		state->renders_show = state->renders_count;
		state->renders_count = 0;

		state->elapsed_fps -= 1.0;
		state->needs_rerender = 1; /* Update the number on the screen */
	}

	state->fps_count++;
	state->renders_count += state->needs_rerender;

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief Responds to user input in the main game state */
game_loop_callback_return_value state_main_game_oninput(void *s, int key) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	state_main_game_circle_clean_light_map(state->map,
		CIRCLE_CENTER_X + state->offsetx, CIRCLE_CENTER_Y + state->offsety, CIRCLE_RADIUS);

	switch (key) {
		case '\x1b':
			return GAME_LOOP_CALLBACK_RETURN_BREAK; /* Exit game on escape */

		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT:
			draw_path(state, key);
			break;

		case '\r':
			if (state->move_count > 0) {
				player_move(state);
				state->x = state->entities.entities[0].x;
				state->y = state->entities.entities[0].y;
			}
	}

	state_main_game_circle_light_map(state->map,
		CIRCLE_CENTER_X + state->offsetx, CIRCLE_CENTER_Y + state->offsety, CIRCLE_RADIUS);

	state->needs_rerender = 1;
	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_state state_main_game_create(void) {
	erase(); /* Performant rendering requires a clean screen to start */

	map m = map_allocate(1024, 1024);
	srand(time(NULL));
	for (int i = 0; i < 1024 * 1024; ++i) { /* Fill map with garbage data (temporary) */
		int r = rand() % 100;
		tile_type type;
		if      (r < 20) type = TILE_WALL;
		else if (r < 40) type = TILE_WATER;
		else             type = TILE_EMPTY;

		tile t = {
			.type = type,
			.light = 0
		};
		m.data[i] = t;
	}
	state_main_game_circle_light_map(m, CIRCLE_CENTER_X, CIRCLE_CENTER_Y, CIRCLE_RADIUS);

	/* Populate the map with random invalid entities (temporary) */
	entity_set entities = entity_set_allocate(1024);
	for (int i = 1; i < 1024; ++i) {
		entities.entities[i].health = 1;
		entities.entities[i].type = rand() % 4 + 1;

		entities.entities[i].x = rand() % 1024;
		entities.entities[i].y = rand() % 1024;
	}

	entities.entities[0].health = 1;
	entities.entities[0].type = ENTITY_PLAYER;
	entities.entities[0].x = 9;
	entities.entities[0].y = 10;

	state_main_game_data data = {
		.offsetx = 0, .offsety = 0,

		.fps_show     = 0, .fps_count     = 0,
		.renders_show = 0, .renders_count = 0,
		.elapsed_fps = 0.0,

		.needs_rerender = 1,

		.map = m,
		.entities = entities,

		.x = entities.entities[0].x,
		.y = entities.entities[0].y,
		.moves = {0},
		.move_count = 0,
		.history = {TILE_EMPTY},
	};

	state_main_game_data *data_ptr = malloc(sizeof(state_main_game_data));
	*data_ptr = data;

	game_loop_callbacks callbacks = {
		.oninput  = state_main_game_oninput,
		.onupdate = state_main_game_onupdate,
		.onrender = state_main_game_onrender,
		.onresize = state_main_game_onresize
	};

	game_state ret = {
		.data = data_ptr,
		.destroy = state_main_game_destroy,
		.callbacks = callbacks
	};
	return ret;
}

void state_main_game_destroy(game_state* state) {
	state_main_game_data *game_data = state_extract_data(state_main_game_data, state);
	map_free(game_data->map);
	entity_set_free(game_data->entities);
	free(state->data);
}

