#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum {
  Start,
  Pause,
  Exit,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef enum {
  STATE_START,
  STATE_SPAWN,
  STATE_MOVING,
  STATE_SHIFT,
  STATE_ATTACHING,
  STATE_GAMEOVER,
  STATE_PAUSE
} Game_stage;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

void user_input(UserAction_t action, bool hold);
GameInfo_t* update_current_state(void);

void gameInit(void);
void gameTerminate(void);

#endif  // GAME_H