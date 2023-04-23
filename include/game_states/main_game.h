/**
 * @file  main_game.h
 * @brief The main game state (where you actually play the game)
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

#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <game_state.h>
#include <map.h>
#include <entities.h>

/** @brief Type of action during the game */
typedef enum {
	MAIN_GAME_IDLING,    /**< Waiting for user input for the next turn */
	MAIN_GAME_ANIMATING, /**< Animating between-turn events */
} state_main_game_action;

/**
 * @struct state_main_game_data
 * @brief Data for the main game state
 *
 * @var state_main_game_data::offsetx
 *   Horizontal offset of the map
 * @var state_main_game_data::offsety
 *   Vertical offset of the map
 *
 * @var state_main_game_data::fps_show
 *   The FPS number to be displayed
 * @var state_main_game_data::fps_count
 *   The number of frames since the last ::state_main_game_data::fps_show update.
 * @var state_main_game_data::renders_show
 *   The number of frames (in state_main_game_data::fps_show) that required rendering
 * @var state_main_game_data::renders_count
 *   The number of frames (in state_main_game_data::fps_count) that required rendering
 * @var state_main_game_data::elapsed_fps
 *   The time elapsed (in seconds) since the last ::state_main_game_data::fps_show update.
 *
 * @var state_main_game_data::needs_rerender
 *   If an update happened (e.g.: user input, window resize) requiring the game to be rendered
 *
 * @var state_main_game_data::action
 *   What is currently happening in the game (see ::state_main_game_action)
 * @var state_main_game_data::animation_step
 *   The index of the current animation step. See ::entity_set_animate.
 * @var state_main_game_data::time_since_last_animation
 *   The time (in seconds) since the last animation step
 *
 * @var state_main_game_data::map
 *   The game map
 * @var state_main_game_data::entities
 *   Entities in the map
 */
typedef struct {
	int offsetx, offsety;

	int fps_show, fps_count, renders_show, renders_count;
	double elapsed_fps;

	int needs_rerender;

	state_main_game_action action;
	size_t animation_step;
	double time_since_last_animation;

	map map;
	entity_set entities;
} state_main_game_data;

/** @brief Creates a state for the main game */
game_state state_main_game_create(void);

/** @brief Destroys a state for the main game (frees `state->data`) */
void state_main_game_destroy(game_state *state);

#endif
