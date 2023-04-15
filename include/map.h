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

typedef enum {
    TILE_EMPTY,
    TILE_WALL,
} tile_type;

typedef struct {
    tile_type type;
} tile;

typedef struct {
    char chr;
} tile_type_render_info;

tile_type_render_info tile_get_render_info(tile_type t);

/**
 * @var map::data
 *   To access coordinate (x, y), use `data[y * width + x]`
 */
typedef struct {
    unsigned width, height;
    tile *data;
} map;

/**
 * @brief Creates (and allocates memory for) a map
 * @returns On error, a map with a `NULL` `data` pointer.
 *
 * Map data will be memory trash.
 */
map map_allocate(unsigned width, unsigned height);

void map_render(map map,
                unsigned map_top, unsigned map_left,
                unsigned term_top, unsigned term_left,
                unsigned height, unsigned width);

#endif
