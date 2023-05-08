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
#include <animation.h>

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

/**
 * @struct combat_bomb_info
 * @brief Information about a bomb drop
 *
 * @var combat_bomb_info::x
 *   Horizontal position of the bomb
 * @var combat_bomb_info::y
 *   Vertical position of the bomb
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
 */
typedef struct combat_arrow_info {
	animation_sequence animation;
} combat_arrow_info;

/* Used to fix #include bug. Allows for inclusion of only the weapon enum */
#ifndef COMBAT_NO_ENTITY_DEPENDENCY

#include <map.h>
#include <entities.h>
#include <game_states/main_game_renderer.h>

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

/**
 * @brief Set the combat target of the @p attacker. No damage will be dealt
 * @details Call ::combat_can_attack before, or this may lead to invalid attacks
 *
 * @param attacker
 *   The entity that will attack the @p attacked
 * @param attacked
 *   The entity that will be attacked by @p attacker
 * @param map
 *   The game map (for light and collision information)
 */
void combat_attack(entity *attacker, const entity *attacked, const map *map);

/**
 * @brief Causes the consequences of the @p step_index -th step of an animation
 * @details Responsible for causing damage on entities
 *
 * @param all        The set of all entities (to deal damage)
 * @param entity_set The set of entities to be animated (the only combat actions considered)
 * @param step_index Number of the animation step to be processed
 *
 * @return 1 if incrementing @p step_index would cause nothing to happen (end of combat
 *         animations), 0 otherwise.
 */
int combat_animation_update(entity_set all, entity_set entity_set, size_t step_index);

/**
 * @brief Animates all combat actions in an entity set.
 * @details This function won't cause damage to entities. Only update the overlay.
 *
 * @param entity_set The set to be animated
 * @param step_index The index of the current animation step. If some entities' combat animations
 *                   have less than this number of steps, they just won't be animated.
 * @param overlay Overlay on top of the map for animation rendering. Can be `NULL` for no rendering
 *
 * @param map_top The top coordinate of the map to be rendered
 * @param map_left The left coordinate of the map to be rendered
 * @param term_top The top coordinate of the terminal where the map will be rendered
 * @param term_left The left coordinate of the terminal where the map will be rendered
 * @param height The height of the map and the parts of the terminal to render
 * @param width The width of the map and the parts of the terminal to render
 */
void combat_entity_set_animate(entity_set entity_set, size_t step_index, ncurses_char *overlay,
                              int map_top , int map_left,
                              int height  , int width);

#endif
#endif

