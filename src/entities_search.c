/**
 * @file  entities_search.c
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

#include <map.h>
#include <entities.h>
#include <core.h>
#include <game_states/main_game.h>
#include <entities_search.h>

#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

int is_valid_position(map *map, entity_type ent, unsigned x, unsigned y) {
	if (x < map->width && y < map->height) {
		tile_type type = map->data[y * map->width + x].type;
		if (ent == ENTITY_CRISTINO)
			return (type == TILE_EMPTY || type == TILE_WATER);
		else
			return (type == TILE_EMPTY);
	}
	else return 0;
}

animation_sequence calculate_path(node *end_node) {
	int length = 0;
	node *current_node = end_node;
	while (current_node != NULL) {
		length++;
		current_node = current_node->parent;
	}

	animation_step *path = malloc(sizeof(animation_step) * length);

	current_node = end_node;
	int i = length - 1;
	while (current_node != NULL) {
		path[i] = current_node->pos;
		i--;
		current_node = current_node->parent;
	}

	animation_sequence ret = {
		.steps = path,
		.length = length,
		.capacity = length
	};

	return ret;
}

animation_step find_nearest_empty_tile(map *map, animation_step pos) {
	float min_distance = INFINITY;
	animation_step nearest_empty_tile = {pos.x,pos.y};

	for (unsigned y = 0; y < map->height; y++) {
		for (unsigned x = 0; x < map->width; x++) {
			if (map->data[y * map->width + x].type == TILE_EMPTY) {
				int distance = manhattan_distance(pos.x, pos.y, (int)x, (int)y);
				if (distance < min_distance) {
					min_distance = distance;
					nearest_empty_tile = (animation_step){(int)x, (int)y};
				}
			}
		}
	}
	return nearest_empty_tile;
}

animation_sequence search_path(map *map, entity_type ent, animation_step start, animation_step end) {

	if (ent != ENTITY_CRISTINO && map->data[end.y * map->width + end.x].type == TILE_WATER) {
		animation_step aux = end;
		end = find_nearest_empty_tile(map, end);
		/* TILE_EMPTY not found near */
		if (end.y == aux.y && end.x == aux.x) {
			return animation_sequence_create();
		}
	}

	/* Possible directions. */
	int dx[] = {0, 0, -1, 1};
	int dy[] = {-1, 1, 0, 0};
	int width = map->width;
	int height = map->height;


	/* Allocate memory for the visited matrix. */
	int **visited = calloc(height, sizeof(int *));
	for (int i = 0; i < height; i++) {
		visited[i] = calloc(width, sizeof(int));
	}

	node start_node;
	start_node.pos = start;
	start_node.parent = NULL;

	node *queue = malloc(sizeof(node) * width * height);
	int front = 0, back = 0;
	queue[back++] = start_node;
	visited[start.y][start.x] = 1; /* If a node was visited it has the corresponding value to 1. */

	animation_sequence path = animation_sequence_create();

	while (front < back) {

		node current_node = queue[front++];
		animation_step current_pos = current_node.pos;

		if (current_pos.x == end.x && current_pos.y == end.y) {
			free(path.steps);
			path = calculate_path(&current_node);
			break;
		}

		if (manhattan_distance(current_pos.x, current_pos.y, start.x, start.y) > 20) break;

		for (int i = 0; i < 4; i++) {

			int new_x = current_pos.x + dx[i];
			int new_y = current_pos.y + dy[i];

			if (new_x >= 0 && new_x < width &&
				  new_y >= 0 && new_y < height &&
				  !visited[new_y][new_x] && is_valid_position(map, ent, new_x, new_y)) {

				visited[new_y][new_x] = 1;

				node new_node;
				new_node.pos.x = new_x;
				new_node.pos.y = new_y;

				new_node.parent = &queue[front - 1];
				queue[back++] = new_node;
			}
		}
	}

	/* Free memory. */
	for (int i = 0; i < height; i++) {
		free(visited[i]);
	}
	free(visited);
	free(queue);

	return path;
}

