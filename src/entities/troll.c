/**
 * @file troll.c
 * @brief Troll implementation
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
#include <entities/troll.h>

entity entity_create_troll(unsigned x, unsigned y, int health) {
	/*Initialize the random number generator seed based on the current time*/
	srand(time(NULL));
	int weapon_index = rand() % 3;

	entity troll = {
		.x = x,
		.y = y,
		.type = ENTITY_TROLL,

		.health = health,
		.weapon = weapon_index,

		.data = NULL,
		.update = NULL
	};

	return troll;
}

