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
 * @return Will return an empty animation if the movement is impossible (wall collision or no
 *         light), or the arrow animation in case of success
 *
 * @param attacker The entity that attacks @p attacked
 * @param attacked The entity attacked by @attacker
 * @param map      The map, for arrow-wall collision information
 */
animation_sequence combat_arrow_movement
	(const entity *attacker, const entity *attacked, const map *map) {

	animation_sequence ret = animation_sequence_create();

	/* Entities must be aligned horizontally or vertically with line of sight */
	if (!(attacker->x == attacked->x || attacker->y == attacked->y)) return ret;

	/* Movement vector of the arrow (each animation frame) */
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
	int dist = manhattan_distance(attacker->x, attacker->y, attacked->x, attacked->y);

	switch (attacker->weapon) {
		/* Simple range-based weapons */
		case WEAPON_HAND:
		case WEAPON_LANTERN:
			return dist <= 2;
		case WEAPON_DAGGER:
			return dist <= 5;
		case WEAPON_IPAD:
			return dist <= 10;

		/*
		 * Arrows: if the attacked entity is aligned with and in sight of the attacker
		 * (i.e., the animation sequence can be formed).
		 */
		case WEAPON_ARROW: {
			animation_sequence seq = combat_arrow_movement(attacker, attacked, map);
			int ret = seq.length > 0;
			animation_sequence_free(seq);
			return ret;
		}

		case WEAPON_BOMB:
			/* Bombs can only be thrown to lit map spots (in-bounds) */
			return attacked->x >= 0                   && attacked->y >= 0 &&
				(unsigned) attacked->x < map->width && (unsigned) attacked->y < map->width
				&& map->data[attacked->y * map->width + attacked->x].light;
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
void combat_deal_damage(weapon w, entity *target, entity_kill_callback onkill, void *cb_data) {
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
			if (onkill)
				onkill(target, cb_data);

			animation_sequence_free(target->animation);
			entity_free_combat_target(target);

			if (target->destroy)
				target->destroy(target);
		}
	}
}

/** @brief Deals random damage to all entities in a location, based on the strength of @p w */
void combat_deal_damage_position(weapon w, entity_set entities, int x, int y,
                                 entity_kill_callback onkill, void *cb_data) {

	for (size_t i = 0; i < entities.count; ++i)
		/* No need to check health >= 0, as combat_deal_damage does that */
		if (entities.entities[i].x == x && entities.entities[i].y == y)
			combat_deal_damage(w, &entities.entities[i], onkill, cb_data);
}

int combat_animation_update(entity_set all, entity_set entity_set, size_t step_index,
                            entity_kill_callback onkill, void *cb_data) {

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
				combat_deal_damage_position(cur.weapon, all, last.x, last.y,
					onkill, cb_data);
			}

		} else if (cur.weapon == WEAPON_BOMB) {
			combat_bomb_info bomb = *((combat_bomb_info *) cur.combat_target);
			length = BOMB_EXPLOSION_LENGTH;

			if (step_index == length)
				for (int y = bomb.y - 1; y <= bomb.y + 1; ++y)
					for (int x = bomb.x - 1; x <= bomb.x + 1; ++x)
						combat_deal_damage_position(cur.weapon, all, x, y,
							onkill, cb_data);

		} else {
			combat_deal_damage(cur.weapon, cur.combat_target, onkill, cb_data);

		}

		if (length >= step_index)
			return 0;
	}
	return 1;
}

/* @brief If in-bounds, place a character in map coordinates in the overlay */
void combat_overlay_write(ncurses_char chr, int x, int y, ncurses_char *overlay,
                          const map_window *wnd) {

	if (map_window_visible(x, y, wnd)) {
		overlay[(y - wnd->map_top) * wnd->width + (x - wnd->map_left)] = chr;
	}
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
				combat_overlay_write(chr, seq.steps[step_index].x, seq.steps[step_index].y,
					overlay, wnd);
			}

		} else if (cur.weapon == WEAPON_BOMB && step_index % 2 == 0) { /* mod 2 for blinking */
			combat_bomb_info bomb = * (combat_bomb_info *) cur.combat_target;

			/* Draw a red square on the bomb area */
			ncurses_char chr = { .attr = COLOR_PAIR(COLOR_RED), .chr = '@' };
			for (int y = bomb.y - 1; y <= bomb.y + 1; ++y)
				for (int x = bomb.x - 1; x <= bomb.x + 1; ++x)
					combat_overlay_write(chr, x, y, overlay, wnd);
		}
	}
}

