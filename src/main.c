#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "game/game.h"
#include "player/player.h"

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        printf("Not enough or too many arguments given\n");

        return 1;
    }

    if (strcmp(argv[1], "x") != 0 && strcmp(argv[1], "o") != 0) {
        printf("Player Symbol argument must be either 'x' or 'o'\n");
        return 1;
    }

    game_t game;
    init_game(&game);

    player_t human_player;
    human_player.symbol = argv[1][0];

    player_t computer_player;
    computer_player.symbol = human_player.symbol == 'x' ? 'o' : 'x';
    
    game.starting_player = arc4random() % 2 == 0 ? &human_player : &computer_player;
    game.current_player = game.starting_player;

    play_game(&game, &human_player, &computer_player);

    return 0;
}