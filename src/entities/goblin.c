/**
 * @file goblin.c
 * @brief Goblin implementation
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

#include <stdlib.h>
#include <time.h>
#include <entities/goblin.h>

/**
 * @brief   Creates a new entity type of type ENTITY_GOBLIN.
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
*/
entity entity_create_goblin(unsigned x, unsigned y, int health) {
	int weapon_index = rand() % 3;

	entity goblin = {
		.x = x,
		.y = y,
		.type = ENTITY_GOBLIN,

		.health = health, .max_health = health,
		.weapon = weapon_index,

		.animation = animation_sequence_create(),
		.combat_target = NULL,

		.data = NULL,
		.destroy = NULL
	};

	return goblin;
}

