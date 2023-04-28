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

