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

#ifndef ENTITIES_SEARCH_H
#define ENTITIES_SEARCH_H

#include <game_state.h>
#include <animation.h>
#include <map.h>
#include <entities.h>

/**
 * @struct node
 * @brief Struct that represents a node in a serach algorithm.
 * @var node::pos
 *   The position of the node.
 * @var node::f
 *  The total cost of the node (heuristic and path).
 * @var node::g
 * 	The path cost from the start node to this node.
 * @var node::h
 * 	The heuristic cost from this node to the final node.
 * @var node::parent
 * 	A pointer to the parent node of this node.
*/
typedef struct node {
	animation_step pos;
	float f, g, h;
	struct node *parent;
} node;

/**
 * @brief Calculates the Manhattan distance between two positions on the map
 *
 * @param x1 The X coordinate of the first position.
 * @param y1 The Y coordinate of the first position.
 * @param x2 The X coordinate of the second position.
 * @param y2 The Y coordinate of the second position.
 * @returns The manhattan distance between the two positions.
 */
float manhattan_distance(unsigned x1, unsigned y1, unsigned x2, unsigned y2);

/**
 * @brief Calculates a heuristic estimate of the cost to reach the destination from a given position.
 *
 * This function uses the Manhattan distance to provide a heuristic estimate of the cost
 * to reach the destination from a given position.
 *
 * @param x1 The X coordinate of the first position.
 * @param y1 The Y coordinate of the first position.
 * @param x2 The X coordinate of the second position.
 * @param y2 The Y coordinate of the second position.
 * @returns The heuristic estimate between two positions.
 */
float heuristic(unsigned x1, unsigned y1, unsigned x2, unsigned y2);

/**
 * @brief Checks if a given position is valid on the map.
 *
 * @param map A pointer to the map containing the dimensions and obstacles.
 * @param ent The entity whose positions are being checked
 * @param x The X coordinate of the position.
 * @param y The Y coordinate of the position.
 * @returns 1 if the position is within the map boundaries and not obstructed, 0 otherwise.
 */
int is_valid_position(map *map, entity_type ent, unsigned x, unsigned y);

/**
 * @brief Calculates the cost of moving from a start position to an end position.
 *
 * If the end position is invalid, the function returns INFINITY (which should not happen).
 *
 * @param map A pointer to the map containing the dimensions and additional data.
 * @param ent The entity whose cost are being calculated
 * @param start The starting position.
 * @param end The ending position.
 * @returns The cost of moving from the start position to the end position, including the heuristic
 * if applicable or INFINITY if the end position is invalid.
 */
float get_cost(map *map, entity_type ent, animation_step start, animation_step end);

/**
 * @brief Returns the node with the lowest f value from the list of open nodes.
 *
 * @param open An array of pointers to open nodes.
 * @param n_open The number of open nodes in the array.
 * @returns A pointer to the node with the lowest f value in the open nodes.
 * if applicable or INFINITY if the end position is invalid.
 */
node *get_lowest_f_node(node **open, int n_opne);

/**
 * @brief Returns a node with a given position from a list of nodes.
 *
 * @param list An array of pointers to nodes.
 * @param n_list The number of nodes in the array.
 * @param pos The position of the node to be returned.
 * @return A pointer to the node with the position or NULL if there isn't any node in the list with
 * that position.
*/
node *get_node_in_list(node **list, int n_list, animation_step pos);

/**
 * @brief Creates a new node with a given position, f value, g value, h value and parent node.
 *
 * @param pos The position of the node.
 * @param f The f value of the node.
 * @param g The g value of the node.
 * @param h The h value of the node.
 * @param parent A pointer to the parent node of the new node.
 * @return A pointer to the newly node.
*/
node *create_node(animation_step pos, float f, float g, float h, node *parent);

/**
 * @brief Frees the memory allocated for a node.
 *
 * @param node A pointer to the node to be freed.
*/
void node_destroy(node *node);

/**
 * @brief Frees the memory allocated for a list of nodes.
 *
 * @param list An array of pointers to nodes.
 * @param n_list The number of nodes in the array.
*/
void list_destroy(node **list, int n_list);

/**
 * @brief Calculates the path from a final node by following their parent nodes back to the initial
 *        node.
 *
 * @param end_node The final node in the path.
 * @return An animation sequence containing all positions
*/
animation_sequence calculate_path(node *end_node);

/**
 * @brief Finds the nearest empty tile to the given position.
 *
 * @param map Pointer to the map struct.
 * @param pos The position to start searching from.
 * @return The nearest empty tile, or (-1,-1) if no empty tile was found.
*/
animation_step find_nearest_empty_tile(map *map, animation_step pos);

/**
 * @brief Implements the A* algorithm to find the shortest path between two positions on the map.
 *
 * It starts with the initial position node and adds its neighbors to the list of open nodes,
 * calculating their f, g and h values. Then selects the node with the lowest f value in the
 * list of open nodes and checks if it is the end node. If it is not, it adds it to the list of
 * closed nodes and continues the process until the end node is found or the list of open nodes is
 * empty, which means there is no possible path between the two positions.
 *
 * @param map A pointer to the map containing the dimensions and additional data.
 * @param ent The entity whose path is being found.
 * @param start The starting position of the path.
 * @param end The ending position of the path.
 * @return An animation sequence. Its length will be zero if no path could be found.
*/
animation_sequence search_path(map *map, entity_type ent, animation_step start, animation_step end);

#endif

