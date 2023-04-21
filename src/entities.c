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

entity_set entity_set_allocate(size_t count) {
	entity_set ret = {
		.entities = malloc(count * sizeof(entity)),
		.count = count
	};
	return ret;
}

void entity_set_free(entity_set entities) {
	free(entities.entities);
}

/**
 * @struct entity_type_render_info
 * @brief Stores rendering information related to rendering a particular entity type.
 * @var entity_type_render_info::chr
 *   Character used to represent the entity when rendered
*/
typedef struct {
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
			break;
		case ENTITY_RAT:
			ret.chr = 'R';
			break;
		case ENTITY_TROLL:
			ret.chr = 'T';
			break;
		case ENTITY_GOBLIN:
			ret.chr = 'G';
			break;
		case ENTITY_CRISTINO:
			ret.chr = 'M';
			break;
		default:
			ret.chr = 'X';
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
	addch(info.chr);
}

void entity_set_render(entity_set entity_set,
                       int map_top , int map_left,
                       int term_top, int term_left,
                       int height  , int width) {

	for (size_t i = 0; i < entity_set.count; ++i){
		entity ent = entity_set.entities[i];

		if (ent.health <= 0) continue; /* Skip invalid entities */

		if(ent.x >= map_left        &&
		   ent.x < map_left + width &&
		   ent.y >= map_top         &&
		   ent.y < map_top + height) {

			move(term_top + (ent.y - map_top), term_left + (ent.x - map_left));
			entity_render(ent);
		}
	}
}

