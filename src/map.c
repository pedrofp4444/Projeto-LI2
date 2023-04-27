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


/**
 * @struct tile_type_render_info
 * @brief Stores information related to rendering a particular tile type.
 * @var tile_type_render_info::color
 *   The ncurses' attribute to render the tile
 * @var tile_type_render_info::chr
 *   Character used to represent the tile when rendered
*/
typedef struct {
	int attr;
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
tile_type_render_info tile_get_render_info(tile_type t) {
	tile_type_render_info ret;

	switch (t) {
		case TILE_NOLIGHT:
			ret.chr = ' '; /* Don't draw unlit tiles (space) */
			ret.attr = COLOR_PAIR(COLOR_WHITE);
			break;
		case TILE_EMPTY:
			ret.chr = '.';
			ret.attr = COLOR_PAIR(COLOR_WHITE) | A_DIM;
			break;
		case TILE_WALL:
			ret.chr = '#';
			ret.attr = COLOR_PAIR(COLOR_WHITE);
			break;
		case TILE_WATER:
			ret.chr = '.';
			ret.attr = COLOR_PAIR(COLOR_BLUE);
			break;
		default:
			/* Not supposed to happen */
			ret.chr = ' ';
			ret.attr = COLOR_PAIR(COLOR_BLACK);
			break;
	}

	return ret;
}

map map_allocate(unsigned width, unsigned height) {
	map ret = {
		.width = width,
		.height = height,
		.data = malloc(width * height * sizeof(tile))
	};

	return ret;
}

void map_free(map map) {
	free(map.data);
}

/**
 * @brief Renders a tile to the terminal.
 *
 * This function renders the given tile to the terminal using the tile's information,
 * which is obtained by using ::tile_get_render_info().
 *
 * @param t The tile to render
*/
void tile_render(tile t) {
	tile_type_render_info info = tile_get_render_info(t.type);
	attron(info.attr);
	addch(info.chr);
	attroff(info.attr);
}

void map_render(map map,
                int map_top , int map_left,
                int term_top, int term_left,
                int height  , int width) {

	for (int y = 0; y < height; ++y) {
		move(term_top + y, term_left);
		for (int x = 0; x < width; ++x) {

			tile to_render = {
				.type = TILE_NOLIGHT
			};

			unsigned mx = map_left + x, my = map_top + y;
			if (mx < map.width && my < map.height && map.data[my * map.width + mx].light) {
				to_render = map.data[my * map.width + mx];
			}

			tile_render(to_render);
		}
	}
}

