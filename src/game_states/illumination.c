/**
 * @file  illumination.c
 * @brief The implementation of illumination.
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

#include <core.h>
#include <map.h>

#include <stdlib.h>
#include <math.h>

/**
 * @brief Checks if there's line of sight between the player and a given position
 * @details This implementation checks for walls in all points that form the line beteween
 *          (@p x, @p) and (@p xp, @and yp) using Bresenham's algorithm
 *
 * @param x  The horizontal position of the player
 * @param y  The vertical position of the player
 * @param xp The horizontal position of the player
 * @param yp The vertical position of the player
 * @param m  The game map
 *
 * @note This function, even though large, is inlined because it is only called from a single place
 *       in the program (::state_main_game_circle_light_map).
 *
 * @author A90817 Hélder Gomes
 */
INLINE int illumination_check_line_of_sight(int x, int y, int xp, int yp, map m) {
	/* Check if there is a wall between the player and this position */
	int visible = 1;

	/* The distance between x and xp, y and yp */
	int dist_x = abs(xp - x);
	int dist_y = abs(yp - y);

	/* Choose if the line is gonna be drawn for the left or right */
	int sx = xp < x ? -1 : 1;
	int sy = yp < y ? -1 : 1;
	/* Choose if the line is gonna be drawn horizontally or vertically */
	int err = dist_x - dist_y;


	/* Bresenham's line algorithm */
	int current_x = x;
	int current_y = y;
	while (current_x != xp || current_y != yp) {
		if (m.data[current_y * m.width + current_x].type == TILE_WALL) {
			visible = 0;
			break;
		}

		int e2 = 2 * err;
		if (e2 > -dist_y) {
			err -= dist_y;
			current_x += sx;
		}

		if (e2 < dist_x) {
			err += dist_x;
			current_y += sy;
		}
	}

	return visible;
}

void state_main_game_circle_light_map(map m, int x, int y, int r) {
	int rsquared = r * r;
	for (int yp = y - r; yp <= y + r; ++yp) {
		int disty = (yp - y) * (yp - y);
		for (int xp = x - r; xp <= x + r; ++xp) {

			/* Only process tiles inside the map and within a given distance of the player */
			if (0 <= xp && xp < (int) m.width && 0 <= yp && yp < (int) m.height) {
				if ((xp - x) * (xp - x) + disty <= rsquared) {

					m.data[yp * m.width + xp].light = illumination_check_line_of_sight(
						x, y, xp, yp, m);
				}
			}
		}
	}
}

void state_main_game_circle_clean_light_map(map m, int x, int y, int r) {
	for (int yp = y - r; yp <= y + r; ++yp)
		for (int xp = x - r; xp <= x + r; ++xp)
			if (0 <= xp && xp < (int) m.width && 0 <= yp && yp < (int) m.height)
				m.data[yp * m.width + xp].light = 0;
}

