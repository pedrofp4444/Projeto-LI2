/**
 * @file map.h
 * @brief Game Map
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

#ifndef MAP_H
#define MAP_H

#include <core.h>

/**
 * @brief Enumerates the types of the tiles that can exist in the game.
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
typedef enum {
	TILE_EMPTY, /**< An empty space */
	TILE_WALL,  /**< A wall tile */
	TILE_WATER, /**< Water puddle */
} tile_type;

/**
 * @struct tile
 * @brief Represents a single tile in the game.
 * @var tile::type
 *   The type of the tile
 * @var tile::light
 *   If the tile is lit up
 *
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
typedef struct {
	tile_type type;
	int light;
} tile;

/**
 * @struct map
 * @brief A pointer for the first element of the tile data array.
 * @var map::width
 *   Width of the map in tiles
 * @var map::height
 *   Height of the map in tiles
 * @var map::data
 *   The `data` variable is a pointer to the first element of the tile data array,
 *   which is dynamically alocated and stores the tile data for the map.
 *   To access coordinate (x, y), use the expression `data[y * width + x]`.
 *   The array size is `width * height`.
 *
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
typedef struct {
	unsigned width;
	unsigned height;
	tile *data;
} map;

/**
 * @brief Creates (and allocates memory for) a map
 *
 * This function dynamically allocates memory for a map with a with and height, and
 * returns a `map` struct that contains the width, height and a pointer to the tile data.
 * Map data will be memory trash.
 *
 * @param width The width of the map in the tiles
 * @param height The height of the map in the tiles
 * @returns On error, a map with a `NULL` `data` pointer.
 *          Otherwise, returns a `map` with allocated memory.
 *
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
map map_allocate(unsigned width, unsigned height);

/*
 * @brief Initializes map data to an empty unlit map
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
void map_zero(map m);

/*
 * @brief Frees memory allocated in ::map_allocate for @p map
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
void map_free(map map);

/**
 * @brief Renders a portion of a map to the terminal
 *
 * This function renders a portion of a map provided in the terminal, starting from the
 * top left corner specified from map and terminal. The portion of the map to be rendered is
 * specified by a given width and height. If any out-of-bounds tiles need to be rendered,
 * then the function will render empty tiles.
 *
 * @param map The map to render
 * @param wnd The window of the map to be rendered
 *
 * @author A104100 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
void map_render(map map, const map_window *wnd);

#endif

