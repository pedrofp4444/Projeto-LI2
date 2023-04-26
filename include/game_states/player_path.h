/**
 * @file player_path.h
 * @brief Draw and update the player movement path
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
 */
void state_main_game_move_player(state_main_game_data *state, int key);

/**
 * @brief   Draws the path (::animation_sequence) of the player on the screen
 * @details Not the full sequence is drawn, only the steps after
 *          ::state_main_game_data::animation_step. That allows for partial path rendering when
 *          the player (and other entities) are being animated.
 *
 * @param state The game state
 * @param map_top The top coordinate of the map to be rendered
 * @param map_left The left coordinate of the map to be rendered
 * @param term_top The top coordinate of the terminal where the map will be rendered
 * @param term_left The left coordinate of the terminal where the map will be rendered
 * @param height The height of the map and the parts of the terminal to render
 * @param width The width of the map and the parts of the terminal to render
 */
void state_main_game_draw_player_path(state_main_game_data *state,
                                      int map_top , int map_left,
                                      int term_top, int term_left,
                                      int height  , int width);

#endif

