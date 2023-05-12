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

#include <game_states/main_game_renderer.h>
#include <game_state.h>
#include <map.h>
#include <score.h>
#include <entities.h>

/** @brief Type of action during the game */
typedef enum {
	MAIN_GAME_MOVEMENT_INPUT,            /**< Waiting for user input for the next movement */
	MAIN_GAME_ANIMATING_PLAYER_MOVEMENT, /**< Animating player movement */
	MAIN_GAME_COMBAT_INPUT,              /**< Waiting for user input for the next attack */
	MAIN_GAME_ANIMATING_PLAYER_COMBAT,   /**< Animating player's attack */
	MAIN_GAME_ANIMATING_MOBS_MOVEMENT,   /**< Animating movement of mobs */
	MAIN_GAME_ANIMATING_MOBS_COMBAT,    /**< Animating attacks of mobs */
} state_main_game_action;

/**
 * @struct state_main_game_data
 * @brief Data for the main game state
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
 * @var state_main_game_data::must_leave
 *   If the game should be exited of (after a message box prompt)
 *
 * @var state_main_game_data::needs_rerender
 *   If an update happened (e.g.: user input, window resize) requiring the game to be rendered
 * @var state_main_game_data::overlay
 *   Overlay on the top of the map (for drawing combat elements like bombs and arrows).
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
 *
 * @var state_main_game_data::score
 *   Player's score (increases by killing entities)
 * @var state_main_game_data::dropped
 *   A weapon dropped by a mob. Will be ::WEAPON_INVALID if no drop happened.
 * @var state_main_game_data::dropped_food
 *   If the last mob killed dropped food
 *
 * @var state_main_game_data::cursorx
 *   Horizontal position (on the map) of the cursor (to choose mob to attack)
 * @var state_main_game_data::cursory
 *   Vertical position (on the map) of the cursor (to choose mob to attack)
 */
typedef struct {
	int fps_show, fps_count, renders_show, renders_count;
	double elapsed_fps;

	int must_leave;

	int needs_rerender;
	ncurses_char *overlay;

	state_main_game_action action;
	size_t animation_step;
	double time_since_last_animation;

	map map;
	entity_set entities;

	player_score score;
	weapon dropped;
	int dropped_food;

	int cursorx, cursory;
} state_main_game_data;

/** @brief Returns the player (first entity) of a ::state_main_game_data pointer */
#define PLAYER(state) ((state)->entities.entities[0])

/** @brief Creates a state for the main game */
game_state state_main_game_create(char name[SCORE_NAME_MAX + 1]);

/** @brief Destroys a state for the main game (frees `state->data`) */
void state_main_game_destroy(game_state *state);

#endif

