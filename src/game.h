#ifndef GAME_H
#define GAME_H

#include <unistd.h>

#include "brick_game/tetris/code.h"
#include "game_info.h"
#include "gui/cli/interface.h"

#define ACTION_BUTTON 32
#define TERMINATE_BUTTON 27
#define PAUSE_BUTTON 112
#define START_BUTTON 115

void updateUserAction();

#endif  // GAME_H