/**
 * @file game_loop.h
 * @brief Ncurses game loop abstraction with callback functions
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

#ifndef GAME_LOOP_H
#define GAME_LOOP_H

/**
 * @brief The return value of game loop callback functions. Indicates whether or to continue the
 *        game loop.
 */
typedef enum {
	GAME_LOOP_CALLBACK_RETURN_SUCCESS, /**< Continue the game loop */
	GAME_LOOP_CALLBACK_RETURN_BREAK,   /**< Exit the game loop successfully */
	GAME_LOOP_CALLBACK_RETURN_ERROR,   /**< Exit the game loop due to an error */
} game_loop_callback_return_value;

/**
 * @brief Callback function for when the game needs to be updated.
 *
 * NOTE: `elapsed` time is in seconds
 */
typedef game_loop_callback_return_value(*game_loop_update_callback)(void *state, double elapsed);

/** @brief Callback function for when the game needs to be rendered */
typedef game_loop_callback_return_value(*game_loop_render_callback)
	(void *state, int width, int height);

/**
 * @brief Callback function for when the terminal window is resized
 *
 * Note that this will also be called on window initialization (when size is first set).
 */
typedef game_loop_callback_return_value(*game_loop_resize_callback)
	(void *state, int width, int height);

/**
 * @brief Set of functions that are called on game loop envents
 *
 * A function can be set to `NULL` and it won't be called.
 */
typedef struct {
	game_loop_update_callback onupdate;
	game_loop_render_callback onrender;
	game_loop_resize_callback onresize;
} game_loop_callbacks;

/**
 * @brief Intialize ncurses
 * @returns 0 on success, 1 on failure
 */
int game_loop_init_ncurses(void);

/**
 * @brief Run the game loop with a set of callback function
 * @returns 0 on success, 1 on failure (includes callback errors)
 *
 * @param state     The game state passed to callbacks
 * @param callbacks The set of callback functions
 */
int game_loop_run(void *state, game_loop_callbacks callbacks);

/**
 * @brief Terminate ncurses usage
 * @returns 0 on success, 1 on failure
 */
int game_loop_terminate_ncurses(void);

#endif

