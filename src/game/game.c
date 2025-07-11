//
// Created by Lasse Wolpmann on 11.7.2025.
//

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include "game.h"
#include "player/cpu/cpu.h"

void init_game(game_t *game) {
    const unsigned char playing_field[4][4] = {
        " 123",
        "1...",
        "2...",
        "3..."
    };

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            game->field[row][col] = playing_field[row][col];
        }
    }

    game->turn = 1;
};

void print_game_field(const game_t *game) {
    for (unsigned char row = 0; row < 4; row++) {
        for (unsigned char col = 0; col < 4; col++) {
            printf(" %c ", game->field[row][col]);
        }
        printf("\n");
    }
    printf("\n");
};

unsigned char get_winning_player(const game_t *game) {
    for (int row = 1; row < 4; row++) {
        if (game->field[row][1] != '.' && game->field[row][1] == game->field[row][2] && game->field[row][2] == game->field[row][3]) {
            return game->field[row][1];
        }
    }

    for (int col = 1; col < 4; col++) {
        if (game->field[1][col] != '.' && game->field[1][col] == game->field[2][col] && game->field[2][col] == game->field[3][col]) {
            return game->field[1][col];
        }
    }

    if (game->field[1][1] != '.' && game->field[1][1] == game->field[2][2] && game->field[2][2] == game->field[3][3]) {
        return game->field[1][1];
    }

    if (game->field[1][3] != '.' && game->field[1][3] == game->field[2][2] && game->field[2][2] == game->field[3][1]) {
        return game->field[1][3];
    }

    int draw = 1;
    for (int row = 1; row < 4; row++) {
        for (int col = 1; col < 4; col++) {
            if (game->field[row][col] == '.') {
                draw = 0;
                break;
            }
        }
    }

    if (draw) {
        return 'd';
    }

    return ' ';
}

bool get_game_state(const game_t *game) {
    const unsigned int buffer_size = 128;
    char buffer[buffer_size];
    int ret;

    const unsigned char winning_player = get_winning_player(game);

    if (winning_player == ' ') {
        ret = snprintf(buffer, buffer_size, "%s", "No winner yet, keep playing.");
    } else {
        if (winning_player == 'd') {
            ret = snprintf(buffer, buffer_size, "Game over: %s", "Draw!");
        } else {
            ret = snprintf(buffer, buffer_size, "Game over: %c has won!", winning_player);
        }
    }

    if (ret >= 0 && ret < buffer_size) {
        printf("%s\n\n", buffer);
        return strcmp(buffer, "No winner yet, keep playing.") == 0 ? false : true;
    }

    return false;
}

void play_game(game_t *game, const player_t *human_player, const player_t *computer_player) {
    print_game_field(game);

    while (game->turn <= MAX_TURNS) {
        play_round(game, human_player, computer_player);
        if (get_game_state(game)) break;
        game->turn++;
    }
}

void play_round(game_t *game, const player_t *human_player, const player_t *computer_player) {
    printf("### Turn %u (%c, %s) ###\n",
        game->turn,
        game->current_player->symbol,
        game->current_player->symbol == human_player->symbol ? "You" : "CPU");

    long int row = 0, col = 0;
    bool field_occupied = true;

    if (game->current_player->symbol == human_player->symbol) {
        while (row <= 0 || row > 3 || col <= 0 || col > 3 || field_occupied) {
            printf("Row: ");
            char row_input[2];
            scanf("%1s", row_input);
            row_input[1] = '\0';
            row = strtol(row_input, NULL, 10);

            int r;
            while ((r = getchar()) != '\n' && r != EOF) { }

            printf("Column: ");
            char col_input[2];
            scanf("%1s", col_input);
            col_input[1] = '\0';
            col = strtol(col_input, NULL, 10);

            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }

            field_occupied = game->field[row][col] != '.';
        }
    } else {
        int best_score = INT_MIN;
        int best_row = 0;
        int best_col = 0;

        // Step 1: Find empty fields
        for (unsigned char r = 1; r < 4; r++) {
            for (unsigned char c = 1; c < 4; c++) {
                if (game->field[r][c] != '.') { continue; }

                game->field[r][c] = computer_player->symbol;
                const int score = minimax(game, 0, 0, human_player, computer_player);
                game->field[r][c] = '.';

                if (score > best_score) {
                    best_score = score;
                    best_row = r;
                    best_col = c;
                }
            }
        }

        row = best_row;
        col = best_col;
    }

    // printf("Row: %ld, Column: %ld, Occupied: %s\n", row, col, field_occupied ? "true" : "false");
    game->field[row][col] = game->current_player->symbol;
    print_game_field(game);

    if (game->current_player == human_player) {
        game->current_player = computer_player;
    } else {
        game->current_player = human_player;
    }
}