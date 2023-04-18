/**
 * @file game_loop.c
 * @brief Ncurses game loop abstraction implementation
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

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <ncurses.h>

/**
 * @brief A game loop helper function to ignore a given signal.
 * @returns 0 on success, other value on error
 */
int game_loop_ignore_signal(int signum) {
	struct sigaction act = {
		.sa_handler = SIG_IGN,
		.sa_flags   = SA_NODEFER,
	};

	return sigaction(signum, &act, NULL);
}

int game_loop_init_ncurses(void) {
	if (initscr()           == NULL) return 1;
	if (start_color()        == ERR) return 1; /* Enable color support */
	if (cbreak()             == ERR) return 1; /* Disable line buffering and control characters */
	if (noecho()             == ERR) return 1; /* Don't show input on the terminal */
	if (nodelay(stdscr, 1)   == ERR) return 1; /* Make getch return ERR if no input is available */
	if (nonl()               == ERR) return 1; /* Transform new lines intro carriage returns */
	if (keypad(stdscr, 1)    == ERR) return 1; /* Let ncurses parse escape sequences */
	if (curs_set(0)          == ERR) return 1; /* Hide the cursor */

	/* Limit of 10ms for ncurses to give up on finding characters for escape sequences */
	ESCDELAY = 10;

	/* Ignore signals that stop the program (coming from key combinations like Ctrl+C) */
	game_loop_ignore_signal(SIGINT);
	game_loop_ignore_signal(SIGTERM);
	game_loop_ignore_signal(SIGTSTP);

	/* Initialize basic color pairs (black background for all basic foreground colors) */
	for (int col = COLOR_BLACK; col <= COLOR_WHITE; ++col)
		init_pair(col, col, COLOR_BLACK);

	return 0;
}

/**
 * @brief Internal game loop for calculating the difference between `struct timespec`s in seconds.
 *
 * Be aware of the precision limits of `double`s when dealing with `struct timespec`s far apart
 * in time (not recommended).
 */
double timespec_dif(struct timespec start, struct timespec end) {
	return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
}

/**
 * @brief Internal game loop function for keeping the terminal window size up to date.
 *
 * @returns The return value of the resize callback in case of a loop exit request. Otherwise,
 *          ::GAME_LOOP_CALLBACK_RETURN_SUCCESS is returned.
 */
game_loop_callback_return_value game_loop_window_size(void *state, int *width, int *height,
                                                      game_loop_resize_callback onresize) {
	int nwidth, nheight;
	getmaxyx(stdscr, nheight, nwidth);

	if (!(nwidth == *width && nheight == *height)) { /* Window size changed */

		if (onresize) {
			int ret = onresize(state, nwidth, nheight);
			if (ret != GAME_LOOP_CALLBACK_RETURN_SUCCESS) return ret;
		}

		*width = nwidth; *height = nheight;
	}

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/**
 * @brief Internal game loop function for reading input and calling the callback if needed
 *
 * @returns The return value of the input callback in case of a loop exit request. Otherwise,
 *          ::GAME_LOOP_CALLBACK_RETURN_SUCCESS is returned.
 */
game_loop_callback_return_value game_loop_handle_input
	(void *state, game_loop_input_callback oninput) {

	if (oninput) { /* Skip reading input if no input callback is defined */
		int c = getch();
		while (c != ERR) {
			int ret = oninput(state, c);
			if (ret != GAME_LOOP_CALLBACK_RETURN_SUCCESS) return ret;

			c = getch();
		}
	}

	return GAME_LOOP_CALLBACK_RETURN_SUCCESS;
}

/**
 * @brief Internal game loop function for sleeping, in order to keep the target fps
 * @returns 0 on success, 1 on error.
 */
int game_loop_keep_fps(struct timespec frame_start, double frame_time) {
	/* Calculate time since the beginning of this frame */
	struct timespec frame_end;
	if (clock_gettime(CLOCK_MONOTONIC, &frame_end)) return 1;
	double delta = timespec_dif(frame_start, frame_end);

	/* Sleep for enough time to target the framerate */
	double sleep_time = 0;
	if (delta < frame_time)
		sleep_time = (frame_time - delta) * 1e9; /* 1e9: s -> ns */

	/*
	* NOTES: - tv_nsec is lower than 1e9 because frame_time can't be larger than a second
	*
	*        - No error handling for nanosleep (errors out on external signals), as it's not a
	*          crucial function to keep the game running.
	*/
	struct timespec sleep = { .tv_sec = 0, .tv_nsec = (long) sleep_time };
	nanosleep(&sleep, NULL);

	return 0;
}

/**
 * @brief Helper macro for ::game_loop_run that, based on the return value of a callback,
 *        determines whether to continue looping and the game loop's return value (if needed).
 */
#define game_loop_return(val) { \
	if (val != GAME_LOOP_CALLBACK_RETURN_SUCCESS) \
		return val == GAME_LOOP_CALLBACK_RETURN_ERROR; \
}

int game_loop_run(void *state, game_loop_callbacks *callbacks, unsigned int fps) {
	int width = -1, height = -1;

	struct timespec last_frame_instant;
	if (clock_gettime(CLOCK_MONOTONIC, &last_frame_instant)) return 1;

	double frame_time = 0;
	if (fps)
		frame_time = 1.0 / fps;
	if (fps == 1) frame_time = 0.9999; /* Edge case when converting to timespec */

	while (1) {
		/* Calculate frame time (since the beginning of the last frame) */
		struct timespec frame_instant;
		if (clock_gettime(CLOCK_MONOTONIC, &frame_instant)) return 1;
		double delta = timespec_dif(last_frame_instant, frame_instant);
		last_frame_instant = frame_instant;

		/* Keep terminal window size up to date */
		int ret = game_loop_window_size(state, &width, &height, callbacks->onresize);
		game_loop_return(ret);

		ret = game_loop_handle_input(state, callbacks->oninput);
		game_loop_return(ret);

		if (callbacks->onupdate) ret = callbacks->onupdate(state, delta);
		game_loop_return(ret);

		if (callbacks->onrender) ret = callbacks->onrender(state, width, height);
		game_loop_return(ret);

		if (game_loop_keep_fps(frame_instant, frame_time)) return 1;
	}

	return 0;
}

int game_loop_terminate_ncurses(void) {
	return endwin() == ERR; /* Restore previous terminal mode */
}

