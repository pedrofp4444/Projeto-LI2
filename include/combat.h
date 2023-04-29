/**
 * @file combat.h
 * @brief Combat system
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

#ifndef COMBAT_H
#define COMBAT_H

#include <stddef.h>

/**
 * @brief Enumerates the types of weapons that can exist in the game.
 */
typedef enum {
	WEAPON_HAND,    /**< Entity's hands */
	WEAPON_DAGGER,  /**< Weak strength weapon */
	WEAPON_ARROW,   /**< Medium strength weapon */
	WEAPON_BOMB,    /**< Strong strength weapon */
	WEAPON_LANTERN, /**< Provides light */
	WEAPON_IPAD,    /**< Extremely strong weapon */
} weapon;

/** @brief Gets the human-readable name of a weapon */
const char *weapon_get_name(weapon w);

/* Used to fix #include bug. Allows for inclusion of only the weapon enum */
#ifndef COMBAT_ONLY_WEAPONS

#include <map.h>
#include <entities.h>
#include <animation.h>

/**
 * @brief Based on the equiped weapon, detect whether an entity can attack another
 *
 * @param attacker
 *   The entity that will attack the @p attacked
 * @param attacked
 *   The entity that will be attacked by @p attacker
 * @param map
 *   The game map (for light and collision information)
 */
int combat_can_attack(const entity *attacker, const entity *attacked, const map *map);

#endif
#endif

