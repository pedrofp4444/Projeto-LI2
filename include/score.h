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

#ifndef SCORE_H
#define SCORE_H

#include <entities.h>

#define SCORE_LIST_MAX 5  /**< @brief The maximmum number of top players */
#define SCORE_NAME_MAX 32 /**< @brief The maximum number of characters in a name */

#define SCORE_FILE ".leaderboard" /**< @brief The file where the scores are stored */

/**
 * @brief The score of a player
 * @author A104348 Humberto Gomes
 */
typedef struct {
	char name[SCORE_NAME_MAX + 1];
	int score;
} player_score;

/**
 * @struct score_list
 * @brief List of scores of top players, ordered from highest from to lowest
 * @note This structure is gigantic, as it's all stack allocated. Keep that in mind if moving it
 *       around.
 *
 * @var score_list::names  The names of the players
 * @var score_list::scores The score of each player
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	player_score scores[SCORE_LIST_MAX];
} score_list;

/** @brief Returns the score a player shoud gain from killing an entity of a given type */
int score_from_entity(entity_type t);

/**
 * @brief Loads a score list from the disk. Creates a new one if the file doesn't exist.
 * @note The outputted file will be machine and compiler dependent (due to time constraints, we
 *       were unable to define a machine-independent format).
 *
 * @author A104348 Humberto Gomes
 */
void score_list_load(score_list *out);

/**
 * @brief Saves a score list to the disk.
 * @note The outputted file will be machine and compiler dependent (due to time constraints, we
 *       were unable to define a machine-independent format).
 * @details This method can fail without warning, as saving player scores isn't crucial.
 *
 * @author A104348 Humberto Gomes
 */
void score_list_save(const score_list *list);

/**
 * @brief Checks if a score can be inserted in a list (higher than the lowers score)
 * @author A104348 Humberto Gomes
 */
int score_list_can_insert(const score_list *list, int score);

/**
 * @brief Inserts a score into a list
 * @author A104348 Humberto Gomes
 */
void score_list_insert(score_list *list, const player_score *score);

#endif

