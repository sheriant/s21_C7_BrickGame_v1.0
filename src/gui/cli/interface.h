#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>
#include <stdlib.h>

#include "../../game_info.h"

void render(gameInfo_t info);
void terminateGame();

#endif  // INTERFACE_H