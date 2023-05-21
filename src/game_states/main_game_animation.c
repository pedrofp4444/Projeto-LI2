/**
 * @file  main_game_animation.c
 * @brief Animations for the main game state
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

#include <stdlib.h>

#include <combat.h>
#include <score.h>
#include <game_states/main_game_animation.h>
#include <game_states/illumination.h>

#define MAIN_GAME_ANIMATION_TIME 0.2
#define WEAPON_DROP_PROBABILITY_PERCENT 20
#define FOOD_DROP_PROBABILITY_PERCENT 50

/**
 * @brief Gets called to update the score and handle mob drops when a mob is killed
 * @param ent The entity killed
 * @param s   A ::game_state pointer
 */
void state_main_game_entity_kill_callback(const entity *ent, void *s) {

	(void) ent;
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	/* Score changes only from player kills */
	if (state->action == MAIN_GAME_ANIMATING_PLAYER_COMBAT) {
		state->score.score += score_from_entity(ent->type);

		/* Randomly drop a weapon */
		if ((rand() % 100) < WEAPON_DROP_PROBABILITY_PERCENT)
			state->dropped = ent->weapon;
		else if ((rand() & 100) < FOOD_DROP_PROBABILITY_PERCENT)
			state->dropped_food = 1;
	}
}

entity_set state_main_game_entities_to_animate(entity_set all, state_main_game_action act) {
	entity_set ret = all;

	switch (act) {
		case MAIN_GAME_ANIMATING_PLAYER_MOVEMENT:
		case MAIN_GAME_ANIMATING_PLAYER_COMBAT:
			ret.count = 1;
			break;
		case MAIN_GAME_ANIMATING_MOBS_MOVEMENT:
		case MAIN_GAME_ANIMATING_MOBS_COMBAT:
			ret.entities++;
			ret.count--;
			break;
		default:
			/* Not supposed to happen */
			break;
	}

	return ret;
}

/**
 * @brief Calls ::entity_set_animate or ::combat_animation_done depending on @p act
 * @returns The return value of the called function (if the animation is done)
 */
int state_main_game_animate_entities(game_state *s, entity_set to_animate,
                                     size_t step_index) {

	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	switch (state->action) {
		case MAIN_GAME_ANIMATING_PLAYER_MOVEMENT:
		case MAIN_GAME_ANIMATING_MOBS_MOVEMENT:
			return entity_set_animate(to_animate, step_index);
		case MAIN_GAME_ANIMATING_MOBS_COMBAT:
		case MAIN_GAME_ANIMATING_PLAYER_COMBAT:
			return combat_animation_update(state->entities, to_animate, step_index,
				state_main_game_entity_kill_callback, s);
		default:
			/* Not supposed to happen. Skip to next action */
			return 1;
	}
}

/**
 * @brief Cleans either animations or combat animations, depending on the current action
 *
 * @param just_animated The animated entity set that need to have the animations reset
 * @param act Current action, used to know which type of animation needs to be reset
 * @param cursorx Location of the cursor's horizontal position
 * @param cursory Location of the cursor's vertical position
 */
void state_main_game_animation_cleanup(entity_set just_animated, state_main_game_action act,
                                       int *cursorx, int *cursory) {

	size_t i;
	switch (act) {
		case MAIN_GAME_ANIMATING_PLAYER_MOVEMENT:
			*cursorx = just_animated.entities[0].x;
			*cursory = just_animated.entities[0].y;
			__attribute__ ((fallthrough)); /* Explicit fallthrough to disable warning */
		case MAIN_GAME_ANIMATING_MOBS_MOVEMENT:
			for (i = 0; i < just_animated.count; ++i)
				if (just_animated.entities[i].health > 0)
					just_animated.entities[i].animation.length = 0;
			break;

		case MAIN_GAME_ANIMATING_MOBS_COMBAT:
		case MAIN_GAME_ANIMATING_PLAYER_COMBAT:

			for (i = 0; i < just_animated.count; ++i)
				if (just_animated.entities[i].health > 0)
					entity_free_combat_target(&just_animated.entities[i]);
			break;
		default:
			break;

	}
}

void state_main_game_animate(game_state *s, double elapsed) {
	state_main_game_data *state = state_extract_data(state_main_game_data, s);

	/* Animate entities (movement or combat) only if that is the case */
	if (state->action != MAIN_GAME_MOVEMENT_INPUT && state->action != MAIN_GAME_COMBAT_INPUT) {
		/* Animation timing */
		if (state->time_since_last_animation >= MAIN_GAME_ANIMATION_TIME) {
			state->time_since_last_animation -= MAIN_GAME_ANIMATION_TIME;

			/* Remove light from last position */
			state_main_game_circle_clean_light_map(
				state->map, PLAYER(state).x, PLAYER(state).y, CIRCLE_RADIUS);

			entity_set to_animate =
				state_main_game_entities_to_animate(state->entities, state->action);

			if (state_main_game_animate_entities(s, to_animate, state->animation_step)) {

				/* End of animation. Clean up and move to next action */
				state_main_game_animation_cleanup(to_animate, state->action,
					&state->cursorx, &state->cursory);

				state->action = (state->action + 1) % 6;
				state->animation_step = 0;
			} else {
				/* Not the end of the animation. Continue */
				state->animation_step++;
			}

			/* Radiate light from new player position */
			state_main_game_circle_light_map(
				state->map, PLAYER(state).x, PLAYER(state).y, CIRCLE_RADIUS);

			state->needs_rerender = 1;
		} else {
			/* Not enought time for the next animation step. Keep waiting */
			state->time_since_last_animation += elapsed;
		}
	}
}

