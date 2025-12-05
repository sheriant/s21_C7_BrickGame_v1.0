#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>

#define FIELD_W 10
#define FIELD_H 20

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int field[FIELD_H][FIELD_W];
  int next_piece[4][4];
  int score;
  int high_score;
  int level;
  int speed;
  bool pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);
GameInfo_t* updateCurrentState(void);

void gameInit(void);
void gameTerminate(void);


#endif // TETRIS_H