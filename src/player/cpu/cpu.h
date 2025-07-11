//
// Created by Lasse Wolpmann on 11.7.2025.
//

#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include "game/game.h"

int minimax(game_t *game, bool is_maximising, int depth, const player_t *human_player, const player_t *computer_player);

#endif //CPU_H
