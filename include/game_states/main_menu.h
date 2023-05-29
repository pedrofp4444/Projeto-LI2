/**
 * @file  main_menu.h
 * @brief The main menu of the game
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

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

/**
 * @struct state_main_menu_data
 * @brief Data for the main menu to work
 *
 * @var state_main_menu_data::needs_rerender If the main menu needs to be drawn on screen
 * @var state_main_menu_data::button The current button chosen by the user
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	int needs_rerender;
	int button;
} state_main_menu_data;

/**
 * @brief Creates the main menu
 * @author A104348 Humberto Gomes
 */
game_state state_main_menu_create(void);

/**
 * @brief Destroys a state for the main menu (frees `state->data`)
 * @author A104348 Humberto Gomes
 */
void state_main_menu_destroy(game_state *state);

#endif
