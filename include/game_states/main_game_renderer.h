/**
 * @file main_game_renderer.h
 * @brief Renderization of screen elements during the game
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

#ifndef MAIN_GAME_RENDERER_H
#define MAIN_GAME_RENDERER_H

#include <core.h>
#include <game_loop.h>
#include <game_states/main_game.h>

/**
 * @brief   Renders the game on the screen, with an adjustable layout.
 * @details Rendering is only done when it needs to be done (after window resizes, user input,
 *          etc.)
 *
 * @param state
 *   Game state (see ::game_state)
 * @param width
 *   Width of the terminal window
 * @param height
 *   Height of the terminal window
 */
game_loop_callback_return_value state_main_game_onrender(void *state, int width, int height);

/**
 * @brief   Responds to changes of the terminal window size.
 * @details Sets ::state_main_game_data::needs_rerender to 1.
 *
 * @param state
 *   Game state (see ::game_state)
 * @param width
 *   Width of the terminal window
 * @param height
 *   Height of the terminal window
 */
game_loop_callback_return_value state_main_game_onresize(void *state, int width, int height);

#endif

