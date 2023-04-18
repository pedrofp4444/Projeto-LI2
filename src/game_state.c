/**
 * @file game_state.c
 * @brief Implementation of the game state abstraction
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

void state_switch(game_state *current, const game_state *new, int destroy) {
	if (destroy && current->destroy)
		current->destroy(current);
	*current = *new;
}

int state_game_loop_run(game_state *state, unsigned int fps) {
	return game_loop_run(state, &state->callbacks, fps);
}

