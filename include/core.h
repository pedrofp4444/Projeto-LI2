/**
 * @file core.h
 * @brief Widely used general-purpose methods
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

#ifndef CORE_H
#define CORE_H

#include <ncurses.h>

#ifdef __NO_INLINE__
	#define INLINE
#else
	#define INLINE __attribute__((always_inline)) inline
#endif

/**
 * @brief The maximum of two values
 * @author A104348 Humberto Gomes
 */
#define max(x, y) (((x) > (y)) ? (x) : (y))
/**
 * @brief The minimum of two values
 * @author A104348 Humberto Gomes
 */
#define min(x, y) (((x) < (y)) ? (x) : (y))

/**
 * @brief The sign of a value (branchless implementation)
 * @author A104348 Humberto Gomes
 */
#define sgn(x) (((x) > 0) - ((x) < 0))

/**
 * @struct  ncurses_char
 * @brief   A structure for containing character and attribute data
 *
 * @var ncurses_char::attr
 *   ncurses' attributes
 * @var ncurses_char::chr
 *   The textual data. On an overlay, it won't be rendered if '\0'.
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	int attr;
	char chr;
} ncurses_char;

/**
 * @struct map_window
 * @brief The window of the map visible on screen and its screen placement information
 *
 * @var map_window::map_top
 *   The top coordinate of the map
 * @var map_window::map_left
 *   The left coordinate of the map
 * @var map_window::term_top
 *   The top coordinate of the terminal
 * @var map_window::term_left
 *   The left coordinate of the terminal
 * @var map_window::height
 *   The height of the map window (map and screen dimensions are the same)
 * @var map_window::width
 *   The width of the map window (map and screen dimensions are the same)
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	int map_top , map_left;
	int term_top, term_left;
	int height  , width;
} map_window;

/* Define the functions if they are inline or in the core.c file (CORE_H_DEFINITIONS) */
#if defined(CORE_H_DEFINITIONS) || !defined(__NO_INLINE__)

	/**
	 * @brief Prints an ::ncurses_char to ::stdscr
 	 * @author A104348 Humberto Gomes
	 */
	INLINE void ncurses_char_print(ncurses_char chr) {
		attron(chr.attr);
		addch(chr.chr);
		attroff(chr.attr);
	}

#else
	INLINE void ncurses_char_print(ncurses_char chr);
#endif

/**
 * @brief Calculates the Manhattan distance between two position
 *
 * @param x1 The X coordinate of the first position.
 * @param y1 The Y coordinate of the first position.
 * @param x2 The X coordinate of the second position.
 * @param y2 The Y coordinate of the second position.
 * @returns The Manhattan distance between the two positions.
 *
 * @author A90817 Mariana Rocha
 */
int manhattan_distance(int x1, int y1, int x2, int y2);

/**
 * @brief Checks if a point (in map coordinates) is inside the visible area
 * @author A104348 Humberto Gomes
 */
int map_window_visible(int x, int y, const map_window *wnd);

/**
 * @brief Converts map coordinates to screen coordinates in a window.
 * @author A104348 Humberto Gomes
 */
void map_window_to_screen(const map_window *wnd, int mapx, int mapy, int *screenx, int *screeny);

#endif

