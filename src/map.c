/**
 * @file map.c
 * @brief Game map implementation
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

#include <stdlib.h>
#include <ncurses.h>
#include <map.h>

tile_type_render_info tile_get_render_info(tile_type t) {
    tile_type_render_info ret;

    switch (t)
    {
    case TILE_EMPTY:
        ret.chr = ' ';
        break;
    case TILE_WALL:
        ret.chr = '#';
        break;
    default:
        ret.chr = ' ';
        break;
    }

    return ret;
}

map map_allocate(unsigned width, unsigned height) {
    map ret = {
        .width = width,
        .height = height,
        .data = malloc(width * height * sizeof(tile))};

    return ret;
}

void tile_render(tile t) {
    tile_type_render_info info = tile_get_render_info(t.type);
    addch(info.chr);
}

void map_render(map map,
                unsigned map_top, unsigned map_left,
                unsigned term_top, unsigned term_left,
                unsigned height, unsigned width) {

    for (unsigned y = 0; y < height; ++y) {
        move(term_top + y, term_left);
        for (unsigned x = 0; x < width; ++x) {

            tile to_render = {
                .type = TILE_EMPTY};

            unsigned mx = map_left + x, my = map_top + y;
            if (mx < map.width && my < map.height) {
                to_render = map.data[my * map.width + mx];
            }

            tile_render(to_render);
        }
    }
}
