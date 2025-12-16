#ifndef CODE_H
#define CODE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "../../game.h"
#include "../../game_info.h"

#define SPEED 500
#define SPEED_STEP 30
#define NEW_LEVEL_THRESHOLD 600
#define MAX_LEVEL 10

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

typedef enum { Initial, Spawn, Moving, Shifting, Attaching, GameOver } Status_t;

typedef struct {
  int status;
  int **field;
  int **block;
  int blockSize;
  int **nextBlock;
  int nextBlockSize;
  int coordX;
  int coordY;
  int score;
  int level;
  int speed;
  int pause;
  unsigned long long startTime;
  unsigned long long timeLeft;
} State_t;

typedef enum { ALPHA, BETA, GAMMA, DELTA, OMEGA, PSI, ZETA } Block;

GameInfo_t updateCurrentState();
void spawnNewBlock();
void moveBlockLeft();
void moveBlockRight();
void shiftBlock();
void attachBlock();
int blockIsAttached();
void rotateBlock();
void initializeState();
void userInput(UserAction_t action);
State_t *getCurrentState();
int canRotateBlock(int **newBlock);
void rotate(int **newBlock, int **oldBlock, int size);
void consumeRows();
void copyMatrix(int **dest, int **src, int H, int W);
int **generateNewBlock(int *blockSize);
void finishGame();
unsigned long long processTimer();
int **createMatrix(int H, int W);
void saveMaxScore();
void updateLevel();
void startGame();
unsigned long long currentTimeMillis();
void pauseGame();
void freeMatrix(int **matrix, int size);

#endif  // CODE_H