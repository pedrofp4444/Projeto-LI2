/**
 * @file entities.h
 * @brief Entities
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

#ifndef ENTITIES_H
#define ENTITIES_H

#include <stddef.h>

#include <combat_types.h>
#include <animation.h>
#include <map.h>
#include <game_state.h>

/**
 * @brief Enumerates the types of the entities that can exist in the game.
 *
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
typedef enum {
	ENTITY_PLAYER,   /**< The player */
	ENTITY_RAT,      /**< A mob of low intelligence */
	ENTITY_GOBLIN,   /**< A mob of medium intelligence and a coward */
	ENTITY_CRISTINO, /**< A mob of high difficulty */
} entity_type;

/**
 * @brief Gets the human-readable name of an entity type
 * @author A104348 Humberto Gomes
 */
const char *entity_get_name(entity_type t);

/**
 * @struct entity
 * @brief Struct that represents a game entity.
 *
 * @var entity::x
 *   X coordinate of the entity on the map
 * @var entity::y
 *   Y coordinate of the entity on the map
 *
 * @var entity::type
 *   The type of the entity
 *
 * @var entity::health
 *   Entity's current health points
 * @var entity::max_health
 *   Entity's maximum health
 *
 * @var entity::weapon
 *   Weapon equipped by the entity
 *
 * @var entity::data
 *   Pointer to additional data (specific to each entity type)
 *
 * @var entity::animation
 *   Animation sequence for an entity.
 * @var entity::combat_target
 *   - `NULL` if an entity won't perform an attack during the current turn
 *   - ::combat_bomb_info* if ::entity::weapon is ::WEAPON_BOMB
 *   - ::combat_arrow_info* if ::entity::weapon is ::WEAPON_ARROW
 *   - ::entity* (target entity) for other values of ::entity::weapon
 *
 * @var entity::destroy
 *   Callback function to the destroy the entity (like in OOP). Must free ::entity::data, if
 *   applicable. If `NULL`, it won't be called.
 *
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
typedef struct entity {
	int x, y;
	entity_type type;

	int health, max_health;
	weapon weapon;

	void *data;

	animation_sequence animation;
	void *combat_target;

	void (*destroy)(struct entity *ent);
} entity;

/*
 * @brief Frees the combat target in an entity and sets it to `NULL`
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
void entity_free_combat_target(entity *ent);

/**
 * @struct entity_set
 * @brief Struct that represents a set of entities in the game.
 * @details To avoid list resizing, not all entities are valid. If `entity::health <= 0`, it is
 *          invalid.
 *
 *          The first entity should always be the player.
 *
 * @var entity_set::entities
 *   Pointer to the contiguous list of entities.
 * @var entity_set::count
 *   Number of entities in the set
 *
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
typedef struct entity_set {
	entity *entities;
	size_t count;
} entity_set;

/**
 * @brief Allocates an ::entity_set with @p count entities. Those will be **unitialized**.
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
entity_set entity_set_allocate(size_t count);

/**
 * @brief Frees memory in an ::entity_set.
 *
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
void entity_set_free(entity_set entities);

/**
 * @brief Gets the entities closest another entity
 * @details The distance criterion is the Manhattan distance
 *
 * @param ent The reference entity (e.g.: for the sidebar, get entities close to the player)
 * @param in  The set of all entities in the map
 * @param map If not `NULL`, only visible entities will be added to @p out
 *
 * @return A set with **a maximum of** @p max_count entities ordered by distance to @p ent.
 *
 * @author A104348 Humberto Gomes
 */
entity_set entity_get_closeby(entity ent, entity_set in, size_t max_count, const map *map);

/**
 * @brief Renders a set of entities on the terminal, within some specified bounds.
 *
 * This function renders a set of entities in a given recatngular area of a larger map on the
 * terminal. The function only renders entities visible in the specified window using
 * ::entity_render().
 *
 * @param entity_set A linked list of entities to render
 * @param map The game map, for light information
 * @param wnd The visible map window
 *
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
void entity_set_render(entity_set entity_set, map map, const map_window *wnd);

/**
 * @brief Animates all entities in an entity set (changes their position)
 *
 * @param entity_set The set to be animated
 * @param step_index The index of the current animation step. If some entities' animations have
 *                   less than this number of steps, they just won't be animated.
 *
 * @return 1 if incrementing @p step_index would cause no entity to be moved (all entities would
 *         have shorter animation lengths than @p step_index, there is no need to continue looping
 *         through indices), 0 otherwise.
 *
 * @author A104348 Humberto Gomes
 */
int entity_set_animate(entity_set entity_set, size_t step_index);

#endif

