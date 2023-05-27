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

#include <core.h>
#include <map.h>

/**
 * @brief Returns the rendering information for a tile type.
 *
 * The function uses a switch statement to determine the appropriate rendering information for the
 * specified tile type. If the `tile_type` parameter is not recognized, the function returns
 * a default `ncurses_char` struct with an empty space character.
 *
 * @param t The `tile_type` to get the rendering information
 * @param light The illumitation of the current tile
 * @return An `ncurses_char` struct which contains the rendering information for a tile type.
 *
 * @author A90817 Hélder Gomes
 * @author A104348 Humberto Gomes
 * @author A90817 Mariana Rocha
 * @author A104082 Pedro Pereira
 */
ncurses_char tile_get_render_info(tile_type t, int light) {
	ncurses_char ret;
	ret.attr = light ? A_NORMAL : A_DIM;

	switch (t) {
		case TILE_EMPTY:
			ret.chr = light ? '.' : ' ';
			ret.attr |= COLOR_PAIR(COLOR_WHITE) | A_DIM;
			break;
		case TILE_WALL:
			ret.chr = '#';
			ret.attr |= COLOR_PAIR(COLOR_WHITE);
			break;
		case TILE_WATER:
			ret.chr = '.';
			ret.attr |= COLOR_PAIR(COLOR_BLUE);
			break;
		default:
			/* Not supposed to happen */
			ret.chr = ' ';
			ret.attr |= COLOR_PAIR(COLOR_BLACK);
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

void map_zero(map m) {
	unsigned tile_count = m.width * m.height;
	for (unsigned i = 0; i < tile_count; ++i) {
		m.data[i].type = TILE_EMPTY;
		m.data[i].light = 0;
	}
}

void map_free(map map) {
	free(map.data);
}


void map_render(map map, const map_window *wnd) {

	for (int y = 0; y < wnd->height; ++y) {
		move(wnd->term_top + y, wnd->term_left);
		for (int x = 0; x < wnd->width; ++x) {

			unsigned mx = wnd->map_left + x, my = wnd->map_top + y;
			if (mx < map.width && my < map.height) {

				tile t = map.data[my * map.width + mx];
				ncurses_char_print(tile_get_render_info(t.type, t.light));
			} else {
				addch(' ');
			}
		}
	}
}

