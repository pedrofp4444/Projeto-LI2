/**
 * @file  menu_tools.h
 * @brief Tools for dealing with menu rendering and input handling
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

#ifndef MENU_TOOLS_H
#define MENU_TOOLS_H

/**
 * @brief Sets the new chosen button of a menu, given a key press, performing bounds checking
 * @param button_count The number of buttons
 * @param current      The number of the current button
 * @param advance      How many buttons to advance (e.g.: +1 means next button, -1 previous button)
 */
int menu_update_button(int button_count, int current, int advance);

/**
 * @brief Draws a box for use in menus
 *
 * ```
 * +----+
 * |    | <- Box aspect
 * +----+
 * ```
 *
 * @param x      The horizontal position of the top-left corner of the box
 * @param y      The vertical position of the top-left corner of the box
 * @param width  The width of the box (includes borders)
 * @param height The height of the box (includes borders)
 */
void menu_draw_box(int x, int y, int width, int height);

#endif

