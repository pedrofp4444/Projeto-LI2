/**
 * @file core.c
 * @brief Used to define core program methods if they aren't inline
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

#define CORE_H_DEFINITIONS /**< For method definitions if inlining is disabled */
#include <core.h>

int map_window_visible(int x, int y, const map_window *wnd) {
	return x >= wnd->map_left               &&
		 x <  wnd->map_left + wnd->width  &&
		 y >= wnd->map_top                &&
		 y <  wnd->map_top  + wnd->height;
}

void map_window_to_screen(const map_window *wnd, int mapx, int mapy, int *screenx, int *screeny) {
	*screeny = wnd->term_top  + (mapy - wnd->map_top);
	*screenx = wnd->term_left + (mapx - wnd->map_left);
}

