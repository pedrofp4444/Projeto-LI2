/**
 * @file main_menu.h
 * @brief Main menu of the game
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

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <ncurses.h>

/**
 * @brief Draws the menu on a specified window.
 *
 * This function draws the menu on the given window, displaying a list of buttons with labels.
 *
 * @param win The window to draw the menu on.
 */
void draw_menu(WINDOW* win);

/**
 * @brief Displays the menu and allows interaction with it.
 *
 * This function initializes the necessary components, creates a window for the menu,
 * and allows the user to navigate through the menu.
 * It also handles the event when the user selects a menu option by pressing the Enter Key.
 * The menu is drawn using the using ::draw_menu().
 *
 * @return An integer value representing the selected menu option:
 *         - `1` for "New Game"
 *         - `2` for "Leave"
 *         - `0` otherwise
 */
int menu();

#endif

