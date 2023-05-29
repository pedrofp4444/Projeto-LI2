/**
 * @file  leaderboard.h
 * @brief The game state to show the leaderboard to the player
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

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <score.h>

/**
 * @struct state_leaderboard_data
 * @brief Data for the leaderboard to work
 *
 * @var state_leaderboard_data::scores Player scores
 * @var state_leaderboard_data::needs_rerender If the leaderboard needs to be drawn
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	score_list scores;
	int needs_rerender;
} state_leaderboard_data;

/**
 * @brief Creates the leaderboard
 * @author A104348 Humberto Gomes
 */
game_state state_leaderboard_create(void);

/**
 * @brief Destroys the leaderboard state (frees `state->data`)
 * @author A104348 Humberto Gomes
 */
void state_leaderboard_destroy(game_state *state);

#endif

