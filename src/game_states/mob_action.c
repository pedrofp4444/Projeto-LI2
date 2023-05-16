/**
 * @file mob_action.c
 * @brief Deal with mob movement and combat actions
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

#include <map.h>
#include <combat.h>
#include <game_states/main_game.h>

void state_main_game_mob_run_ai(entity *mob, state_main_game_data *state) {
	if (combat_can_attack(mob, &PLAYER(state), &state->map)) {
			combat_attack(mob, &PLAYER(state), &state->map);
	}
}

void state_main_game_mobs_run_ai(state_main_game_data *state) {

	for (size_t i = 1; i < state->entities.count; ++i) {
		entity *ent = state->entities.entities + i;
		if (ent->health > 0 && ent->x >= 0 && (unsigned) ent->x < state->map.width &&
		                       ent->y >= 0 && (unsigned) ent->y < state->map.height) {

			if (state->map.data[ent->y * state->map.width + ent->x].light)
				state_main_game_mob_run_ai(ent, state);
		}
	}
}
