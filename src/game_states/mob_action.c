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
#include <entities_search.h>

#include <stdlib.h>
#include <time.h>

/**
 * @brief Animate the mob movement and the attack.
 * @param mob A pointer to the mob
 * @param state A pointer to the main game state data.
 * @param distance_x The horizontal distance between the mob and the player
 * @param distance_y The vertical distance between the mob and the player
 *
 * @author A90817 Mariana Rocha
 * @author A104100 Hélder Gomes
 * @author A90817 Pedro Pereira
 */
void state_main_game_mob_run_ai(entity *mob, state_main_game_data *state, int distance_x, int distance_y) {


	// Pathfinding
	animation_step start = { .x = mob->x, .y = mob->y };
	animation_step end = { .x = PLAYER(state).x + distance_x, .y = PLAYER(state).y + distance_y };
	mob->animation = search_path(&(state->map), mob->type, start, end);

	// Combat
	animation_step old = { .x = mob->x, .y = mob->y };
	if (mob->animation.length != 0) {
		mob->x = mob->animation.steps[mob->animation.length - 1].x;
		mob->y = mob->animation.steps[mob->animation.length - 1].y;
	}

	if (combat_can_attack(mob, &PLAYER(state), &state->map)) {
		combat_attack(mob, &PLAYER(state), &state->map);
	}
	mob->x = old.x; mob->y = old.y;
}

/**
 * @brief Animate all the visible mobs by the player.
 * @param state A pointer to the main game state data.
 *
 * @author A90817 Mariana Rocha
 * @author A104100 Hélder Gomes
 * @author A90817 Pedro Pereira
 */
void state_main_game_mobs_run_ai(state_main_game_data *state) {

	int possible_distances[] = {-3, -2, -1, 0, 1, 2, 3};

	for (size_t i = 1; i < state->entities.count; ++i) {
		entity *ent = state->entities.entities + i;
		if (ent->health > 0 && ent->x >= 0 && (unsigned) ent->x < state->map.width &&
		                       ent->y >= 0 && (unsigned) ent->y < state->map.height) {

			if (state->map.data[ent->y * state->map.width + ent->x].light) {
				int seed_x = rand() % 7;
				int seed_y = rand() % 7;
				state_main_game_mob_run_ai(ent, state, possible_distances[seed_x], possible_distances[seed_y]);
			}
		}
	}
}
