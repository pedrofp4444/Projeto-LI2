/**
 * @file  entities_search.h
 * @brief The implementation of the search for the player (A* algorithm)
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

#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include <map.h>

/**
 * @brief Shows the tiles that the player can see.
 *
 * This function uses the Bresenham's algorithm to calculate the tiles that the player can see.
 *
 * @param m The map containing the dimensions and obstacles.
 * @param x The X coordinate of the player.
 * @param y The Y coordinate of the player.
 * @param r The radius of the vision circle
*/
void state_main_game_circle_light_map(map m, int x, int y, int r);

#endif