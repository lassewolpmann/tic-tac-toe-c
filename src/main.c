#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

unsigned char playing_field[4][4] = {
    " 123",
    "1...",
    "2...",
    "3..."
};

void print_game_field() {
    for (unsigned char row = 0; row < 4; row++) {
        for (unsigned char col = 0; col < 4; col++) {
            printf(" %c ", playing_field[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

unsigned char get_winning_player() {
    for (int row = 1; row < 4; row++) {
        if (playing_field[row][1] != '.' && playing_field[row][1] == playing_field[row][2] && playing_field[row][2] == playing_field[row][3]) {
            return playing_field[row][1];
        }
    }

    for (int col = 1; col < 4; col++) {
        if (playing_field[1][col] != '.' && playing_field[1][col] == playing_field[2][col] && playing_field[2][col] == playing_field[3][col]) {
            return playing_field[1][col];
        }
    }

    if (playing_field[1][1] != '.' && playing_field[1][1] == playing_field[2][2] && playing_field[2][2] == playing_field[3][3]) {
        return playing_field[1][1];
    }

    if (playing_field[1][3] != '.' && playing_field[1][3] == playing_field[2][2] && playing_field[2][2] == playing_field[3][1]) {
        return playing_field[1][3];
    }

    int draw = 1;
    for (int row = 1; row < 4; row++) {
        for (int col = 1; col < 4; col++) {
            if (playing_field[row][col] == '.') {
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

void print_game_state(char *winning_player_buffer, int size) {
    char buffer[size];
    int ret;

    const unsigned char winning_player = get_winning_player();

    if (winning_player == ' ') {
        ret = snprintf(buffer, size, "%s", "No winner yet, keep playing.");
    } else {
        if (winning_player == 'd') {
            ret = snprintf(buffer, size, "Game over: %s", "Draw!");
        } else {
            ret = snprintf(buffer, size, "Game over: %c has won!", winning_player);
        }
    }

    if (ret >= 0 && ret < size) strncpy(winning_player_buffer, buffer, size);
}

int min(const int a, const int b) { return (a < b) ? a : b; }

int max(const int a, const int b) { return (a > b) ? a : b; }

int minimax(const int is_maximising, const int depth, const unsigned char player_symbol, const unsigned char cpu_symbol) {
    const unsigned char winning_player = get_winning_player();

    if (winning_player == cpu_symbol) return 10 - depth;
    if (winning_player == player_symbol) return depth - 10;
    if (winning_player == 'd') return 0;

    // printf("Maximising: %s\n", is_maximising ? "true" : "false");
    // print_game_field();

    int best_score = is_maximising ? INT_MIN : INT_MAX;

    for (unsigned char r = 1; r < 4; r++) {
        for (unsigned char c = 1; c < 4; c++) {
            if (playing_field[r][c] != '.') { continue; }

            playing_field[r][c] = is_maximising ? cpu_symbol : player_symbol;
            const int score = minimax(!is_maximising, depth + 1, player_symbol, cpu_symbol);
            playing_field[r][c] = '.';
            best_score = is_maximising ? max(score, best_score) : min(score, best_score);
        }
    }

    return best_score;
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        printf("Not enough arguments given\n");

        return 1;
    }

    const unsigned int player_is_x = strcmp(argv[1], "x") == 0 ? 1 : 0;
    const unsigned int player_is_o = strcmp(argv[1], "o") == 0 ? 1 : 0;

    if (!player_is_x && !player_is_o) {
        printf("Player Symbol argument must be either 'x' or 'o'\n");
        return 1;
    }

    const unsigned char player_symbol = player_is_x ? 'x' : 'o';
    const unsigned char cpu_symbol = player_is_x ? 'o' : 'x';

    printf("Player: %c, CPU: %c\n", player_symbol, cpu_symbol);

    const unsigned long int p = arc4random() % 2;

    const unsigned char starting_player = p == 0 ? player_symbol : cpu_symbol;

    unsigned char current_player = starting_player;

    printf("Starting player: %c\n\n", starting_player);
    print_game_field();

    for (unsigned char turn = 1; turn <= 9; turn++) {
        printf("### Turn %u (%c, %s) ###\n", turn, current_player, current_player == player_symbol ? "You" : "CPU");

        long int row = 0, col = 0;
        int field_occupied = 1;

        if (current_player == player_symbol) {
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

                field_occupied = playing_field[row][col] != '.';
            }
        } else {
            int best_score = INT_MIN;
            int best_row = 0;
            int best_col = 0;

            // Step 1: Find empty fields
            for (unsigned char r = 1; r < 4; r++) {
                for (unsigned char c = 1; c < 4; c++) {
                    if (playing_field[r][c] != '.') { continue; }

                    playing_field[r][c] = cpu_symbol;
                    const int score = minimax(0, 0, player_symbol, cpu_symbol);
                    playing_field[r][c] = '.';

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
        playing_field[row][col] = current_player;
        print_game_field();

        current_player = current_player == player_symbol ? cpu_symbol : player_symbol;

        char game_state_buffer[128];
        print_game_state(game_state_buffer, sizeof(char) * 128);

        printf("%s\n\n", game_state_buffer);
        if (strcmp(game_state_buffer, "No winner yet, keep playing.") == 0) continue;
        break;
    }

    print_game_field();

    return 0;
}