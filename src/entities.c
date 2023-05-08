/**
 * @file entities.c
 * @brief Entities implementation
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
#include <entities.h>

const char *entity_get_name(entity_type t) {
	switch (t) {
		case ENTITY_PLAYER:
			return "Player";
		case ENTITY_RAT:
			return "Rat";
		case ENTITY_GOBLIN:
			return "Goblin";
		case ENTITY_CRISTINO:
			return "Cristino";
		default:
			/* Not supposed to happen */
			return "Unknown";
	}
}

void entity_free_combat_target(entity *ent) {
	if (ent->combat_target) {
		if (ent->weapon == WEAPON_ARROW) {
			animation_sequence_free(((combat_arrow_info *)ent->combat_target)->animation);
			free(ent->combat_target);
		} else if (ent->weapon == WEAPON_BOMB) {
			free(ent->combat_target);
		}
	}
	ent->combat_target = NULL;
}

entity_set entity_set_allocate(size_t count) {
	entity_set ret = {
		.entities = malloc(count * sizeof(entity)),
		.count = count
	};
	return ret;
}

void entity_set_free(entity_set entities) {
	for (size_t i = 0; i < entities.count; ++i) {
		if (entities.entities[i].health <= 0) continue; /* Skip invalid entities */

		animation_sequence_free(entities.entities[i].animation);
		entity_free_combat_target(&entities.entities[i]);

		if (entities.entities[i].destroy)
			entities.entities[i].destroy(&entities.entities[i]);
	}

	free(entities.entities);
}

/**
 * @brief Inserts an entity in an ordered entity list
 * @details Auxiliary function for ::entity_get_closeby
 * @param ent          The entity to be inserted
 * @param dist         The distance of @p ent to the reference entity
 * @param chg          The entity list to be changed
 * @param dists        The list of distances of each entity to the reference entity
 * @param count        The current number of elements
 * @param can_increase The size of the list can be increased
 */
void entity_insert(entity ent, int dist, entity *chg, int *dists, size_t count, int can_increase) {
	if (can_increase) {
		/* Regular insertion */
		int i;
		for (i = count - 1; i >= 0 && dist < dists[i]; --i) {
			chg[i + 1] = chg[i]; dists[i + 1] = dists[i];
		}
		chg [i + 1] = ent; dists[i + 1] = dist;
	} else {
		/* Find insertion position */
		size_t pos = count;
		for (int i = count - 1; i >= 0 && dist < dists[i]; --i)
			pos = i;

		if (pos < count) {
			/* Discard last element, moving others forward */
			for (size_t i = count - 1; i > pos; --i) {
				chg[i] = chg[i - 1]; dists[i] = dists[i - 1];
			}

			/* Add current entity */
			chg[pos] = ent; dists[pos] = dist;
		}
	}
}

entity_set entity_get_closeby(entity ent, entity_set in, size_t max_count, const map *map) {
	entity *out = malloc(max_count * sizeof(entity));
	/* Distances of each outputted entity to the player (for sorting purposes) */
	int *dists = malloc(max_count * sizeof(int));
	size_t out_count = 0;

	for (size_t i = 0; i < in.count; ++i) {
		entity cur = in.entities[i];

		if (cur.health <= 0) continue;

		if (map) {
			/* Ignore out-of-bounds entities */
			if (cur.x < 0                      || cur.y < 0                        ||
			    (unsigned) cur.x >= map->width || (unsigned) cur.y >= map->height) continue;

			/* Ignore unlit entities */
			if (map->data[cur.y * map->width + cur.x].light == 0) continue;
		}

		int dist = abs(ent.x - cur.x) + abs(ent.y - cur.y);

		/* Insert the entity on the output list. */
		if (out_count < max_count) {
			entity_insert(cur, dist, out, dists, out_count, 1);
			out_count++;
		} else {
			entity_insert(cur, dist, out, dists, out_count, 0);
		}
	}

	entity_set ret = {
		.entities = out, .count = out_count
	};
	free(dists);
	return ret;
}

/**
 * @struct entity_type_render_info
 * @brief Stores rendering information related to rendering a particular entity type.
 * @var entity_type_render_info::attributes
 *   Ncurses' attributes to render the entity
 * @var entity_type_render_info::chr
 *   Character used to represent the entity when rendered
*/
typedef struct {
	int attributes;
	char chr;
} entity_type_render_info;

/**
 * @brief Returns the rendering information for a entity type.
 *
 * Determines the appropriate rendering information for the specified entity type.
 *
 * @param t The `entity_type` to get the rendering information
 * @return A `entity_type_render_info` struct which contains the rendering information for a
 * entity type.
*/
entity_type_render_info entity_get_render_info(entity_type t) {
	entity_type_render_info ret;

	switch (t) {
		case ENTITY_PLAYER:
			ret.chr = 'O';
			ret.attributes = COLOR_PAIR(COLOR_WHITE) | A_BOLD;
			break;
		case ENTITY_RAT:
			ret.chr = 'R';
			ret.attributes = COLOR_PAIR(COLOR_BLUE) | A_BOLD;
			break;
		case ENTITY_GOBLIN:
			ret.chr = 'G';
			ret.attributes = COLOR_PAIR(COLOR_GREEN) | A_BOLD;
			break;
		case ENTITY_CRISTINO:
			ret.chr = 'M';
			ret.attributes = COLOR_PAIR(COLOR_MAGENTA) | A_BOLD;
			break;
		default:
			/* Not supposed to happen */
			ret.chr = 'X';
			ret.attributes = COLOR_PAIR(COLOR_RED) | A_BOLD;
			break;
	}

	return ret;
}

/**
 * @brief Renders a entity on the terminal.
 *
 * This function renders the given entity to the terminal using the entity's information,
 * which is obtained by using ::entity_get_render_info().
 *
 * @param t The entity to render
*/
void entity_render(entity t) {
	entity_type_render_info info = entity_get_render_info(t.type);

	attron(info.attributes);
	addch(info.chr);
	attrset(A_NORMAL);
}

void entity_set_render(entity_set entity_set, map map,
                       int map_top , int map_left,
                       int term_top, int term_left,
                       int height  , int width) {

	for (size_t i = 0; i < entity_set.count; ++i){
		entity ent = entity_set.entities[i];

		if (ent.health <= 0) continue; /* Skip invalid entities */

		if(ent.x >= map_left        &&
		   ent.x < map_left + width &&
		   ent.y >= map_top         &&
		   ent.y < map_top + height &&
		   map.data[ent.y * map.width + ent.x].light) {

			move(term_top + (ent.y - map_top), term_left + (ent.x - map_left));
			entity_render(ent);
		}
	}
}

/** @brief Animates an entity */
void entity_animate(entity *e, size_t step_index) {
	if (step_index < e->animation.length) {
		e->x = e->animation.steps[step_index].x;
		e->y = e->animation.steps[step_index].y;
	}
}

int entity_set_animate(entity_set entity_set, size_t step_index) {
	int stop = 1; /* Return value, whether all animations are finished */

	for (size_t i = 0; i < entity_set.count; ++i) {

		entity *ent = &entity_set.entities[i];

		if (ent->health <= 0) continue; /* Skip invalid entities */

		entity_animate(ent, step_index);

		if (step_index + 1 < ent->animation.length) { /* Unfinished animation */
			stop = 0;
		}
	}

	return stop;
}

