/**
 * @file goblin.h
 * @brief Goblin
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

#ifndef GOBLIN_H
#define GOBLIN_H

#include <entities.h>

/**
 * @brief   Creates a new entity type of type ENTITY_GOBLIN.
 * @details The function assigns a random weapon to the entity, limited to ::WEAPON_HAND,
 *          ::WEAPON_DAGGER, ::WEAPON_ARROW, ::WEAPON_BOMB.
 *
 * @param x The x coordinate of the entity on the map
 * @param y The y coordinate of the entity on the map
 * @param health The entity health points
 * @return The newly created entity
 *
 * @author A90817 Mariana Rocha
 * @author A104348 Humberto Gomes
 * @author A104100 Hélder Gomes
 * @author A104086 Pedro Pereira
 */
entity entity_create_goblin(unsigned x, unsigned y, int health);

#endif

