/**
 * @file  msg_box.h
 * @brief A game state for displaying text messages
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

#ifndef MSG_BOX_H
#define MSG_BOX_H

#include <game_state.h>

/**
 * @brief Callback for exiting the message box.
 *
 * @param state
 *   The parent game state
 * @param chosen_button
 *   The button chosen by the user
 *
 * @author A104348 Humberto Gomes
 */
typedef void(*state_msg_box_exit_callback)(void *state, int chosen_button);

/**
 * @struct state_msg_box_data
 * @brief Data for a message box
 *
 * @var state_msg_box_data::message
 *   Text message to be shown
 *
 * @var state_msg_box_data::buttons
 *   Text of the message box buttons
 * @var state_msg_box_data::button_count
 *   Number of buttons
 * @var state_msg_box_data::chosen_button
 *   Index of the button chosen by the user
 *
 * @var state_msg_box_data::needs_rerender
 *   If an update happened, requiring a screen pass.
 *
 * @var state_msg_box_data::parent
 *   The parent game state. Will be switched to when the user clicks a button.
 * @var state_msg_box_data::exit_callback
 *   See ::state_msg_box_exit_callback. Can be `NULL` for no callback.a
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	char *message;

	char **buttons;
	int button_count;
	int chosen_button;

	int needs_rerender;

	game_state parent;
	state_msg_box_exit_callback exit_callback;
} state_msg_box_data;

/**
 * @brief Creates a state for a message box
 * @details When using ::state_switch, **do not destroy the original state**.
 *
 * @param parent
 *   Parent game state, that will be returned to when the user leaves the message box
 * @param exit_callback
 *   Called with the chosen user button after leaving the message box and setting the game state.
 *
 * @param msg
 *   String message to display. **No newlines supported**
 *
 * @param buttons
 *   List of the buttons' texts
 * @param button_count
 *   Number of buttons in ::button_count
 * @param default_button
 *   The index of the selected button
 *
 * @author A104348 Humberto Gomes
 */
game_state state_msg_box_create(game_state parent, state_msg_box_exit_callback exit_callback,
                                const char *msg,
                                const char **buttons, int button_count, int default_button);

/**
 * @brief Destroys a state for a msg_box (frees `state->data`)
 * @author A104348 Humberto Gomes
 */
void state_msg_box_destroy(game_state *state);

#endif

