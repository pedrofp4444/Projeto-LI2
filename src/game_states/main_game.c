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

#include <combat.h>
#include <game_states/main_game.h>
#include <game_states/main_game_renderer.h>
#include <game_states/main_game_animation.h>
#include <game_states/player_path.h>
#include <game_states/msg_box.h>
#include <game_states/illumination.h>
#include <generate_map.h>
#include <entities_search.h>

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

#define CIRCLE_RADIUS 15

/** @brief Responds to the passage of time in the game to measure FPS and animate the game */
game_loop_callback_return_value state_main_game_onupdate(void *s, double elapsed) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	if (state->must_leave)
		return GAME_LOOP_CALLBACK_RETURN_BREAK;

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

	state_main_game_animate(state, elapsed);

	if (PLAYER(state).health <= 0) {
		/* TODO - game over screen */
		return GAME_LOOP_CALLBACK_RETURN_BREAK;
	}

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief Is called when the exit confimation message box is left */
void state_main_game_msg_box_callback(void *s, int button) {
	if (button == 1) /* OK button */
		state_extract_data(state_main_game_data, s)->must_leave = 1;
}

/** @brief Uses a message box to ask the user if they want to leave the game */
void state_main_game_exit_confirmation(game_state *state) {
	const char *buttons[2] = { "Cancel", "OK" };
	game_state msg = state_msg_box_create(*state, state_main_game_msg_box_callback,
	                                      "Leave the game?", buttons, 2, 0);
	state_switch(state, &msg, 0);
}

/** @brief Responds to user input in the main game state */
game_loop_callback_return_value state_main_game_oninput(void *s, int key) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	switch (key) {
		case '\x1b': /* Escape */
			if (state->action == MAIN_GAME_MOVEMENT_INPUT &&
			    PLAYER(state).animation.length > 0) {

				/* Reset player movement */
				PLAYER(state).animation.length = 0;
			}
			else if (state->action == MAIN_GAME_COMBAT_INPUT &&
			         !(state->cursorx == PLAYER(state).x &&
			           state->cursory == PLAYER(state).y)) {

				/* Reset cursor position */
				state->cursorx = PLAYER(state).x;
				state->cursory = PLAYER(state).y;
			} else {
				/* Ask to leave game */
				state_main_game_exit_confirmation((game_state *) s);
			}
			break;

		case '\r':
			if (state->action == MAIN_GAME_MOVEMENT_INPUT) {
				state->action = MAIN_GAME_ANIMATING_PLAYER_MOVEMENT;
			} else if (state->action == MAIN_GAME_COMBAT_INPUT) {
				state_main_game_attack_cursor(state, (game_state *) s);

				/* DEBUG purpose - move all entities to the right */
				for (size_t i = 1; i < state->entities.count; ++i) {
					if (state->entities.entities[i].health <= 0) continue;
					animation_step step = {
						.x = state->entities.entities[i].x + 1,
						.y = state->entities.entities[i].y,
					};
					state->entities.entities[i].animation.length = 1;
					state->entities.entities[i].animation.steps[0] = step;
				}
			}
			break;

		case 's': case 'S': /* Skip player combat */
			if (state->action == MAIN_GAME_COMBAT_INPUT)
				state->action = MAIN_GAME_ANIMATING_MOBS_MOVEMENT;
			break;

		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT:
			if (state->action == MAIN_GAME_MOVEMENT_INPUT) {
				state_main_game_move_player(state, key);
			} else if (state->action == MAIN_GAME_COMBAT_INPUT) {
				state_main_game_move_cursor(state, key);
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

	state_main_game_data data = {
		.fps_show     = 0, .fps_count     = 0,
		.renders_show = 0, .renders_count = 0,
		.elapsed_fps = 0.0,

		.must_leave = 0,

		.needs_rerender = 1,
		.overlay = NULL,

		.action = MAIN_GAME_MOVEMENT_INPUT,
		.animation_step = 0,
		.time_since_last_animation = 0,
	};

	generate_map_random(&data);

	data.cursorx = data.map.width  / 2;
	data.cursory = data.map.height / 2;

	state_main_game_circle_light_map(
		data.map, data.entities.entities[0].x, data.entities.entities[0].y, CIRCLE_RADIUS);

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
