/**
 * @file animation.h
 * @brief Implementation of object animation
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

#include <animation.h>
#include <stdlib.h>


animation_sequence animation_sequence_create(void) {
	animation_sequence ret = {
		.steps = malloc(ANIMATION_SEQUENCE_STARTING_CAPACITY * sizeof(animation_step)),
		.length = 0,
		.capacity = ANIMATION_SEQUENCE_STARTING_CAPACITY
	};
	return ret;
}

void animation_sequence_free(animation_sequence s) {
	free(s.steps);
}

void animation_add_step(animation_sequence *sequence, animation_step add) {
	if (sequence->length >= sequence->capacity) {
		sequence->capacity *= 2;
		sequence->steps = realloc(sequence->steps, sequence->capacity * sizeof(animation_step));
	}

	sequence->steps[sequence->length] = add;
	sequence->length++;
}

