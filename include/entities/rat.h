/**
 * @file rat.h
 * @brief Rat
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

#ifndef RAT_H
#define RAT_H

#include <entities.h>

/**
 * @brief Creates a new entity type of type ENTITY_RAT 
 * with especified coordinates and health points. The only available weapon is the rat's hands.
 * 
 * @param x The x coordinate of the entity on the map
 * @param y The y coordinate of the entity on the map
 * @param health The entity health points
 * @return The newly created entity
*/
entity entity_create_rat(unsigned x, unsigned y, int health);

#endif
