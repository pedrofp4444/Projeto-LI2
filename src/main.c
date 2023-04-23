/**
 * @file main.c
 * @brief Contains the entry point to the program
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

#include <game_state.h>
#include <game_states/main_game.h>
#include <stdio.h>

/** @brief The entry point for the game */
int main(void) {
	int err = game_loop_init_ncurses();
	if (err) {
		/* Don't handle errors. Just try to return to a canonical terminal mode */
		game_loop_terminate_ncurses();
		puts("Could not initialize ncurses!");
		return 1;
	}

	game_state state = state_main_game_create();

	err = state_game_loop_run(&state, 60);
	if (err) {
		/* Don't handle errors. Just try to return to a canonical terminal mode */
		state_main_game_destroy(&state);
		game_loop_terminate_ncurses();
		puts("An error occurred in the game");
		return 1;
	}

	state_main_game_destroy(&state);

	err = game_loop_terminate_ncurses();
	return err;
}
