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

#include <entities_search.h>

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

#define CIRCLE_RADIUS 15
#define MAIN_GAME_ANIMATION_TIME 0.3

/* @brief **DEGUB** function for drawing a circle of light on the map */
void state_main_game_circle_light_map(map m, int x, int y, int r) {
	int rsquared = r * r;
	for (int yp = y - r; yp <= y + r; ++yp) {
		int disty = (yp - y) * (yp - y);

		for (int xp = x - r; xp <= x + r; ++xp) {
			if (0 <= xp && xp < (int) m.width && 0 <= yp && yp < (int) m.height) {
				if ((xp - x) * (xp - x) + disty <= rsquared)
					m.data[yp * m.width + xp].light = 1;
			}
		}
	}
}

/* @brief **DEGUB** function for clearing a circle of light on the map */
void state_main_game_circle_clean_light_map(map m, int x, int y, int r) {
	for (int yp = y - r; yp <= y + r; ++yp)
		for (int xp = x - r; xp <= x + r; ++xp)
			if (0 <= xp && xp < (int) m.width && 0 <= yp && yp < (int) m.height)
				m.data[yp * m.width + xp].light = 0;
}

/** @brief Responds to the passage of time in the game to measure FPS and animate the game */
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

	/* Animate entities */
	if (state->action == MAIN_GAME_ANIMATING) {
		/* Animation timing */
		if (state->time_since_last_animation >= MAIN_GAME_ANIMATION_TIME) {
			state->time_since_last_animation = 0;

			state_main_game_circle_clean_light_map(
				state->map, PLAYER(state).x, PLAYER(state).y, CIRCLE_RADIUS);

			/* Actual entity moving */
			if (entity_set_animate(state->entities, state->animation_step)) {
				/* Done animating */
				state->action = MAIN_GAME_IDLING;
				state->animation_step = 0;

				/* Clear all entities' animations */
				for (size_t i = 0; i < state->entities.count; ++i)
					state->entities.entities[i].animation.length = 0;

			} else {
				/* Some entities have animation steps left */
				state->animation_step++;
			}


			state_main_game_circle_light_map(
				state->map, PLAYER(state).x, PLAYER(state).y, CIRCLE_RADIUS);

		} else {
			state->time_since_last_animation += elapsed;
		}

		state->needs_rerender = 1;
	}

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief **DEBUG** function for testing A*. Move closest entity to the player to its position */
void state_main_game_move_entities(state_main_game_data *state) {
	if (state->action == MAIN_GAME_IDLING) {
		state->action = MAIN_GAME_ANIMATING;

		/* Find the closest entity to the player */
		int closest_index = -1;
		float min_dist = INFINITY;
		for (size_t i = 1; i < state->entities.count; ++i) {
			entity current = state->entities.entities[i];
			if (current.health <= 0) continue;

			/* x^2 + y^2. Avoid expensive square root */
			float dist = (PLAYER(state).x - current.x) * (PLAYER(state).x - current.x) +
			             (PLAYER(state).y - current.y) * (PLAYER(state).y - current.y);

			if (dist < min_dist) {
				closest_index = i;
				min_dist = dist;
			}
		}
		entity *closest = &state->entities.entities[closest_index];

		/* Use A* to find the player (old position) */
		animation_step start = { .x = closest->x     , .y = closest->y      };
		animation_step end   = { .x = PLAYER(state).x, .y = PLAYER(state).y };

		animation_sequence_free(closest->animation);
		closest->animation = search_path(&state->map, start, end);
	}
}

/** @brief Responds to user input in the main game state */
game_loop_callback_return_value state_main_game_oninput(void *s, int key) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	switch (key) {
		case '\x1b':
			return GAME_LOOP_CALLBACK_RETURN_BREAK; /* Exit game on escape */

		case '\r': /* On ENTER, animate all entities moving three blocks left - temporary */
			if (state->action == MAIN_GAME_IDLING) {
				state_main_game_move_entities(state);
			}
			break;

		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT:
			if (state->action == MAIN_GAME_IDLING) {
				state_main_game_move_player(state, key);
			}
			break;

		default:
			break;
	}

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

	/* Populate the map with random invalid entities (temporary) */
	entity_set entities = entity_set_allocate(70000);
	for (int i = 1; i < 70000; ++i) {
		entities.entities[i].animation = animation_sequence_create();

		int max = (rand() % 15) + 1;
		entities.entities[i].max_health = max;
		entities.entities[i].health = (rand() % max) + 1;
		entities.entities[i].type = rand() % 4 + 1;

		entities.entities[i].weapon = rand() % 6;

		entities.entities[i].destroy = NULL;

		entities.entities[i].x = rand() % 1024;
		entities.entities[i].y = rand() % 1024;
	}

	/* Player entity (temporary) */
	entities.entities[0].health = 1;
	entities.entities[0].max_health = 2;
	entities.entities[0].weapon = WEAPON_DAGGER;
	entities.entities[0].animation = animation_sequence_create();
	entities.entities[0].type = ENTITY_PLAYER;
	entities.entities[0].destroy = NULL;
	entities.entities[0].x = 512;
	entities.entities[0].y = 512;

	state_main_game_circle_light_map(
		m, entities.entities[0].x, entities.entities[0].y, CIRCLE_RADIUS);

	state_main_game_data data = {
		.fps_show     = 0, .fps_count     = 0,
		.renders_show = 0, .renders_count = 0,
		.elapsed_fps = 0.0,

		.needs_rerender = 1,
		.overlay = NULL,

		.action = MAIN_GAME_IDLING,
		.animation_step = 0,
		.time_since_last_animation = 0,

		.map = m,
		.entities = entities,
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
	if (game_data->overlay)
		free(game_data->overlay);
	free(state->data);
}

