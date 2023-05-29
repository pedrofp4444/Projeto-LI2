/**
 * @file  name_input.h
 * @brief Where a player inputs their name before playing
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

#ifndef NAME_INPUT_H
#define NAME_INPUT_H

#include <score.h>

/**
 * @struct state_name_input_data
 * @brief Data for the name input box to work
 *
 * @var state_name_input_data::needs_rerender If the input needs to be drawn on screen
 * @var state_name_input_data::button The current button chosen by the user
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	int needs_rerender;
	char name[SCORE_NAME_MAX + 1];
} state_name_input_data;

/**
 * @brief Creates the name input
 * @author A104348 Humberto Gomes
 */
game_state state_name_input_create(void);

/**
 * @brief Destroys a state for the name input (frees `state->data`)
 * @author A104348 Humberto Gomes
 */
void state_name_input_destroy(game_state *state);

#endif

