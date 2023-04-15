/**
 * @file entities.h
 * @brief Entities
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

#ifndef ENTITIES_H
#define ENTITIES_H

typedef enum {
    ENTITY_PLAYER,
    ENTITY_RAT,
    ENTITY_TROLL,
    ENTITY_GOBLIN,
    ENTITY_CRISTINO,
} entity_type;

typedef enum {
    WEAPON_HAND,
    WEAPON_DAGGER,
    WEAPON_ARROW,
    WEAPON_BOMB,
    WEAPON_LANTERN,
    WEAPON_IPAD,
} weapon;

typedef struct {
    unsigned x, y;
    entity_type type;
    
    int health;
    weapon weapon;

    void *data;
    void (*update)(void);
} entity;

#endif
