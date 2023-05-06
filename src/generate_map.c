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

#define ENTITY_RAT_HEALTH 20
#define ENTITY_GOBLIN_HEALTH 50
#define ENTITY_CRISTINO_HEALTH 200

#define STARTER_CIRCLE 5

/**
 * @def FOR_GRID_BORDER(row, col, border, data)
 * @brief Macro that defines a nested for loop to iterate over a 2D grid, ignoring the border.
 * This macro defines a nested for loop that iterates over the rows and columns of a 2D grid,
 * starting from the specified border and ending at the opposite border. It is useful when
 * iterating over a grid with a border that should be ignored.
 * @param row The name of the row variable used in the loop.
 * @param col The name of the column variable used in the loop.
 * @param border The number of border rows and columns to ignore.
 * @param map Struct containing the map.
 */
#define FOR_GRID_BORDER(row, col, border, map) \
	for (unsigned row = border; row < map.height - border; row++) \
		for (unsigned col = border; col < map.width - border; col++)

/**
 * @brief Counts the number of tiles of a certain type within a given radius around a given position.
 * @param map A pointer to the main game data.
 * @param row The row of the center position.
 * @param col The column of the center position.
 * @param radius The radius of the search.
 * @param tile The type of tile to be counted.
 * @return The number of tiles of the specified type found within the radius.
 * This function counts the number of tiles of a certain type within a given radius around a given
 * position.
 * It takes a pointer to the main game data, the row and column of the center position, the radius
 * of the search,
 * and the type of tile to be counted. It returns the number of tiles of the specified type found
 * within the radius.
 * The function loops through all positions within the radius and checks whether the tile at that
 * position is of the specified type.
 * If it is, it increments the count. The function returns the final count after all positions
 * have been checked.
 */
int radius_count(map map, unsigned row, unsigned col, unsigned radius, tile_type tile) {
	int count = 0;
	for (unsigned c = col - radius; c <= col + radius; c++)
		for (unsigned r = row - radius; r <= row + radius; r++)
			if (r < map.height && c < map.width && map.data[r * map.width + c].type == tile)
				count++;
	return count;
}

/**
 * @brief Generates random walls and empty spaces in the map and smoothes the walls to make them
 * look more natural
 * @param map Struct containing the map
 * @param radius1 The radius used in the smoothing process for the walls
 * @param radius2 The radius used in the smoothing process for the walls
 * @param tile The type of tile that represents walls
 * @details The function generates random walls and empty spaces in the map by filling the cells of
 * the grid with the tile type or the empty tile type based on a random number generated using the
 * rand() function. Then, the walls are smoothed by replacing the wall tile type of each cell with
 * the empty tile type if it has fewer than radius2 wall neighbors or more than radius1 wall
 * neighbors within a given radius. This process is repeated twice.
 * @note The function uses the FOR_GRID_BORDER macro to iterate through the grid borders and avoid
 * accessing out-of-bounds cells.
 */
void generate_random(map map, int radius1, int radius2, tile_type tile) {
	srand(time(NULL));

	tile_type** allocated_map = (tile_type**) malloc(map.height * sizeof(tile_type*));
	memset(allocated_map, 0, map.height * sizeof(tile_type*));
	for(unsigned i = 0; i < map.height; i++) {
		allocated_map[i] = (tile_type*) malloc(map.width * sizeof(tile_type));
	}

	// Initialize empty map data to prevent access to uninitialized data
	FOR_GRID_BORDER(r, c, 0, map) {
		map.data[r * map.width + c].type = TILE_EMPTY;
		map.data[r * map.width + c].light = 0;
	}

	// Randomly generate the tile everywhere
	FOR_GRID_BORDER(r, c, 2, map) {
		map.data[r * map.width + c].type = ((rand()%100) < TILE_PERCENTAGE) ? tile : TILE_EMPTY;
	}

	// Smooths the tiles
	for (int i = 0; i < 5; i++) {
		FOR_GRID_BORDER(r, c, 1, map) {
			allocated_map[r][c] = (radius_count(map, r, c, 1, tile) >= radius1) ||
				(radius_count(map, r, c, 2, tile) <= radius2) ? tile : TILE_EMPTY;
		}

		FOR_GRID_BORDER(r, c, 1, map) {
			map.data[r * map.width + c].type = allocated_map[r][c];
		}
	}

	// smooths the tiles again
	for (int i = 0; i < 5; i++) {
		FOR_GRID_BORDER(r, c, 1, map) {
			allocated_map[r][c] = (radius_count(map, r, c, 1, tile) >= 5) ? tile : TILE_EMPTY;
		}

		FOR_GRID_BORDER(r, c, 1, map) {
			map.data[r * map.width + c].type = allocated_map[r][c];
		}
	}

	for(unsigned i = 0; i < map.height; i++) {
		free(allocated_map[i]);
	}
	free(allocated_map);
}


/**
 * @brief Intersects two maps, creating a third map that represents the intersection.
 * Given two maps, map1 and map2, this function creates a new map, result,
 * representing the intersection of the two maps. The resulting map will have the same dimensions
 * as map1 and map2.
 * The intersection of the two maps is defined as follows:
 *     If both map1 and map2 have a water tile in the same position, the resulting tile in
 * result will be a water tile.
 *     If both map1 and map2 have an empty tile in the same position, the resulting tile in
 * result will be an empty tile.
 *     Otherwise, the resulting tile in result will be a wall tile.
 * @param map1 A pointer to the first map.
 * @param map2 A pointer to the second map.
 * @param result A pointer to the resulting map.
 * @note The resulting map must have the same dimensions as map1 and map2.
 */
void intersect_maps(map map1, map map2, map result) {

	// Intersect both maps
	FOR_GRID_BORDER(r, c, 1, map1) {
		result.data[r * result.width + c].type =
		(
			(map1.data[r * map1.width + c].type == TILE_WATER) &&
			(map2.data[r * map2.width + c].type == TILE_EMPTY)) ?
			TILE_WATER :
		(
			((map1.data[r * map1.width + c].type == TILE_EMPTY) &&
			(map2.data[r * map2.width + c].type == TILE_EMPTY)) ?
			TILE_EMPTY : TILE_WALL);
	}
}

/**
 * @brief Draw a wall border around the map
 * This function draws a wall border around the map by setting the corresponding tiles to the
 * TILE_WALL type.
 * @param map Struct containing the map
 */
void draw_border(map map) {

	// Draw a border around the map
	for (unsigned i = 0; i < map.width; i++) {
		map.data[i].type = TILE_WALL;
		map.data[(map.height - 1) * map.width + i].type = TILE_WALL;
	}
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
		} while (data->map.data[y * data->map.width + x].type == TILE_WATER ||
				 data->map.data[y * data->map.width + x].type == TILE_WALL);

		if (seed < 50) {
			data->entities.entities[i] = entity_create_rat(x, y, ENTITY_RAT_HEALTH);
		} else if (seed >= 50 && seed < 85) {
			data->entities.entities[i] = entity_create_goblin(x, y, ENTITY_GOBLIN_HEALTH);
		} else data->entities.entities[i] = entity_create_cristino(x, y, ENTITY_CRISTINO_HEALTH);
	}
}

/**
 * @brief Spawns the player entity and opens a safe starting area on the map.
 *
 * @param data Pointer to the main game state data.
 */
void player_spawn(state_main_game_data *data){

	// Set Player Data
	data->entities.entities[0] = entity_create_player(data->map.width / 2, data->map.height / 2, 1);

	// Open a safe place to start
	for (unsigned y = data->map.height / 2 - STARTER_CIRCLE; y < data->map.height; y++) {
        for (unsigned x = data->map.width / 2 - STARTER_CIRCLE; x < data->map.width; x++) {
            if ((x - data->entities.entities[0].x) * (x - data->entities.entities[0].x)
				+ (y - data->entities.entities[0].y) * (y - data->entities.entities[0].y)
				<= STARTER_CIRCLE * STARTER_CIRCLE) {
                data->map.data[y * data->map.width + x].type = TILE_EMPTY;
            }
        }
    }
}

void generate_map_random(state_main_game_data *data) {

	data->map = map_allocate(MAP_WIDTH, MAP_HEIGHT);

	data->entities = entity_set_allocate(ENTITY_COUNT);

	// Randomly generate water map
	generate_random(data->map, 6, 1, TILE_WATER);

	// Allocate wall map
	map new_map = map_allocate(MAP_WIDTH, MAP_HEIGHT);

	// Randomly generate wall map
	generate_random(new_map, 5, 2, TILE_WALL);

	// Intersect the two maps
	intersect_maps(data->map, new_map, data->map);

	free(new_map.data);

	// draw border
	draw_border(data->map);

	// Populate the map
	entity_spawn(data);

	// Place player on map
	player_spawn(data);
}

