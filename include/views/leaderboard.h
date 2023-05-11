/**
 * @file leaderboard.h
 * @brief Leaderboard of the game
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

#include <ncurses.h>

/**
 * @struct Player
 * @brief Struct that represents a player.
 *
 * @var Player::name
 *   Player's name.
 * @var Player::score
 *   Player's score.
*/
typedef struct {
	char* name;
	int score;
} Player;

/**
 * @brief Draws the leaderboard on a specified window.
 *
 * This function draws the leaderboard on the given window using the provided player data.
 * The leaderboard includes the rank, name, and score of each player.
 *
 * @param win The window to draw the leaderboard on.
 */
void draw_leaderboard(WINDOW* win);

/**
 * @brief Displays the leaderboard and allows interaction with it.
 *
 * This function initializes the necessary components, creates a window for the
 * leaderboard, and allows the user to navigate through the leaderboard.
 * The leaderboard is drawn using ::draw_leaderboard().
 */
void leaderboard();

#endif

