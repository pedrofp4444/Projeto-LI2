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

/**
 * @brief Enumerates the types of the tiles that can exist in the game.
*/

typedef enum {
    TILE_EMPTY, /**< An empty space */
    TILE_WALL,  /**< A wall tile */
} tile_type;

/**
 * @struct tile
 * @brief Represents a single tile in the game.
 * @var tile::type
 *   The type of the tile
*/
typedef struct {
    tile_type type;
} tile;

/**
 * @struct tile_type_render_info
 * @brief Stores information related to rendering a particular tile type.
 * @var tile_type_render_info::chr
 *   Character used to represent the tile when rendered
*/
typedef struct {
    char chr;
} tile_type_render_info;

/**
 * @brief Returns the rendering information for a tile type.
 * 
 * The function uses a switch statement to determine the appropriate rendering information for the 
 * specified tile type. If the `tile_type` parameter is not recognized, the function returns 
 * a default `tile_type_render_info` struct with an empty space character.
 * 
 * @param t The `tile_type` to get the rendering information
 * @return A `tile_type_render_info` struct which contains the rendering information for a 
 * tile type.
*/
tile_type_render_info tile_get_render_info(tile_type t);

/**
 * @struct map
 * @brief A pointer for the first element of the tile data array.
 * @var map::data
 * 
 * @var map::width
 *   Width of the map in the tiles
 * @var map::height
 *   Height of the map in the tiles
 * @var map::data
 *   The `data` variable is a pointer to the first element of the tile data array,
 *   which is dynamically alocated and stores the tile data for the map.
 *   To access coordinate (x, y), use the expression `data[y * width + x]`.
 *   The array size is `with*height` and each element is of the type `tile`.
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
 * Otherwise, returns a `map` with allocated memory.
 */
map map_allocate(unsigned width, unsigned height);

/**
 * @brief Renders a portion of a map to the terminal
 * 
 * This function renders a portion of a map provided in the terminal, starting from the
 * top left corner specified from map and terminal. The portion of the map to be rendered is
 * specified by a given width and height. In case of the top left corner of the map is outside
 * of the map boundaries, then the function will render empty tiles.
 * 
 * @param map The map to render
 * @param map_top The top coordinate of the map to be rendered
 * @param map_left The left coordinate of the map to be rendered
 * @param term_top The top coordinate of the terminal where the map will be rendered
 * @param term_left The left coordinate of the terminal where the map will be rendered
 * @param height The height of the map and the parts of the terminal to render
 * @param width The width of the map and the parts of the terminal to render
*/
void map_render(map map,
                unsigned map_top, unsigned map_left,
                unsigned term_top, unsigned term_left,
                unsigned height, unsigned width);

#endif
