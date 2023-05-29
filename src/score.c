/**
 * @file  score.h
 * @brief Methods for incrementing player score, saving it and loading it from disk
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <score.h>

int score_from_entity(entity_type t) {
	switch (t) {
		case ENTITY_RAT:
			return 10;
		case ENTITY_GOBLIN:
			return 20;
		case ENTITY_CRISTINO:
			return 100;
		default:
			return 0;
	}
}

/**
 * @brief Score list criterion for sorting
 * @author A104348 Humberto Gomes
 */
int score_list_criterion(const void *a, const void *b) {
	return ((const player_score *) b)->score - ((const player_score *) a)->score;
}

void score_list_load(score_list *out) {
	FILE *f = fopen(SCORE_FILE, "r");

	if (f == NULL) { /* Failed to open file. Empty score list */
		memset(out, 0, sizeof(score_list));
		return;
	}

	size_t read = fread(out, sizeof(score_list), 1, f);
	if (read == 0) { /* Failed to load data */
		memset(out, 0, sizeof(score_list));
		fclose(f);
		return;
	}

	/* Sort list before outputting */
	qsort(out->scores, SCORE_LIST_MAX, sizeof(player_score), score_list_criterion);
	fclose(f);
}

void score_list_save(const score_list *list) {
	FILE *f = fopen(SCORE_FILE, "w");

	if (f == NULL) return; /* Failed to open file */

	fwrite(list, sizeof(score_list), 1, f);
	fclose(f);
}

int score_list_can_insert(const score_list *list, int score) {
	return score > list->scores[SCORE_LIST_MAX - 1].score;
}

void score_list_insert(score_list *list, const player_score *score) {
	if (!score_list_can_insert(list, score->score)) return;

	int i;
	for (i = SCORE_LIST_MAX - 1; i >= 0 && list->scores[i].score > score->score; ++i) {
		list->scores[i] = list->scores[i - 1];
	}
	list->scores[i] = *score;
}

