/**
 * @file game_state.h
 * @brief Abstraction for game state separation
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

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <game_loop.h>

/**
 * @struct game_state
 * @brief A game state composed of persistent game data and game loop update functions.
 * @details May be used to represent a menu, the main game, ...
 *
 * @var game_state::data
 *   Game data passed from update to update.
 * @var game_state::destroy
 *   Function called after switching game states. Acts like a destructor in OO languages. It must
 *   free all dynamically allocated memory in game_state::data. If set to `NULL`, this method
 *   won't be called.
 * @var game_state::callbacks
 *   See ::game_loop_callbacks.
 */
typedef struct game_state {
	void *data;
	void (*destroy)(const struct game_state*);
	game_loop_callbacks   callbacks;
} game_state;


/** @brief Extracts a `type *` from a `void *` where a ::game_state is located.
 *  @details Useful for getting game state data from inside a game loop callback
 *           (::game_loop_callbacks).
 */
#define state_extract_data(type, state) ((type *) ((game_state *) (state))->data)

/**
 * @brief Destroys the @p current game state and sets it to the @p new game state.
 */
void state_switch(game_state *current, const game_state *new);

/**
 * @brief Game state abstraction of ::game_loop_run
 * @details Functions like ::game_loop_run, but the state passed from callback to callback is a
 *          ::game_state. That way, transition between menus and game is made easier, as the
 *          callback functions can be simply switched.
 *
 *          Note that the @p state **will not be destroyed**.
 */
int state_game_loop_run(game_state *state, unsigned int fps);

#endif

