/**
 * @file  score.h
 * @brief Methods for incrementing player score, saving it and loading it from disk
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

#include <score.h>

int score_from_entity(entity_type t) {
	switch (t) {
		case ENTITY_RAT:
			return 10;
		case ENTITY_GOBLIN:
			return 20;
		case ENTITY_CRISTINO:
			return 100;
		default:
			return 0;
	}
}
