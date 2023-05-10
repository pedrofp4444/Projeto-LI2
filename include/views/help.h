/**
 * @file help.h
 * @brief Instructions of the game
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

#ifndef HELP_H
#define HELP_H

#include <ncurses.h>

/**
 * @brief Draws the help menu on a specified window.
 *
 * This function draws the help menu on the given window, providing information and instructions
 * about the game and its controls.
 *
 * @param win The window to draw the help menu on.
 */
void draw_help(WINDOW* win);

/**
 * @brief Displays the help menu.
 *
 * This function initializes the necessary components, creates a window for the help menu.
 * It also handles the event when the user presses the Enter key to go back.
 * The help menu is drawn using ::draw_help().
 */
void help();

#endif

