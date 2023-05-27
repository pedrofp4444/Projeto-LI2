/**
 * @file  help.h
 * @brief The help screen accessible by the main menu
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

#ifndef HELP_H
#define HELP_H

/**
 * @struct state_help_data
 * @brief Data for the help screen
 *
 * @var state_help_data::needs_rerender If the help message needs to be drawn to the screen
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	int needs_rerender;
} state_help_data;

/**
 * @brief Creates the help screen
 * @author A104348 Humberto Gomes
 */
game_state state_help_create(void);

/**
 * @brief Destroys the help message state (frees `state->data`)
 * @author A104348 Humberto Gomes
 */
void state_help_destroy(game_state* state);

#endif

