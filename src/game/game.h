//
// Created by Lasse Wolpmann on 11.7.2025.
//

#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <stdint.h>
#include "player/player.h"

#define MAX_TURNS 9

typedef struct {
    unsigned char field[4][4];
    uint8_t turn;
    const player_t *starting_player;
    const player_t *current_player;
} game_t;

void init_game(game_t *game);
void print_game_field(const game_t *game);
unsigned char get_winning_player(const game_t *game);
bool get_game_state(const game_t *game);
void play_game(game_t *game, const player_t *human_player, const player_t *computer_player);
void play_round(game_t *game, const player_t *human_player, const player_t *computer_player);

#endif //GAME_FIELD_H
