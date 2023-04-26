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
#include <ncurses.h>
#include <time.h>
#include <generate_map.h>
#include <map.h>

/**
 * @def FOR_GRID_BORDER(row, col, border, data)
 * @brief Macro that defines a nested for loop to iterate over a 2D grid, ignoring the border.
 * This macro defines a nested for loop that iterates over the rows and columns of a 2D grid, starting from
 * the specified border and ending at the opposite border. It is useful when iterating over a grid with a
 * border that should be ignored.
 * @param row The name of the row variable used in the loop.
 * @param col The name of the column variable used in the loop.
 * @param border The number of border rows and columns to ignore.
 * @param data A pointer to the struct containing the map and other data.
*/
#define FOR_GRID_BORDER(row, col, border, data) \
	for (unsigned row = border; row < data->map.height - border; row++) \
		for (unsigned col = border; col < data->map.width - border; col++)

/**
 * @brief Counts the number of tiles of a certain type within a given radius around a given position.
 * @param data A pointer to the main game data.
 * @param row The row of the center position.
 * @param col The column of the center position.
 * @param radius The radius of the search.
 * @param tile The type of tile to be counted.
 * @return The number of tiles of the specified type found within the radius.
 * This function counts the number of tiles of a certain type within a given radius around a given position.
 * It takes a pointer to the main game data, the row and column of the center position, the radius of the search,
 * and the type of tile to be counted. It returns the number of tiles of the specified type found within the radius.
 * The function loops through all positions within the radius and checks whether the tile at that position is of the specified type.
 * If it is, it increments the count. The function returns the final count after all positions have been checked.
*/
int radius_count(state_main_game_data *data, unsigned row, unsigned col, unsigned radius, tile_type tile) {
	int count = 0;
	for (unsigned r = row - radius; r <= row + radius; r++) { // percorre linhas
		for (unsigned c = col - radius; c <= col + radius; c++) { // percorre colunas
			if (r < data->map.height && c < data->map.width &&
			data->map.data[r * data->map.width + c].type == tile)
			count++;
		}
	}
	return count;
}

/**
 * @brief Generates random walls and empty spaces in the map and smoothes the walls to make them look more natural
 * @param data Pointer to the main game state data
 * @param radius1 The radius used in the smoothing process for the walls
 * @param radius2 The radius used in the smoothing process for the walls
 * @param tile The type of tile that represents walls
 * @details The function generates random walls and empty spaces in the map by filling the cells of the grid with the tile type or the empty tile type based on a random number generated using the rand() function. Then, the walls are smoothed by replacing the wall tile type of each cell with the empty tile type if it has fewer than radius2 wall neighbors or more than radius1 wall neighbors within a given radius. This process is repeated twice.
 * @note The function uses the FOR_GRID_BORDER macro to iterate through the grid borders and avoid accessing out-of-bounds cells.
*/
void generate_random(state_main_game_data *data, int radius1, int radius2, tile_type tile) {
	srand(time(NULL));

	// Randomly generate the tile everywhere
	FOR_GRID_BORDER(r, c, 2, data) {
		data->map.data[r * data->map.width + c].type = ((rand()%100) < 45) ? tile : TILE_EMPTY;
	}

	// Smooths the tiles
	for (int i = 0; i < 5; i++) {
		tile_type mapa[data->map.height][data->map.width];
		FOR_GRID_BORDER(r, c, 1, data) {
			mapa[r][c] = (radius_count(data, r, c, 1, tile) >= radius1) ||
			(radius_count(data, r, c, 2, tile) <= radius2) ? tile : TILE_EMPTY;
		}
		FOR_GRID_BORDER(r, c, 1, data) {
			data->map.data[r * data->map.width + c].type = mapa[r][c];
		}
	}

	// smooths the tiles again
	for (int i = 0; i < 5; i++) {
		tile_type mapa[data->map.height][data->map.width];
		FOR_GRID_BORDER(r, c, 1, data) {
			mapa[r][c] = (radius_count(data, r, c, 1, tile) >= 5) ? tile : TILE_EMPTY;
		}
		FOR_GRID_BORDER(r, c, 1, data) {
			data->map.data[r * data->map.width + c].type = mapa[r][c];
		}
	}
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
void intersect_maps(state_main_game_data *map1,
					state_main_game_data *map2,
					state_main_game_data *result) {

	// Interseccionar os dois mapas
	FOR_GRID_BORDER(r, c, 1, map1) {
		result->map.data[r * result->map.width + c].type =
		(
			(map1->map.data[r * map1->map.width + c].type == TILE_WATER) &&
			(map2->map.data[r * map2->map.width + c].type == TILE_EMPTY)) ?
			TILE_WATER :
		(
			((map1->map.data[r * map1->map.width + c].type == TILE_EMPTY) &&
			(map2->map.data[r * map2->map.width + c].type == TILE_EMPTY)) ?
			TILE_EMPTY : TILE_WALL);
	}
}

/**
 * @brief Draw a wall border around the map
 * This function draws a wall border around the map by setting the corresponding tiles to the TILE_WALL type.
 * @param data Pointer to the main game data struct
*/
void draw_border(state_main_game_data *data) {
	// Desenha uma parede em toda a borda do mapa
	for (unsigned i = 0; i < data->map.width; i++) {
		data->map.data[i].type = TILE_WALL;
		data->map.data[(data->map.height - 1) * data->map.width + i].type = TILE_WALL;
	}
	for (unsigned i = 0; i < data->map.height; i++) {
		data->map.data[i * data->map.width].type = TILE_WALL;
		data->map.data[(i + 1) * data->map.width - 1].type = TILE_WALL;
	}
}

/**
 * @brief Spawn entities randomly on the game map.
 * This function randomly spawns entities on the game map. It generates a random position for each entity
 * and assigns a random type and health value. The entity positions are checked to ensure that they do not
 * overlap with walls or water tiles on the map.
 * @param data A pointer to the game data structure.
*/
void entity_spawn(state_main_game_data *data){

	for (int i = 1; i < 2573; ++i) {
		data->entities.entities[i].health = 1;
		data->entities.entities[i].type = rand() % 4 + 1;

		unsigned x, y;
		do {
			x = rand() % 1024;
			y = rand() % 1024;
		} while (data->map.data[y * data->map.width + x].type == TILE_WATER ||
				 data->map.data[y * data->map.width + x].type == TILE_WALL);

		data->entities.entities[i].x = x;
		data->entities.entities[i].y = y;
	}
}

void generate_map_random(state_main_game_data *data) {

	// Gerar mapa de água aleatoriamente
	generate_random(data, 6, 1, TILE_WATER);

	// Alocar mapa de paredes
	map m = map_allocate(1024, 1024);

	state_main_game_data wall_map = {

		.fps_show	 = 0, .fps_count	 = 0,
		.renders_show = 0, .renders_count = 0,
		.elapsed_fps = 0.0,

		.needs_rerender = 1,

		.map = m,
	};

	// Gerar mapa de parede aleatoriamente
	generate_random(&wall_map, 5, 2, TILE_WALL);

	// Intersecionar os dois mapas
	intersect_maps(data, &wall_map, data);

	free(m.data);

	// Desenhar borda
	draw_border(data);

	// Popular o mapa
	entity_spawn(data);
}

