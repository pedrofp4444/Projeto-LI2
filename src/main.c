/**
 * @file main.c
 * @brief Contains the entry point to the program
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

#include <game_loop.h>
#include <ncurses.h>

typedef struct {
	double fps;

	int offsetx, offsety;
} game_state;

game_loop_callback_return_value oninput(void *s, int key) {
	game_state *state = (game_state *) s;

	switch (key) {
		case '\x1b':
			return GAME_LOOP_CALLBACK_RETURN_BREAK; /* Exit game on escape */

		case KEY_UP:
			state->offsety--;
			break;
		case KEY_DOWN:
			state->offsety++;
			break;

		case KEY_LEFT:
			state->offsetx--;
			break;
		case KEY_RIGHT:
			state->offsetx++;
			break;
	}

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_loop_callback_return_value onupdate(void *s, double elapsed) {
	game_state *state = (game_state *) s;
	state->fps = 1.0 / elapsed;
	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

game_loop_callback_return_value onrender(void *s, int width, int height) {
	game_state *state = (game_state *) s;

	erase();

	/* Print the FPS on the top right corner */
	move(0, 0);
	printw("FPS: %d", (int) state->fps);

	/* Print the window size in the center of the window (plus input offset) */
	char str[64];
	int len = sprintf(str, "%d x %d", width, height);

	move(height / 2 + state->offsety, (width - len) / 2 + state->offsetx);
	printw("%s", str);

	refresh();

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/** @brief The entry point for the game */
int main(void) {
	int err = game_loop_init_ncurses();
	if (err) {
		/* Don't handle errors. Just try to return to a canonical terminal mode */
		game_loop_terminate_ncurses();
		return 1;
	}

	game_loop_callbacks callbacks = {
		.oninput  = oninput,
		.onupdate = onupdate,
		.onrender = onrender,
		.onresize = NULL
	};

	game_state state = { .fps = 0.0 };

	err = game_loop_run(&state, callbacks);
	if (err) {
		/* Don't handle errors. Just try to return to a canonical terminal mode */
		game_loop_terminate_ncurses();
		return 1;
	}

	err = game_loop_terminate_ncurses();
	return err;
}

