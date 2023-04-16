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

typedef struct {
    char chr;
} entity_type_render_info;

entity_type_render_info entity_get_render_info(entity_type t) {
    entity_type_render_info ret;

    switch (t) {
      case ENTITY_PLAYER:
          ret.chr = 'O';
          break;
      case ENTITY_RAT:
          ret.chr = 'R';
          break;
      default:
          ret.chr = 'X';
          break;
    }

    return ret;
}

void entity_render(entity t) {
    entity_type_render_info info = entity_get_render_info(t.type);
    addch(info.chr);
}

void entity_set_render(entity_set entity_set,
                        int map_top , int map_left,
                        int term_top, int term_left,
                        int height  , int width) {

    while(entity_set){

        if(entity_set->ent.x >= map_left &&
            entity_set->ent.x < map_left + width &&
            entity_set->ent.y >= map_top &&
            entity_set->ent.y < map_top + height) {

            move(term_top + (entity_set->ent.y - map_top), term_left + (entity_set->ent.x - map_left));

            entity_render(entity_set->ent);
        }

        entity_set = entity_set->next;
    }
}

