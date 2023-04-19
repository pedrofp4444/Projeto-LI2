/**
 * @file  main_game.h
 * @brief The main game state (where you catually play the game)
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

/**
 * @struct state_main_game_data
 * @brief Data for the main game state
 *
 * @var state_main_game_data::offsetx
 *   Horizontal offset of the '@' character from the center
 * @var state_main_game_data::offsety
 *   Vertical offset of the '@' character from the center
 */
typedef struct {
	int offsetx, offsety;
} state_main_game_data;

/** @brief Creates a state for the main game */
game_state state_main_game_create(void);

/** @brief Destroys a state for the main game (frees `state->data`) */
void state_main_game_destroy(game_state *state);

#endif
