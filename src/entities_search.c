/**
 * @file  entities_search.c
 * @brief The implementation of the search for the player 
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

#include <map.h>
#include <entities.h>
#include <game_states/main_game.h>
#include <entities_search.h>

#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

float distance_position(unsigned x1, unsigned y1, unsigned x2, unsigned y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float heuristic(unsigned x1, unsigned y1, unsigned x2, unsigned y2) {
  return distance_position(x1, y1, x2, y2);
}

int is_valid_position(map *map, unsigned x, unsigned y) {
  return (x < map->width && y < map->height &&
	        map->data[y * map->width + x].type == TILE_EMPTY);
}

float get_cost(map *map, position start, position end) {

	float cost = distance_position(start.x, start.y, end.x, end.y);
    if (!is_valid_position(map, end.x, end.y)) {
        cost += INFINITY;
    }
    return cost;
}

node *get_lowest_f_node(node **open, int n_open) {
  node *lowest_f_node = open[0];
  for (int i = 1; i < n_open; i++) {
    if (open[i]->f < lowest_f_node->f) {
      lowest_f_node = open[i];
    }
  }
  return lowest_f_node;
}

node *get_node_in_list(node **list, int n_list, position pos) {
  for (int i = 0; i < n_list; i++) {
    if (list[i]->pos.x == pos.x && list[i]->pos.y == pos.y) {
      return list[i];
    }
  }
  return NULL;
}

node *create_node(position pos, float f, float g, float h, node *parent) {
  node *new_node = malloc(sizeof(node));
  new_node->pos = pos;
  new_node->f = f;
  new_node->g = g;
  new_node->h = h;
  new_node->parent = parent;
  return new_node;
}

void node_destroy(node *node) {
  free(node);
}

void list_destroy(node **list, int num_list) {
  for (int i = 0; i < num_list; i++) {
    node_destroy(list[i]);
  }
}

position *calculate_path(node *end_node, int *path_length) {
  int length = 0;
  node *current_node = end_node;
  while (current_node != NULL) {
    length++;
    current_node = current_node->parent;
  }
  position *path = malloc(sizeof(position) * length);
  current_node = end_node;
  int i = length - 1;
  while (current_node != NULL) {
    path[i] = current_node->pos;
    i--;
    current_node = current_node->parent;
  }
  *path_length = length;
  return path;
}

position *search_path(map *map, position start, position end, int *path_length) {

  node *start_node = create_node(start, 0, 0, 0, NULL);
  node *end_node = create_node(end, 0, 0, 0, NULL);
  node **open = malloc(sizeof(node *));
  node **closed = malloc(sizeof(node *));
  int n_open = 0, n_closed = 0;
  open[n_open++] = start_node;

  while (n_open > 0) {
    node *current_node = get_lowest_f_node(open, n_open);
    if (current_node->pos.x == end_node->pos.x && current_node->pos.y == end_node->pos.y) {
      position *path = calculate_path(current_node, path_length);
      list_destroy(open, n_open);
      list_destroy(closed, n_closed);
      node_destroy(start_node);
      node_destroy(end_node);
      return path;
    }

    for (unsigned x = -1; x <= 1; x++) {
      for (unsigned y = -1; y <= 1; y++) {
        if (x == 0 && y == 0) continue;
        unsigned new_x = current_node->pos.x + x;
        unsigned new_y = current_node->pos.y + y;
        if (!is_valid_position(map, new_x, new_y)) continue;

        position new_pos = {new_x, new_y};
        node *new = get_node_in_list(closed, n_closed, new_pos);
        if (new != NULL) continue;

        float cost = get_cost(map, current_node->pos, new_pos);
        float g = current_node->g + cost;
        node *open_node = get_node_in_list(open, n_open, new_pos);
        if (open_node == NULL || g < open_node->g) {
          float h = heuristic(new_x, new_y, end_node->pos.x, end_node->pos.y);
          float f = g + h;
          node *new_node = create_node(new_pos, f, g, h, current_node);
          if (open_node != NULL) {
            node_destroy(open_node);
          } else {
            n_open++;
            open = realloc(open, sizeof(node *) * n_open);
          }
          open[n_open - 1] = new_node;
        }
      }
    }

    n_closed++;
    closed = realloc(closed, sizeof(node *) * n_closed);
    closed[n_closed - 1] = current_node;

    for (int i = 0; i < n_open; i++) {
      if (open[i] == current_node) {
        for (int j = i; j < n_open - 1; j++) {
          open[j] = open[j + 1];
        }
        n_open--;
        open = realloc(open, sizeof(node *) * n_open);
        break;
      }
    }
  }

  list_destroy(open, n_open);
  list_destroy(closed, n_closed);
  node_destroy(start_node);
  node_destroy(end_node);
  return NULL;
}

