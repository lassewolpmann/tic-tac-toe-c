//
// Created by Lasse Wolpmann on 11.7.2025.
//

#include <limits.h>
#include <stdbool.h>

#include "cpu.h"
#include "player/player.h"

int min(const int a, const int b) { return (a < b) ? a : b; }
int max(const int a, const int b) { return (a > b) ? a : b; }

int minimax(game_t *game, const bool is_maximising, const int depth, const player_t *human_player, const player_t *computer_player) {
    const unsigned char winning_player = get_winning_player(game);

    if (winning_player == computer_player->symbol) return 10 - depth;
    if (winning_player == human_player->symbol) return depth - 10;
    if (winning_player == 'd') return 0;

    // printf("Maximising: %s\n", is_maximising ? "true" : "false");
    // print_game_field();

    int best_score = is_maximising ? INT_MIN : INT_MAX;

    for (unsigned char r = 1; r < 4; r++) {
        for (unsigned char c = 1; c < 4; c++) {
            if (game->field[r][c] != '.') { continue; }

            game->field[r][c] = is_maximising ? computer_player->symbol : human_player->symbol;
            const int score = minimax(game, !is_maximising, depth + 1, human_player, computer_player);
            game->field[r][c] = '.';
            best_score = is_maximising ? max(score, best_score) : min(score, best_score);
        }
    }

    return best_score;
}