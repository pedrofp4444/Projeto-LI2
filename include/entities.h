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

/**
 * @brief Enumerates the types of the entities that can exist in the game.
*/
typedef enum {
    ENTITY_PLAYER, /**< The player */
    ENTITY_RAT, /**< A mob of low intelligence */
    ENTITY_TROLL, /**< A mob of medium intelligence and a coward */
    ENTITY_GOBLIN, /**< A mob of high intelligence */
    ENTITY_CRISTINO, /**< A mob of high difficulty */
} entity_type;

/**
 * @brief Enumerates the types of weapons that can exist in the game.
*/
typedef enum {
    WEAPON_HAND, /**< Entity hands */
    WEAPON_DAGGER, /**< Weak weapon */
    WEAPON_ARROW, /**< Medium weapon */
    WEAPON_BOMB, /**< Strong weapon */
    WEAPON_LANTERN, /**< Provides light */
    WEAPON_IPAD, /**< Extremely strong weapon */
} weapon;

/**
 * @struct weapon
 * @brief Represents a game entity.
 * @var weapon::WEAPON_HAND
 *   Entity's hands
 * @var weapon::WEAPON_ARROW
 *   Medium strength weapon
 * @var weapon::WEAPON_BOMB
 *   Strong weapon
 * @var weapon::WEAPON_LANTERN
 *   Provides light
 * @var weapon::WEAPON_IPAD
 *   Extremely strong weapon
*/
typedef struct {
    unsigned x, y; /**< Coordinates (x,y) of the entity on the map */
    entity_type type; /**< Entity type (player or mob) */
    
    int health; /**< Entity health points */
    weapon weapon; /**< Weapon equipped by the entity */

    void *data; /**< Pointer to additional data */
    void (*update)(void); /**< Pointer to an entity update function */
} entity;

#endif
