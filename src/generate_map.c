/**
 * @file generate_map.c
 * @brief Game map generation implementation
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

#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <generate_map.h>
#include <map.h>

/*
Macro que itera sobre o mapa
border é a largura da borda
*/
#define FOR_GRID_BORDER(row, col, border, data) \
	for (unsigned row = border; row < data->map.height - border; row++) \
		for (unsigned col = border; col < data->map.width - border; col++)

/*
Calculámos o número de paredes dentro de um determinado raio a partir de uma dada posição, aka, (row, col).
Em cada iteração, verificámos se cada ponto está dentro dos limites do mapa
(r >= 0 && r < data->map.height && c >= 0 && c < data->map.width) e se for uma parede, count++.
*/
int radius_count(state_main_game_data *data, unsigned row, unsigned col, unsigned radius, tile_type tile) {
	int count = 0;
	for (unsigned r = row - radius; r <= row + radius; r++) { // percorre linhas
		for (unsigned c = col - radius; c <= col + radius; c++) { // percorre colunas
			if (r < data->map.height && c < data->map.width &&
			data->map.data[r * data->map.width + c].type == tile)
			count++;
		}
	}
	return count;
}

void generate_random(state_main_game_data *data, int radius1, int radius2, tile_type tile) {
	srand(time(NULL));

	// Gerar paredes e vazio aleatoriamente
	FOR_GRID_BORDER(r, c, 2, data) {
		data->map.data[r * data->map.width + c].type = ((rand()%100) < 45) ? tile : TILE_EMPTY;
	}

	// Suaviza as paredes
	for (int i = 0; i < 5; i++) {
		tile_type mapa[data->map.height][data->map.width];
		FOR_GRID_BORDER(r, c, 1, data) {
			mapa[r][c] = (radius_count(data, r, c, 1, tile) >= radius1) ||
			(radius_count(data, r, c, 2, tile) <= radius2) ? tile : TILE_EMPTY;
		}
		FOR_GRID_BORDER(r, c, 1, data) {
			data->map.data[r * data->map.width + c].type = mapa[r][c];
		}
	}

	// Suaviza as paredes novamente
	for (int i = 0; i < 5; i++) {
		tile_type mapa[data->map.height][data->map.width];
		FOR_GRID_BORDER(r, c, 1, data) {
			mapa[r][c] = (radius_count(data, r, c, 1, tile) >= 5) ? tile : TILE_EMPTY;
		}
		FOR_GRID_BORDER(r, c, 1, data) {
			data->map.data[r * data->map.width + c].type = mapa[r][c];
		}
	}
}

void intersect_maps(state_main_game_data *map1,
					state_main_game_data *map2,
					state_main_game_data *result) {

	// Interseccionar os dois mapas
	FOR_GRID_BORDER(r, c, 1, map1) {
		result->map.data[r * result->map.width + c].type =
		(
			(map1->map.data[r * map1->map.width + c].type == TILE_WATER) &&
			(map2->map.data[r * map2->map.width + c].type == TILE_EMPTY)) ?
			TILE_WATER :
		(
			((map1->map.data[r * map1->map.width + c].type == TILE_EMPTY) &&
			(map2->map.data[r * map2->map.width + c].type == TILE_EMPTY)) ?
			TILE_EMPTY : TILE_WALL);
	}
}

void draw_border(state_main_game_data *data) {
	// Desenha uma parede em toda a borda do mapa
	for (unsigned i = 0; i < data->map.width; i++) {
		data->map.data[i].type = TILE_WALL;
		data->map.data[(data->map.height - 1) * data->map.width + i].type = TILE_WALL;
	}
	for (unsigned i = 0; i < data->map.height; i++) {
		data->map.data[i * data->map.width].type = TILE_WALL;
		data->map.data[(i + 1) * data->map.width - 1].type = TILE_WALL;
	}
}

void entity_spawn(state_main_game_data *data){

	for (int i = 1; i < 2573; ++i) {
		data->entities.entities[i].health = 1;
		data->entities.entities[i].type = rand() % 4 + 1;

		unsigned x, y;
		do {
			x = rand() % 1024;
			y = rand() % 1024;
		} while (data->map.data[y * data->map.width + x].type == TILE_WATER ||
				 data->map.data[y * data->map.width + x].type == TILE_WALL);

		data->entities.entities[i].x = x;
		data->entities.entities[i].y = y;
	}
}

void generate_map_random(state_main_game_data *data) {

	// Gerar mapa de água aleatoriamente
	generate_random(data, 6, 1, TILE_WATER);

	// Alocar mapa de paredes
	map m = map_allocate(1024, 1024);

	state_main_game_data wall_map = {

		.fps_show	 = 0, .fps_count	 = 0,
		.renders_show = 0, .renders_count = 0,
		.elapsed_fps = 0.0,

		.needs_rerender = 1,

		.map = m,
	};

	// Gerar mapa de parede aleatoriamente
	generate_random(&wall_map, 5, 2, TILE_WALL);

	// Intersecionar os dois mapas
	intersect_maps(data, &wall_map, data);

	free(m.data);

	// Desenhar borda
	draw_border(data);

	// Popular o mapa
	entity_spawn(data);
}

