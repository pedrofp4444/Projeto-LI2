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

#include <map.h>
#include <game_state.h>
#include <stddef.h>

/**
 * @brief Enumerates the types of the entities that can exist in the game.
*/
typedef enum {
	ENTITY_PLAYER,   /**< The player */
	ENTITY_RAT,      /**< A mob of low intelligence */
	ENTITY_TROLL,    /**< A mob of medium intelligence and a coward */
	ENTITY_GOBLIN,   /**< A mob of high intelligence */
	ENTITY_CRISTINO, /**< A mob of high difficulty */
} entity_type;

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
 * @var entity::destroy
 *   Callback function to the destroy the entity (like in OOP). Must free ::entity::data, if
 *   applicable. If `NULL`, it won't be called.
*/
typedef struct entity {
	int x, y;
	entity_type type;

	int health, max_health;
	weapon weapon;

	void *data;

	void (*destroy)(struct entity *ent);
} entity;

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
*/
typedef struct entity_set {
	entity *entities;
	size_t count;
} entity_set;

/** @brief Allocates an ::entity_set with @p count entities. Those will be **unitialized**. */
entity_set entity_set_allocate(size_t count);

/** @brief Frees memory in an ::entity_set. */
void entity_set_free(entity_set entities);

/**
 * @brief Renders a set of entities on the terminal, within some specified bounds.
 *
 * This function renders a set of entities in a given recatngular area of a larger map on the
 * terminal. The function only renders entities visible in the specified window using
 * ::entity_render().
 *
 * @param entity_set A linked list of entities to render
 * @param map The game map, for light information
 * @param map_top The top coordinate of the map to be rendered
 * @param map_left The left coordinate of the map to be rendered
 * @param term_top The top coordinate of the terminal where the map will be rendered
 * @param term_left The left coordinate of the terminal where the map will be rendered
 * @param height The height of the map and the parts of the terminal to render
 * @param width The width of the map and the parts of the terminal to render
*/
void entity_set_render(entity_set entity_set, map map,
                       int map_top , int map_left,
                       int term_top, int term_left,
                       int height  , int width);

#endif

