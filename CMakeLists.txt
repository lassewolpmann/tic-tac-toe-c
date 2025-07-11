cmake_minimum_required(VERSION 3.31)
project(tic_tac_toe C)

set(CMAKE_C_STANDARD 99)

include_directories(src)
add_executable(tic_tac_toe
	src/main.c
	src/game/game.c
	src/player/player.c
	src/player/cpu/cpu.c
)
