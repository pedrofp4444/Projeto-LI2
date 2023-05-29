/**
 * @file  menu_tools.c
 * @brief Tools for dealing with menu rendering and input handling
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

#include <ncurses.h>

int menu_update_button(int button_count, int current, int advance) {
	int next = current + advance;
	if (!(0 <= next && next < button_count))
		next = current; /* Out-of-bounds. Go to the previous button */

	return next;
}


/**
 * @brief Renders vertical lines for a menu rectangle.
 * @details
 *   - Also adds the `+` characters on the corners;
 *
 * @param x
 *   X coordinate of the line's topmost point
 * @param y
 *   Y coordinate of the line's topmost point
 * @param height
 *   The height of the line (including + corners)
 *
 * @author A104348 Humberto Gomes
 */
void menu_draw_vertical_line(int x, int y, int height) {
	for (int i = 0; i < height; ++i) {
		move(y + i, x);
		addch((i == 0 || i == height - 1) ? '+' : '|');
	}
}

/**
 * @brief Renders horizontal lines for the message box rectangle.
 * @details
 *  - The corners are not rendered, as they are handled by ::menu_draw_vertical_line
 *
 * @param x
 *   X coordinate of the line's leftmost point (corner, not rendered)
 * @param y
 *   Y coordinate of the line's leftmost point (corner, not rendered)
 * @param width
 *   The width of the line (includes corners)
 *
 * @author A104348 Humberto Gomes
 */
void menu_draw_horizontal_line(int x, int y, int width) {
	for (int i = 1; i < width - 1; ++i) { /* This loop bounds are for skipping the corners  */
		move(y, x + i);
		addch('-');
	}
}

void menu_draw_box(int x, int y, int width, int height) {
	menu_draw_vertical_line  (x, y, height);
	menu_draw_vertical_line  (x + width - 1, y, height);
	menu_draw_horizontal_line(x, y, width);
	menu_draw_horizontal_line(x, y + height - 1, width);
}

