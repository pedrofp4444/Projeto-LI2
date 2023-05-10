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

/**
 * @brief  Calculates the movement of an arrow for an attack
 * @return Will return an animation with 0 lenght if the movement is impossible
 */
animation_sequence combat_arrow_movement
	(const entity *attacker, const entity *attacked, const map *map) {

	animation_sequence ret = animation_sequence_create();

	/* Entities must be aligned horizontally or vertically with line of sight */
	if (!(attacker->x == attacked->x || attacker->y == attacked->y)) return ret;

	int dx = sgn(attacked->x - attacker->x), dy = sgn(attacked->y - attacker->y);

	animation_step pos = {
		.x = attacker->x, .y = attacker->y
	};

	/* Check for walls and unlit spots in the middle of the path */
	while (!(attacked->x == pos.x && attacked->y == pos.y)) {

		if (!(pos.x >= 0                    && pos.y >= 0 &&
		      (unsigned) pos.x < map->width && (unsigned) pos.y < map->height)) {

			ret.length = 0;
			return ret; /* Out of bounds arrow */
		}

		if (map->data[pos.y * map->width + pos.x].type == TILE_WALL ||
		    map->data[pos.y * map->width + pos.x].light == 0) {

			ret.length = 0;
			return ret; /* Wall in the middle of the path or unlit area */
		}

		pos.x += dx; pos.y += dy;
		animation_sequence_add_step(&ret, pos);
	}

	return ret;
}

int combat_can_attack(const entity *attacker, const entity *attacked, const map *map) {
	int dist = abs(attacker->x - attacked->x) + abs(attacker->y - attacked->y);

	switch (attacker->weapon) {
		case WEAPON_HAND:
		case WEAPON_LANTERN:
			return dist <= 2;
		case WEAPON_DAGGER:
			return dist <= 5;

		case WEAPON_ARROW: {
			animation_sequence seq = combat_arrow_movement(attacker, attacked, map);
			int ret = seq.length > 0;
			animation_sequence_free(seq);
			return ret;
		}

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

void combat_attack(entity *attacker, const entity *attacked, const map *map) {
	switch (attacker->weapon) {
		case WEAPON_ARROW: {
			combat_arrow_info *t = malloc(sizeof(combat_arrow_info));
			t->animation = combat_arrow_movement(attacker, attacked, map);
			attacker->combat_target = t;
		}
		break;

		case WEAPON_BOMB: {
			combat_bomb_info *t = malloc(sizeof(combat_bomb_info));
			t->x = attacked->x;
			t->y = attacked->y;
			attacker->combat_target = t;
		}
		break;

		case WEAPON_HAND:
		case WEAPON_DAGGER:
		case WEAPON_LANTERN:
		case WEAPON_IPAD:
		default:
			attacker->combat_target = (entity *) attacked;
			break;
	}
}

#define BOMB_EXPLOSION_LENGTH 4

/** @brief Deals random damage to @p target based on the strength of @w */
void combat_deal_damage(weapon w, entity *target) {
	if (target->health > 0) {
		switch (w) {
			case WEAPON_HAND:
			case WEAPON_LANTERN:
				target->health--;
				break;

			case WEAPON_DAGGER:
			case WEAPON_ARROW:
				target->health -= (rand() % 3) + 1; /* 1 <= damage <= 3 */
				break;

			case WEAPON_BOMB:
				target->health -= (rand() % 2) + 2; /* 2 <= damage <= 3 */
				break;

			case WEAPON_IPAD:
				target->health -= (rand() % 3) + 3; /* 3 <= damage <= 5 */
				break;

			default:
				break; /* Unknown weapon */
		}

		/* Check if the entity has been killed to destroy it */
		if (target->health <= 0) {
			animation_sequence_free(target->animation);
			entity_free_combat_target(target);

			if (target->destroy)
				target->destroy(target);
		}
	}
}

/** @brief Deals random damage to all entities in a location, based on the strength of @p w */
void combat_deal_damage_position(weapon w, entity_set entities, int x, int y) {
	for (size_t i = 0; i < entities.count; ++i)
		/* No need to check health >= 0, as combat_deal_damage does that */
		if (entities.entities[i].x == x && entities.entities[i].y == y)
			combat_deal_damage(w, &entities.entities[i]);
}

int combat_animation_update(entity_set all, entity_set entity_set, size_t step_index) {
	for (size_t i = 0; i < entity_set.count; ++i) {
		entity cur = entity_set.entities[i];

		/* Skip dead and inactive entities */
		if (cur.health <= 0 || cur.combat_target == NULL) continue;

		size_t length = 0;
		if (cur.weapon == WEAPON_ARROW) {
			animation_sequence anim = ((combat_arrow_info *) cur.combat_target)->animation;
			length = anim.length;

			/* Don't attack entities in the middle of the path */
			if (length != 0 && length - 1 == step_index) {
				animation_step last = anim.steps[length - 1];
				combat_deal_damage_position(cur.weapon, all, last.x, last.y);
			}

		} else if (cur.weapon == WEAPON_BOMB) {
			combat_bomb_info bomb = *((combat_bomb_info *) cur.combat_target);
			length = BOMB_EXPLOSION_LENGTH;

			if (step_index == length)
				for (int y = bomb.y - 1; y <= bomb.y + 1; ++y)
					for (int x = bomb.x - 1; x <= bomb.x + 1; ++x)
						combat_deal_damage_position(cur.weapon, all, x, y);

		} else {
			combat_deal_damage(cur.weapon, cur.combat_target);

		}

		if (length >= step_index)
			return 0;
	}
	return 1;
}

/* @brief If in-bounds, place a character in map coordinates in the overlay */
void combat_write_overlay_write(ncurses_char chr, int x, int y, ncurses_char *overlay,
                                       int map_top , int map_left,
                                       int height  , int width) {

	x -= map_left; y -= map_top;
	if (x >= 0 && y >= 0 && x < width && y < height)
		overlay[y * width + x] = chr;
}

void combat_entity_set_animate(entity_set entity_set, size_t step_index,
                               ncurses_char *overlay, const map_window *wnd) {

	for (size_t i = 0; i < entity_set.count; ++i) {
		entity cur = entity_set.entities[i];

		/* Skip dead and inactive entities */
		if (cur.health <= 0 || cur.combat_target == NULL) continue;

		/* Draw arrows and bombs (only visible animations) */
		if (cur.weapon == WEAPON_ARROW) {
			animation_sequence seq = ((combat_arrow_info *) cur.combat_target)->animation;

			/* Draw a slash in the position of the arrow */
			if (step_index < seq.length) {
				ncurses_char chr = { .attr = COLOR_PAIR(COLOR_WHITE), .chr = '/' };
				combat_write_overlay_write(chr,
					seq.steps[step_index].x, seq.steps[step_index].y, overlay,
					wnd->map_top, wnd->map_left, wnd->height, wnd->width);
			}
		} else if (cur.weapon == WEAPON_BOMB && step_index % 2 == 0) { /* mod 2 for blinking */
			combat_bomb_info bomb = * (combat_bomb_info *) cur.combat_target;

			/* Draw a red square on the bomb area */
			ncurses_char chr = { .attr = COLOR_PAIR(COLOR_RED), .chr = '@' };
			for (int y = bomb.y - 1; y <= bomb.y + 1; ++y)
				for (int x = bomb.x - 1; x <= bomb.x + 1; ++x)
					combat_write_overlay_write(chr, x, y, overlay,
						wnd->map_top, wnd->map_left, wnd->height, wnd->width);
		}
	}
}

