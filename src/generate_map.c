/**
 * @file generate_map.c
 * @brief Game map generation implementation
 */

/*
 *   Copyright 2023 Hélder Gomes, Humberto Gomes, Mariana Rocha, Pedro Pereira
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *	   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <core.h>
#include <generate_map.h>
#include <map.h>

#include <entities/rat.h>
#include <entities/goblin.h>
#include <entities/cristino.h>
#include <entities/player.h>

#define MAP_WIDTH 1024
#define MAP_HEIGHT 1024

#define ENTITY_COUNT 2500
#define TILE_PERCENTAGE 45

#define ENTITY_RAT_HEALTH 5
#define ENTITY_GOBLIN_HEALTH 10
#define ENTITY_CRISTINO_HEALTH 20

#define STARTER_CIRCLE 5

/**
 * @def FOR_GRID_BORDER(row, col, border, data)
 * @brief Macro that defines a nested for loop to iterate over a 2D grid map, ignoring the border.
 *
 * @param row The name of the row variable used in the loop.
 * @param col The name of the column variable used in the loop.
 * @param border The number of border rows and columns to ignore at the edges
 *               (see macro definition).
 * @param map Game map
 */
#define FOR_GRID_BORDER(row, col, border, map) \
	for (unsigned row = border; row < map.height - border; row++) \
		for (unsigned col = border; col < map.width - border; col++)

/**
 * @brief Counts the number of tiles of a certain type within a given radius around a given
 *        position.
 * @return The number of tiles of the specified type found within the radius.
 *
 * @param map A pointer to the game map.
 * @param row The row of the center position.
 * @param col The column of the center position.
 * @param radius The radius of the search.
 * @param tile The type of tile to be counted.
 */
INLINE int radius_count(map map, unsigned row, unsigned col, unsigned radius, tile_type tile) {

	/* IMPLEMENTATION NOTES:
	 *
	 * Most of the map generation time is spent on this function, so this ugly implementation was
	 * needed for decent performance.
	 *
	 * The minimum and maximum column and row values are calculated before looping, to avoid
	 * performing bounds checking every iteration.
	 *
	 * The index of the first element of a row is only calculated once per row using an
	 * accumulated sum, to avoid expensive multiplications every iteration.
	 */

	register int count = 0;
	int maxc = (int) min(col + radius, map.width  - 1),
	    maxr = (int) min(row + radius, map.height - 1);

	int r = max(0, (int) row - (int) radius);
	int row_start = r * map.width;

	for (; r <= maxr; r++) {
		for (int c = max(0, (int) col - (int) radius); c <= maxc; c++) {
			count += map.data[row_start + c].type == tile;
		}

		row_start += map.width;
	}

	return count;
}

/** @brief Swaps two maps */
void map_swap(map *a, map *b) {
	map tmp = *a;
	*a = *b;
	*b = tmp;
}

/**
 * @brief Fills a map with natural-looking blobs of a certain tile type.
 *
 * @param scratch_map A map that will be used for intermediate computations (will be altered).
 *                    Must be the same size as @p map.
 * @param map Struct containing the map (for output)
 * @param radius1 The radius used in the smoothing process for the walls
 * @param radius2 The radius used in the smoothing process for the walls
 * @param tile The type of tile to be placed in the empty map
 *
 * @details
 *
 * 1. The function fills the map grid randomly with tiles of type @p tile and empty spaces.
 * 2. The walls are smoothed by replacing the wall tile type of each cell with
 *    the empty tile type if it has fewer than @p radius2 wall neighbors or more than radius1 wall
 *    neighbors within a given radius.
 * 3. This process is repeated again, but with a fixed internal radius.
 */
void generate_random(map scratch_map, map map, int radius1, int radius2, tile_type tile) {
	srand(time(NULL));

	// Initialize empty map data to prevent access to uninitialized data
	map_zero(map);
	map_zero(scratch_map);

	// Randomly generate the tile everywhere
	unsigned tile_count = map.width * map.height;
	for (unsigned i = 0; i < tile_count; ++i) {
		map.data[i].type = ((rand()%100) < TILE_PERCENTAGE) ? tile : TILE_EMPTY;
	}

	// Smooths the tiles
	for (int i = 0; i < 5; i++) {
		FOR_GRID_BORDER(r, c, 1, map) {
			scratch_map.data[r * map.width + c].type =
				(radius_count(map, r, c, 1, tile) >= radius1) ||
				(radius_count(map, r, c, 2, tile) <= radius2) ?
				tile : TILE_EMPTY;
		}

		map_swap(&map, &scratch_map);
	}

	// Smooths the tiles again
	for (int i = 0; i < 5; i++) {
		FOR_GRID_BORDER(r, c, 1, map) {
			scratch_map.data[r * map.width + c].type =
				(radius_count(map, r, c, 1, tile) >= 5) ? tile : TILE_EMPTY;
		}

		map_swap(&map, &scratch_map);
	}

	/* Because the number of swaps is even (5 + 5 = 10), there is no need for copies at the end */
}

/**
 * @brief Intersects two maps, creating a third map that represents the intersection.
 * @details Given two maps, @p map1 and @p map2, this function edits the @p result map. The
 *          resulting map will have the same dimensions as @p map1 and @p map2. For every position:
 *
 * If the tile in @p map1 is ::TILE_EMPTY, the tile from @p map2 will be chosen. Otherwise, the
 * tile from @p map1 is placed in @p result.
 *
 * @param map1 The first map.
 * @param map2 The second map.
 * @param result The resulting map, that will be written to.
 * @note @p map1, @p map2 and @p result must have the same dimentions.
 */
void intersect_maps(map map1, map map2, map result) {

	FOR_GRID_BORDER(r, c, 0, result) {
		result.data[r * result.width + c].type =
			map1.data[r * result.width + c].type == TILE_EMPTY ?
			map2.data[r * result.width + c].type :
			map1.data[r * result.width + c].type;
	}
}

/**
 * @brief Draw a wall border around the map
 * @details This function draws a wall border around the map by setting the corresponding tiles to
 *          the TILE_WALL type.
 * @param map Game map to modify
 */
void draw_border(map map) {

	// Horizontal walls
	for (unsigned i = 0; i < map.width; i++) {
		map.data[i].type = TILE_WALL;
		map.data[(map.height - 1) * map.width + i].type = TILE_WALL;
	}

	// Vertical walls
	for (unsigned i = 0; i < map.height; i++) {
		map.data[i * map.width].type = TILE_WALL;
		map.data[(i + 1) * map.width - 1].type = TILE_WALL;
	}
}

/**
 * @brief Spawn entities randomly on the game map.
 * This function randomly spawns entities on the game map. It generates a random position for each
 * entity and assigns a random type and health value. The entity positions are checked to ensure
 * that they do not overlap with walls or water tiles on the map.
 * @param data A pointer to the game data structure.
 */
void entity_spawn(state_main_game_data *data){

	for (int i = 1; i < ENTITY_COUNT; ++i) {

		int seed = rand() % 100 + 1;

		unsigned x, y;
		do {
			x = rand() % MAP_WIDTH;
			y = rand() % MAP_WIDTH;
		} while (data->map.data[y * data->map.width + x].type != TILE_EMPTY);

		if (seed < 50) {
			data->entities.entities[i] = entity_create_rat(x, y, ENTITY_RAT_HEALTH);
		} else if (seed >= 50 && seed < 85) {
			data->entities.entities[i] = entity_create_goblin(x, y, ENTITY_GOBLIN_HEALTH);
		} else {
			data->entities.entities[i] = entity_create_cristino(x, y, ENTITY_CRISTINO_HEALTH);
		}
	}
}

/**
 * @brief Spawns the player entity and opens a safe starting area on the map.
 * @param data Pointer to the main game state data.
 */
void player_spawn(state_main_game_data *data){

	// Set player data. TODO - set health
	int playerx = data->map.width / 2, playery = data->map.height / 2;
	data->entities.entities[0] = entity_create_player(playerx, playery, 1);

	// Open a safe place to start
	for (unsigned y = playery - STARTER_CIRCLE; y < data->map.height; y++) {
		for (unsigned x = playerx - STARTER_CIRCLE; x < data->map.width; x++) {

			// dist((x, y), player) <= STARTER_CIRCLE
			if ((x - playerx) * (x - playerx) + (y - playery) * (y - playery)
				<= STARTER_CIRCLE * STARTER_CIRCLE) {

				data->map.data[y * data->map.width + x].type = TILE_EMPTY;
			}
		}
	}
}

void generate_map_random(state_main_game_data *data) {

	map scratch_map = map_allocate(MAP_WIDTH, MAP_HEIGHT); /* For temporary calculations */

	data->map = map_allocate(MAP_WIDTH, MAP_HEIGHT);
	data->entities = entity_set_allocate(ENTITY_COUNT);

	// Randomly generate water map
	generate_random(scratch_map, data->map, 6, 1, TILE_WATER);

	// Randomly generate new map with walls
	map wall_map = map_allocate(MAP_WIDTH, MAP_HEIGHT);
	generate_random(scratch_map, wall_map, 5, 2, TILE_WALL);

	// Intersect the two maps
	intersect_maps(wall_map, data->map, data->map);

	// Draw border with walls
	draw_border(data->map);

	// Populate the map with entities and the player
	entity_spawn(data);
	player_spawn(data);

	// Free temporary data
	map_free(wall_map);
	map_free(scratch_map);
}

