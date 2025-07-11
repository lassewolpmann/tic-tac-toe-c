# Simple tic-tac-toe game written in C
## Build instructions
```shell
mkdir build
cd build/
cmake ../
```

## Run instructions
```shell
cmake --build build/
build/tic_tac_toe
```
The command expects one argument for the player symbol. Valid input is either x or o.

Play as x:
```shell
build/tic_tac_toe x
```
Play as o:
```shell
build/tic_tac_toe o
```