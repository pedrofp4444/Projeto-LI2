/**
 * @file  entities_search.h
 * @brief The implementation of the search for the player (BFS algorithm)
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

#ifndef ENTITIES_SEARCH_H
#define ENTITIES_SEARCH_H

#include <game_state.h>
#include <animation.h>
#include <map.h>
#include <entities.h>

/**
 * @struct node
 * @brief Struct that represents a node in BFS algorithm.
 * @var node::pos
 *   The position of the node.
 * @var node::parent
 * 	A pointer to the parent node of this node.
*/
typedef struct node {
	animation_step pos;
	struct node *parent;
} node;

/**
 * @brief Checks if a given position is valid on the map.
 *
 * @param map A pointer to the map containing the dimensions and obstacles.
 * @param ent The entity whose positions are being checked.
 * @param x The X coordinate of the position.
 * @param y The Y coordinate of the position.
 * @returns 1 if the position is within the map boundaries and not obstructed, 0 otherwise.
 */
int is_valid_position(map *map, entity_type ent, unsigned x, unsigned y);

/**
 * @brief Calculates the path from a final node by following their parent nodes back to the initial
 *        node.
 *
 * @param end_node The final node in the path.
 * @return An animation sequence containing all positions.
*/
animation_sequence calculate_path(node *end_node);

/**
 * @brief Finds the nearest empty tile to the given position.
 *
 * @param map Pointer to the map struct.
 * @param pos The position to start searching from.
 * @return The nearest empty tile, or the same position if no empty tile was found.
*/
animation_step find_nearest_empty_tile(map *map, animation_step pos);

/**
 * @brief Implements the Breadth-first search algorithm to find the shortest path between two
 * positions on the map.
 *
 * @param map A pointer to the map containing the dimensions and additional data.
 * @param ent The entity whose path is being found.
 * @param start The starting position of the path.
 * @param end The ending position of the path.
 * @return An animation sequence representing the path. If no path is found or an error occurs,
 * an empty animation sequence is returned.
*/
animation_sequence search_path(map *map, entity_type ent, animation_step start, animation_step end);

#endif

