/**
 * @file  main_game_animation.h
 * @brief Animations for the main game state
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

#ifndef MAIN_GAME_ANIMATION_H
#define MAIN_GAME_ANIMATION_H

#include <game_states/main_game.h>

/**
 * @brief Does everything animation related for the main game
 * @details Deals with animation timings, screen updates and entity updates.
 *
 * @param state The game state
 * @param elapsed Elapsed time since the last update
 */
void state_main_game_animate(state_main_game_data *state, double elapsed);

#endif

