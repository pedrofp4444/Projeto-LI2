/**
 * @file combat_types.h
 * @brief Combat system types (separated for headers to work propperly)
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

#ifndef COMBAT_TYPES_H
#define COMBAT_TYPES_H

#include <animation.h>

/**
 * @brief Enumerates the types of weapons that can exist in the game.
 * @author A90817 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
typedef enum {
	WEAPON_HAND,    /**< Entity's hands */
	WEAPON_DAGGER,  /**< Weak strength weapon */
	WEAPON_ARROW,   /**< Medium strength weapon */
	WEAPON_BOMB,    /**< Strong strength weapon */
	WEAPON_IPAD,    /**< Extremely strong weapon */
	WEAPON_INVALID  /**< Only to be used for weapon drop message boxes */
} weapon;

/**
 * @brief Gets the human-readable name of a weapon
 * @author A104348 Humberto Gomes
 */
const char *weapon_get_name(weapon w);

/**
 * @struct combat_bomb_info
 * @brief Information about a bomb drop
 *
 * @var combat_bomb_info::x
 *   Horizontal position of the bomb
 * @var combat_bomb_info::y
 *   Vertical position of the bomb
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	int x, y;
} combat_bomb_info;

/**
 * @struct combat_arrow_info
 * @brief Information about a thrown arrow
 *
 * @var combat_arrow_info::animation
 *   Path of the arrow on the map
 *
 * @author A104348 Humberto Gomes
 */
typedef struct combat_arrow_info {
	animation_sequence animation;
} combat_arrow_info;

#endif

