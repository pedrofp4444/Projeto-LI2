/**
 * @file animation.h
 * @brief Object animation support
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

#ifndef ANIMATION_H
#define ANIMATION_H

#include <stddef.h>

/**
 * @brief   An animation step consists of a new position for an object
 * @details Because all steps last for the same time, no time information is needed
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	int x, y;
} animation_step;

/*
 * @struct animation_sequence
 * @brief A sequence of animation steps
 *
 * @var animation_sequence::steps
 *   The pointer to the array of animation steps
 * @var animation_sequence::lenght
 *   The number of animation steps
 * @var animation_sequence::capacity
 *   The maximum number of steps that ::animation_sequence::steps can hold.
 *
 * @author A104348 Humberto Gomes
 */
typedef struct {
	animation_step *steps;
	size_t length, capacity;
} animation_sequence;

#define ANIMATION_SEQUENCE_STARTING_CAPACITY 16

/**
 * @brief Creates an animation sequence with no steps
 * @author A104348 Humberto Gomes
 */
animation_sequence animation_sequence_create(void);

/**
 * @brief Frees memory for an ::animation_sequence
 * @author A104348 Humberto Gomes
 */
void animation_sequence_free(animation_sequence s);

/**
 * @brief Adds an ::animation_step to and ::animation_sequence
 * @author A104348 Humberto Gomes
 */
void animation_sequence_add_step(animation_sequence *sequence, animation_step add);

#endif

