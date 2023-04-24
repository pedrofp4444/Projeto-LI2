/**
 * @file player_path.h
 * @brief Draw player path
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
 * @brief Draws the path of the player based on the input move.
 * 
 * @param state A pointer to the main game state data.
 * @param move The input move.
*/
void draw_path(state_main_game_data *state, int move);

/**
 * @brief Updates the player's position on the map according to the stored moves.
 * 
 * @param state A pointer to the main game state data.
*/
void player_move (state_main_game_data *state);

#endif


