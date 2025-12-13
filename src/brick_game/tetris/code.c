#include "code.h"

State_t *getCurrentState() {
  static State_t state_t;
  return &state_t;
}

int **createMatrix(int H, int W) {
  int **matrix = (int **)malloc(H * sizeof(int *));
  for (int i = 0; i < H; i++) {
    int *row = (int *)malloc(W * sizeof(int));
    matrix[i] = row;
    for (int j; j < W; j++) {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

int **generateNewBlock(int *blockSize) {
  int blockType = rand() % 7;
  int rotations = rand() % 4;
  int **block = createMatrix(*blockSize, *blockSize);
  int **temp = createMatrix(*blockSize, *blockSize);

  if (blockType == ALPHA)
    *blockSize = 4;
  else if (blockType == DELTA)
    *blockSize = 2;
  else
    *blockSize = 3;

  if (blockType == ALPHA) {  // [x][x][x][x]
    block[0][0] = 1;         // [ ][ ][ ][ ]
    block[0][1] = 1;         // [ ][ ][ ][ ]
    block[0][2] = 1;         // [ ][ ][ ][ ]
    block[0][3] = 1;
  } else if (blockType == BETA) {  // [x][x][x]
    block[1][0] = 1;               // [x][ ][ ]
    block[0][0] = 1;               // [ ][ ][ ]
    block[0][1] = 1;
    block[0][2] = 1;
  } else if (blockType == GAMMA) {  // [x][x][x]
    block[0][0] = 1;                // [ ][ ][x]
    block[0][1] = 1;                // [ ][ ][ ]
    block[0][2] = 1;
    block[1][2] = 1;
  } else if (blockType == DELTA) {  // [x][x]
    block[0][0] = 1;                // [x][x]
    block[0][1] = 1;
    block[1][0] = 1;
    block[1][1] = 1;
  } else if (blockType == OMEGA) {  // [x][x][ ]
    block[0][0] = 1;                // [ ][x][x]
    block[0][1] = 1;                // [ ][ ][ ]
    block[1][1] = 1;
    block[1][2] = 1;
  } else if (blockType == PSI) {  // [ ][x][ ]
    block[0][1] = 1;              // [x][x][x]
    block[1][0] = 1;              // [ ][ ][ ]
    block[1][1] = 1;
    block[1][2] = 1;
  } else if (blockType == ZETA) {
    block[1][0] = 1;  // [ ][x][x]
    block[1][1] = 1;  // [x][x][ ]
    block[0][1] = 1;  // [ ][ ][ ]
    block[0][2] = 1;
  }

  for (int i = 0; i < rotations; i++) {
    rotate(temp, block, *blockSize);
    copyMatrix(block, temp, *blockSize, *blockSize);
  }

  freeMatrix(temp, *blockSize);
  return block;
}

void freeMatrix(int **matrix, int size) {
  for (int i = 0; i < size; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

void copyMatrix(int **dest, int **src, int H, int W) {
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      dest[i][j] = src[i][j];
    }
  }
}

void rotate(int **newBlock, int **oldBlock, int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      newBlock[i][j] = 0;
    }
  }

  if (size == 4) {
    if (oldBlock[0][0] == 1) {
      newBlock[0][1] = 1;
      newBlock[1][1] = 1;
      newBlock[2][1] = 1;
      newBlock[3][1] = 1;
    } else {
      newBlock[1][0] = 1;
      newBlock[1][1] = 1;
      newBlock[1][2] = 1;
      newBlock[1][3] = 1;
    }
  } else if (size == 2) {
  } else {
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        newBlock[j][size - 1 - i] = oldBlock[i][j];
      }
    }
  }
}

void initializeState() {
  State_t *state = getCurrentState();
  state->status = Initial;

  int **field = createMatrix(FIELD_H, FIELD_W);

  state->field = field;
  state->score = 0;
  state->level = 1;
  state->speed = SPEED;
  state->timeLeft = state->speed;
  state->pause = 0;
  state->coordX = -1;
  state->coordY = 4;

  int nextBlockSize;
  int **nextBlock = generateNewBlock(&nextBlockSize);
  int **block = createMatrix(nextBlockSize, nextBlockSize);
  copyMatrix(block, nextBlock, nextBlockSize, nextBlockSize);
  state->nextBlockSize = nextBlockSize;
  state->nextBlock = nextBlock;
  state->blockSize = nextBlockSize;
  state->block = block;

  srand(currentTimeMillis());
}

unsigned long long currentTimeMillis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (unsigned long long)(tv.tv_sec) * 1000 +
         (unsigned long long)(tv.tv_usec) / 1000;
}

unsigned long long processTimer() {
  State_t *state = getCurrentState();
  unsigned long long timeLeft;

  if (state->status == Initial)
    timeLeft = -1;
  else {
    unsigned long long elapsedTime = currentTimeMillis() - state->startTime;
    if (elapsedTime >= state->timeLeft)
      timeLeft = 0;
    else {
      state->timeLeft -= elapsedTime;
      state->startTime = currentTimeMillis();
      timeLeft = state->timeLeft;
    }
  }
  return timeLeft;
}

void userInput(UserAction_t action) {
  State_t *state = getCurrentState();

  if ((action == Start) && (state->status == Initial))
    startGame();
  else if ((action == Left) && (state->status == Moving))
    moveBlockLeft();
  else if ((action == Right) && (state->status == Moving))
    moveBlockRight();
  else if ((action == Down) && (state->status == Moving))
    state->status = Attaching;
  else if ((action == Action) && (state->status == Moving))
    rotateBlock();
  else if ((action == Pause) && (state->status != Initial))
    pauseGame();
  else if (action == Terminate) {
    freeMatrix(state->field, FIELD_H);
    freeMatrix(state->block, state->blockSize);
    freeMatrix(state->nextBlock, state->nextBlockSize);
  }
}

void startGame() {
  State_t *state = getCurrentState();
  state->status = Spawn;
}

int blockIsAttached() {
  State_t *state = getCurrentState();
  int attached = 0;
  for (int i = 0; i < state->blockSize; i++) {
    for (int j = 0; j < state->blockSize; j++) {
      int newX = state->coordX + 1 - i;
      int newY = state->coordY + j;

      if ((state->block[i][j] == 1) && (newX >= FIELD_H))
        attached = 1;
      else if ((state->block[i][j] == 1) && (newX >= 0) &&
               (state->field[newX][newY] == 1))
        attached = 1;
    }
  }
  return attached;
}

void moveBlockLeft() {
  State_t *state = getCurrentState();
  int canMoveLeft = 1;
  for (int i = 0; i < state->blockSize; i++) {
    for (int j = 0; j < state->blockSize; j++) {
      int newX = state->coordX - i;
      int newY = state->coordY - 1 + j;
      if ((state->block[i][j] == 1) && (newY < 0))
        canMoveLeft = 0;
      else if (newX >= 0 && newY >= 0 &&
               (state->block[i][j] + state->field[newX][newY]) == 2)
        canMoveLeft = 0;
    }
  }
  if (canMoveLeft == 1) (state->coordY)--;
  int attached = blockIsAttached();
  if (attached == 0)
    state->status = Moving;
  else
    state->status = Attaching;
}

void moveBlockRight() {
  State_t *state = getCurrentState();
  int canMoveRight = 1;
  for (int i = 0; i < state->blockSize; i++) {
    for (int j = 0; j < state->blockSize; j++) {
      int newX = state->coordX - i;
      int newY = state->coordY + 1 + j;
      if ((state->block[i][j] == 1) && (newY >= FIELD_W))
        canMoveRight = 0;
      else if ((newX >= 0) && (newY < FIELD_W) &&
               (state->block[i][j] + state->field[newX][newY] == 2))
        canMoveRight = 0;
    }
  }
  if (canMoveRight == 1) (state->coordY)++;
  int attached = blockIsAttached();
  if (attached == 0)
    state->status = Moving;
  else
    state->status = Attaching;
}

int canRotateBlock(int **newBlock) {
  State_t *state = getCurrentState();
  int canRotate = 1;
  int **oldBlock = state->block;
  state->block = newBlock;

  for (int i = 0; i < state->blockSize; i++) {
    for (int j = 0; j < state->blockSize; j++) {
      int newX = state->coordX - i;
      int newY = state->coordY + j;
      if ((state->block[i][j] == 1) && ((newY < 0) || (newY > FIELD_W)))
        canRotate = 0;
      else if (newY >= 0 && newY < FIELD_W && newX >= 0 &&
               (state->block[i][j] + state->field[newX][newY]) == 2)
        canRotate = 0;
    }
  }
  state->block = oldBlock;
  return canRotate;
}

void rotateBlock() {
  State_t *state = getCurrentState();
  int **newBlock = createMatrix(state->blockSize, state->blockSize);
  rotate(newBlock, state->block, state->blockSize);

  if (canRotateBlock(newBlock) == 1) {
    freeMatrix(state->block, state->blockSize);
    state->block = newBlock;
  } else
    freeMatrix(newBlock, state->blockSize);

  if (blockIsAttached())
    state->status = Attaching;
  else
    state->status = Moving;
}

void pauseGame() {
  State_t *state = getCurrentState();

  if (state->pause == 0) {
    state->status = 1;
    unsigned long long elapsedTime = currentTimeMillis() - state->startTime;
    if (elapsedTime >= state->timeLeft)
      state->timeLeft = 0;
    else
      state->timeLeft -= elapsedTime;
  } else {
    state->pause = 0;
    state->startTime = currentTimeMillis();
  }
}

void spawnNewBlock() {
  State_t *state = getCurrentState();

  freeMatrix(state->block, state->blockSize);
  state->blockSize = state->nextBlockSize;
  state->block = state->nextBlock;

  state->coordX = -1;
  if (state->blockSize == 2)
    state->coordY = 4;
  else
    state->coordY = 3;

  int nextBlockSize = 0;
  int **nextBlock = generateNewBlock(&nextBlockSize);
  state->nextBlockSize = nextBlockSize;
  state->nextBlock = nextBlock;

  state->status = Moving;
  state->startTime = currentTimeMillis();
  state->timeLeft = state->speed;
}

void shiftBlock() {
  State_t *state = getCurrentState();

  if (blockIsAttached())
    state->status = Attaching;
  else {
    (state->coordX)++;
    state->startTime = currentTimeMillis();
    state->timeLeft = state->speed;
    state->status = Moving;
  }
}

void saveMaxScore() {
  State_t *state = getCurrentState();
  FILE *file;
  int highScore = 0;

  file = fopen("max_score.txt", "r");
  fscanf(file, "%d", &highScore);

  if (state->score > highScore) {
    file = fopen("max_score.txt", "w");
    fprintf(file, "%d", state->score);
    fclose(file);
  }
}

void updateLevel() {
  State_t *state = getCurrentState();
  int newLevel = state->score / NEW_LEVEL_THRESHOLD + 1;
  if (newLevel > MAX_LEVEL) newLevel = MAX_LEVEL;
  state->speed -= (newLevel - state->level) * SPEED_STEP;
  state->level = newLevel;
}

void consumeRows() {
  State_t *state = getCurrentState();
  int consumeRows = 0;
  for (int i = FIELD_H - 1; i > 0; i--) {
    int countBlocks = 0;

    for (int j = 0; j < FIELD_W; j++) {
      countBlocks = state->field[i][j];
    }

    if (countBlocks == FIELD_W) {
      for (int z = i; z > 0; z--) {
        for (int j = 0; j < FIELD_W; j++) {
          state->field[z][j] = state->field[z - 1][j];
        }
      }
      i++;
      consumeRows++;
    }
  }

  if (consumeRows == 1)
    state->score += 100;
  else if (consumeRows == 2)
    state->score += 300;
  else if (consumeRows == 3)
    state->score += 700;
  else if (consumeRows == 4)
    state->score += 1500;

  saveMaxScore();
  updateLevel();
}

void attachBlock() {
  State_t *state = getCurrentState();
  int gameOver = 0;

  for (int i = 0; i < state->blockSize; i++) {
    for (int j = 0; j < state->blockSize; j++) {
      int newX = state->coordX - i;
      int newY = state->coordY + j;

      if (newX >= 0 && newX < FIELD_H && newY >= 0 && newY < FIELD_W &&
          state->block[i][j] == 1)
        state->field[newX][newY] = 1;
      else if (state->block[i][j] == 1 && newX < 0)
        gameOver = 1;
    }
  }

  if (gameOver)
    state->status = GameOver;
  else {
    consumeRows();
    state->status = Spawn;
  }
}

void finishGame() {
  State_t *state = getCurrentState();

  freeMatrix(state->block, state->blockSize);
  freeMatrix(state->nextBlock, state->nextBlockSize);
  freeMatrix(state->field, FIELD_H);

  initializeState();
}

GameInfo_t updateCurrentState() {
  State_t *state = getCurrentState();
  GameInfo_t info;
  int **field = createMatrix(FIELD_H, FIELD_W);

  copyMatrix(field, state->field, FIELD_H, FIELD_W);
  for (int i = 0; i < state->blockSize; i++) {
    for (int j = 0; j < state->blockSize; j++) {
      int newX = state->coordX - i;
      int newY = state->coordY + j;
      if (state->block[i][j] && newX >= 0 && newY >= FIELD_W)
        field[newX][newY] = 1;
    }
  }

  info.field = state->field;
  info.score = state->score;
  info.level = state->level;
  info.speed = state->speed;
  int **next = createMatrix(4, 4);
  copyMatrix(next, state->nextBlock, state->nextBlockSize,
             state->nextBlockSize);
  int highScore = 0;
  FILE *file = fopen("max_score.txt", "r");
  fscanf(file, "%d", &highScore);
  fclose(file);
  info.highScore = highScore;
  info.pause = 0;
  if (state->status == Initial) info.pause = GameStart;
  if (state->pause) info.pause = GamePause;
  return info;
}