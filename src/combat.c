/**
 * @file combat.c
 * @brief Combat system implementation
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
#include <combat.h>

const char *weapon_get_name(weapon w) {
	switch (w) {
		case WEAPON_HAND:
			return "Hand";
		case WEAPON_DAGGER:
			return "Dagger";
		case WEAPON_ARROW:
			return "Arrows";
		case WEAPON_BOMB:
			return "Bomb";
		case WEAPON_LANTERN:
			return "Lantern";
		case WEAPON_IPAD:
			return "iPad";
		default:
			return "Unknown";
	}
}

/** @brief The sign of a number (branchless implementation) */
#define sgn(x) (((x) > 0) - ((x) < 0))

int combat_can_attack(const entity *attacker, const entity *attacked, const map *map) {
	int dist = abs(attacker->x - attacked->x) + abs(attacker->y - attacked->y);

	switch (attacker->weapon) {
		case WEAPON_HAND:
		case WEAPON_LANTERN:
			return dist <= 2;
		case WEAPON_DAGGER:
			return dist <= 5;

		case WEAPON_ARROW: {
			/* Entities must be aligned horizontally or vertically with line of sight */
			if (!(attacker->x == attacked->x || attacker->y == attacked->y)) return 0;

			int dx = sgn(attacked->x - attacker->x), dy = sgn(attacked->y - attacker->y);
			int posx = attacker->x, posy = attacker->y;

			/* Check for walls in the middle of the path */
			while (!(attacked->x == posx && attacked->y == posy)) {

				if (!(posx >= 0                    && posy >= 0 &&
				      (unsigned) posx < map->width && (unsigned) posy < map->height))
					return 0; /* Out of bounds arrow */

				if (map->data[posy * map->width + posx].type == TILE_WALL)
					return 0; /* Wall in the middle of the path */

				posx += dx; posy += dy;
			}
			return 1;
		}
		break;

		case WEAPON_BOMB:
			/* Bombs can only be thrown to lit map spots (in-bounds) */
			if (attacked->x >= 0                    && attacked->y >= 0 &&
			    (unsigned) attacked->x < map->width && (unsigned) attacked->y < map->width)
				return map->data[attacked->y * map->width + attacked->x].light;
			return 0;

		case WEAPON_IPAD:
			return dist <= 10;
		default:
			/* Unknown weapon can't attack */
			return 0;
	}
}

