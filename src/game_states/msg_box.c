/**
 * @file  msg_box.c
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

#include <game_state.h>
#include <game_states/msg_box.h>

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/** @brief Responds to user input in a message box. */
game_loop_callback_return_value state_msg_box_oninput(void *s, int key) {
	state_msg_box_data *state = state_extract_data(state_msg_box_data, s);

	switch (key) {
		/* Respond to arrow keys for button switching (with bounds checking) */
		case KEY_LEFT:
			if (state->chosen_button > 0) {
				state->chosen_button--;
				state->needs_rerender = 1;
			}
			break;

		case KEY_RIGHT:
			if (state->chosen_button < state->button_count - 1) {
				state->chosen_button++;
				state->needs_rerender = 1;
			}
			break;

		/* Button choice. Confirm it to parent game state. */
		case '\r': {
			/* Copy needed data as state will be freed */
			int chosen_button = state->chosen_button;
			state_msg_box_exit_callback exit_callback = state->exit_callback;
			game_state parent = state->parent;

			state_switch((game_state *) s, &parent, 1);
			if (exit_callback)
				exit_callback((game_state *) s, chosen_button);
			break;
		}

		default:
			break;
	}

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief Renders vertical lines for the message box rectangle.
 *  @details
 *    - Also adds the `+` characters on the corners;
 *    - Lines (like all message boxes) are always 7 characters tall.
 *
 *  @param x
 *    X coordinate of the line's topmost point
 *  @param y
 *    Y coordinate of the line's topmost point
 */
void msg_box_draw_vertical_line(int x, int y) {
	for (int i = 0; i < 7; ++i) {
		move(y + i, x);
		addch((i == 0 || i == 6) ? '+' : '|');
	}
}

/** @brief Renders horizontal lines for the message box rectangle.
 *  @details
 *    - The corners are not rendered, as they are handled by ::msg_box_draw_vertical_line
 *
 *  @param x
 *    X coordinate of the line's leftmost point (corner, not rendered)
 *  @param y
 *    Y coordinate of the line's leftmost point (corner, not rendered)
 *  @param width
 *    The width of the line (includes corners)
 */
void msg_box_draw_horizontal_line(int x, int y, int width) {
	for (int i = 1; i < width - 1; ++i) {
		move(y, x + i);
		addch('-');
	}
}

/** @brief Renders a message box (only if re-rendering is needed) */
game_loop_callback_return_value state_msg_box_onrender(void *s, int width, int height) {
	state_msg_box_data *state = state_extract_data(state_msg_box_data, s);

	if (!state->needs_rerender)
		return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
	state->needs_rerender = 0;

	erase();

	/* 1 - +-------+ */
	/* 2 - |       | */
	/* 3 - | MSG   | */
	/* 4 - |       | */
	/* 5 - |       | */
	/* 6 - |   BTN | */
	/* 7 - +-------+ */

	/* Calculate width of the message box (maximum between message and buttons + padding) */
	int msg_width = strlen(state->message);

	int buttons_width = 0;
	for (int i = 0; i < state->button_count; ++i) {
		buttons_width += strlen(state->buttons[i]) + 1;
	}
	buttons_width--; /* No space for the last space */

	int box_width = ((msg_width > buttons_width) ? msg_width : buttons_width) + 3;

	/* Box coordinates */
	int left = (width - box_width) / 2, top = (height - 7) / 2;

	/* Draw rectangle around information ( box_width x 7 ) */
	msg_box_draw_vertical_line  (left, top);
	msg_box_draw_vertical_line  (left + box_width, top);
	msg_box_draw_horizontal_line(left, top    , box_width + 1);
	msg_box_draw_horizontal_line(left, top + 6, box_width + 1);

	/* Draw message */
	move(top + 2, left + 2); /* Left align with padding */
	printw("%s", state->message);

	/* Draw buttons */
	move(top + 5, left + box_width - buttons_width - 1); /* Right align with padding */
	for (int i = 0; i < state->button_count; ++i) {
		if (i == state->chosen_button)
			attron(A_REVERSE);

		printw("%s", state->buttons[i]);

		if (i == state->chosen_button)
			attroff(A_REVERSE);

		addch(' ');
	}

	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief Called when the terminal window is resized to request for a message box redraw */
game_loop_callback_return_value state_msg_box_onresize(void *s, int width, int height) {
	(void) width; (void) height;

	state_msg_box_data *state = state_extract_data(state_msg_box_data, s);
	state->needs_rerender = 1;

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_state state_msg_box_create(game_state parent, state_msg_box_exit_callback exit_callback,
                                const char *msg,
                                const char **buttons, int button_count, int default_button) {

	/* Allocate space for the message (owned by this game state) */
	char *msg_cpy = malloc(strlen(msg) + 1);
	strcpy(msg_cpy, msg);

	/* Allocate space for the text of every button */
	char **buttons_cpy = malloc(button_count * sizeof(char *));
	for (int i = 0; i < button_count; ++i) {
		buttons_cpy[i] = malloc(strlen(buttons[i]) + 1);
		strcpy(buttons_cpy[i], buttons[i]);
	}

	state_msg_box_data data = {
		.message = msg_cpy,

		.buttons       = buttons_cpy,
		.button_count  = button_count,
		.chosen_button = default_button,

		.needs_rerender = 1,

		.parent = parent,
		.exit_callback = exit_callback
	};
	state_msg_box_data *data_ptr = malloc(sizeof(state_msg_box_data));
	*data_ptr = data;

	game_loop_callbacks callbacks = {
		.oninput  = state_msg_box_oninput,
		.onupdate = NULL,
		.onrender = state_msg_box_onrender,
		.onresize = state_msg_box_onresize,
	};

	game_state ret = {
		.data = data_ptr,
		.destroy = state_msg_box_destroy,
		.callbacks = callbacks
	};

	return ret;
}

void state_msg_box_destroy(game_state* state) {
	state_msg_box_data *msg_state = state_extract_data(state_msg_box_data, state);

	free(msg_state->message);

	/* Free the list of buttons */
	for (int i = 0; i < msg_state->button_count; ++i)
		free(msg_state->buttons[i]);
	free(msg_state->buttons);

	free(state->data);
}

