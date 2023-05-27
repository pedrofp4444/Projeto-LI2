/**
 * @file player_action.h
 * @brief Deal with player movement and combat actions
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

#ifndef PLAYER_PATH_H
#define PLAYER_PATH_H

#include <game_states/main_game.h>

/**
 * @brief Responds to an arrow key to move the player (change its animation for the next turn).
 * @param state Game state
 * @param key   Ncurses' pressed key
 *
 * @author A90817 Mariana Rocha
 * @author A104348 Humberto Gomes
 */
void state_main_game_move_player(state_main_game_data *state, int key);

/**
 * @brief Responds to the arrow keys to move the cursor (choose mob to attack)
 * @param state Game state
 * @param key   Ncurses' pressed key
 *
 * @author A104348 Humberto Gomes
 */
void state_main_game_move_cursor(state_main_game_data *state, int key);

/**
 * @brief Tries to attack the entity in the position of the cursor.
 * @details May show message boxes if the attack is impossible
 *
 * @param state     Main game data
 * @param box_state Game state to be restored after eventual message boxes
 *
 * @author A104348 Humberto Gomes
 */
void state_main_game_attack_cursor(state_main_game_data *state, game_state *box_state);

/**
 * @brief   Draws the path (::animation_sequence) of the player on the screen
 * @details Not the full sequence is drawn, only the steps after
 *          ::state_main_game_data::animation_step. That allows for partial path rendering when
 *          the player (and other entities) are being animated.
 *
 * @param state The game state
 * @param wnd The visible map window
 *
 * @author A90817 Mariana Rocha
 * @author A104348 Humberto Gomes
 */
void state_main_game_draw_player_path(state_main_game_data *state, const map_window *wnd);

/**
 * @brief Draws the cursor for choosing entities on the screen
 *
 * @param state The game state
 * @param wnd The visible map window
 *
 * @author A104348 Humberto Gomes
 */
void state_main_game_draw_cursor(state_main_game_data *state, const map_window *wnd);

#endif

