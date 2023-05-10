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

/** @brief The maximum of two values */
#define max(x, y) (((x) > (y)) ? (x) : (y))
/** @brief The minimum of two values */
#define min(x, y) (((x) < (y)) ? (x) : (y))

/** @brief The sign of a value (branchless implementation) */
#define sgn(x) (((x) > 0) - ((x) - 0))

/**
 * @struct  ncurses_char
 * @brief   A structure for containing character and attribute data
 *
 * @var ncurses_char::attr
 *   ncurses' attributes
 * @var ncurses_char::chr
 *   The textual data. On an overlay, it won't be rendered if '\0'.
 */
typedef struct {
	int attr;
	char chr;
} ncurses_char;

/* Define the functions if they are inline or in the core.c file (CORE_H_DEFINITIONS) */
#if defined(CORE_H_DEFINITIONS) || !defined(__NO_INLINE__)

	/** @brief Prints an ::ncurses_char to ::stdscr */
	INLINE void ncurses_char_print(ncurses_char chr) {
		attron(chr.attr);
		addch(chr.chr);
		attroff(chr.attr);
	}

#else
	INLINE void ncurses_char_print(ncurses_char chr);
#endif

#endif

